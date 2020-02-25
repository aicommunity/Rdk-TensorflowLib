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

    /// Задание параметров конфигурации.
    /// Создание графа постобработки данных
    bool SetConfigParams(const std::string& config_path);


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

    bool SetConfigPath(const std::string& path);

    bool InitModel(const std::string &file_name, const double &gpu_fraction, const int& device_number=0) override;
protected:

    ///Создание графа постобработки
    ///Расчёт AnchorBoxes
    bool CreatePostProcGraph();

    /// Постобработка результатов
    bool FilterPredicton();

    std::string ConfigPath="";

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
