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
//#include <sys/time.h>
#include "../../Core/Interface/ttfsession.h"


using namespace TTF;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TTfSession MNISTsession;

    std::string PathMNIST = "/home/alexab/Try2/TensorboardPB/frozen_inference_graph.pb";
    std::cout<<MNISTsession.GetStatus().ToString() <<std::endl;

    MNISTsession.Init();



    MNISTsession.LoadPbModel(PathMNIST, {"detection_boxes", "detection_classes", "detection_scores" ,"num_detections"}, "image_tensor");

    //MNISTsession.LoadPbModel(PathMNIST, {"InceptionV3/Predictions/Reshape_1"}, "input", 1, CHAR_TO_FLOAT_NORMALIZE);
    std::string PathToImage = "/home/alexab/Try2/Example2.jpg";

    //MNISTsession.SetResolutionParams(299,299);
    cv::Mat img=cv::imread(PathToImage, 1);

    MNISTsession.SetInputCvMatNew(img);

    MNISTsession.Run();

    auto Result1 = MNISTsession.GetOutput();

    //string labels = "/home/alexab/Try2/imagenet_slim_labels.txt";

    //tensorflow::Status print_status = PrintTopLabels(Result1, labels);

    int Num = Result1[3].scalar<float>()(0);

    auto Labels = Result1[2].matrix<float>();

    auto Boxes = Result1[0].tensor<float,3>();

    cv::Mat image = img.clone();
    std::cout<<"HelloONE "<< Num<<std::endl;
    for(int i=0; i < Num; i++)
    {
        cv::rectangle(image, {int(image.cols*(Boxes(0,i,1))), int(image.rows*(Boxes(0,i,0)))},
                             {int(image.cols*(Boxes(0,i,3))), int(image.rows*(Boxes(0,i,2)))}, cv::Scalar(0,0,0),2);
        cv::putText(image, Labels(0,i), {int(image.cols*(Boxes(0,i,1))), int(image.rows*(Boxes(0,i,0)))},);

    }

    std::cout<<"HelloTRO"<<std::endl;
    cv::imwrite("/home/alexab/new1.jpg",image);

    return a.exec();
}
