#ifndef TTFSESSIONSQDET_H
#define TTFSESSIONSQDET_H


#include "ttfsession.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace TTF
{

class TTfSessionSqDet : public TTfSession
{
public:
    TTfSessionSqDet();

    ~TTfSessionSqDet();

    /// ������� ���������� ������������.
    /// �������� ����� ������������� ������
    bool SetConfigParams(const std::string& config_path);


    /// ������ ������, �������������� �����������, ���������� ���������� � Output
    bool Run(void) override;

    /*!
     * \brief ��������������� cv::Mat � ������.
     *  ������������ ������ OpenCV.
     *  ���������� ������ ������������ � InputTensor ���������� ������.
     * \param image ������� ����������� � ������� cv::Mat
     * � ������� �� ��������, ����� ���������� ����� SetImgParams
     */
    bool SetInputDataCvMeth(cv::Mat& image) override;

    bool SetConfigPath(const std::string& path);

    bool InitModel(const std::string &file_name, const double &gpu_fraction, const int& device_number=0) override;
protected:

    ///�������� ����� �������������
    ///������ AnchorBoxes
    bool CreatePostProcGraph();

    /// ������������� �����������
    bool FilterPredicton();

    std::string ConfigPath="";

    ///������ ��� �������������
    tensorflow::Session* SessionForPostProc;

    ///����� ��� ������������� ������
    tensorflow::Tensor AnchorBoxes;

    int AnchorHeight=0;

    int AnchorWidth=0;

    int Anchors=0;

    int AnchorPerGrid=0;

    std::vector <float> AnchorSeedValues={};

    /// ������������ �������� ���-�� �����������
    int MaxDetections=0;

    float ExpThresh=0;

    ///����� ��� ��������� ������������
    float NMthreshold=0;

    ///����� �� �����������
    float ProbThreshold=0;




};

}
#endif // TTFSESSIONSQDET_H
