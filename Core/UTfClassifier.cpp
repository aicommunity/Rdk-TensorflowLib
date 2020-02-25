#ifndef RDK_UCLASSIFIER_CPP
#define RDK_UCLASSIFIER_CPP

#include "UTfClassifier.h"


#define CLASS_UNDEFINED -2
#define CLASS_LOWQUAL -3

namespace RDK {

// Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfClassifier::UTfClassifier(void):
    ConfidenceThreshold("ConfidenceThreshold", this),
    SaveDebugResults("SaveDebugResults", this),
    InputImage("InputImage",this),
    InputImages("InputImages",this),
    OutputClasses("OutputClasses",this),
    OutputConfidences("OutputConfidences", this),
    DebugImage("DebugImage",this),
    ClassificationTime("ClassificationTime",this),
    NumClasses("NumClasses",this,&UTfClassifier::SetNumClasses)
{
    TfObject = new TTF::TTfSession;
}

UTfClassifier::~UTfClassifier(void)
{
    delete TfObject;
}
// --------------------------


// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool UTfClassifier::SetNumClasses(const int &value)
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
bool UTfClassifier::ATfDefault(void)
{
    NumClasses=0;
    return true;
}

// Сброс процесса счета без потери настроек
bool UTfClassifier::ATfReset(void)
{
    ClassificationTime=0.0;
    OutputClasses->Resize(0,1);
    OutputConfidences->Resize(0, NumClasses);
    return true;
}

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
bool UTfClassifier::ATfBuild()
{
    //NumClasses=TfObject->GetNumClasses();
    return true;
}

// Выполняет расчет этого объекта
bool UTfClassifier::ATfCalculate(void)
{



    if(InputImages.IsConnected() && InputImages->size()>0)
    {
        OutputClasses->Assign(InputImages->size(),1, CLASS_UNDEFINED);
        OutputConfidences->Assign(InputImages->size(), NumClasses,0.0);
        for(int i=0; i<InputImages->size(); i++)
        {
            UBitmap &bmp = (*InputImages)[i];

            MDVector<double> output_confidences;
            int class_id;
            bool is_classified;
            bool classify_res=ClassifyBitmap(bmp, output_confidences, ConfidenceThreshold, class_id, is_classified);

            if(classify_res)
            {
                for(int k=0; k<output_confidences.GetSize(); k++)
                {
                   (*OutputConfidences)(i, k) = output_confidences(k);
                }
                (*OutputClasses)[i] = (is_classified)?class_id:CLASS_LOWQUAL;
            }
        }
    }
    else
    {
        if(InputImage.IsConnected())
        {
            OutputClasses->Assign(1,1, CLASS_UNDEFINED);
            OutputConfidences->Assign(1, NumClasses,0.0);
            UBitmap &bmp = *InputImage;

            MDVector<double> output_confidences;
            int class_id;
            bool is_classified;
            bool classify_res=ClassifyBitmap(bmp, output_confidences, ConfidenceThreshold, class_id, is_classified);

            for(int k=0; k<output_confidences.GetSize(); k++)
            {
                (*OutputConfidences)(0, k) = output_confidences(k);
            }

            (*OutputClasses)[0] = (is_classified)?class_id:CLASS_LOWQUAL;
        }
    }



    return true;
}
// --------------------------

bool UTfClassifier::ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified)
{

    ClassificationTime=0.0;
    clock_t start_frame = clock();
    //auto begin = std::chrono::steady_clock::now();

    //Обрабатываем иображение для входа
    if(!TfObject->SetInputDataCvMeth(bmp))
    {
        DebugString=TfObject->GetDebugStr();
       // LogMessageEx(RDK_EX_WARNING,__FUNCTION__,DebugString);
        return false;
    }

    //Запуск рассчёта нейросети
    if(!TfObject->Run())
    {
        DebugString=TfObject->GetDebugStr();
       // LogMessageEx(RDK_EX_WARNING,__FUNCTION__,DebugString);
        return false;
    }

    std::vector<float> result;

    for(int i=0; i<TfObject->GetOutput()[0].dim_size(1); i++)
    {
        result.push_back(TfObject->GetOutput()[0].matrix<float>()(0,i));
    }

    if(result.size()!=NumClasses)
    {
        DebugString="Number of classes is wrong";
        return false;
    }

    int max_id = -1;
    double max_conf = -100;

    output_confidences.Resize(result.size());

    for(int k=0; k<result.size(); k++)
    {
        output_confidences(k) = result[k];
        if(result[k]>max_conf)
        {
            max_conf = result[k];
            max_id = k;
        }
    }

    if(max_conf<conf_thresh)
    {
        is_classified=false;
    }
    else
    {
        is_classified=true;
    }

    class_id = max_id;
    clock_t end_frame = clock();
    ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;
    /*
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    double(elapsed_ms.count())/1000;

    clock_t end_frame = clock();
    ClassificationTime =double(elapsed_ms.count())/1000;
    ClassificationTime = (double)(end_frame - start_frame) / CLOCKS_PER_SEC;

    std::string img_path = Environment->GetCurrentDataDir()+"classification_time_Cv_rdk";
    std::ofstream CheckTime;
    CheckTime.open(img_path, std::ios::app);
    CheckTime << ClassificationTime << "\n";
    CheckTime.close();
    */


    if(SaveDebugResults)
    {
        static int iiii=0;
        std::string img_path = Environment->GetCurrentDataDir()+"classification_results";
        if(RDK::CreateNewDirectory(img_path.c_str())==0)
        {
            std::stringstream save_path;
            save_path<<img_path<<"/"<<class_id;
            if(RDK::CreateNewDirectory(save_path.str().c_str())==0)
            {
                RDK::UBitmap TempBitmap;
                bmp.ConvertTo(TempBitmap);
                TempBitmap.SwapRGBChannels();
                //TempBitmap.ReflectionX();

                jpge::params param;
                param.m_quality=100;

                save_path<<"/"<<iiii<<"_"<<class_id<<"_"<<max_conf<<".jpg";
                //   jpge::jpeg_encoder jpeg_e;
                jpge::compress_image_to_jpeg_file(save_path.str().c_str(), TempBitmap.GetWidth(), TempBitmap.GetHeight(), 3,
                                               TempBitmap.GetData(),param);

            }
        iiii++;
        }
    }

    return true;
}

}
#endif



