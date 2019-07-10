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


#include "label_image.h"

class TTfSession
{
protected:

    std::vector<std::pair<std::string, tensorflow::Tensor>> Input;

    std::vector<tensorflow::Tensor> Output;

    tensorflow::Session* Session;

    tensorflow::Status Status;

    tensorflow::GraphDef GraphDef;

    std::string OutputName;

    std::string InputName;

    int ImgHeight;

    int ImgWidght;

public:

    TTfSession(void);

    virtual ~TTfSession(void);

    void Init(void);

    void UnInit(void);

    int LoadModel(const std::string &file_name, const std::string &output_name, const std::string &input_name,
                  const int &img_height, const int &img_width);

    void SetInputTensor(const tensorflow::Tensor &input_tensor);

    void SetInputImg(const std::string &image_path);

    void SetInputRawImg(unsigned char* data, int width, int height, int channels_num);

    void Run(void);

    const std::vector<tensorflow::Tensor>& GetOutput(void);

    const tensorflow::Tensor& GetInputTensor(void);

    const tensorflow::Status& GetStatus(void);

};

#endif // TTFSESSION_H
