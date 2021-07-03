#ifndef RDK_UTFSQDETECTOR_CPP
#define RDK_UTFSQDETECTOR_CPP



#include "UTfSqDetector.h"

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
UTfSqDetector::UTfSqDetector(void):
    ConfigPath("ConfigPath",this,&UTfSqDetector::SetConfigPath)
{
    TfObject = &TfDetector;
}

UTfSqDetector::~UTfSqDetector(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------
bool UTfSqDetector::SetConfigPath(const std::string &value)
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
UTfSqDetector* UTfSqDetector::New(void)
{
 return new UTfSqDetector;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool UTfSqDetector::ATfDefault(void)
{

    return true;
}

// ����� �������� ����� ��� ������ ��������
bool UTfSqDetector::ATfReset()
{

    return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool UTfSqDetector::ATfBuild()
{
    std::string RealPath_1=ConfigPath;
    if(GetEnvironment())
    {
        RealPath_1=GetEnvironment()->GetCurrentDataDir()+std::string(ConfigPath);
    }

    if(!TfDetector.SetConfigPath(RealPath_1))
    {
        return false;
    }
    return true;
}



// --------------------------

bool UTfSqDetector::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
    if(!BuildDone)
    {
        DebugString=TfObject->GetDebugStr();
        Ready=false;
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string(DebugString));
        return false;
    }

    if(!TfObject->SetInputDataCvMeth(bmp))
    {
        DebugString=TfObject->GetDebugStr();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string(DebugString));
        return false;
    }

    //������ �������� ���������
    if(!TfObject->Run())
    {
        DebugString=TfObject->GetDebugStr();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string(DebugString));
        return false;
    }

    int num_detections=TfObject->GetOutput()[1].dim_size(0);


    output_rects.Resize(num_detections,4);
    output_classes.Resize(num_detections,1);
    reliabilities.Resize(num_detections,1);

    for(int y=0; y<num_detections; y++)
    {
        // SqueezeDet ������� ��������� �� ������� ����������� ����. �������
        // ������� ���� �������������� ������ �� ��������� �������� �����������
        //Xmin Ymin Xmax Ymax
        double wm = double(InputImage->GetWidth()) / double(ExpectedWidth);
        double hm = double(InputImage->GetHeight())/ double(ExpectedHeight);

        output_rects(y,0)=TfObject->GetOutput()[0].matrix<float>()(y,1)*wm;
        output_rects(y,1)=TfObject->GetOutput()[0].matrix<float>()(y,0)*hm;
        output_rects(y,2)=TfObject->GetOutput()[0].matrix<float>()(y,3)*wm;
        output_rects(y,3)=TfObject->GetOutput()[0].matrix<float>()(y,2)*hm;

        reliabilities(y,0)=(TfObject->GetOutput()[2].tensor<float,1>()(y));
        output_classes(y,0)=int(TfObject->GetOutput()[1].tensor<long long int,1>()(y));
    }
    return true;
}




}

#endif
