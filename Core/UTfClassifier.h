#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"

namespace RDK {

class UTfClassifier: public RDK::UNet
{
protected: // Параметры
ULProperty<std::string,UTfClassifier> ModelPath;

ULProperty<std::string,UTfClassifier> InputNodeName;

ULProperty<std::vector<std::string>,UTfClassifier> OutputNodeName;

ULProperty<float,UTfClassifier> ImgDiv;

ULProperty<float,UTfClassifier> ImgSub;

ULProperty<double,UTfClassifier> GpuFraction;

ULProperty<bool,UTfClassifier> GpuGrow;

TTF::TTfSession TfObject;
protected: // Входы и выходы
UPropertyInputData<UBitmap,UTfClassifier, ptPubInput> InputImage;

UPropertyOutputData<UBitmap,UTfClassifier> DebugImage;

protected: // Переменные состояния
ULProperty<int,UTfClassifier,ptPubState> NumberOfClass;
ULProperty<float,UTfClassifier,ptPubState> DebugFloat;
ULProperty<std::string,UTfClassifier,ptPubState> DebugString;

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
bool SetModelPath(const std::string &value);

bool SetInputNodeName(const std::string &value);

bool SetOutputNodeName(const std::vector<std::string> &value);

bool SetImgDiv(const float &value);

bool SetImgSub(const float &value);

bool SetGpuFraction(const double &value);

bool SetGpuGrow(const bool &value);

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
virtual bool ADefault(void);

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);

// Сброс процесса счета без потери настроек
virtual bool AReset(void);

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
// --------------------------
};


}

#endif



