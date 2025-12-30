#include "mainwindow.h"
#include <QApplication>
#include "../../../Core/Interface/ttfsession.h"


using namespace TTF;



#define StopIfBad(val) if(!val)                         \
{ std::cout << FirstExample.GetDebugStr() << std::endl; \
  return a.exec();}                                     \

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    cv::Mat img=cv::imread("/home/alexab/Try2/grace_hopper.jpg",1);

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"},},"input"));

    StopIfBad(FirstExample.InitModel("/home/alexab/Try2/inception_v3_2016_08_28_frozen.pb",0.9,true))

    StopIfBad(FirstExample.SetImgParams(0,255));

    StopIfBad(FirstExample.SetInputDataTfMeth(img));

    StopIfBad(FirstExample.Run());

    auto Result1 = FirstExample.GetOutput();

    std::string labels = "/home/alexab/Try2/imagenet_slim_labels.txt";

    //вывод в консоль вероятностей принадлежности к классу и имя классов
    //tensorflow::Status print_status = PrintTopLabels(Result1, labels);



    return a.exec();
}
