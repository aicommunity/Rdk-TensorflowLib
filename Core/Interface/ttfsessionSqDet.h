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

    /// Задание параметров конфигурации.
    /// Создание графа постобработки данных
    bool SetConfigParams(const int& anchor_height, const int& anchor_width, const int& anchor_per_grid,
                         const int& max_detection, const float& exp_thresh, const float& nm_thresh,
                         const float& prob_thresh, const int& img_width, const int& img_height,
                         const int&num_classes, const std::vector <float> anchor_seed_value);


    /// Запуск сессии, постаобработка результатов, сохранение результата в Output
    bool Run(void) override;

    /*!
     * \brief Преобразовывает cv::Mat в тензор.
     *  Используются методы OpenCV.
     *  Полученный тензор записывается в InputTensor экземпляра класса.
     * \param image входное изображение в формате cv::Mat
     * В отличие от родителя, здесь вызывается метод SetImgParams
     */
    bool SetInputDataCvMeth(cv::Mat& image) override;

protected:

    ///Создание графа постобработки
    ///Расчёт AnchorBoxes
    bool CreatePostProcGraph();

    /// Постобработка результатов
    bool FilterPredicton();

    ///Сессия для постобработки
    tensorflow::Session* SessionForPostProc;

    ///Нужно для постобработки боксов
    tensorflow::Tensor AnchorBoxes;

    int AnchorHeight=0;

    int AnchorWidth=0;

    int Anchors=0;

    int AnchorPerGrid=0;

    std::vector <float> AnchorSeedValues={};

    /// Максимальное желаемое кол-во обнаружений
    int MaxDetections=0;

    float ExpThresh=0;

    ///Порог для подавения немаксимумов
    float NMthreshold=0;

    ///Порог по вероятности
    float ProbThreshold=0;




};

}
#endif // TTFSESSIONSQDET_H
