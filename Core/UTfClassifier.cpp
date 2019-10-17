#ifndef RDK_UCLASSIFIER_CPP
#define RDK_UCLASSIFIER_CPP

#include "UTfClassifier.h"

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfClassifier::UTfClassifier(void)
    : UsePb("UsePb", this, &UTfClassifier::SetUsePb),
      PbModelPath("PbModelPath",this, &UTfClassifier::SetPbModelPath),
      MetaModelPath("MetaModelPath",this, &UTfClassifier::SetMetaModelPath),
      CkptPath("CkptPath",this,&UTfClassifier::SetCkptPath),
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
      DebugString("DebugString",this),
      BuildDone(false)
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
bool UTfClassifier::SetUsePb(const bool &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetMetaModelPath(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetCkptPath(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetPbModelPath(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetInputNodeName(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetOutputNodeName(const std::vector<std::string> &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetImgDiv(const float &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetImgSub(const float &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetGpuFraction(const double &value)
{
    Ready=false;
    return true;
}

bool UTfClassifier::SetGpuGrow(const bool &value)
{
    Ready=false;
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
    UsePb=true;

    MetaModelPath="";

    CkptPath="";

    PbModelPath="";

    InputNodeName="";

    OutputNodeName={"",};

    ImgDiv=1;

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
    //Задание параметров модели нейросети
    if(!TfObject.SetGraphParams(OutputNodeName,InputNodeName))
    {
        DebugString=TfObject.GetDebugStr();
        BuildDone=false;
        return true;
    }

    //Загрузка модели нейросети
    if(UsePb)
    {
        if(!TfObject.InitModel(PbModelPath,GpuFraction,GpuGrow))
        {
            DebugString=TfObject.GetDebugStr();
            BuildDone=false;
            return true;
        }
    }
    else
    {
        if(!TfObject.InitModel(MetaModelPath,CkptPath,GpuFraction,GpuGrow))
        {
            DebugString=TfObject.GetDebugStr();
            BuildDone=false;
            return true;
        }
    }

    //Установка параметров нормализации
    if(!TfObject.SetImgParams(ImgSub,ImgDiv))
    {
        DebugString=TfObject.GetDebugStr();
        BuildDone=false;
        return true;
    }

    DebugString=TfObject.GetDebugStr();
    BuildDone=true;
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
   //Если модель собрана с ошибками
   if(!BuildDone)
   {
       DebugString=TfObject.GetDebugStr();
       return false;
   }

   //Преобразование UbitMap изображения в формат RGB
   UBitmap NewOne;
   NewOne.SetColorModel(ubmRGB24);
   InputImage->SwapRGBChannels(&NewOne);

/*
   cv::Mat m(NewOne.GetHeight(),NewOne.GetWidth(), CV_8UC3, NewOne.GetData());

   cv::Mat input;
   m.copyTo(input);*/

   //Преобразование входного изображения под нужный размер
   if(!TfObject.SetInputDataTfMeth(NewOne))
   {
       DebugString=TfObject.GetDebugStr();
       NumberOfClass=1;
       return true;
   }

   //Запуск рассчёта нейросети
   if(!TfObject.Run())
   {
       DebugString=TfObject.GetDebugStr();
       NumberOfClass=2;
       return true;
   }

   //Получение полученного результата (выхода нейросети)
   DebugFloat=TfObject.GetOutput()[0].matrix<float>()(0,653);
   NumberOfClass=10;
   DebugString=TfObject.GetOutput()[0].DebugString();

    return true;
}
// --------------------------

}
#endif



