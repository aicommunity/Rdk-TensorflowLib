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

#include "../../Core/Interface/ttfsession.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TTfSession MNISTsession;

    std::string PathMNIST = "/home/alexab/Try2/inception_v3_2016_08_28_frozen.pb";

    MNISTsession.Init();

    MNISTsession.LoadModel(PathMNIST, "InceptionV3/Predictions/Reshape_1", "input", 299, 299);

    std::string PathToImage = "/home/alexab/Try2/grace_hopper.jpg";

    cv::Mat img=cv::imread(PathToImage, 1);

    MNISTsession.SetInputCvMat(img);

    //MNISTsession.SetInputCvMatNew(img);

    MNISTsession.Run();

    auto Result1 = MNISTsession.GetOutput();

    string labels = "/home/alexab/Try2/imagenet_slim_labels.txt";

    tensorflow::Status print_status = PrintTopLabels(Result1, labels);

    tensorflow::Tensor A = MNISTsession.GetInputTensor();

    cv::Mat image = MNISTsession.TensorToMat(A);

    if(cv::imwrite("/home/alexab/newImage1.jpg", image))
    {
        std::cout << "GJ" <<std::endl;
    }
    else
    {
        std::cout << "NO" <<std::endl;
    }
    std::cout << A.shape().dim_size(2) <<std::endl;

    //изображение с помощью тенсорфлоу
    std::string PathToImage1 = "/home/alexab/newImage1.jpg";

    cv::Mat img1=cv::imread(PathToImage1, 1);

    //с помощью опенсиви
    std::string PathToImage0 = "/home/alexab/newImage.jpg";

    cv::Mat img0=cv::imread(PathToImage0, 1);

    cv::Mat img3 = img1 - img0;

    cv::imwrite("/home/alexab/substract.jpg", img3);

    return a.exec();
}
