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

    /*unsigned char Array[400*400*3];

    for(int i=0; i< 400*400*3; i++)
    {
        Array[i] = rand()%255;
    }

    MNISTsession.SetInputRawImg(Array,400,400,3);
    */

    std::string PathToImage = "/home/alexab/Try2/grace_hopper.jpg";

    cv::Mat img=cv::imread(PathToImage, 1);

    //MNISTsession.SetInputCvMat(img);

    MNISTsession.SetInputCvMatNew(img);

    MNISTsession.Run();

    auto Result1 = MNISTsession.GetOutput();

    string labels = "/home/alexab/Try2/imagenet_slim_labels.txt";
    tensorflow::Status print_status = PrintTopLabels(Result1, labels);

    tensorflow::Tensor A = MNISTsession.GetInputTensor();

    cv::Mat image = MNISTsession.TensorToMat(A);

    if(cv::imwrite("/home/alexab/newImage.jpg", image))
    {
        std::cout << "GJ" <<std::endl;
    }
    else
    {
        std::cout << "NO" <<std::endl;
    }
    //std::cout << A.shape() <<std::endl;

    return a.exec();
}
