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
  return 0;}
\
int CheckAccuracy();

int SimpleInception();

int DetectFrames(QString str, std::string path, std::string savefolder);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
/*
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

    StopIfBad(FirstExample.InitModel("/home/user/Rtv-VideoAnalytics/Bin/Configs/DemoNN/AggrClassificationEleronTF/abz_classifier.pb",0.5,true));

    StopIfBad(FirstExample.SetImgParams({0,0,0},255,false));

    cv::Mat img=cv::imread("/home/user/Videos/abz_3/aggr_classifier_results/2/False/49407_8596_26537_0.512951.jpg",1);
    StopIfBad(FirstExample.SetInputDataCvMeth(img));
    StopIfBad(FirstExample.Run());
    auto Result1 = FirstExample.GetOutput()[0];


    std::cout << Result1.matrix<float>() << "\n" <<std::endl;


    for(int i=0;i<Images.size();i++)
    {
        cv::Mat img=cv::imread(Images[i],1);
        StopIfBad(FirstExample.SetInputDataCvMeth(img));
        StopIfBad(FirstExample.Run());
        auto Result1 = FirstExample.GetOutput()[0];
        std::cout << i << std::endl;

        std::cout << Result1.matrix<float>() << "\n" <<std::endl;

    }
*/






    SimpleInception();

/*
    clock_t end_frame = clock();

    ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;


    std::string img_path = Environment->GetCurrentDataDir()+"classification_time";
    std::ofstream CheckTime;
    CheckTime.open(img_path, std::ios::app);
    CheckTime << ClassificationTime << "\n";
    CheckTime.close();
*/

    return a.exec();

}

int CheckAccuracy()
{
    std::vector<std::string> ImagesBackground;
    QDir dir("/home/user/eleron_validation/0");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString line = fileInfo.fileName();
        ImagesBackground.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
    }

    std::vector<std::string> ImagesPeople;
    dir=("/home/user/eleron_validation/1");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString line = fileInfo.fileName();
        ImagesPeople.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
    }

    std::vector<std::string> ImagesCars;
    dir=("/home/user/eleron_validation/2");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString line = fileInfo.fileName();
        ImagesCars.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
    }

    std::vector<std::string> ImagesGroup;
    dir=("/home/user/eleron_validation/3");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString line = fileInfo.fileName();
        ImagesGroup.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
    }

    TTfSession FirstExample;
    /*
    StopIfBad(FirstExample.SetGraphParams({{"dense_2/Softmax",}},"vgg16_input"));

    StopIfBad(FirstExample.InitModel("/home/user/Eleron_new.pb",0.5,true));
    */
    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"}},"input"));

    StopIfBad(FirstExample.InitModel("/home/user/TF/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb",0.5,true))

    StopIfBad(FirstExample.SetImgParams({0,0,0},255,false));

    int FalseBackground=0;
    int FalsePeople=0;
    int FalseCars=0;
    int FalseGroup=0;

    for(int i=0;i<ImagesBackground.size();i++)
    {
        std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesBackground[i],1);

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        auto Result1 = FirstExample.GetOutput()[0];

        int max_id = -1;
        double max_conf = -100;
        //std::cout << Result1.matrix<float>()<< std::endl;
        for(int k=0; k<FirstExample.GetOutput()[0].dim_size(1); k++)
        {

            if(Result1.matrix<float>()(0,k)>max_conf)
            {
                max_conf = Result1.matrix<float>()(0,k);
                max_id = k;
            }
        }

        if(max_id!=0)
        {
            FalseBackground++;
        }

    }

    for(int i=0;i<ImagesPeople.size();i++)
    {
        std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesPeople[i],1);

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        auto Result1 = FirstExample.GetOutput()[0];
        //std::cout << Result1.matrix<float>()<< std::endl;
        int max_id = -1;
        double max_conf = -100;

        for(int k=0; k<FirstExample.GetOutput()[0].dim_size(1); k++)
        {

            if(Result1.matrix<float>()(0,k)>max_conf)
            {
                max_conf = Result1.matrix<float>()(0,k);
                max_id = k;
            }
        }

        if(max_id!=1)
        {
            FalsePeople++;
        }

    }

    for(int i=0;i<ImagesCars.size();i++)
    {
        std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesCars[i],1);

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        auto Result1 = FirstExample.GetOutput()[0];

        int max_id = -1;
        double max_conf = -100;

        for(int k=0; k<FirstExample.GetOutput()[0].dim_size(1); k++)
        {

            if(Result1.matrix<float>()(0,k)>max_conf)
            {
                max_conf = Result1.matrix<float>()(0,k);
                max_id = k;
            }
        }

        if(max_id!=2)
        {
            FalseCars++;
        }

    }

    for(int i=0;i<ImagesGroup.size();i++)
    {
        std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesGroup[i],1);

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        auto Result1 = FirstExample.GetOutput()[0];

        int max_id = -1;
        double max_conf = -100;

        for(int k=0; k<FirstExample.GetOutput()[0].dim_size(1); k++)
        {

            if(Result1.matrix<float>()(0,k)>max_conf)
            {
                max_conf = Result1.matrix<float>()(0,k);
                max_id = k;
            }
        }

        if(max_id!=3)
        {
            FalseGroup++;
        }

    }

    std::cout << "0:\t" << ImagesBackground.size() <<std::endl;
    std::cout << "False:\t" << FalseBackground <<std::endl << "\n";

    std::cout << "1:\t" << ImagesPeople.size() <<std::endl;
    std::cout << "False:\t" << FalsePeople <<std::endl << "\n";

    std::cout << "2:\t" << ImagesCars.size() <<std::endl;
    std::cout << "False:\t" << FalseCars <<std::endl << "\n";

    std::cout << "3:\t" << ImagesGroup.size() <<std::endl;
    std::cout << "False:\t" << FalseGroup <<std::endl << "\n";

    int NumPictures=ImagesBackground.size()+ImagesPeople.size()+ImagesCars.size()+ImagesGroup.size();
    int NumFalses=FalseCars+FalsePeople+FalseBackground+FalseGroup;

    double accuracy=double(NumPictures-NumFalses)/NumPictures;

    std::cout << "All:\t" << NumPictures <<std::endl;
    std::cout << "AllFalse:\t" << NumFalses <<std::endl;
    std::cout << "Accuracy:\t" << accuracy <<std::endl;

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

    cv::Mat img=cv::imread("/home/user/TF/trash/grace_hopper.jpg",1);

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"}},"input"));

    StopIfBad(FirstExample.InitModel("/home/user/TF/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb",0.5,true))

    StopIfBad(FirstExample.SetImgParams({0,0,0},255));

    StopIfBad(FirstExample.SetInputDataCvMeth(img));

    StopIfBad(FirstExample.Run());

    auto Result1 = FirstExample.GetOutput();

    std::string labels = "/home/user/TF/FrozenModels/inceptionv3_2016/imagenet_slim_labels.txt";

    PrintTopLabels(Result1, labels);

    FirstExample.UnInit();

    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"}},"input"));

    StopIfBad(FirstExample.InitModel("/home/user/TF/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb",0.1,true))

    StopIfBad(FirstExample.SetImgParams({0,0,0},255));

    StopIfBad(FirstExample.SetInputDataCvMeth(img));

    StopIfBad(FirstExample.Run());

     Result1 = FirstExample.GetOutput();

    labels = "/home/user/TF/FrozenModels/inceptionv3_2016/imagenet_slim_labels.txt";

    PrintTopLabels(Result1, labels);

}
