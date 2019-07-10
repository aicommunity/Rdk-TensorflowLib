#include "ttfsession.h"

TTfSession::TTfSession(void)
{

}

TTfSession::~TTfSession(void)
{

}

void TTfSession::Init(void)
{
    Status = tensorflow::NewSession(tensorflow::SessionOptions(), &Session);

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

// Definitely not working
void TTfSession::SetInputRawImg(unsigned char* data, int width, int height, int channels_num)
{
    //Have no idea how to do it right way
    /*
    tensorflow::Tensor NewOne(tensorflow::DataType::DT_UINT8, tensorflow::TensorShape({1,height,width,channels_num}));

    tensorflow::StringPiece tmp_data = NewOne.tensor_data();

    memcpy(const_cast<char*>(tmp_data.data()), data , height*width*channels_num*sizeof(char));

    auto root = tensorflow::Scope::NewRootScope();
    
    auto float_caster = tensorflow::ops::Cast(root.WithOpName("float_caster"), NewOne, tensorflow::DT_FLOAT);

    auto resized = tensorflow::ops::ResizeBilinear(
        root, float_caster,
        tensorflow::ops::Const(root.WithOpName("size"),
        {ImgHeight, ImgWidght}));

    auto a = tensorflow::ops::Div(root.WithOpName("output_name"), tensorflow::ops::Sub(root, resized, {0}), {255});

    // This runs the GraphDef network definition that we've just constructed, and
    // returns the results in the output tensor.

    tensorflow::ClientSession session(root);
    */

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

void TTfSession::UnInit(void)
{
    Status = Session->Close();
    if (!Status.ok())
    {
        std::cout << Status.ToString() << "\n";
    }
}

const tensorflow::Tensor &TTfSession::GetInputTensor(void)
{
    return Input[0].second;
}

