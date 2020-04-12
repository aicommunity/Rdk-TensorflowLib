#ifndef RDK_TAGGRCLASSIFIERTF_H
#define RDK_TAGGRCLASSIFIERTF_H



#include "../../Rtv-ObjectClassificationLib/Core/TAggrClassifierBase.h"
#include "UTfClassifier.h"

namespace RDK {

class TAggrClassifierTF: public RTV::TAggrClassifierBase
{
protected: // Параметры

public: // Коммуникационные данные

/// Исходное изображение
UPropertyInputData<MDMatrix<int>,TAggrClassifierTF> InputClassifierResults;

/// Входной массив с уверенностями всех обработанных изображений
UPropertyInputData<MDMatrix<double>, TAggrClassifierTF> InputConfidences;

protected: // Данные

int NumClasses;
UTfClassifier UBitmapClassifier;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
TAggrClassifierTF(void);
virtual ~TAggrClassifierTF(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool SetNumClasses(const int& num);
const int &GetNumClasses(void) const;
// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual TAggrClassifierTF* New(void);
// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault2(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild2(void);

// Сброс процесса счета.
virtual bool AReset2(void);

// Расчет компонента
virtual bool ACalculate2(void);

// --------------------------
/// Осуществляет распознавание объекта на изображении
/// В векторе result возвращает оценка принадлежности объекта каждому из классов
virtual bool Recognize(MDVector<double> &result);
};



}
#endif // RDK_TAGGRCLASSIFIERTF_H
