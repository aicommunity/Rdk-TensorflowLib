#include <QCoreApplication>

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

#include <fstream>
#include <iostream>

#include "ttfsession.h"


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    TTfSession MNISTsession;

    std::string PathMNIST = "/home/alexab/Try2/inception_v3_2016_08_28_frozen.pb";

    MNISTsession.Init();

    MNISTsession.LoadModel(PathMNIST, "InceptionV3/Predictions/Reshape_1", "input", 299, 299);

    /*std::vector<tensorflow::Tensor> resized_tensors;
    std::string ImagePath = "/home/alexab/Try2/cat.jpg";

    tensorflow::Status read_tensor_status = ReadTensorFromImageFile(ImagePath, 299, 299, 0, 255, &resized_tensors);


    if (!read_tensor_status.ok())
    {
      LOG(ERROR) << read_tensor_status;
      return -1;
    }
    */

    MNISTsession.SetInputImg("/home/alexab/Try2/cat.jpg");

    MNISTsession.Run();

    auto Result1 = MNISTsession.GetOutput();

    string labels = "/home/alexab/Try2/imagenet_slim_labels.txt";

    tensorflow::Status print_status = PrintTopLabels(Result1, labels);

    tensorflow::Tensor A = MNISTsession.GetInputTensor();

    std::cout << A.tensor<float, 4>().data()[5] <<std::endl;

    return a.exec();
}
