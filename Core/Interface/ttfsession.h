#ifndef TTFSESSION_H
#define TTFSESSION_H

//#include "tensorflow/cc/ops/standard_ops.h"
//#include "tensorflow/core/framework/graph.pb.h"
//#include "tensorflow/core/framework/tensor.h"
//#include "tensorflow/core/graph/default_device.h"
//#include "tensorflow/core/graph/graph_def_builder.h"
//#include "tensorflow/core/lib/core/threadpool.h"
//#include "tensorflow/core/lib/strings/str_util.h"
//#include "tensorflow/core/lib/strings/stringprintf.h"
//#include "tensorflow/core/platform/init_main.h"
//#include "tensorflow/core/platform/logging.h"
//#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
//#include "tensorflow/cc/client/client_session.h"
//#include "tensorflow/cc/framework/gradients.h"
//#include "tensorflow/core/kernels/training_ops.h"
#include "tensorflow/core/protobuf/meta_graph.pb.h"
#include "tensorflow/cc/ops/standard_ops.h"

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

#include "../../../../Rdk/Deploy/Include/rdk.h"




namespace TTF
{

///������������ ����� ������
enum TfErrorCode{
    OK,
    BAD_STATUS,
    EMPTY_INPUT_NAME,
    INPUT_NODE_DOESNT_EXIST_IN_GRAPH,
    EMPTY_INPUT_TENSOR,
    DIVISION_BY_ZERO,
    EMPTY_OUTPUT_NAME,
    OUTPUT_NODE_DOESNT_EXIST_IN_GRAPH,
    WRONG_IMAGE_CHANNELS_NUMBER,
    TYPE_UNSOPPORTED_FOR_CV_METH,
    COPY_DATA_FROM_NULL_PTR,
    EXCEPTION,
    BAD_CONFIG
};

/*!
 * \brief ����� ��� ������������� ������ ��������� TF
 */
class TTfSession
{
protected:

    ///��������� ���� ������
    TfErrorCode ErCode=OK;

    ///������ ��� ����������
    std::string ExceptionString;

    ///������ ������� ����� � �� ����
    std::vector<std::pair<std::string, tensorflow::Tensor>> Input;

    ///������ �������� �����
    std::vector<tensorflow::Tensor> Output;

    ///��������� �� ������ ��� ������� ������
    tensorflow::Session* Session;

    ///������
    tensorflow::Status Status=Status.OK();

    ///�����, ��� �������� ��� ����
    tensorflow::GraphDef GraphDef;

    ///�������� �������� �����
    std::vector<std::string> OutputName;

    ///��� �������� ����
    std::string InputName="";

    ///�������� �������� ������� (����� ��� ������������)
    float Divide=1;

    ///���������� �� �������� ������� (����� ��� ������������)
    std::vector <float> Substract={0,0,0};

    ///��� ������ �������� �������
    tensorflow::DataType InputDataType;

    ///������ �������� ����������� (�������)
    int ImgHeight=-1;

    ///������ �������� ����������� (�������)
    int ImgWidth=-1;

    ///���-�� ������� �������� ����������� (�������)
    int ImgChannels=0;

    ///� ������ 3-�� ��������� �����������, �������� �� ������ ����� (BGR-true, RGB-false)
    bool ImgBgr=false;

    ///���-�� ������� � ������ ��������������
    int NumberOfClasses=0;

    ///GPU
    bool GpuGrow=false;

    double GpuFraction=0.5;
    ///����, ����������� ���� �� ������� ������ ��� �������������
    bool IsTransSessCreated = false;

    ///��������� �� ������ ��� �������������� �������� �������
    tensorflow::Session* SessionForTransform;

    ///�������� ������ ��� ������������� �������� �������
    std::vector<tensorflow::Tensor> OutputForTransform;

    ///���� ��� ������������� �������� �������
    tensorflow::GraphDef GraphForTransform;

    ///����� ������� ��������� ���������� �������� ����������� (�������)
    ///����� ��������� ������� �������� �������� � �������� ����� � �����
    bool CheckInOutNodes();

    /*!
     * \brief �������������� ����� ������������� ������.
     * \param gpu_fraction ���� ������������� ������ GPU
     * \param allow_gpu_grow �������� �� ��� ������ �����, ���� �� ���� ������������� (true->�������� ����������)
     */
    bool InitSession(const double &gpu_fraction, const bool& allow_gpu_grow);

    ///������ �������� ������ � ������� �����
    const std::vector<std::string> DebugStr={
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

public:

    TTfSession(void);

    virtual ~TTfSession(void);

    /*!
     * \brief ������������� ������.
     * \brief �������� ������������ PB ������ ����� � ������
     * \brief ������ ������ ���� ��� ������������� CUDA
     * \param file_name ���� � pb ������ �����
     * \param gpu_fraction ���� ������������� ������ GPU
     * \param allow_gpu_grow �������� �� ��� ������ �����, ���� �� ���� ������������� (true->�������� ����������)
     */
    virtual bool InitModel(const std::string &file_name, const double &gpu_fraction, const bool& allow_gpu_grow=false, const int& device_number=0);

    /*!
     * \brief ��������������� ������.
     * ����������� ������ ��� ��������� ������ ���������
     */
    bool UnInit(void);

    /*!
     * \brief ������� ���� �������� � ��������� ����
     * \param output_name ������ ��� ������� �����
     * \param input_name ��� �������� ����
     */
    bool SetGraphParams(const std::vector<std::string> &output_name, const std::string &input_name);

    /*!
     * \brief �������� �������� ������� � ������������� ������� ������
     * \param input_tensor ����������� ������
     */
    bool SetInputTensor(const tensorflow::Tensor &input_tensor);

    /*!
     * \brief ������� ���������� �������� �������.
     * \param div �������� �������� �������
     * \param sub ���������� �� �������� �������
     */
    bool SetImgParams(const std::vector<float> & sub={0,0,0}, const float & div=1, const bool& bgr=0);

    /*!
     * \brief ��������������� cv::Mat � ������.
     * ������������ ������ TF.
     * ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� cv::Mat
     */
    bool SetInputDataTfMeth(cv::Mat& image);

    /*!
     * \brief ��������������� RDK::UBitmap � ������.
     * ������������ ������ TF.
     * ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� RDK::UBitmap&
     */
    bool SetInputDataTfMeth(RDK::UBitmap& image);

    /*!
     * \brief ��������������� cv::Mat � ������.
     *  ������������ ������ OpenCV.
     *  ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� cv::Mat
     */
    virtual bool SetInputDataCvMeth(cv::Mat& image);

    /*!
     * \brief ��������������� RDK::UBitmap � ������.
     *  ������������ ������ OpenCV.
     *  ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� RDK::UBitmap
     */
    bool SetInputDataCvMeth(RDK::UBitmap& image);

    ///������ ������, ���������� ���������� � Output ��������� ������
    virtual bool Run(void);

    ///��������� ����������
    const std::vector<tensorflow::Tensor>& GetOutput(void);

    ///��������� �������� �������
    const tensorflow::Tensor& GetInputTensor(void);

    ///��������� ������ � ������� ��������� ���������� ������
    const std::string GetDebugStr(void);

    ///��������� ���������� ����������� � ���� ������� (������, ������, ���-�� �������)
    std::vector<int> GetImgParams(void);

    ///��������� ���-�� ������� ��������������
    int GetNumClasses();
};

}
#endif // TTFSESSION_H
