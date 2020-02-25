#ifndef RDK_UTFSQDETECTOR_H
#define RDK_UTSQFDETECTOR_H

#include "UTfComponent.h"

namespace RDK {

class UTfSqDetector: public UTfComponent
{
protected: // Параметры

///Отвечает за домножение полученных координат на ширину и высоту
ULProperty<bool,UTfSqDetector, ptPubParameter> UseRelativeCoords;

///Рисовать или не рисовать отладочные изображения
ULProperty<bool,UTfSqDetector, ptPubParameter> UseDebugImage;

///Рисовать или не рисовать отладочные изображения
ULProperty<std::string,UTfSqDetector, ptPubParameter> ConfigPath;
protected: // Входы и выходы

/// Входное изображение
UPropertyInputData<UBitmap, UTfSqDetector> InputImage;

/// Выходное отладочное изображение
UPropertyOutputData<UBitmap, UTfSqDetector> DebugImage;

/// Выходная матрица. Количество строк по числу объектов
/// Формат матрицы:
/// Высота по количеству объектов
/// Ширина 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, UTfSqDetector, ptOutput | ptPubState> OutputObjects;

/// Выходная матрица только прямоугольников с экранными координатами
UPropertyOutputData<MDMatrix<double>, UTfSqDetector, ptOutput | ptPubState> OutputRects;

/// Выходная матрица идентификаторов классов
UPropertyOutputData<MDMatrix<int>, UTfSqDetector, ptOutput | ptPubState> OutputClasses;

/// Выходная матрица оценок достоверностей
UPropertyOutputData<MDMatrix<double>, UTfSqDetector, ptOutput | ptPubState> OutputReliability;

protected: // Переменные состояния

TTF::TTfSessionSqDet TfDetector;

UGraphics Graph;
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfSqDetector(void);
virtual ~UTfSqDetector(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------
bool SetConfigPath(const std::string &value);
// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual UTfSqDetector* New(void);
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
#endif // RDK_UTFSQDETECTOR_H
