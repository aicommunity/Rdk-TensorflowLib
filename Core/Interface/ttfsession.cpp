#include "ttfsession.h"

namespace  TTF
{

bool TTfSession::CheckInOutNodes()
{
    //�������� �� ������� ����� �������� � ��������� ����
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

    //����� �������� �����
    tensorflow::NodeDef Node;

    for(int i=0; i<GraphDef.node_size();i++)
    {
        Node = GraphDef.node(i);
        if(Node.name()==InputName)
        {
            break;
        }
        //���� ���� �� ��� ������
        if(i==(GraphDef.node_size()-1))
        {
            ErCode = INPUT_NODE_DOESNT_EXIST_IN_GRAPH;
            return false;
        }
    }

    //������� ���������� �������� ������� �� ���������� ����
    InputDataType = Node.attr().at("dtype").type();
    ImgHeight =     int(Node.attr().at("shape").shape().dim(1).size());
    ImgWidth  =     int(Node.attr().at("shape").shape().dim(2).size());
    ImgChannels  =  int(Node.attr().at("shape").shape().dim(3).size());

    //�������� �� ������������� �������� �����
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
    GpuGrow=allow_gpu_grow;
    GpuFraction=gpu_fraction;
    //����������� ���������� ������
    tensorflow::SessionOptions opts;
    opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(gpu_fraction);
    opts.config.mutable_gpu_options()->set_allow_growth(allow_gpu_grow);


    //������������� ������
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

bool TTfSession::InitModel(const std::string &file_name, const double &gpu_fraction, const bool& allow_gpu_grow, const int& device_number)
{
    if(!InitSession(gpu_fraction, allow_gpu_grow))
    {
        return false;
    }

    //������ pb ������ � ����
    Status = ReadBinaryProto(tensorflow::Env::Default(), file_name, &GraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }



    //���������� ����� � ������
    Status = Session->Create(GraphDef);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //TODO ����� GPU
    /*
    std::string device = "/device:GPU:"+std::to_string(device_number);
    for (int i = 0; i < GraphDef.node_size(); ++i)
    {
      auto node = GraphDef.mutable_node(i);
      node->set_device(device);

    }
    */

    //������ ������ �������� �������� � �������� �����
    if(!CheckInOutNodes())
    {
        return false;
    }

    //������ ������ ���� ���������
    //��� ���� ��������
    cv::Mat warmUp(ImgHeight, ImgWidth, CV_MAKETYPE(CV_32F,ImgChannels), cv::Scalar(0,0,0));
    tensorflow::Tensor NewOne(InputDataType, tensorflow::TensorShape({1,warmUp.rows,warmUp.cols,warmUp.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), warmUp.data, size_t(warmUp.rows*warmUp.cols*warmUp.channels()*tensorflow::DataTypeSize(InputDataType)));

    //��������� �������� �������
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
    //�� �� ����
    if(!(this->GetOutput().empty()) && this->GetOutput()[0].dims()>2)
    {
        NumberOfClasses=this->GetOutput()[0].dim_size(1);
    }
    */
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
    GraphDef.Clear();

    if(IsTransSessCreated)
    {
        Status = SessionForTransform->Close();
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        GraphForTransform.Clear();

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
    //�������� �� ������� ����� �������� � ��������� ����
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

    //����������� ���� �������� � ��������� ����
    OutputName = output_name;
    InputName = input_name;

    ErCode=OK;
    return true;
}

bool TTfSession::SetInputTensor(const tensorflow::Tensor &input_tensor)
{
   //�������� �� ������� �������� � �������
   if(InputName.empty())
   {
       ErCode = EMPTY_INPUT_NAME;
       return false;
   }
   //�������� �� ������� ��������� � �������
   if(!input_tensor.NumElements())
   {
       ErCode = EMPTY_INPUT_TENSOR;
       return false;
   }

   Input = { {InputName, input_tensor},};
   ErCode=OK;
   return true;
}

bool TTfSession::SetImgParams(const std::vector<float> & sub, const float & div, const bool& bgr)
{
    //�������� �� ������� �� ����
    if(!int(div))
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }

    if(Divide!=div || Substract!=sub || ImgBgr!=bgr)
    {
        IsTransSessCreated=false;
    }
    ImgBgr = bgr;
    Divide = div;
    Substract = sub;

    ErCode=OK;
    return true;
}

/*
bool TTfSession::SetInputDataTfMeth(RDK::UBitmap& image)
{


    //�������������� UbitMap ����������� � ������ RGB
    RDK::UBitmap input;
    input.SetRes(image.GetWidth(), image.GetHeight(), image.GetColorModel());
    image.CopyTo(0,0,input);

    if(ImgChannels!=input.GetPixelByteLength())
    {
        ErCode = WRONG_IMAGE_CHANNELS_NUMBER;
        return false;
    }

    //�������� �� ����������� ������� ����������
    if((Divide)==0.0f)
    {
        ErCode = DIVISION_BY_ZERO;
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



    //��� �� ������ �������������� ���� ��� �������������
    if(!IsTransSessCreated)
    {
        //�������� ����� ���������� ��� �������������� ������� ������ � ������ ���
        tensorflow::Scope root = tensorflow::Scope::NewRootScope();
        //���� ����� ���������� ������ � ������ ������� �����������, ������ � ������� char
        auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);
        //���� ����� ���������� ������ � ����� �������� �����������
        auto Size = tensorflow::ops::Placeholder(root.WithOpName("NewSize"), tensorflow::DataType::DT_INT32);
        //���� ��������� �������
        auto resized = tensorflow::ops::ResizeBicubic(root, a, Size);
        //auto resized = tensorflow::ops::ResizeBilinear(root, a, Size);
        //���� �������������� ��� ������ �������
        auto caster = tensorflow::ops::Cast(root, resized, InputDataType);
        //���� ��������� �� �������
        auto substracted = tensorflow::ops::Sub(root, caster,tensorflow::ops::Cast(root,{0},InputDataType));
        if(ImgChannels>1)
        {
            substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract[0],Substract[1],Substract[2]},InputDataType));
        }
        else
        {
            substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract[0]},InputDataType));
        }
        //���� ������� ������ �������
        tensorflow::ops::Div(root.WithOpName("out"), substracted, tensorflow::ops::Cast(root,{Divide},InputDataType));
        //������� Scope � ������ GraphDef
        Status = root.ToGraphDef(&GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //������������� ������ ��������������
        tensorflow::SessionOptions opts;
        opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(GpuFraction);
        opts.config.mutable_gpu_options()->set_allow_growth(GpuGrow);

        Status = tensorflow::NewSession(opts, &SessionForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //���������� ����� �������������� � ������
        Status = SessionForTransform->Create(GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        IsTransSessCreated=true;
    }

    //���� ������ �� ����� ������, ������� ������ �������� �����������
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=input.GetHeight();
        ImgWidth=input.GetWidth();
    }
    //������ � ������� ������������ � ������� � ������� char
    tensorflow::Tensor NewOne(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,input.GetHeight(),input.GetWidth(),input.GetPixelByteLength()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();
    //������� �� BGR � RGB
    if(input.GetData()==nullptr)
    {
        ErCode = TfErrorCode::COPY_DATA_FROM_NULL_PTR;
        return false;
    }

    //����������� ������ �� cv::Mat � ������� ������
    memcpy(const_cast<char*>(tmp_data.data()), (input.GetData()), (unsigned long)(input.GetByteLength()));

    //������ �� ���������� ������ ������� �����������
    tensorflow::Tensor SizeTensor(tensorflow::DataType::DT_INT32, tensorflow::TensorShape({2}));
    SizeTensor.tensor<int32_t,1>()(0)=ImgHeight;
    SizeTensor.tensor<int32_t,1>()(1)=ImgWidth;

    //������ ��������������, ��������� ������� ����������� � ����� �������� �������
    Status = SessionForTransform->Run({{"input",NewOne},{"NewSize",SizeTensor}},{"out"},{},&OutputForTransform);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //��������� �������� �������
    if(!SetInputTensor(OutputForTransform[0]))
    {
        return false;
    }

    OutputForTransform.clear();

    ErCode=OK;
    return true;
}

bool TTfSession::SetInputDataCvMeth(RDK::UBitmap& image)
{
    cv::Mat imageMat=cv::Mat(image.GetHeight(), image.GetWidth(), CV_8UC3, image.GetData()).clone();

    if(!SetInputDataCvMeth(imageMat))
    {
        return false;
    }

    ErCode=OK;


    return true;
}
*/

bool TTfSession::SetInputDataTfMeth(cv::Mat& image)
{


    //�������� �� ����������� ������� ����������
    if((Divide)==0.0f)
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }

    //��� �� ������ �������������� ���� ��� �������������
    if(!IsTransSessCreated)
    {
        //�������� ����� ���������� ��� �������������� ������� ������ � ������ ���
        tensorflow::Scope root = tensorflow::Scope::NewRootScope();
        //���� ����� ���������� ������ � ������ ������� �����������, ������ � ������� char
        auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);
        //���� ����� ���������� ������ � ����� �������� �����������
        auto Size = tensorflow::ops::Placeholder(root.WithOpName("NewSize"), tensorflow::DataType::DT_INT32);
        //���� ��������� �������
        auto resized = tensorflow::ops::ResizeBilinear(root, a, Size);
        //���� �������������� ��� ������ �������
        auto caster = tensorflow::ops::Cast(root, resized, InputDataType);
        //���� ��������� �� �������
        auto substracted = tensorflow::ops::Sub(root, caster,tensorflow::ops::Cast(root,{0},InputDataType));
        if(ImgChannels>1)
        {
            substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract[0],Substract[1],Substract[2]},InputDataType));
        }
        else
        {
            substracted = tensorflow::ops::Sub(root, caster, tensorflow::ops::Cast(root,{Substract[0]},InputDataType));
        }
        //���� ������� ������ �������
        tensorflow::ops::Div(root.WithOpName("out"), substracted, tensorflow::ops::Cast(root,{Divide},InputDataType));
        //������� Scope � ������ GraphDef
        Status = root.ToGraphDef(&GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //������������� ������ ��������������

        tensorflow::SessionOptions opts;
        opts.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(GpuFraction);
        opts.config.mutable_gpu_options()->set_allow_growth(GpuGrow);

        Status = tensorflow::NewSession(opts, &SessionForTransform);

        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        //���������� ����� �������������� � ������
        Status = SessionForTransform->Create(GraphForTransform);
        if(!Status.ok())
        {
            ErCode = TfErrorCode::BAD_STATUS;
            return false;
        }
        IsTransSessCreated=true;
    }

    //���� ������ �� ����� ������, ������� ������ �������� ����������� cv::Mat
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=image.rows;
        ImgWidth=image.cols;
    }
    //������ � ������� ������������ � ������� � ������� char
    tensorflow::Tensor NewOne(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,image.rows,image.cols,image.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    //������� �� BGR � RGB
    cv::Mat input;
    image.copyTo(input);
    if(input.channels()>1 && !ImgBgr)
    {
        cv::cvtColor(image, input, CV_BGR2RGB);
    }


    //����������� ������ �� cv::Mat � ������� ������
    memcpy(const_cast<char*>(tmp_data.data()), input.data, (unsigned long)(input.rows*input.cols*input.channels())*sizeof(char));

    //������ �� ���������� ������ ������� �����������
    tensorflow::Tensor SizeTensor(tensorflow::DataType::DT_INT32, tensorflow::TensorShape({2}));
    SizeTensor.tensor<int32_t,1>()(0)=ImgHeight;
    SizeTensor.tensor<int32_t,1>()(1)=ImgWidth;

    //������ ��������������, ��������� ������� ����������� � ����� �������� �������
    Status = SessionForTransform->Run({{"input",NewOne},{"NewSize",SizeTensor}},{"out"},{},&OutputForTransform);
    if(!Status.ok())
    {
        ErCode = TfErrorCode::BAD_STATUS;
        return false;
    }
    //��������� �������� �������
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

    //�������� �� ����������� ������� ����������
    if((Divide)==0.0)
    {
        ErCode = DIVISION_BY_ZERO;
        return false;
    }

    //�������� �� ��������� ���-�� ������� �������� ���� � ����������� ������� cv::Mat
    if(ImgChannels!=image.channels())
    {
        ErCode = WRONG_IMAGE_CHANNELS_NUMBER;
        return false;
    }

    //���� ������ �� ����� ������, ������� ������ �������� �����������
    if(ImgHeight==-1 && ImgWidth==-1)
    {
        ImgHeight=image.rows;
        ImgWidth=image.cols;
    }
    cv::Mat input;
    image.copyTo(input);
    //������� �� BGR � RGB
    if(ImgChannels>1 && !ImgBgr)
    {
        cv::cvtColor(image, input, CV_BGR2RGB);
    }

    //��������� ����. �� ��� ���� ��������������
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
        ErCode=TYPE_UNSOPPORTED_FOR_CV_METH;
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

    //��������� �������
    cv::resize(input,input,cv::Size(ImgWidth,ImgHeight));

    //std::cout << input <<std::endl;
     //sleep(5);
    //���������� ����������� ����������� � ������
    tensorflow::Tensor NewOne(InputDataType, tensorflow::TensorShape({1,input.rows,input.cols,input.channels()}));
    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), input.data, size_t(input.rows*input.cols*input.channels()*tensorflow::DataTypeSize(InputDataType)));

    //��������� �������� �������
    if(!SetInputTensor(NewOne))
    {
        return false;
    }


    ErCode=OK;


    return true;
}

bool TTfSession::Run(void)
{
    //�������� �� ������� �������� �������
    if(Input.empty())
    {
        ErCode = EMPTY_INPUT_TENSOR;
        return false;
    }
    //�������� �� ������� ����� �������� ����
    if(OutputName.empty())
    {
        ErCode = EMPTY_OUTPUT_NAME;
        return false;
    }

    //������ ������
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

///��������� ���������� ����������� � ���� ������� (������, ������, ���-�� �������)
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
    if(ErCode==BAD_STATUS)
    {
        return Status.ToString();
    }

    return DebugStr[ErCode];
}

int TTfSession::GetNumClasses()
{
    return NumberOfClasses;
}

}
