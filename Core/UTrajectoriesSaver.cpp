#ifndef RDK_UTRAJECTORIESSAVER_CPP
#define RDK_UTRAJECTORIESSAVER_CPP


#include "UTrajectoriesSaver.h"

namespace RDK {
using namespace RTV;
// Методы
// --------------------------
// Конструкторы и деструкторы
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
// Методы управления параметрами
// ---------------------
// ---------------------




// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------

// Выделяет память для новой чистой копии объекта этого класса
UTrajectoriesSaver* UTrajectoriesSaver::New(void)
{
     return new UTrajectoriesSaver;
}

// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool UTrajectoriesSaver::ADefault(void)
{
    OnlyClassified = false;
    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool UTrajectoriesSaver::ABuild(void)
{
    return true;
}

// Сброс процесса счета без потери настроек
bool UTrajectoriesSaver::AReset(void)
{
    return true;
}

// Выполняет расчет этого объекта
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

     // создание рабочего изображения
     bmp.SetRes(object_rect.Width,object_rect.Height,InputImage->GetColorModel());

     InputImage->GetRect(object_rect.X, object_rect.Y, bmp);


     //Вероятно сюда приходят те траектории, которые мы анализируем
     //Наверное они приходят каждый раз, пока существуют

     std::string traj_path = Environment->GetCurrentDataDir()+"trajectory_images/";
     //Создаем директорию для всех траекторий
     if(RDK::CreateNewDirectory(traj_path.c_str())==0)
     {
         //Создаем директорию для агрегата (текущего)
         std::string trajectory_dir = traj_path+sntoa((*PTrajectories)[(*PAllAggregates)[i].AgTrjIndx].TrjID)+"/"; //(*AllAggregates)[i].AgTrjIndx)
         if(RDK::CreateNewDirectory(trajectory_dir.c_str())==0)
         {
             //Формируем имя картинки - это путь до траектории, и
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
