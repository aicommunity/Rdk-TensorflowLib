#include "ttfsession.h"
#include "tensorflow/cc/ops/standard_ops.h"
namespace  TTF
{

bool TTfSession::CheckInOutNodes()
{
    //Проверки на наличие имени входного и выходного узла
    if(InputName.empty())
    {
        ErCode=EMPTY_INPUT_NAME;
        return false;
    }

    if(OutputName.empty())
    {
        ErCode=EMPTY_OUTPUT_NAME;
        return false;
    }

    //Поиск входного звена
    tensorflow::NodeDef Node;
    for(int i=0; i<GraphDef.node_size();i++)
    {
        Node = GraphDef.node(i);
        if(Node.name()==InputName)
        {
            break;
        }
        //Если узел не был найден
        if(i==(GraphDef.node_size()-1))
        {
            ErCode = INPUT_NODE_DOESNT_EXIST_IN_GRAPH;
            return false;
        }
    }
    //Задание параметров входного тензора из параметров узла
    InputDataType = Node.attr().at("dtype").type();
    ImgHeight =     int(Node.attr().at("shape").shape().dim(1).size());
    ImgWidth  =     int(Node.attr().at("shape").shape().dim(2).size());
    ImgChan  =      int(Node.attr().at("shape").shape().dim(3).size());

    //Проверка на сущетсвование выходного узла
    for(uint j=0; j < OutputName.size(); j++)
    {
        for(int i=0; i<GraphDef.node_size();i++)
        {
            Node = GraphDef.node(i);
            if(Node.name()==OutputName[j])
            {
                break;
            }
            if(i==(GraphDef.node_size()-1))
            {
                ErCode = OUTPUT_NODE_DOESNT_EXIST_IN_GRAPH;
                return false;
            }
        }
    }

    ErCode=OK;
    return true;
}

bool TTfSession::InitSession(const double &gpu_fraction, const bool& allow_gpu_grow)
{
    //Определение параметров сессии
    tensorflow::SessionOptions opts;
    opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_fraction);
    opts.config.mutable_gpu_options()->set_allow_growth(allow_gpu_grow);

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

bool TTfSession::InitModel(const std::string &file_name, const double &gpu_fraction, const bool& allow_gpu_grow)
{
    if(!InitSession(gpu_fraction, allow_gpu_grow))
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

    //Запуск метода проверки входного и выходных узлов
    if(!CheckInOutNodes())
    {
        return false;
    }

    ErCode=OK;
    return true;
}

bool TTfSession::InitModel(const std::string &path_to_meta, const std::string &path_to_ckpt, const double &gpu_fraction, const bool& allow_gpu_grow)
{
    if(!InitSession(gpu_fraction, allow_gpu_grow))
    {
        return false;
    }

    //Загрузка графа из meta файла в MetaGraphDef
    Status = ReadBinaryProto(tensorflow::Env::Default(), path_to_meta, &MetaGraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    //Добавление графа в сессию
    Status = Session->Create(MetaGraphDef.graph_def());
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    //Чтение параметров (весов) в граф
    tensorflow::Tensor checkpointPathTensor(tensorflow::DataType::DT_STRING, tensorflow::TensorShape());

    checkpointPathTensor.scalar<std::string>()() = path_to_ckpt;
    //Чтение весов модели из файла checkpoint'а
    Status = Session->Run({{MetaGraphDef.saver_def().filename_tensor_name(), checkpointPathTensor },},
                          {},{MetaGraphDef.saver_def().restore_op_name()},nullptr);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    //Запуск метода проверки входного и выходных узлов
    if(!CheckInOutNodes())
    {
        return false;
    }

    ErCode=OK;
    return true;
}

bool TTfSession::UnInit(void)
{
    Status = Session->Close();
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    Status = SessionForTransform->Close();
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    ImgWidth=ImgHeight=-1;
    ImgChan=0;
    Divide=Substract=0;

    InputName.clear();
    OutputName.clear();

    GraphDef.Clear();
    GraphForTransform.Clear();
    MetaGraphDef.Clear();

    IsTransSessCreated = false;

    Output.clear();
    OutputForTransform.clear();
    Input.clear();

    Status=(Status.OK());
    ErCode=OK;
    return true;
}

bool TTfSession::SetGraphParams(const std::vector<std::string> &output_name, const std::string &input_name)
{
    //Проверки на наличие имени входного и выходного узла
    if(input_name.empty())
    {
        ErCode=EMPTY_INPUT_NAME;
        return false;
    }
    if(output_name.empty())
    {
        ErCode=EMPTY_OUTPUT_NAME;
        return false;
    }

    //Определение имен входного и выходного узла
    OutputName = output_name;
    InputName = input_name;

    ErCode=OK;
    return true;
}

bool TTfSession::SetInputTensor(const tensorflow::Tensor &input_tensor)
{
   //Проверка на наличие названия у тензора
   if(InputName.empty())
   {
       ErCode = EMPTY_INPUT_NAME;
       return false;
   }
   //Проверка на наличие элементов в тензоре
   if(!input_tensor.NumElements())
   {
       ErCode = EMPTY_INPUT_TENSOR;
       return false;
   }
   Input = { {InputName, input_tensor}, };
   ErCode=OK;
   return true;
}

bool TTfSession::SetImgParams(const float & sub, const float & div)
{
    //Проверка на деление на ноль
    if(!int(div))
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }
    if(Divide!=div || Substract!=sub)
    {
        IsTransSessCreated=false;
    }
    Divide = div;
    Substract = sub;
    ErCode=OK;
    return true;
}

bool TTfSession::SetInputDataTfMeth(const RDK::UBitmap& image)
{
    //Проверка на определение входных параметров
    if(int(Divide)==0)
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }

    //Был ли создан дополнительный граф для трансформации
    if(!IsTransSessCreated)
    {
        //Создание графа вычислений для преобразования входной тензор в нужный вид
        tensorflow::Scope root = tensorflow::Scope::NewRootScope();
        //Сюда будет задаваться тензор с сырыми данными изображения, данные в формате char
        auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);
        //Сюда будет задаваться тензор с новым размером изображения
        auto Size = tensorflow::ops::Placeholder(root.WithOpName("NewSize"), tensorflow::DataType::DT_INT32);
        //Узел изменения размера
        auto resized = tensorflow::ops::ResizeBilinear(root, a, Size);
        //Узел преобразования тип данных тензора
        auto caster = tensorflow::ops::Cast(root, resized, InputDataType);
        //Узел вычитания из тензора
        auto substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract},InputDataType));
        //Узел деления данных тензора
        tensorflow::ops::Div(root.WithOpName("out"), substracted, tensorflow::ops::Cast(root,{Divide},InputDataType));
        //Перевод Scope в формат GraphDef
        Status = root.ToGraphDef(&GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //Инициализация сессии преобразования
        Status = tensorflow::NewSession(tensorflow::SessionOptions(), &SessionForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //Добавление графа преобразования в сессию
        Status = SessionForTransform->Create(GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        IsTransSessCreated=true;
    }

    //Если размер не задан строго, берется размер входного изображения cv::Mat
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=image.GetHeight();
        ImgWidth=image.GetWidth();
    }
    //Тензор с входным изображением с данными в формате char
    tensorflow::Tensor NewOne(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,image.GetHeight(),image.GetWidth(),image.GetPixelByteLength()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();
    //Перевод из BGR в RGB
    //cv::cvtColor(image, image, CV_BGR2RGB);
    if(image.GetData()==nullptr)
    {
        ErCode = TfErrorCode::COPY_DATA_FROM_NULL_PTR;
        return false;
    }

    //Копирование данных из cv::Mat в входной тензор
    memcpy(const_cast<char*>(tmp_data.data()), (image.GetData()), (unsigned long)(image.GetByteLength()));

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

    ErCode=OK;
    return true;
}



bool TTfSession::SetInputDataTfMeth(cv::Mat& image)
{
    //Проверка на определение входных параметров
    if(int(Divide)==0)
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }



    //Был ли создан дополнительный граф для трансформации
    if(!IsTransSessCreated)
    {
        //Создание графа вычислений для преобразования входной тензор в нужный вид
        tensorflow::Scope root = tensorflow::Scope::NewRootScope();
        //Сюда будет задаваться тензор с сырыми данными изображения, данные в формате char
        auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);
        //Сюда будет задаваться тензор с новым размером изображения
        auto Size = tensorflow::ops::Placeholder(root.WithOpName("NewSize"), tensorflow::DataType::DT_INT32);
        //Узел изменения размера
        auto resized = tensorflow::ops::ResizeBilinear(root, a, Size);
        //Узел преобразования тип данных тензора
        auto caster = tensorflow::ops::Cast(root, resized, InputDataType);
        //Узел вычитания из тензора
        auto substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract},InputDataType));
        //Узел деления данных тензора
        tensorflow::ops::Div(root.WithOpName("out"), substracted, tensorflow::ops::Cast(root,{Divide},InputDataType));
        //Перевод Scope в формат GraphDef
        Status = root.ToGraphDef(&GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //Инициализация сессии преобразования
        Status = tensorflow::NewSession(tensorflow::SessionOptions(), &SessionForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //Добавление графа преобразования в сессию
        Status = SessionForTransform->Create(GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        IsTransSessCreated=true;
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

        cv::cvtColor(image, image, CV_BGR2RGB);


    //Копирование данных из cv::Mat в входной тензор
    memcpy(const_cast<char*>(tmp_data.data()), image.data, (unsigned long)(image.rows*image.cols*image.channels())*sizeof(char));

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

    ErCode=OK;
    return true;
}

bool TTfSession::SetInputDataCvMeth(cv::Mat& image)
{
    //Проверка на определение входных параметров
    if(int(Divide)==0)
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }

    //Проверка на равенство кол-ва каналов входного узла и изображения формата cv::Mat
    if(ImgChan!=image.channels())
    {
        ErCode = CV_MAT_HAS_WRONG_CHANNELS_NUMBER;
        return false;
    }

    //Если размер не задан строго, берется размер входного изображения
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=image.rows;
        ImgWidth=image.cols;
    }

    //Перевод из BGR в RGB
    cv::cvtColor(image, image, CV_BGR2RGB);
    //Изменение размера
    cv::resize(image,image,cv::Size(ImgWidth,ImgHeight));

    //Изменение типа. Не все типы поддерживаются
    if(InputDataType==tensorflow::DataType::DT_FLOAT)
    {
        image.convertTo(image, CV_MAKETYPE(CV_32F,ImgChan));
    }
    else if(InputDataType==tensorflow::DataType::DT_UINT8)
    {
        image.convertTo(image, CV_MAKETYPE(CV_8U,ImgChan));
    }
    else
    {
        ErCode=TYPE_UNSOPPORTED_FOR_CV_METH;
        return false;
    }

    //Вычитание и деление
    image = image - double(Substract);
    image = image / double(Divide);

    //Сохранение полученного изображения в тензор
    tensorflow::Tensor NewOne(InputDataType, tensorflow::TensorShape({1,image.rows,image.cols,image.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();
    memcpy(const_cast<char*>(tmp_data.data()), image.data, size_t(image.rows*image.cols*image.channels()*tensorflow::DataTypeSize(InputDataType)));

    //Установка входного тензора
    if(!SetInputTensor(NewOne))
    {
        return false;
    };

    ErCode=OK;
    return true;
}

bool TTfSession::Run(void)
{
    //Проверка на наличие входного тензора
    if(Input.empty())
    {
        ErCode = EMPTY_INPUT_TENSOR;
        return false;
    }
    //Проверка на наличие имени входного узла
    if(OutputName.empty())
    {
        ErCode = EMPTY_OUTPUT_NAME;
        return false;
    }

    //Запуск сессии
    Status = Session->Run(Input, OutputName, {}, &Output);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }

    ErCode=OK;
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

const std::string TTfSession::GetDebugStr(void)
{
    if(ErCode==BAD_STATUS)
    {
        return Status.ToString();
    }

    return DebugStr[ErCode];
}

}
