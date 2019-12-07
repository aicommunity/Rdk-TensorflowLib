#ifndef RDK_TAGGRCLASSIFIERTF_CPP
#define RDK_TAGGRCLASSIFIERTF_CPP



#include "TAggrClassifierTF.h"

namespace RDK {

// ������
// --------------------------
// ������������ � �����������
// --------------------------
TAggrClassifierTF::TAggrClassifierTF(void):
    InputClassifierResults("InputClassifierResults",this),
    InputConfidences("InputConfidences",this)
{
 AddStaticComponent("UTfClassifier", "ClassifierTF", &UBitmapClassifier);

}


TAggrClassifierTF::~TAggrClassifierTF(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------
bool TAggrClassifierTF::SetNumClasses(const int& num)
{
    this->NumClasses = num;
    //���������� ������ ����������
    //this->UBitmapClassifier.NumClasses = num;

    return true;
}

//---------------------
const int &TAggrClassifierTF::GetNumClasses() const
{
    return NumClasses;
}

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
TAggrClassifierTF* TAggrClassifierTF::New(void)
{
 return new TAggrClassifierTF;
}
// --------------------------


// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool TAggrClassifierTF::ADefault2(void)
{
 ResizeX = 96;
 ResizeY = 96;
 ResizeType = 1;
 NumClasses=4;

 UBitmapClassifier.Default();
 UBitmapClassifier.SetActivity(true);
 //UBitmapClassifier.Init();

 return true;
}


// ����� �������� ����� ��� ������ ��������
bool TAggrClassifierTF::AReset2(void)
{
    UBitmapClassifier.Reset();

    bool res1 = CreateLink("", "ClassificationBitmaps",  "ClassifierTF", "InputImages");
    bool res2 = CreateLink("ClassifierTF", "OutputClasses", "",  "InputClassifierResults");
    bool res3 = CreateLink("ClassifierTF", "OutputConfidences", "",  "InputConfidences");

    return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool TAggrClassifierTF::ABuild2()
{
    return true;
}

// ��������� ������ ����� �������
bool TAggrClassifierTF::ACalculate2(void)
{
    if(!InputClassifierResults.IsConnected())
    {
     LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TAggrClassifierTF error: InputClassifierResults is not connected, returning true"));
     return true;
    }

    if(!InputConfidences.IsConnected())
    {
     LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TAggrClassifierTF error: InputConfidences is not connected, returning true"));
     return true;
    }

    UBitmapClassifier.Calculate();
    /*if(aggrClasses.size() != InputClassifierResults->size())
    {
     LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string("TAggrClassifierTF error: InputClassifierResults have wrong size (not equal aggrClasses)"));
     return true;
    }*/

    aggrClasses=*InputClassifierResults;


    int num_cls = (*InputConfidences).GetCols();
    int num_objs = (*InputConfidences).GetRows();

    if(num_cls==0 || num_objs==0)
        return true;

    std::vector<double> tmp;
    tmp.resize(num_cls, 0);
    aggrConfidences.resize(num_objs, tmp);

    for(int i=0; i<num_objs; i++)
    {
        //int w = NumClasses; //TODO: ������ ��������� ���������� �������
        for(int j=0; j<num_cls; j++)
        {
            aggrConfidences[i][j] = (*InputConfidences)(i,j);
        }
    }
    return true;
}
// --------------------------
/// ������������ ������������� ������� �� �����������
/// � ������� result ���������� ������ �������������� ������� ������� �� �������
bool TAggrClassifierTF::Recognize(MDVector<double> &result)
{
 return true;
}

}

#endif
