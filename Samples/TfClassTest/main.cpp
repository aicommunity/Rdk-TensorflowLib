#include <QCoreApplication>
#include <QDir>

#include <fstream>
#include <iostream>
//#include <sys/time.h>
#include "../../Core/Interface/ttfsession.h"
#include "../../Core/Interface/label_image.h"


using namespace TTF;

#define StopIfBad(val) if(!val)                         \
{ std::cout << FirstExample.GetDebugStr() << std::endl; \
  return 0;}                                     \


int SimpleInception();

int DetectFrames(QString str, std::string path, std::string savefolder);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::vector<std::string> Images;
    QDir dir("/home/vladburin/1_Folder/TF/smth/trash/CheckSlassifier/eleron_val/cars");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString line = fileInfo.fileName();
        Images.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
    }

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"dense_2/Softmax",}},"vgg16_input"));

    StopIfBad(FirstExample.InitModel("/home/vladburin/PycharmProjects/TF_test/person_classifier_1.pb",0.5,true))

    StopIfBad(FirstExample.SetImgParams({0,0,0},255,false));

    for(int i=0;i<Images.size();i++)
    {
        cv::Mat img=cv::imread(Images[i],1);
        StopIfBad(FirstExample.SetInputDataCvMeth(img));
        StopIfBad(FirstExample.Run());
        auto Result1 = FirstExample.GetOutput()[0];
        std::cout << i << std::endl;

        std::cout << Result1.matrix<float>() << "\n" <<std::endl;

    }






    return a.exec();

}

int DetectFrames(QString str, std::string path, std::string savefolder)
{

    std::vector<std::string> Images;
    QDir dir(str);
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString line = fileInfo.fileName();
        Images.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
    }

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"detection_boxes", "detection_classes", "detection_scores" ,"num_detections"}}, "image_tensor"));

    StopIfBad(FirstExample.InitModel(path,0.5,true))

    StopIfBad(FirstExample.SetImgParams({0,0,0},1));

    for(int i=0;i<Images.size();i++)
    {
        cv::Mat img=cv::imread(Images[i],1);

        StopIfBad(FirstExample.SetInputDataTfMeth(img));

        StopIfBad(FirstExample.Run());

        auto Result1 = FirstExample.GetOutput();

        //один из выходных тензоров хранит кол-во распознанных объектов
        int Num = Result1[3].scalar<float>()(0);
        //другой хранит номера классов распознанных объектов
        auto Classes = Result1[1].matrix<float>();
        //другой хранит вероятности принадлежности к классам
        auto Labels = Result1[2].matrix<float>();
        //другой хранит координаты боксов
        auto Boxes = Result1[0].tensor<float,3>();
        //вывод на исходное изображение боксов, номеров классов и вероятностей
        cv::Mat image = img.clone();

        cv::cvtColor(image, image, CV_BGR2RGB);

        for(int i=0; i < Num; i++)
        {
            cv::rectangle(image, {int(image.cols*(Boxes(0,i,1))), int(image.rows*(Boxes(0,i,0)))},
                                {int(image.cols*(Boxes(0,i,3))), int(image.rows*(Boxes(0,i,2)))}, cv::Scalar(0,0,0),2);

           cv::putText(image, std::to_string(Labels(0,i)), {int(image.cols*(Boxes(0,i,1))), int(image.rows*(Boxes(0,i,0)))}, 0, 1, {0,0,255},2);
           cv::putText(image, std::to_string(Classes(0,i)), {int(image.cols*(Boxes(0,i,1))), int(image.rows*(Boxes(0,i,0)))+30}, 0, 1, {0,0,255},2);
        }

        cv::imwrite("/home/vladburin/"+savefolder+"/detect_"+std::to_string(i) +".jpeg",image);
    }

    std::cout << "done" <<std::endl;
}

int SimpleInception()
{

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

}
