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
    CV_MAT_HAS_WRONG_CHANNELS_NUMBER,
    TYPE_UNSOPPORTED_FOR_CV_METH,
    COPY_DATA_FROM_NULL_PTR
};

/*!
 * \brief ����� ��� ������������� ������ ��������� TF
 */
class TTfSession
{
protected:

    ///��������� ���� ������
    TfErrorCode ErCode=OK;

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

    ///�����, ��� �������� ���� ����
    tensorflow::MetaGraphDef MetaGraphDef;

    ///�������� �������� �����
    std::vector<std::string> OutputName;

    ///��� �������� ����
    std::string InputName="";

    ///�������� �������� ������� (����� ��� ������������)
    float Divide=0;

    ///���������� �� �������� ������� (����� ��� ������������)
    float Substract=0;

    ///��� ������ �������� �������
    tensorflow::DataType InputDataType;

    ///������ �������� ����������� (�������)
    int ImgHeight=-1;

    ///������ �������� ����������� (�������)
    int ImgWidth=-1;

    ///���-�� ������� �������� ����������� (�������)
    int ImgChan=0;

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
        "Node wasn't found in the Graph",
        "Empty input tensor. Set it with SetInputData",
        "Division by zero. Set image parameters with SetImgParams",
        "Set output node name with SetGraphParams",
        "Some of output nodes wasn't found in the graph",
        "Number of channels in input cv::mat doesn't equal to input tensor channel number",
        "This data type are not supported for SetInputDataCvMeth. Use SetInputDataTfMeth"
    };

public:

    TTfSession(void);

    virtual ~TTfSession(void);

    /*!
     * \brief ������������� ������.
     * \brief �������� ������������ PB ������ ����� � ������
     * \param file_name ���� � pb ������ �����
     * \param gpu_fraction ���� ������������� ������ GPU
     * \param allow_gpu_grow �������� �� ��� ������ �����, ���� �� ���� ������������� (true->�������� ����������)
     */
    bool InitModel(const std::string &file_name, const double &gpu_fraction, const bool& allow_gpu_grow=false);

    /*!
     * \brief ������������� ������.
     * \brief �������� ������ �� ��������� � ������
     * \param path_to_meta ���� � meta ����� ����� (��� �������� ������ �����)
     * \param path_to_ckpt ���� � ������ ckpt ������� (��� �������� �������� ����������)
     * \param gpu_fraction ���� ������������� ������ GPU
     * \param allow_gpu_grow �������� �� ��� ������ �����, ���� �� ���� ������������� (true->�������� ����������)
     */
    bool InitModel(const std::string &path_to_meta, const std::string &path_to_ckpt,
                   const double &gpu_fraction, const bool& allow_gpu_grow=false);

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
    bool SetImgParams(const float & sub=0, const float & div=1);

    /*!
     * \brief ��������������� cv::Mat � ������.
     * ������������ ������ TF.
     * ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� cv::Mat
     */
    bool SetInputDataTfMeth(cv::Mat& image);

    /*!
     * \brief ��������������� ����� ������ ����������� � ������.
     * ������������ ������ TF.
     * ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� cv::Mat
     */
    bool SetInputDataTfMeth(const RDK::UBitmap& image);

    /*!
     * \brief ��������������� cv::Mat � ������.
     *  ������������ ������ OpenCV.
     *  ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� cv::Mat
     */
    bool SetInputDataCvMeth(cv::Mat& image);



    ///������ ������, ���������� ���������� � Output ��������� ������
    bool Run(void);

    ///��������� ����������
    const std::vector<tensorflow::Tensor>& GetOutput(void);

    ///��������� �������� �������
    const tensorflow::Tensor& GetInputTensor(void);

    ///��������� ������ � ������� ��������� ���������� ������
    const std::string GetDebugStr(void);
};

}
#endif // TTFSESSION_H
