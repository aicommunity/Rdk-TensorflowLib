#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"

namespace RDK {

class UTfClassifier: public RDK::UNet
{
protected: // Параметры
///Флаг отвечающий за тип загружаемой модели (true - pb модель, false - meta+ckpt модель)
ULProperty<bool,UTfClassifier> UsePb;

///Путь к модели нейронной сети (pb модель)
ULProperty<std::string,UTfClassifier> PbModelPath;

///Пути к модели нейронной сети (meta+ckpt модель)
ULProperty<std::string,UTfClassifier> MetaModelPath;

ULProperty<std::string,UTfClassifier> CkptPath;

///Имя входного узла нейронной сети
ULProperty<std::string,UTfClassifier> InputNodeName;

///Массив имён выходных узлов нейронной сети
ULProperty<std::vector<std::string>,UTfClassifier> OutputNodeName;

///Делитель для данных изображения (нормализация)
ULProperty<float,UTfClassifier> ImgDiv;

///Вычитаемое из данных изображения (изображения)
ULProperty<float,UTfClassifier> ImgSub;

///Доля использования памяти GPU
ULProperty<double,UTfClassifier> GpuFraction;

///Флаг отвечающий за выделение памяти GPU по мере необходимости
ULProperty<bool,UTfClassifier> GpuGrow;

///Объект для использования моделей нейронных сетей
TTF::TTfSession TfObject;
protected: // Входы и выходы
UPropertyInputData<UBitmap,UTfClassifier, ptPubInput> InputImage;

UPropertyOutputData<UBitmap,UTfClassifier> DebugImage;

protected: // Переменные состояния

ULProperty<int,UTfClassifier,ptPubState> NumberOfClass;

ULProperty<float,UTfClassifier,ptPubState> DebugFloat;

ULProperty<std::string,UTfClassifier,ptPubState> DebugString;

///Флаг, отвечающий за успешную сборку
bool BuildDone;

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


bool SetUsePb(const bool &value);

bool SetMetaModelPath(const std::string &value);

bool SetCkptPath(const std::string &value);

bool SetPbModelPath(const std::string &value);

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



