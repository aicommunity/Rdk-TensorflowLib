#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "UTfComponent.h"

namespace RDK {

class UTfClassifier: public UTfComponent
{
protected: // Параметры

protected: // Входы и выходы

/// Входное изображение
/// Игнорируется, если подключен векторный вход InputImages
UPropertyInputData<UBitmap,UTfClassifier, ptPubInput> InputImage;

/// Массив входных изображений
UPropertyInputData<std::vector<UBitmap>, UTfClassifier, ptPubInput> InputImages;

/// Количество классов объектов (какой размер будет у вектора)
ULProperty<int,UTfClassifier, ptPubParameter> NumClasses;

///Порог уверенности: если класс не превышает порога уверенности, то он выставляется в 0, все классы выставляются в 0.
///TODO: Это пихать ДО OneHot'а. И проверить, чтобы класс выдавало в виде '-1', а уверенности все 0
ULProperty<double,UTfClassifier, ptPubParameter> ConfidenceThreshold;

/// Выходная матрица с классами объектов
UPropertyOutputData<MDMatrix<int>,UTfClassifier, ptPubOutput> OutputClasses;

/// Выходная матрица. Количество столбцов по числу объектов, количество строк в столбце по числу классов
/// Каждое значение - уверенность класса
UPropertyOutputData<MDMatrix<double>, UTfClassifier> OutputConfidences;

/// Время, затраченное на классификацию, секунды
ULProperty<double,UTfClassifier, ptPubState> ClassificationTime;

UPropertyOutputData<UBitmap,UTfComponent, ptPubOutput> DebugImage;

protected: // Переменные состояния


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



