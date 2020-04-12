#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <fstream>
#include <iostream>
//#include <sys/time.h>
#include "../../Core/Interface/ttfsession.h"
#include "../../Core/Interface/label_image.h"
#include "../../Core/Interface/ttfsessionSqDet.h"


//#include <boost/config/compiler/visualc.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/foreach.hpp>

using namespace TTF;

#define StopIfBad(val) if(!val)                         \
{ std::cout << FirstExample.GetDebugStr() << std::endl; \
  return 0;}
\


int CheckAccuracy(std::string input_model,std::vector<std::string> outputs,std::string inputs);

int CheckAccuracyNew(std::string input_model,std::vector<std::string> input_and_output,std::vector<std::string> paths);

int SimpleInception();

int DetectFrames(QString str, std::string path, std::string savefolder);

int DebugSqueezeDet(std::string img_path);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //DebugSqueezeDet("/home/vladburin/TF/trash/CameraImgs/images01010.jpeg");


    /*
    //eleron
    CheckAccuracy("/home/user/networks_for_test/Tested/eleron-inception3-12-0.91803.pb",{{"predictions/Softmax"},},"input_1");

    //abz
    CheckAccuracy("/home/user/networks_for_test/Tested/abz-densenet-121-38-0.72573.pb",{{"dense_1/Softmax"},},"densenet121_input");

    clock_t end_frame = clock();

    ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
    auto begin = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::string img_path = Environment->GetCurrentDataDir()+"classification_time";
    std::ofstream CheckTime;
    CheckTime.open(img_path, std::ios::app);
    CheckTime << ClassificationTime << "\n";
    CheckTime.close();
*/


    CheckAccuracyNew("/home/vladburin/ABZnetwork/Eleron_new.pb",{"vgg16_input","dense_2/Softmax"},{"/home/vladburin/TF/trash/eleron_val/background","/home/vladburin/TF/trash/eleron_val/cars","/home/vladburin/TF/trash/eleron_val/peoples"});

    return a.exec();

}


int DebugSqueezeDet(std::string img_path)
{
    TTfSessionSqDet FirstExample;

    StopIfBad(FirstExample.SetConfigPath("/home/vladburin/Squeeze/squeezedet_keras-master/kitti.config"));

    StopIfBad(FirstExample.SetGraphParams({{"lambda_1/Pad"}},"input"));

    StopIfBad(FirstExample.SetImgParams({0,0,0},1,true));

    StopIfBad(FirstExample.InitModel("/home/vladburin/Squeeze/SqueezeDet_kitti_new.pb",0.5));

    cv::Mat img=cv::imread(img_path,1);

    StopIfBad(FirstExample.SetInputDataCvMeth(img));

    StopIfBad(FirstExample.Run());

    //StopIfBad(FirstExample.Run());


    //Обработка полученных классов
    auto Result1 = FirstExample.GetOutput();

    //кол-во объектов
    int Num = Result1[0].dim_size(0);
    //номера классов распознанных объектов

    auto Classes = Result1[1].tensor<long long int,1>();
    //вероятность принадлежности к классу
    auto Labels = Result1[2].tensor<float,1>();
    //координаты бокса
    auto Boxes = Result1[0].matrix<float>();

    //вывод на исходное изображение боксов, номеров классов и вероятностей
    cv::Mat image = img.clone();

    cv::cvtColor(image, image, CV_BGR2RGB);


    float x_scale = float(img.cols)/1248.f;
    float y_scale = float(img.rows)/384.f;


    for(int i=0; i < Num; i++)
    {
       cv::rectangle(image, {int(x_scale*(Boxes(i,1))), int(y_scale*(Boxes(i,0)))},
                            {int(x_scale*(Boxes(i,3))), int(y_scale*(Boxes(i,2)))}, cv::Scalar(0,0,0),2);

       cv::putText(image, std::to_string(Labels(i)), {int(x_scale*(Boxes(i,1))), int(y_scale*(Boxes(i,0)))}, 0, 1, {0,0,255},2);
       cv::putText(image, std::to_string(Classes(i)), {int(x_scale*(Boxes(i,1))), int(y_scale*(Boxes(i,0)))+30}, 0, 1, {0,0,255},2);
    }


    cv::cvtColor(image, image, CV_BGR2RGB);

    cv::namedWindow( "Original",cv::WINDOW_AUTOSIZE );
    cv::namedWindow( "Detected",cv::WINDOW_AUTOSIZE );

    cv::imshow( "Original", img);
    cv::imshow( "Detected", image);

    cv::waitKey(0);


    return 0;
}

int CheckAccuracyNew(std::string input_model,std::vector<std::string> input_and_output,std::vector<std::string> paths)
{
    std::vector<int> Falses(paths.size(),0);
    std::vector<int> Predictions(paths.size(),0);

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({input_and_output[1]},input_and_output[0]));

    StopIfBad(FirstExample.InitModel(input_model,0.9,true));

    StopIfBad(FirstExample.SetImgParams({0,0,0},255,false));

    double ClassificationTime;
    std::vector<double> TimeArray;

    for (int k=0;k<paths.size();k++)
    {
        std::vector<std::string> Images;

        QDir dir(QString::fromStdString(paths.at(k)));
        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);
        QFileInfoList list = dir.entryInfoList();

        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            QString line = fileInfo.fileName();
            Images.push_back(std::string(dir.path().toUtf8().constData())+"/"+ std::string(line.toUtf8().constData()));
        }


        for(int j=0;j<Images.size();j++)
        {
            cv::Mat img=cv::imread(Images[j],1);

            ClassificationTime=0.0;
            clock_t start_frame = clock();

            StopIfBad(FirstExample.SetInputDataCvMeth(img));

            StopIfBad(FirstExample.Run());

            clock_t end_frame = clock();
            ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
            TimeArray.push_back(ClassificationTime);

            auto Result1 = FirstExample.GetOutput()[0];

            int max_id = -1;
            double max_conf = -100;
            //std::cout << Result1.matrix<float>()<< std::endl;
            for(int g=0; g<FirstExample.GetOutput()[0].dim_size(1); g++)
            {

                if(Result1.matrix<float>()(0,g)>max_conf)
                {
                    max_conf = Result1.matrix<float>()(0,g);
                    max_id = g;
                }
            }
            if(max_id!=k)
            {
                Falses.at(k)++;
            }

        }
        Predictions.at(k)=Images.size();
    }

    std::cout << input_model << std::endl;

    for(int h=0;h<paths.size();h++)
    {
        std::cout << h<<":\t" << Predictions[h] <<std::endl;
        std::cout << "False:\t" << Falses[h] <<std::endl << "\n";
    }

    int NumPictures=0;
    for(std::vector<int>::iterator it = Predictions.begin(); it != Predictions.end(); ++it)
        NumPictures += *it;

    int NumFalses=0;
    for(std::vector<int>::iterator it = Falses.begin(); it != Falses.end(); ++it)
        NumFalses += *it;

    double accuracy=double(NumPictures-NumFalses)/NumPictures;

    std::cout << "All:\t" << NumPictures <<std::endl;
    std::cout << "AllFalse:\t" << NumFalses <<std::endl;
    std::cout << "Accuracy:\t" << accuracy <<std::endl;


    double avarage=0;
    for (int i=0;i<TimeArray.size();i++)
    {
        avarage+=TimeArray[i];
    }

    avarage=avarage/TimeArray.size();
    std::cout << "Average:\t" << avarage <<std::endl;

    double temp=0;
    for (int i=0;i<TimeArray.size();i++)
    {
        temp+=pow((TimeArray[i]-avarage),2);
    }

    temp=temp/TimeArray.size();

    temp=sqrt(temp);
    std::cout <<"Standard Dev:\t"<< temp <<std::endl;



}

int CheckAccuracy(std::string input_model,std::vector<std::string> outputs,std::string inputs)
{
    std::vector<std::string> ImagesBackground;
    QDir dir("/home/user/eleron_validation/0");
    //QDir dir("/home/user/abz_val/background");
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
    //dir=("/home/user/abz_val/peoples");
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
    //dir=("/home/user/abz_val/cars");
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

    StopIfBad(FirstExample.SetGraphParams(outputs,inputs));
    std::string model=input_model;
    StopIfBad(FirstExample.InitModel(model,0.9,true));

    StopIfBad(FirstExample.SetImgParams({0,0,0},255,false));

    int FalseBackground=0;
    int FalsePeople=0;
    int FalseCars=0;
    int FalseGroup=0;
    std::cout << "start" << std::endl;

    double ClassificationTime;
    std::vector<double> TimeArray;

    for(int i=0;i<ImagesBackground.size();i++)
    {
        //std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesBackground[i],1);

        ClassificationTime=0.0;
        clock_t start_frame = clock();

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        clock_t end_frame = clock();
        ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
        TimeArray.push_back(ClassificationTime);


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
        //std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesPeople[i],1);

        ClassificationTime=0.0;
        clock_t start_frame = clock();

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        clock_t end_frame = clock();
        ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
        TimeArray.push_back(ClassificationTime);

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
        //std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesCars[i],1);

        ClassificationTime=0.0;
        clock_t start_frame = clock();

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        clock_t end_frame = clock();
        ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
        TimeArray.push_back(ClassificationTime);

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
        //std::cout<< i <<std::endl;
        cv::Mat img=cv::imread(ImagesGroup[i],1);

        ClassificationTime=0.0;
        clock_t start_frame = clock();

        StopIfBad(FirstExample.SetInputDataCvMeth(img));

        StopIfBad(FirstExample.Run());

        clock_t end_frame = clock();
        ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
        TimeArray.push_back(ClassificationTime);

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

    std::cout << model << std::endl;
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


    double avarage=0;
    for (int i=0;i<TimeArray.size();i++)
    {
        avarage+=TimeArray[i];
    }

    avarage=avarage/TimeArray.size();
    std::cout << "Average:\t" << avarage <<std::endl;

    double temp=0;
    for (int i=0;i<TimeArray.size();i++)
    {
        temp+=pow((TimeArray[i]-avarage),2);
    }

    temp=temp/TimeArray.size();

    temp=sqrt(temp);
    std::cout <<"Standard Dev:\t"<< temp <<std::endl;

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

    cv::Mat img=cv::imread("/home/vladburin/TF/trash/grace_hopper.jpg",1);

    TTfSession FirstExample;

    StopIfBad(FirstExample.SetGraphParams({{"InceptionV3/Predictions/Reshape_1"}},"input"));

    StopIfBad(FirstExample.InitModel("/home/vladburin/TF/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb",0.5))

    StopIfBad(FirstExample.SetImgParams({0,0,0},255));

    StopIfBad(FirstExample.SetInputDataCvMeth(img));


    StopIfBad(FirstExample.Run());

    auto Result1 = FirstExample.GetOutput();

    std::string labels = "/home/vladburin/TF/FrozenModels/inceptionv3_2016/imagenet_slim_labels.txt";

    PrintTopLabels(Result1, labels);


}
