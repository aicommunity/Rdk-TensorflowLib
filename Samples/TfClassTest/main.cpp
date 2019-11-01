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

    cv::Mat img=cv::imread("/home/vladburin/1_Folder/TF/smth/trash/grace_hopper.jpg",1);

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"}},"input"));

    StopIfBad(FirstExample.InitModel("/home/vladburin/1_Folder/TF/smth/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb",0.5,true))

    StopIfBad(FirstExample.SetImgParams({0,0,0},255));

    StopIfBad(FirstExample.SetInputDataTfMeth(img));

    StopIfBad(FirstExample.Run());

    auto Result1 = FirstExample.GetOutput();

    std::string labels = "/home/vladburin/1_Folder/TF/smth/FrozenModels/inceptionv3_2016/imagenet_slim_labels.txt";



    PrintTopLabels(Result1, labels);
    std::cout << "\n"<<std::endl;


    std::cout << FirstExample.GetOutput()[0].dim_size(1)<<std::endl;


    return a.exec();
    //return 0;
}
