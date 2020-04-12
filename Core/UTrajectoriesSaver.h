#ifndef RDK_UTRAJECTORIESSAVER_H
#define RDK_UTRAJECTORIESSAVER_H

#include "../../../Rdk/Deploy/Include/rdk.h"


#include "../../../Rdk/Core/Engine/UNet.h"
#include "../../Rtv-BasicLib/Core/RTVSupport.h"
#include "../../Rtv-BasicLib/Core/RTVBinarySerialize.h"
#include "../../Rtv-DetectionLib/Core/RTVALabeling.h"
#include "../../Rdk-CRLib/Core/UCRSample.h"
#include "../../Rdk-CRLib/Core/UCRPerseptron.h"

namespace RDK {

using namespace RTV;

class UTrajectoriesSaver : public RDK::UNet
{

protected: // Параметры

    ///Сохранять ли только в случае, если класс определен
    ULProperty<bool, UTrajectoriesSaver, ptPubParameter> OnlyDefined;


protected: // Входы и выходы
    /// Список агрегатов
    UPropertyInputData<RTV::TAggrVector,UTrajectoriesSaver> PAllAggregates;

    /// Список траекторий
    UPropertyInputData<RTV::TAggrTrjVector,UTrajectoriesSaver> PTrajectories;

    /// Исходное изображение
    UPropertyInputData<UBitmap,UTrajectoriesSaver> InputImage;

protected: // Переменные состояния


public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTrajectoriesSaver(void);
virtual ~UTrajectoriesSaver(void);
// --------------------------

// ---------------------
// Методы управления параметрами
// ---------------------
// ---------------------


bool SetOnlyDefined(const bool &value);


// ---------------------
// Методы управления переменными состояния
// ---------------------
// ---------------------

// --------------------------
// Системные методы управления объектом
// --------------------------
// Выделяет память для новой чистой копии объекта этого класса
virtual UTrajectoriesSaver* New(void);
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



#endif // RDK_UTRAJECTORIESSAVER_H
