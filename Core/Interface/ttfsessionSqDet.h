#ifndef TTFSESSIONSQDET_H
#define TTFSESSIONSQDET_H


#include "ttfsession.h"

namespace TTF
{

class TTfSessionSqDet : public TTfSession
{
public:
    TTfSessionSqDet();

    ~TTfSessionSqDet();

    /// ������� ���������� ������������.
    /// �������� ����� ������������� ������
    bool SetConfigParams(const int& anchor_height, const int& anchor_width, const int& anchor_per_grid,
                         const int& max_detection, const float& exp_thresh, const float& nm_thresh,
                         const float& prob_thresh, const int& img_width, const int& img_height,
                         const int&num_classes, const std::vector <float> anchor_seed_value);


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

protected:

    ///�������� ����� �������������
    ///������ AnchorBoxes
    bool CreatePostProcGraph();

    /// ������������� �����������
    bool FilterPredicton();

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
