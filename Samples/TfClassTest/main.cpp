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

    std::string PathMNIST = "/home/alexab/Try2/TensorboardPB/frozen_inference_graph.pb";

    MNISTsession.Init();

    MNISTsession.LoadPbModel(PathMNIST, {"detection_boxes", "detection_scores", "detection_classes", "num_detections"}, "image_tensor");

    std::string PathToImage = "/home/alexab/Try2/Example4.jpeg";

    cv::Mat img=cv::imread(PathToImage, 1);

    MNISTsession.SetInputCvMat(img);

    MNISTsession.Run();

    auto Result1 = MNISTsession.GetOutput();

    std::cout <<  Result1[0].shape() << std::endl;
    std::cout <<  Result1[1].shape() << std::endl;
    std::cout <<  Result1[2].shape() << std::endl;
    std::cout <<  Result1[3].shape() << std::endl;

    std::cout <<  Result1[0].tensor<float,3>() << std::endl;
    std::cout <<  Result1[1].tensor<float,2>() << std::endl;
    std::cout <<  Result1[2].tensor<float,2>() << std::endl;
    std::cout <<  Result1[3].scalar<float>() << std::endl;

    auto DataToRead = Result1[1].matrix<float>();

    auto NumberOfClasses = Result1[3].scalar<float>();

    int Num = float(NumberOfClasses(0));

    auto Boxes = Result1[0].tensor<float,3>();

    float ksd = Result1[1].matrix<float>()(0,3);

    tensorflow::Tensor A = MNISTsession.GetInputTensor();

    std::cout << A.shape() << std::endl;
    std::cout << Num <<std::endl;
    std::cout << DataToRead(0,3) << std::endl;

    cv::Mat image = MNISTsession.TensorToMat(A);

    int x0= int(image.cols*(Boxes(0,16,1)));
    int y0 = int(image.rows*(Boxes(0,16,0)));
    int x1= int(image.cols*(Boxes(0,16,3)));
    int y1 = int(image.rows*(Boxes(0,16,2)));

    std::cout <<image.rows <<std::endl;
    std::cout <<image.cols <<std::endl;


    for(int i=0; i < Num; i++)
    {
        cv::rectangle(image, {int(image.cols*(Boxes(0,i,1))), int(image.rows*(Boxes(0,i,0)))},
                             {int(image.cols*(Boxes(0,i,3))), int(image.rows*(Boxes(0,i,2)))}, cv::Scalar(0,0,0),2);
    }

    cv::imwrite("/home/alexab/new.jpg",image);

    return a.exec();
}
