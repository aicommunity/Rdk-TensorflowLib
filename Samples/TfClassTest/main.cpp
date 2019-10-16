#include <QCoreApplication>

#include <fstream>
#include <iostream>
//#include <sys/time.h>
#include "../../Core/Interface/ttfsession.h"
#include "../../Core/Interface/label_image.h"

using namespace TTF;

#define StopIfBad(val) if(!val)                         \
{ std::cout << FirstExample.GetDebugStr() << std::endl; \
  return a.exec();}                                     \

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cv::Mat img=cv::imread("/home/vladburin/1_Folder/CV_Study/img/grace_hopper.jpg",1);

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"},},"input"));

    StopIfBad(FirstExample.InitModel("/home/vladburin/1_Folder/TF/smth/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb",0.9,true))

    StopIfBad(FirstExample.SetImgParams(0,255));

    StopIfBad(FirstExample.SetInputDataTfMeth(img));

    StopIfBad(FirstExample.Run());

    auto Result1 = FirstExample.GetOutput();

    std::string labels = "/home/vladburin/1_Folder/TF/smth/FrozenModels/inceptionv3_2016/imagenet_slim_labels.txt";

    //вывод в консоль вероятностей принадлежности к классу и имя классов
    tensorflow::Status print_status = PrintTopLabels(Result1, labels);


    return a.exec();
}
