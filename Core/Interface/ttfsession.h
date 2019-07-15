#ifndef TTFSESSION_H
#define TTFSESSION_H

#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/core/protobuf/meta_graph.pb.h"

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

#include "label_image.h"


enum ExtraChange
{
    NO_NEED,
    CAST_CHAR_TO_FLOAT,
    NORMALIZE,
};

class TTfSession
{
protected:

    std::vector<std::pair<std::string, tensorflow::Tensor>> Input;

    std::vector<tensorflow::Tensor> Output;

    tensorflow::Session* Session;

    tensorflow::Status Status;

    tensorflow::GraphDef GraphDef;

    tensorflow::MetaGraphDef MetaGraphDef;

    std::vector<std::string> OutputName;

    std::string InputName;

    int ImgHeight;

    int ImgWidght;

public:

    TTfSession(void);

    virtual ~TTfSession(void);

    ///Инициализация сессии
    void Init(void);

    ///Деинициализация сессии
    void UnInit(void);

    ///Загрузка замороженной модели графа в сессию с указанием размера входного изображения
    int LoadPbModel(const std::string &file_name, const std::vector<std::string> &output_name, const std::string &input_name,
                  const int &img_height, const int &img_width, const bool &is_resize_needed=0, ExtraChange=NO_NEED);

    ///Загрузка замороженной модели графа в сессию без указания размера входного изображения
    int LoadPbModel(const std::string &file_name, const std::vector<std::string> &output_name, const std::string &input_name);

    ///Загрузка модели из чекпоинта в сесиию
    int LoadCkptModel(const std::string &path_to_meta, const std::string &path_to_ckpt,
                      const std::vector<std::string>  &output_name, const std::string &input_name,
                      const int &img_height, const int &img_width);

    ///Изменение разрешения
    void ChangeResolution(const int &img_height, const int &img_width);

    ///Загрузка входного тенсора
    void SetInputTensor(const tensorflow::Tensor &input_tensor);

    ///Загрузка входного тенсора из изображения
    void SetInputImg(const std::string &image_path);

    ///Загрузка входного тенсора из объекта Mat, используя tensorflow
    void SetInputCvMat(cv::Mat& image);

    ///Загрузка входного тенсора из объекта Mat, используя opencv
    void SetInputCvMatNew(cv::Mat& image);

    ///Загрузка входного тенсора из сырых данных
    void SetInputRawImg(unsigned char* data, int width, int height, int channels_num);

    ///Преобразование тенсора в Mat
    cv::Mat TensorToMat(tensorflow::Tensor& tensor);

    ///Запуск сессии, сохранение результата в Output
    void Run(void);

    const std::vector<tensorflow::Tensor>& GetOutput(void);

    const tensorflow::Tensor& GetInputTensor(void);

    const tensorflow::Status& GetStatus(void);

};


#endif // TTFSESSION_H
