#ifndef RDK_UCLASSIFIER_CPP
#define RDK_UCLASSIFIER_CPP

#include "UTfClassifier.h"

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfClassifier::UTfClassifier(void)
    : ModelPath("ModelPath",this, &UTfClassifier::SetModelPath),
      InputNodeName("InputNodeName",this,&UTfClassifier::SetInputNodeName),
      OutputNodeName("OutputNodeName",this,&UTfClassifier::SetOutputNodeName),
      ImgDiv("ImgDiv",this,&UTfClassifier::SetImgDiv),
      ImgSub("ImgSub",this,&UTfClassifier::SetImgSub),
      GpuFraction("GpuFraction",this,&UTfClassifier::SetGpuFraction),
      GpuGrow("GpuGrow",this,&UTfClassifier::SetGpuGrow),
      InputImage("InputImage",this),
      DebugImage("DebugImage",this),
      NumberOfClass("NumberOfClass", this),
      DebugFloat("DebugFloat",this),
      DebugString("DebugString",this)
{
}

UTfClassifier::~UTfClassifier(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool UTfClassifier::SetModelPath(const std::string &value)
{
    Ready=false;
   // ModelPath=value;
    return true;
}

bool UTfClassifier::SetInputNodeName(const std::string &value)
{
    Ready=false;
    //InputNodeName=value;
    return true;
}

bool UTfClassifier::SetOutputNodeName(const std::vector<std::string> &value)
{
    Ready=false;
    //OutputNodeName=value;
    return true;
}

bool UTfClassifier::SetImgDiv(const float &value)
{
    Ready=false;
    //ImgDiv=value;
    return true;
}

bool UTfClassifier::SetImgSub(const float &value)
{
    Ready=false;
    //ImgSub=value;
    return true;
}

bool UTfClassifier::SetGpuFraction(const double &value)
{
    Ready=false;
    //GpuFraction=value;
    return true;
}

bool UTfClassifier::SetGpuGrow(const bool &value)
{
    Ready=false;
    //GpuGrow=value;
    return true;
}

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
UTfClassifier* UTfClassifier::New(void)
{
 return new UTfClassifier;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool UTfClassifier::ADefault(void)
{
    ModelPath="";//"/home/vladburin/1_Folder/TF/smth/FrozenModels/inceptionv3_2016/inception_v3_2016_08_28_frozen.pb";

    InputNodeName="";//"input";

    OutputNodeName={"",};//{"InceptionV3/Predictions/Reshape_1",};

    ImgDiv=1;//255;

    ImgSub=0;

    GpuFraction=0.5;

    GpuGrow=false;
    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool UTfClassifier::ABuild(void)
{

    if(!TfObject.SetGraphParams(OutputNodeName,InputNodeName))
    {
        DebugString=TfObject.GetDebugStr();
        NumberOfClass=2;
        return true;
    }

    if(!TfObject.InitModel(std::string(ModelPath),GpuFraction,GpuGrow))
    {
        DebugString=TfObject.GetDebugStr();
        NumberOfClass=1;
        return true;
    }

    if(!TfObject.SetImgParams(ImgSub,ImgDiv))
    {
        DebugString=TfObject.GetDebugStr();
        NumberOfClass=3;
        return true;
    }
    NumberOfClass=4;
    return true;
}

// Сброс процесса счета без потери настроек
bool UTfClassifier::AReset(void)
{
 return true;
}

// Выполняет расчет этого объекта
bool UTfClassifier::ACalculate(void)
{
   UBitmap NewOne;
   NewOne.SetColorModel(ubmRGB24);
   InputImage->SwapRGBChannels(&NewOne);
/*
   cv::Mat m(NewOne.GetHeight(),NewOne.GetWidth(), CV_8UC3, NewOne.GetData());

   cv::Mat input;
   m.copyTo(input);*/
   if(!TfObject.SetInputDataTfMeth(NewOne))
   {
       DebugString=TfObject.GetDebugStr();
       NumberOfClass=1;
       return true;
   }

   if(!TfObject.Run())
   {
       DebugString=TfObject.GetDebugStr();
       NumberOfClass=2;
       return true;
   }
   DebugFloat=TfObject.GetOutput()[0].matrix<float>()(0,653);
   NumberOfClass=10;//TfObject.GetOutput().size();
   //TfObject.GetOutput()[0].matrix<float,1>()(653);

   DebugString=TfObject.GetOutput()[0].DebugString();

    return true;
}
// --------------------------

}
#endif



