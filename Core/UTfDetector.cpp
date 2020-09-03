#ifndef RDK_UTFDETECTOR_CPP
#define RDK_UTFDETECTOR_CPP



#include "UTfDetector.h"

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfDetector::UTfDetector(void)
{
    TfObject = &TfDetector;
}

UTfDetector::~UTfDetector(void)
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
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
UTfDetector* UTfDetector::New(void)
{
 return new UTfDetector;
}
// --------------------------


// --------------------------
// Скрытые методы управления счетом
// --------------------------
// Восстановление настроек по умолчанию и сброс процесса счета
bool UTfDetector::ATfDefault(void)
{

    return true;
}

// Сброс процесса счета без потери настроек
bool UTfDetector::ATfReset(void)
{

    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool UTfDetector::ATfBuild()
{
    return true;
}

// --------------------------

bool UTfDetector::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
    if(!BuildDone)
        return false;

    if(!TfObject->SetInputDataCvMeth(bmp))
    {
        DebugString=TfObject->GetDebugStr();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string(DebugString));
        return false;
    }

    //Запуск рассчёта нейросети
    if(!TfObject->Run())
    {
        DebugString=TfObject->GetDebugStr();
        LogMessageEx(RDK_EX_WARNING,__FUNCTION__,std::string(DebugString));
        return false;
    }
    int num_detections;
    if(TfObject->GetOutput().size()==4)
    {
        num_detections=int(TfObject->GetOutput()[3].scalar<float>()());
    }
    else
    {
       num_detections=TfObject->GetOutput()[1].dim_size(1);

    }


    output_rects.Resize(num_detections,4);
    output_classes.Resize(num_detections,1);
    reliabilities.Resize(num_detections,1);

    for(int y=0; y<num_detections; y++)
    {
        output_rects(y,0)=TfObject->GetOutput()[0].tensor<float,3>()(0,y,1);
        output_rects(y,1)=TfObject->GetOutput()[0].tensor<float,3>()(0,y,0);
        output_rects(y,2)=TfObject->GetOutput()[0].tensor<float,3>()(0,y,3);
        output_rects(y,3)=TfObject->GetOutput()[0].tensor<float,3>()(0,y,2);

        reliabilities(y,0)=(TfObject->GetOutput()[2].matrix<float>()(0,y));
        output_classes(y,0)=int(TfObject->GetOutput()[1].matrix<float>()(0,y));
    }
    return true;
}

}

#endif
