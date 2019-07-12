#include "ttfsession.h"


TTfSession::TTfSession(void)
{

}

TTfSession::~TTfSession(void)
{
    UnInit();
}

void TTfSession::Init(void)
{
    Status = tensorflow::NewSession(tensorflow::SessionOptions(), &Session);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }
}

void TTfSession::UnInit(void)
{
    Status = Session->Close();
    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }
}

int TTfSession::LoadModel(const std::string &file_name, const std::string &output_name, const std::string &input_name, const int &img_height, const int &img_width)
{
    OutputName = output_name;
    InputName = input_name;
    ImgHeight = img_height;
    ImgWidght = img_width;

    Status = ReadBinaryProto(tensorflow::Env::Default(), file_name, &GraphDef);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
        return 1;
    }

    //Add the graph to the session
    Status = Session->Create(GraphDef);
    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
        return 1;
    }
    return 0;
}

void TTfSession::SetInputTensor(const tensorflow::Tensor &input_tensor)
{
   Input = { {InputName, input_tensor}, };
}

void TTfSession::SetInputImg(const std::string &image_path)
{
    std::vector<tensorflow::Tensor> resized_tensor;
    Status = ReadTensorFromImageFile(image_path, ImgHeight, ImgWidght, 0, 255, &resized_tensor);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    SetInputTensor(resized_tensor[0]);
}

void TTfSession::SetInputRawImg(unsigned char* data, int width, int height, int channels_num)
{
    auto NewOne = tensorflow::Tensor(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,height,width,channels_num}));

    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), data, height*width*channels_num*sizeof(char));

    auto root = tensorflow::Scope::NewRootScope();

    auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);

    auto float_caster = tensorflow::ops::Cast(root, a, tensorflow::DT_FLOAT);

    auto resized = tensorflow::ops::ResizeBilinear(root, float_caster,
                                                   tensorflow::ops::Const(root, {ImgHeight, ImgWidght}));

    auto substracted = tensorflow::ops::Sub(root, resized, {float(0)});

    tensorflow::ops::Div(root.WithOpName("out"), substracted, {float(255)});

    std::vector<tensorflow::Tensor> output;

    std::vector<std::pair<std::string, tensorflow::Tensor>> inputs = { {"input", NewOne}, };

    tensorflow::GraphDef graph;

    Status = root.ToGraphDef(&graph);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    std::unique_ptr<tensorflow::Session> session(tensorflow::NewSession(tensorflow::SessionOptions()));

    Status = session->Create(graph);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    Status =session->Run({inputs}, {"out"}, {}, &output);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    SetInputTensor(output[0]);

}

void TTfSession::SetInputCvMat(cv::Mat& image)
{
    cv::cvtColor(image, image, CV_BGR2RGB);

    auto NewOne = tensorflow::Tensor(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,image.rows,image.cols,image.channels()}));

    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), image.data, image.rows*image.cols*image.channels()*sizeof(char));

    auto root = tensorflow::Scope::NewRootScope();

    auto a = tensorflow::ops::Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_UINT8);

    auto float_caster = tensorflow::ops::Cast(root, a, tensorflow::DT_FLOAT);

    auto resized = tensorflow::ops::ResizeBilinear(root, float_caster,
                                                   tensorflow::ops::Const(root, {ImgHeight, ImgWidght}));

    auto substracted = tensorflow::ops::Sub(root, resized, {float(0)});

    tensorflow::ops::Div(root.WithOpName("out"), substracted, {float(255)});

    std::vector<tensorflow::Tensor> output;

    std::vector<std::pair<std::string, tensorflow::Tensor>> inputs = { {"input", NewOne}, };

    tensorflow::GraphDef graph;

    Status = root.ToGraphDef(&graph);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    std::unique_ptr<tensorflow::Session> session(tensorflow::NewSession(tensorflow::SessionOptions()));

    Status = session->Create(graph);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    Status =session->Run({inputs}, {"out"}, {}, &output);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }

    SetInputTensor(output[0]);
}

void TTfSession::SetInputCvMatNew(cv::Mat& image)
{
    cv::cvtColor(image, image, CV_BGR2RGB);

    cv::resize(image,image,cv::Size(ImgHeight,ImgWidght));

    image.convertTo(image, CV_32FC3, 1.f/255);

    //cv::normalize(image,image, 0, 1, cv::NORM_MINMAX);

    //cv::subtract(image,0,image,cv::noArray(),3);

    tensorflow::Tensor NewOne(tensorflow::DataType::DT_FLOAT, tensorflow::TensorShape({1,ImgHeight,ImgWidght,image.channels()}));

    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), image.data, image.rows*image.cols*image.channels()*sizeof(float));

    SetInputTensor(NewOne);
}

void TTfSession::Run(void)
{
    Status = Session->Run(Input, {OutputName}, {}, &Output);

    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }
}

const std::vector<tensorflow::Tensor>& TTfSession::GetOutput(void)
{
    return Output;
}

const tensorflow::Status &TTfSession::GetStatus(void)
{
    return Status;
}

const tensorflow::Tensor &TTfSession::GetInputTensor(void)
{
    return Input[0].second;
}

cv::Mat TTfSession::TensorToMat(tensorflow::Tensor& tensor)
{
    cv::Mat image(tensor.shape().dim_size(1), tensor.shape().dim_size(2), CV_32FC3);

    tensorflow::StringPiece tmp_data = tensor.tensor_data();

    memcpy(image.data, const_cast<char*>(tmp_data.data()), image.rows*image.cols*image.channels()*sizeof(float));

    cv::Mat1f temp;

    temp = image*(255.f);

    temp.convertTo(image, CV_8UC3);
    cv::cvtColor(image, image, CV_BGR2RGB);
    return image;
}
