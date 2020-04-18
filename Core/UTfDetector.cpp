#ifndef RDK_UTFDETECTOR_CPP
#define RDK_UTFDETECTOR_CPP



#include "UTfDetector.h"

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfDetector::UTfDetector(void):
    InputImage("InputImage",this),
    UseDebugImage("UseDebugImage",this),
    UseRelativeCoords("UseRelativeCoords",this),
    OutputObjects("OutputObjects",this),
    DebugImage("DebugImage",this),
    OutputRects("OutputRects",this),
    OutputClasses("OutputClasses",this),
    OutputReliability("OutputReliability",this)
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

// Выполняет расчет этого объекта
bool UTfDetector::ATfCalculate(void)
{
    if(!InputImage.IsConnected())
     return true;

    UBitmap &bmp = *InputImage;
    if(!Detect(bmp, *OutputRects, *OutputClasses, *OutputReliability))
        return true;

    OutputObjects->Resize(OutputRects->GetRows(), 6);
    for(int i=0;i<OutputRects->GetRows();i++)
    {
     double wm = (*UseRelativeCoords)?(InputImage->GetWidth()):(1);
     double hm = (*UseRelativeCoords)?(InputImage->GetHeight()):(1);
     (*OutputObjects)(i,0) = (int)((*OutputRects)(i,1)*wm);
     (*OutputObjects)(i,1) = (int)((*OutputRects)(i,0)*hm);
     (*OutputObjects)(i,2) = (int)((*OutputRects)(i,3)*wm);
     (*OutputObjects)(i,3) = (int)((*OutputRects)(i,2)*hm);
     (*OutputObjects)(i,4)=(*OutputReliability)(i,0);
     (*OutputObjects)(i,5)=(*OutputClasses)(i,0);
    }

    if(UseDebugImage)
    {
     DebugImage->SetColorModel(ubmRGB24,false);
     InputImage->ConvertTo(*DebugImage);

     Graph.SetCanvas(DebugImage);

     UAFont *class_font=GetFont("Tahoma",20);

     for(int i=0; i<OutputRects->GetRows(); i++)
     {
        int xmin, ymin, xmax, ymax;

        xmin = (int)((*OutputObjects)(i,0));
        ymin = (int)((*OutputObjects)(i,1));
        xmax = (int)((*OutputObjects)(i,2));
        ymax = (int)((*OutputObjects)(i,3));



        double conf = (*OutputReliability)(i,0);
        int cls = (*OutputClasses)(i,0);


  //      if(conf<0.2)
  //        continue;

        Graph.SetPenColor(0x00FF00);
        Graph.SetPenWidth(3);
        Graph.Rect(xmin, ymin, xmax, ymax);



        std::stringstream ss;
        ss<<cls<<"["<<conf<<"]";

        if(class_font)
        {
          Graph.SetFont(class_font);
          Graph.Text(ss.str(),xmin, ymax+3);
        }
     }
    }

    return true;

}
// --------------------------

bool UTfDetector::Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities)
{
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
        for(int x=0; x<4;x++)
        {
            output_rects(y,x)=TfObject->GetOutput()[0].tensor<float,3>()(0,y,x);
        }
        reliabilities(y,0)=(TfObject->GetOutput()[2].matrix<float>()(0,y));
        output_classes(y,0)=int(TfObject->GetOutput()[1].matrix<float>()(0,y));
    }
    return true;
}

}

#endif
