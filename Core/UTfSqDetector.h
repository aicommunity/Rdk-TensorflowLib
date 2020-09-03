#ifndef RDK_UTFSQDETECTOR_H
#define RDK_UTSQFDETECTOR_H

#include "UTfComponent.h"
#include "../../Rdk-CRLib/Core/UDetectorBase.h"

namespace RDK {

class UTfSqDetector:  public UDetectorBase, public UTfComponent
{
protected: // Параметры


///Рисовать или не рисовать отладочные изображения
ULProperty<std::string,UTfSqDetector, ptPubParameter> ConfigPath;
protected: // Входы и выходы



protected: // Переменные состояния

TTF::TTfSessionSqDet TfDetector;

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


// --------------------------

virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
};



}
#endif // RDK_UTFSQDETECTOR_H
