#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "UTfComponent.h"
#include "../../Rdk-CRLib/Core/UClassifierBase.h"
namespace RDK {

class UTfClassifier: public UClassifierBase, public UTfComponent
{
protected: // Параметры


/// Сохранять в отладочную папку <config_path>/classification_result/номер класса
/// по получившимся классам результаты классификации (для анализа/отладки)
ULProperty<bool, UTfClassifier> SaveDebugResults;

protected: // Входы и выходы


protected: // Переменные состояния

TTF::TTfSession TfClassifier;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfClassifier(void);
virtual ~UTfClassifier(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool SetNumClasses(const int &value);
// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual UTfClassifier* New(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ATfDefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ATfBuild(void);

// Сброс процесса счета без потери настроек
virtual bool ATfReset(void);

// Выполняет расчет этого объекта
virtual bool ATfCalculate(void);
// --------------------------
/// Обрабатывает одно изображение
virtual bool ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified);

};



}

#endif



