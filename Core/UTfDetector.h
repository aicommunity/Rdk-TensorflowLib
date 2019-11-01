#ifndef RDK_UTFDETECTOR_H
#define RDK_UTFDETECTOR_H

#include "UTfComponent.h"

namespace RDK {

class UTfDetector: public UTfComponent
{
protected: // Параметры

///Отвечает за домножение полученных координат на ширину и высоту
ULProperty<bool,UTfDetector, ptPubParameter> UseRelativeCoords;

///Рисовать или не рисовать отладочные изображения
ULProperty<bool,UTfDetector, ptPubParameter> UseDebugImage;

protected: // Входы и выходы

/// Входное изображение
UPropertyInputData<UBitmap, UTfDetector> InputImage;

/// Выходное отладочное изображение
UPropertyOutputData<UBitmap, UTfDetector> DebugImage;

/// Выходная матрица. Количество строк по числу объектов
/// Формат матрицы:
/// Высота по количеству объектов
/// Ширина 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, UTfDetector, ptOutput | ptPubState> OutputObjects;

/// Выходная матрица только прямоугольников с экранными координатами
UPropertyOutputData<MDMatrix<double>, UTfDetector, ptOutput | ptPubState> OutputRects;

/// Выходная матрица идентификаторов классов
UPropertyOutputData<MDMatrix<int>, UTfDetector, ptOutput | ptPubState> OutputClasses;

/// Выходная матрица оценок достоверностей
UPropertyOutputData<MDMatrix<double>, UTfDetector, ptOutput | ptPubState> OutputReliability;

protected: // Переменные состояния

UGraphics Graph;
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfDetector(void);
virtual ~UTfDetector(void);
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
virtual UTfDetector* New(void);
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

virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
};



}
#endif // RDK_UTFDETECTOR_H
