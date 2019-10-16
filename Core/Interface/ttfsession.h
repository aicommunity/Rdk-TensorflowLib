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

///Перечисление кодов ошибок
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
 * \brief Класс для использования модели нейросети TF
 */
class TTfSession
{
protected:

    ///Состояние кода ошибки
    TfErrorCode ErCode=OK;

    ///Вектор входных узлов и их имен
    std::vector<std::pair<std::string, tensorflow::Tensor>> Input;

    ///Вектор выходных узлов
    std::vector<tensorflow::Tensor> Output;

    ///Указатель на сессию для запуска модели
    tensorflow::Session* Session;

    ///Статус
    tensorflow::Status Status=Status.OK();

    ///Класс, где хранится сам граф
    tensorflow::GraphDef GraphDef;

    ///Класс, где хранится мета граф
    tensorflow::MetaGraphDef MetaGraphDef;

    ///Названия выходных узлов
    std::vector<std::string> OutputName;

    ///Имя входного узла
    std::string InputName="";

    ///Делитель входного тензора (нужен для нормализации)
    float Divide=0;

    ///Вычитаемое из входного тензора (нужен для нормализации)
    float Substract=0;

    ///Тип данных входного тензора
    tensorflow::DataType InputDataType;

    ///Высота входного изображения (тензора)
    int ImgHeight=-1;

    ///Ширина входного изображения (тензора)
    int ImgWidth=-1;

    ///Кол-во каналов входного изображения (тензора)
    int ImgChan=0;

    ///Флаг, указывающий была ли сделана сессия для трансформации
    bool IsTransSessCreated = false;

    ///Указатель на сессию для преобразования входного тензора
    tensorflow::Session* SessionForTransform;

    ///Выходной вектор для преобразовани входного тензора
    std::vector<tensorflow::Tensor> OutputForTransform;

    ///Граф для преобрзования входного тензора
    tensorflow::GraphDef GraphForTransform;

    ///Метод задания начальных параметров входного изображения (тензора)
    ///Также проверяет наличие заданных входного и выходных узлов в графе
    bool CheckInOutNodes();

    /*!
     * \brief Дополнительный метод инициализации сессии.
     * \param gpu_fraction доля использования памяти GPU
     * \param allow_gpu_grow выделять ли всю память сразу, либо по мере необходимости (true->выделять постепенно)
     */
    bool InitSession(const double &gpu_fraction, const bool& allow_gpu_grow);

    ///Вектор описания ошибок в формате строк
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
     * \brief Инициализация сессии.
     * \brief Загрузка замороженной PB модели графа в сессию
     * \param file_name путь к pb модели графа
     * \param gpu_fraction доля использования памяти GPU
     * \param allow_gpu_grow выделять ли всю память сразу, либо по мере необходимости (true->выделять постепенно)
     */
    bool InitModel(const std::string &file_name, const double &gpu_fraction, const bool& allow_gpu_grow=false);

    /*!
     * \brief Инициализация сессии.
     * \brief Загрузка модели из чекпоинта в сесиию
     * \param path_to_meta путь к meta файлу графа (там хранится модель графа)
     * \param path_to_ckpt путь к файлам ckpt формата (там хранятся значения переменных)
     * \param gpu_fraction доля использования памяти GPU
     * \param allow_gpu_grow выделять ли всю память сразу, либо по мере необходимости (true->выделять постепенно)
     */
    bool InitModel(const std::string &path_to_meta, const std::string &path_to_ckpt,
                   const double &gpu_fraction, const bool& allow_gpu_grow=false);

    /*!
     * \brief Деинициализация сессии.
     * Обязательно делать при изменении модели нейросети
     */
    bool UnInit(void);

    /*!
     * \brief Задание имен входного и выходного узла
     * \param output_name вектор имён входных узлов
     * \param input_name имя входного узла
     */
    bool SetGraphParams(const std::vector<std::string> &output_name, const std::string &input_name);

    /*!
     * \brief Загрузка входного тензора в соответсующий атрибут класса
     * \param input_tensor загружаемый тензор
     */
    bool SetInputTensor(const tensorflow::Tensor &input_tensor);

    /*!
     * \brief Задание параметров входного тензора.
     * \param div делитель входного тензора
     * \param sub вычитаемое из входного тензора
     */
    bool SetImgParams(const float & sub=0, const float & div=1);

    /*!
     * \brief Преобразовывает cv::Mat в тензор.
     * Используются методы TF.
     * Полученный тензор записывается в InputTensor экземпляра класса.
     * \param image входное изображение в формате cv::Mat
     */
    bool SetInputDataTfMeth(cv::Mat& image);

    /*!
     * \brief Преобразовывает сырые данные изобрежения в тензор.
     * Используются методы TF.
     * Полученный тензор записывается в InputTensor экземпляра класса.
     * \param image входное изображение в формате cv::Mat
     */
    bool SetInputDataTfMeth(const RDK::UBitmap& image);

    /*!
     * \brief Преобразовывает cv::Mat в тензор.
     *  Используются методы OpenCV.
     *  Полученный тензор записывается в InputTensor экземпляра класса.
     * \param image входное изображение в формате cv::Mat
     */
    bool SetInputDataCvMeth(cv::Mat& image);



    ///Запуск сессии, сохранение результата в Output экземляра класса
    bool Run(void);

    ///Получение результата
    const std::vector<tensorflow::Tensor>& GetOutput(void);

    ///Получение входного тензора
    const tensorflow::Tensor& GetInputTensor(void);

    ///Получение строки о текущем состоянии экземпляра класса
    const std::string GetDebugStr(void);
};

}
#endif // TTFSESSION_H
