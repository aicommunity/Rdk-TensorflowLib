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
    InputImage("InputImage",this),
    InputImages("InputImages",this),
    OutputClasses("OutputClasses",this),
    NumClasses("NumClasses",this),
    ConfidenceThreshold("ConfidenceThreshold", this),
    OutputConfidences("OutputConfidences", this),
    ClassificationTime("ClassificationTime",this),
    DebugImage("DebugImage",this)
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
    NumClasses=4;
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

// Выполняет расчет этого объекта
bool UTfClassifier::ATfCalculate(void)
{
    ClassificationTime=0.0;
    clock_t start_frame = clock();

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

    clock_t end_frame = clock();
    ClassificationTime = ((double) (end_frame - start_frame)) / CLOCKS_PER_SEC;
    return true;
}
// --------------------------

bool UTfClassifier::ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified)
{
    //Обрабатываем иображение для входа
    if(!TfObject.SetInputDataTfMeth(*InputImage))
    {
        DebugString=TfObject.GetDebugStr();
        return true;
    }

    //Запуск рассчёта нейросети
    if(!TfObject.Run())
    {
        DebugString=TfObject.GetDebugStr();
        return true;
    }

    std::vector<float> result;

    for(int i=0; i<TfObject.GetOutput()[0].dim_size(1); i++)
    {
        result.push_back(TfObject.GetOutput()[0].matrix<float>()(0,i));
    }

    if(result.size()!=NumClasses)
    {
        DebugString="Number of classes is wrong";
        return true;
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

    return true;
}

}
#endif



