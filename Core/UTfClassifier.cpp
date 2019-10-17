#ifndef RDK_UCLASSIFIER_CPP
#define RDK_UCLASSIFIER_CPP

#include "UTfClassifier.h"

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
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
// ������ ���������� �����������
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
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
UTfClassifier* UTfClassifier::New(void)
{
 return new UTfClassifier;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
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

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool UTfClassifier::ABuild(void)
{
    //������� ���������� ������ ���������
    if(!TfObject.SetGraphParams(OutputNodeName,InputNodeName))
    {
        DebugString=TfObject.GetDebugStr();
        BuildDone=false;
        return true;
    }

    //�������� ������ ���������
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

    //��������� ���������� ������������
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

// ����� �������� ����� ��� ������ ��������
bool UTfClassifier::AReset(void)
{
 return true;
}

// ��������� ������ ����� �������
bool UTfClassifier::ACalculate(void)
{
   //���� ������ ������� � ��������
   if(!BuildDone)
   {
       DebugString=TfObject.GetDebugStr();
       return false;
   }

   //�������������� UbitMap ����������� � ������ RGB
   UBitmap NewOne;
   NewOne.SetColorModel(ubmRGB24);
   InputImage->SwapRGBChannels(&NewOne);

/*
   cv::Mat m(NewOne.GetHeight(),NewOne.GetWidth(), CV_8UC3, NewOne.GetData());

   cv::Mat input;
   m.copyTo(input);*/

   //�������������� �������� ����������� ��� ������ ������
   if(!TfObject.SetInputDataTfMeth(NewOne))
   {
       DebugString=TfObject.GetDebugStr();
       NumberOfClass=1;
       return true;
   }

   //������ �������� ���������
   if(!TfObject.Run())
   {
       DebugString=TfObject.GetDebugStr();
       NumberOfClass=2;
       return true;
   }

   //��������� ����������� ���������� (������ ���������)
   DebugFloat=TfObject.GetOutput()[0].matrix<float>()(0,653);
   NumberOfClass=10;
   DebugString=TfObject.GetOutput()[0].DebugString();

    return true;
}
// --------------------------

}
#endif



