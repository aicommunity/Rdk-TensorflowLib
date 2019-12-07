#ifndef RDK_UTFCOMPONENT_CPP
#define RDK_UTFCOMPONENT_CPP


#include "UTfComponent.h"

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfComponent::UTfComponent(void)
    : UsePb("UsePb", this, &UTfComponent::SetUsePb),
      PbModelPath("PbModelPath",this, &UTfComponent::SetPbModelPath),
      MetaModelPath("MetaModelPath",this, &UTfComponent::SetMetaModelPath),
      CkptPath("CkptPath",this,&UTfComponent::SetCkptPath),
      InputNodeName("InputNodeName",this,&UTfComponent::SetInputNodeName),
      OutputNodeName("OutputNodeName",this,&UTfComponent::SetOutputNodeName),
      ImgDiv("ImgDiv",this,&UTfComponent::SetImgDiv),
      ImgSub("ImgSub",this,&UTfComponent::SetImgSub),
      UseBGR("UseBGR",this,&UTfComponent::SetUseBGR),
      GpuFraction("GpuFraction",this,&UTfComponent::SetGpuFraction),
      GpuGrow("GpuGrow",this,&UTfComponent::SetGpuGrow),
      DebugString("DebugString",this),
      ExpectedHeight("ExpectedHeight",this),
      ExpectedWidth("ExpectedWidth",this),
      ExpectedChannels("ExpectedChannels",this),
      BuildDone(false)
{
}

UTfComponent::~UTfComponent(void)
{
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool UTfComponent::SetUsePb(const bool &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetMetaModelPath(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetCkptPath(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetPbModelPath(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetInputNodeName(const std::string &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetOutputNodeName(const std::vector<std::string> &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetImgDiv(const float &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetImgSub(const std::vector<float> &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetUseBGR(const bool &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetGpuFraction(const double &value)
{
    Ready=false;
    return true;
}

bool UTfComponent::SetGpuGrow(const bool &value)
{
    Ready=false;
    return true;
}

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool UTfComponent::ADefault(void)
{
    UsePb=true;

    MetaModelPath="";

    CkptPath="";

    PbModelPath="";

    InputNodeName="";

    OutputNodeName={"",};

    ImgDiv=1;

    ImgSub={0.f,0.f,0.f};

    UseBGR=0;

    GpuFraction=1.0;

    GpuGrow=true;

    return ATfDefault();
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool UTfComponent::ABuild(void)
{
    //Задание параметров модели нейросети
    if(!TfObject.SetGraphParams(OutputNodeName,InputNodeName))
    {
        DebugString=TfObject.GetDebugStr();
        BuildDone=false;
        return true;
    }
    std::string RealPath=GetEnvironment()->GetCurrentDataDir()+std::string(PbModelPath);
    //Загрузка модели нейросети
    if(UsePb)
    {
        //GetEnvironment()->GetCurrentDataDir()+
        if(!TfObject.InitModel(RealPath,GpuFraction,GpuGrow))
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
    if(!TfObject.SetImgParams(ImgSub,ImgDiv,UseBGR))
    {
        DebugString=TfObject.GetDebugStr();
        BuildDone=false;
        return true;
    }


    std::vector<int> Params=TfObject.GetImgParams();

    ExpectedHeight=Params[0];
    ExpectedWidth=Params[1];
    ExpectedChannels=Params[2];


    DebugString=TfObject.GetDebugStr();
    return ATfBuild();
}

// Сброс процесса счета без потери настроек
bool UTfComponent::AReset(void)
{
    return ATfReset();
}

// Выполняет расчет этого объекта
bool UTfComponent::ACalculate(void)
{
   //Если модель собрана с ошибками
   if(!BuildDone)
   {
       DebugString=TfObject.GetDebugStr();
       return true;
   }


   return ATfCalculate();
}
// --------------------------


}

#endif
