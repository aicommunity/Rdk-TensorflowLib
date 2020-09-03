#include "ttfsession.h"

#define USE_RDK 1

namespace  TTF
{

const char* TTfSession::DebugStr[]={
    "Everything OK",
    "",
    "Set input node name with SetGraphParams",
    "Input node wasn't found in the Graph",
    "Empty input tensor",
    "Division by zero. Set image parameters with SetImgParams",
    "Set output node name with SetGraphParams",
    "Some of output nodes wasn't found in the graph",
    "Number of channels in input doesn't equal to input tensor channel number",
    "Desired data type are not supported for SetInputDataCvMeth. Use SetInputDataTfMeth",
    "Try to copy from NULL pointer",
    "",
    "Wrong config"
};

bool TTfSession::CheckInOutNodes(tensorflow::GraphDef& graph_def)
{
    try{

        //Проверки на наличие имени входного и выходного узла
        if(InputName.empty())
        {
            ErCode = TfErrorCode::EMPTY_INPUT_NAME;
            return false;
        }

        if(OutputName.empty())
        {
            ErCode = TfErrorCode::EMPTY_OUTPUT_NAME;
            return false;
        }

        //Поиск входного звена
        tensorflow::NodeDef Node;

        for(int i=0; i<graph_def.node_size();i++)
        {
            Node = graph_def.node(i);
            if(Node.name()==InputName)
            {
                break;
            }
            //Если узел не был найден
            if(i==(graph_def.node_size()-1))
            {
                ErCode = TfErrorCode::INPUT_NODE_DOESNT_EXIST_IN_GRAPH;
                return false;
            }
        }

        //Задание параметров входного тензора из параметров узла
        InputDataType = Node.attr().at("dtype").type();
        ImgHeight =     int(Node.attr().at("shape").shape().dim(1).size());
        ImgWidth  =     int(Node.attr().at("shape").shape().dim(2).size());
        ImgChannels  =  int(Node.attr().at("shape").shape().dim(3).size());

        //Проверка на существование выходных узлов
        for(int j=0; j < OutputName.size(); j++)
        {
            for(int i=0; i<graph_def.node_size();i++)
            {
                Node = graph_def.node(i);
                if(Node.name()==OutputName[j])
                {
                    break;
                }
                if(i==(graph_def.node_size()-1))
                {
                    ErCode = TfErrorCode::OUTPUT_NODE_DOESNT_EXIST_IN_GRAPH;
                    return false;
                }
            }
        }
    }
    catch (std::exception const& e)
    {
        ErCode = TfErrorCode::EXCEPTION;
        ExceptionString=e.what();
        return false;
    }

    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSession::InitSession(const double &gpu_fraction)
{
    GpuFraction=gpu_fraction;
    //Определение параметров сессии
    tensorflow::SessionOptions opts;
    //opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_fraction);
    opts.config.mutable_gpu_options()->set_allow_growth(true);
    opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.6);

    //Инициализация сессии
    Status = tensorflow::NewSession(opts, &Session);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    return true;
}

TTfSession::TTfSession(void)
{

}

TTfSession::~TTfSession(void)
{

}

bool TTfSession::InitModel(const std::string &file_name, const double &gpu_fraction, const int& device_number)
{

     tensorflow::GraphDef GraphDef;

    if(!InitSession(gpu_fraction))
    {
        return false;
    }

    //Чтение pb модели в граф
    Status = ReadBinaryProto(tensorflow::Env::Default(), file_name, &GraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    //Добавление графа в сессию
    Status = Session->Create(GraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    //TODO Номер GPU
    /*
    std::string device = "/device:GPU:"+std::to_string(device_number);
    for (int i = 0; i < GraphDef.node_size(); ++i)
    {
      auto node = GraphDef.mutable_node(i);
      node->set_device(device);

    }
    */

    //Запуск метода проверки входного и выходных узлов
    if(!CheckInOutNodes(GraphDef))
    {
        return false;
    }

    //Первый запуск сети медленный
    //Для того разогрев
    cv::Mat warmUp(ImgHeight, ImgWidth, CV_MAKETYPE(CV_32F,ImgChannels), cv::Scalar(0,0,0));
    tensorflow::Tensor NewOne(InputDataType, tensorflow::TensorShape({1,warmUp.rows,warmUp.cols,warmUp.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), warmUp.data, size_t(warmUp.rows*warmUp.cols*warmUp.channels()*tensorflow::DataTypeSize(InputDataType)));

    //Установка входного тензора
    if(!SetInputTensor(NewOne))
    {
        return false;
    }
    /*
    if(!SetInputDataCvMeth(warmUp))
        return false;
    */

    if(!Run())
        return false;
    /*
    //Для классификатора имеет смысл
    if(!(this->GetOutput().empty()) && this->GetOutput()[0].dims()>2)
    {
        NumberOfClasses=this->GetOutput()[0].dim_size(1);
    }
    */
    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSession::UnInit(void)
{
    /*Status = Session->Close();
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    */


    if(IsTransSessCreated)
    {
        Status = SessionForTransform->Close();
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }


    }

    ImgWidth=ImgHeight=-1;
    ImgChannels=0;
    Divide=1;
    Substract={0,0,0};

    InputName.clear();
    OutputName.clear();

    IsTransSessCreated = false;

    Output.clear();
    OutputForTransform.clear();
    Input.clear();

    Status=(Status.OK());
    //ErCode=OK;
    return true;
}

bool TTfSession::SetGraphParams(const std::vector<std::string> &output_name, const std::string &input_name)
{
    //Проверки на наличие имени входного и выходного узла
    if(input_name.empty())
    {
        ErCode = TfErrorCode::EMPTY_INPUT_NAME;
        return false;
    }
    if(output_name.empty())
    {
        ErCode = TfErrorCode::EMPTY_OUTPUT_NAME;
        return false;
    }

    //Определение имен входного и выходного узла
    OutputName = output_name;
    InputName = input_name;

    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSession::SetInputTensor(const tensorflow::Tensor &input_tensor)
{
   //Проверка на наличие названия у тензора
   if(InputName.empty())
   {
       ErCode = TfErrorCode::EMPTY_INPUT_NAME;
       return false;
   }
   //Проверка на наличие элементов в тензоре
   if(!input_tensor.NumElements())
   {
       ErCode = TfErrorCode::EMPTY_INPUT_TENSOR;
       return false;
   }

   Input = { {InputName, input_tensor},};
   ErCode = TfErrorCode::OK;
   return true;
}

bool TTfSession::SetImgParams(const std::vector<float> & sub, const float & div, const bool& bgr)
{
    //Проверка на деление на ноль
    if(!int(div))
    {
        ErCode = TfErrorCode::DIVISION_BY_ZERO;
        return false;
    }

    if(Divide!=div || Substract!=sub || ImgBgr!=bgr)
    {
        IsTransSessCreated=false;
    }
    ImgBgr = bgr;
    Divide = div;
    Substract = sub;

    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSession::CreateGraphForTransform()
{
    tensorflow::GraphDef GraphDef;

    //Создание графа вычислений для преобразования входного тензора в нужный вид
    tensorflow::Scope root = tensorflow::Scope::NewRootScope();
    //Сюда будет задаваться тензор с сырыми данными изображения, данные в формате char
    auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);
    //Сюда будет задаваться тензор с новым размером изображения
    auto Size = tensorflow::ops::Placeholder(root.WithOpName("NewSize"), tensorflow::DataType::DT_INT32);
    //Узел изменения размера
    auto resized = tensorflow::ops::ResizeBicubic(root, a, Size);
    //auto resized = tensorflow::ops::ResizeBilinear(root, a, Size);
    //Узел преобразования тип данных тензора
    auto caster = tensorflow::ops::Cast(root, resized, InputDataType);
    //Узел вычитания из тензора
    auto substracted = tensorflow::ops::Sub(root, caster,tensorflow::ops::Cast(root,{0},InputDataType));
    if(ImgChannels>1)
    {
        substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract[0],Substract[1],Substract[2]},InputDataType));
    }
    else
    {
        substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract[0]},InputDataType));
    }
    //Узел деления данных тензора
    tensorflow::ops::Div(root.WithOpName("out"), substracted, tensorflow::ops::Cast(root,{Divide},InputDataType));
    //Перевод Scope в формат GraphDef
    Status = root.ToGraphDef(&GraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //Инициализация сессии преобразования
    tensorflow::SessionOptions opts;
    opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.6);
    opts.config.mutable_gpu_options()->set_allow_growth(true);

    Status = tensorflow::NewSession(opts, &SessionForTransform);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //Добавление графа преобразования в сессию
    Status = SessionForTransform->Create(GraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    IsTransSessCreated=true;
    return true;
}

#if USE_RDK == 1
bool TTfSession::SetInputDataTfMeth(RDK::UBitmap& image)
{
    //Преобразование UbitMap изображения в формат RGB
    RDK::UBitmap input;
    input.SetRes(image.GetWidth(), image.GetHeight(), image.GetColorModel());
    image.CopyTo(0,0,input);

    if(ImgChannels!=input.GetPixelByteLength())
    {
        ErCode = TfErrorCode::WRONG_IMAGE_CHANNELS_NUMBER;
        return false;
    }

    //Проверка на определение входных параметров
    if((Divide)==0.0f)
    {
        ErCode = TfErrorCode::DIVISION_BY_ZERO;
        return false;
    }



    if(ImgChannels>1 && !ImgBgr)
    {
         image.SwapRGBChannels(&input);
    }

    //if(image.GetColorModel() == RDK::ubmRGB24)
    //{
    //    image.SwapRGBChannels(&input);
    //}



    //Был ли создан дополнительный граф для трансформации
    if(!IsTransSessCreated)
    {
        if(!CreateGraphForTransform())
        {
            return false;
        }
    }

    //Если размер не задан строго, берется размер входного изображения
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=input.GetHeight();
        ImgWidth=input.GetWidth();
    }
    //Тензор с входным изображением с данными в формате char
    tensorflow::Tensor NewOne(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,input.GetHeight(),input.GetWidth(),input.GetPixelByteLength()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();
    //Перевод из BGR в RGB
    if(input.GetData()==nullptr)
    {
        ErCode = TfErrorCode::COPY_DATA_FROM_NULL_PTR;
        return false;
    }

    //Копирование данных из cv::Mat в входной тензор
    memcpy(const_cast<char*>(tmp_data.data()), (input.GetData()), (unsigned long)(input.GetByteLength()));

    //Тензор со значениями нового размера изображения
    tensorflow::Tensor SizeTensor(tensorflow::DataType::DT_INT32, tensorflow::TensorShape({2}));
    SizeTensor.tensor<int32_t,1>()(0)=ImgHeight;
    SizeTensor.tensor<int32_t,1>()(1)=ImgWidth;

    //Запуск преобразования, передавая входное изображение и новые значения размера
    Status = SessionForTransform->Run({{"input",NewOne},{"NewSize",SizeTensor}},{"out"},{},&OutputForTransform);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //Установка входного тензора
    if(!SetInputTensor(OutputForTransform[0]))
    {
        return false;
    }

    OutputForTransform.clear();

    ErCode = TfErrorCode::OK;
    return true;
}

bool TTfSession::SetInputDataCvMeth(RDK::UBitmap& image)
{
    if(image.GetData()==nullptr)
    {
        ErCode = TfErrorCode::COPY_DATA_FROM_NULL_PTR;
        return false;
    }

    cv::Mat imageMat=cv::Mat(image.GetHeight(), image.GetWidth(), CV_8UC3, image.GetData()).clone();

    if(!SetInputDataCvMeth(imageMat))
    {
        return false;
    }

    ErCode = TfErrorCode::OK;


    return true;
}
#endif
bool TTfSession::SetInputDataTfMeth(cv::Mat& image)
{


    //Проверка на определение входных параметров
    if((Divide)==0.0f)
    {
        ErCode = TfErrorCode::DIVISION_BY_ZERO;
        return false;
    }

    //Был ли создан дополнительный граф для трансформации
    if(!IsTransSessCreated)
    {
        if(!CreateGraphForTransform())
        {
            return false;
        }
    }

    //Если размер не задан строго, берется размер входного изображения cv::Mat
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=image.rows;
        ImgWidth=image.cols;
    }
    //Тензор с входным изображением с данными в формате char
    tensorflow::Tensor NewOne(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,image.rows,image.cols,image.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    //Перевод из BGR в RGB
    cv::Mat input;
    image.copyTo(input);
    if(input.channels()>1 && !ImgBgr)
    {
        cv::cvtColor(image, input, cv::COLOR_BGR2RGB);
    }


    //Копирование данных из cv::Mat в входной тензор
    memcpy(const_cast<char*>(tmp_data.data()), input.data, (unsigned long)(input.rows*input.cols*input.channels())*sizeof(char));

    //Тензор со значениями нового размера изображения
    tensorflow::Tensor SizeTensor(tensorflow::DataType::DT_INT32, tensorflow::TensorShape({2}));
    SizeTensor.tensor<int32_t,1>()(0)=ImgHeight;
    SizeTensor.tensor<int32_t,1>()(1)=ImgWidth;

    //Запуск преобразования, передавая входное изображение и новые значения размера
    Status = SessionForTransform->Run({{"input",NewOne},{"NewSize",SizeTensor}},{"out"},{},&OutputForTransform);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //Установка входного тензора
    if(!SetInputTensor(OutputForTransform[0]))
    {
        return false;
    }

    OutputForTransform.clear();

    ErCode = TfErrorCode::OK;


    return true;


}

bool TTfSession::SetInputDataCvMeth(cv::Mat& image)
{

    //Проверка на определение входных параметров
    if((Divide)==0.0f)
    {
        ErCode = TfErrorCode::DIVISION_BY_ZERO;
        return false;
    }

    //Проверка на равенство кол-ва каналов входного узла и изображения формата cv::Mat
    if(ImgChannels!=image.channels())
    {
        ErCode = TfErrorCode::WRONG_IMAGE_CHANNELS_NUMBER;
        return false;
    }

    //Если размер не задан строго, берется размер входного изображения
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=image.rows;
        ImgWidth=image.cols;
    }
    cv::Mat input;
    image.copyTo(input);
    //Перевод из BGR в RGB
    if(ImgChannels>1 && !ImgBgr)
    {
        cv::cvtColor(image, input, cv::COLOR_BGR2RGB);
    }

    //Изменение типа. Не все типы поддерживаются
    if(InputDataType==tensorflow::DataType::DT_FLOAT)
    {
        input.convertTo(input, CV_MAKETYPE(CV_32F,ImgChannels));
    }
    else if(InputDataType==tensorflow::DataType::DT_UINT8)
    {
        input.convertTo(input, CV_MAKETYPE(CV_8U,ImgChannels));
    }
    else
    {
        ErCode = TfErrorCode::TYPE_UNSOPPORTED_FOR_CV_METH;
        return false;
    }   

    if(ImgChannels>1)
    {
        cv::subtract(input,cv::Scalar(Substract[0],Substract[1],Substract[2]),input);

    }
    else
    {
        cv::subtract(input,cv::Scalar(Substract[0]),input);

    }

    //std::cout << input <<std::endl;
    //sleep(5);
    input = input / double(Divide);

    //Изменение размера
    cv::resize(input,input,cv::Size(ImgWidth,ImgHeight));

    //std::cout << input <<std::endl;
     //sleep(5);
    //Сохранение полученного изображения в тензор
    tensorflow::Tensor NewOne(InputDataType, tensorflow::TensorShape({1,input.rows,input.cols,input.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), input.data, size_t(input.rows*input.cols*input.channels()*tensorflow::DataTypeSize(InputDataType)));

    //Установка входного тензора
    if(!SetInputTensor(NewOne))
    {
        return false;
    }


    ErCode = TfErrorCode::OK;


    return true;
}

bool TTfSession::Run(void)
{
    //Проверка на наличие входного тензора
    if(Input.empty())
    {
        ErCode = TfErrorCode::EMPTY_INPUT_TENSOR;
        return false;
    }
    //Проверка на наличие имени входного узла
    if(OutputName.empty())
    {
        ErCode = TfErrorCode::EMPTY_OUTPUT_NAME;
        return false;
    }

    //Запуск сессии
    Status = Session->Run(Input, OutputName, {}, &Output);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    ErCode = TfErrorCode::OK;
    return true;
}

const std::vector<tensorflow::Tensor>& TTfSession::GetOutput(void)
{
    return Output;
}

const tensorflow::Tensor &TTfSession::GetInputTensor(void)
{
    return Input[0].second;
}

std::vector<int> TTfSession::GetImgParams(void)
{
    std::vector<int> Params;
    Params.push_back(ImgHeight);
    Params.push_back(ImgWidth);
    Params.push_back(ImgChannels);

    return Params;
}

const std::string TTfSession::GetDebugStr(void)
{
    if(ErCode == TfErrorCode::BAD_STATUS)
    {
        return Status.ToString();
    }

    if(ErCode == TfErrorCode::EXCEPTION)
    {
        return ExceptionString;
    }
    return DebugStr[int(ErCode)];
}

int TTfSession::GetNumClasses()
{
    return NumberOfClasses;
}

}
