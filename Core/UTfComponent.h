#ifndef RDK_UTFCOMPONENT_H
#define RDK_UTFCOMPONENT_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"
namespace RDK {

class UTfComponent:public RDK::UNet
{

protected: // Параметры
///Флаг отвечающий за тип загружаемой модели (true - pb модель, false - meta+ckpt модель)
ULProperty<bool,UTfComponent, ptPubParameter> UsePb;

///Путь к модели нейронной сети (pb модель)
ULProperty<std::string,UTfComponent, ptPubParameter> PbModelPath;

///Пути к модели нейронной сети (meta+ckpt модель)
ULProperty<std::string,UTfComponent, ptPubParameter> MetaModelPath;

ULProperty<std::string,UTfComponent, ptPubParameter> CkptPath;

///Имя входного узла нейронной сети
ULProperty<std::string,UTfComponent, ptPubParameter> InputNodeName;

///Массив имён выходных узлов нейронной сети
ULProperty<std::vector<std::string>,UTfComponent, ptPubParameter> OutputNodeName;

///Делитель для данных изображения (нормализация)
ULProperty<float,UTfComponent> ImgDiv;

///Вычитаемое из данных изображения (изображения)
ULProperty<std::vector<float>,UTfComponent, ptPubParameter> ImgSub;

///Флаг отвечающий за тип загружаемой модели (true - pb модель, false - meta+ckpt модель)
ULProperty<bool,UTfComponent, ptPubParameter> UseBGR;

///Доля использования памяти GPU
ULProperty<double,UTfComponent, ptPubParameter> GpuFraction;

///Флаг отвечающий за выделение памяти GPU по мере необходимости
ULProperty<bool,UTfComponent, ptPubParameter> GpuGrow;

///Объект для использования моделей нейронных сетей
TTF::TTfSession TfObject;
protected: // Входы и выходы


protected: // Переменные состояния
ULProperty<std::string,UTfComponent,ptPubState> DebugString;

///Ожидаемые для сети параметры изображения
ULProperty<int,UTfComponent,ptPubState> ExpectedHeight;
ULProperty<int,UTfComponent,ptPubState> ExpectedWidth;
ULProperty<int,UTfComponent,ptPubState> ExpectedChannels;



///Флаг, отвечающий за успешную сборку
bool BuildDone;

public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTfComponent(void);
virtual ~UTfComponent(void);
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

bool SetImgSub(const std::vector<float> &value);

bool SetUseBGR(const bool &value);

bool SetGpuFraction(const double &value);

bool SetGpuGrow(const bool &value);

// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------

// --------------------------
// Скрытые методы управления счетом
// --------------------------
protected:
// Восстановление настроек по умолчанию и сброс процесса счета
virtual bool ADefault(void);
virtual bool ATfDefault(void)=0;

// Обеспечивает сборку внутренней структуры объекта
// после настройки параметров
// Автоматически вызывает метод Reset() и выставляет Ready в true
// в случае успешной сборки
virtual bool ABuild(void);
virtual bool ATfBuild(void)=0;

// Сброс процесса счета без потери настроек
virtual bool AReset(void);
virtual bool ATfReset(void)=0;

// Выполняет расчет этого объекта
virtual bool ACalculate(void);
virtual bool ATfCalculate(void)=0;
// --------------------------
};


}



#endif // RDK_UTFCOMPONENT_H
