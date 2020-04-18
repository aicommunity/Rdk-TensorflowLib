#ifndef RDK_UTRAJECTORIESSAVER_CPP
#define RDK_UTRAJECTORIESSAVER_CPP


#include "UTrajectoriesSaver.h"

namespace RDK {
using namespace RTV;
// ������
// --------------------------
// ������������ � �����������
// --------------------------
UTrajectoriesSaver::UTrajectoriesSaver(void)
    : OnlyClassified("OnlyDefined", this),
      PAllAggregates("PAllAggregates",this),
      PTrajectories("PTrajectories",this),
      InputImage("InputImage",this)
{
}

UTrajectoriesSaver::~UTrajectoriesSaver(void)
{
}
// --------------------------


// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------




// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------

// �������� ������ ��� ����� ������ ����� ������� ����� ������
UTrajectoriesSaver* UTrajectoriesSaver::New(void)
{
     return new UTrajectoriesSaver;
}

// --------------------------
// ������� ������ ���������� ������
// --------------------------
// �������������� �������� �� ��������� � ����� �������� �����
bool UTrajectoriesSaver::ADefault(void)
{
    OnlyClassified = false;
    return true;
}

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
bool UTrajectoriesSaver::ABuild(void)
{
    return true;
}

// ����� �������� ����� ��� ������ ��������
bool UTrajectoriesSaver::AReset(void)
{
    return true;
}

// ��������� ������ ����� �������
bool UTrajectoriesSaver::ACalculate(void)
{
    UBitmap bmp;
    for(size_t i=0;i<PAllAggregates->size();i++)
    {
     TAggr &aggr=(*PAllAggregates)[i];

     if(aggr.AgID<0)
         continue;

     if(PTrajectories)
     {
      if(int(PTrajectories->size())<=(*PAllAggregates)[i].AgTrjIndx)
       continue;
      const TAggrTrj &trj=(*PTrajectories)[(*PAllAggregates)[i].AgTrjIndx];

      if(!trj.IsActive)
       continue;
     }

     if(OnlyClassified)
        if(aggr.ObjectClass<=0)
            continue;


     UBRect object_rect;
     object_rect.X=aggr.AgXY.Left;
     object_rect.Y=aggr.AgXY.Top;
     object_rect.Width=abs(aggr.AgXY.Right-aggr.AgXY.Left);
     object_rect.Height=abs(aggr.AgXY.Bottom-aggr.AgXY.Top);

     if(object_rect.Width <=0 || object_rect.Height<=0)
      continue;

     // �������� �������� �����������
     bmp.SetRes(object_rect.Width,object_rect.Height,InputImage->GetColorModel());

     InputImage->GetRect(object_rect.X, object_rect.Y, bmp);


     //�������� ���� �������� �� ����������, ������� �� �����������
     //�������� ��� �������� ������ ���, ���� ����������

     std::string traj_path = Environment->GetCurrentDataDir()+"trajectory_images/";
     //������� ���������� ��� ���� ����������
     if(RDK::CreateNewDirectory(traj_path.c_str())==0)
     {
         //������� ���������� ��� �������� (��������)
         std::string trajectory_dir = traj_path+sntoa((*PTrajectories)[(*PAllAggregates)[i].AgTrjIndx].TrjID)+"/"; //(*AllAggregates)[i].AgTrjIndx)
         if(RDK::CreateNewDirectory(trajectory_dir.c_str())==0)
         {
             //��������� ��� �������� - ��� ���� �� ����������, �
             std::string image_name = trajectory_dir+sntoa(aggr.AgID)+"_"+sntoa(aggr.ObjectClass)+"_"+sntoa(aggr.ObjectClassQuality)+"_"+sntoa((*PTrajectories)[(*PAllAggregates)[i].AgTrjIndx].Trj.size())+".jpg";

             RDK::UBitmap TempBitmap;
             bmp.ConvertTo(TempBitmap);
             TempBitmap.SwapRGBChannels();
             //TempBitmap.ReflectionX();

             jpge::params param;
             param.m_quality=100;

             jpge::compress_image_to_jpeg_file(image_name.c_str(), TempBitmap.GetWidth(), TempBitmap.GetHeight(), 3,
                                            TempBitmap.GetData(),param);
             //index+=1;

         }
     }

    }






   return true;
}
// --------------------------



}

#endif //RDK_UTRAJECTORIESSAVER_H
