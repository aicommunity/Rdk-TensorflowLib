#ifndef RDK_UTensorflowLib_H
#define RDK_UTensorflowLib_H

#include "../../../Rdk/Deploy/Include/rdk.h"

#include "UTfClassifier.h"
#include "UTfDetector.h"
#include "TAggrClassifierTF.h"

namespace RDK {

class RDK_LIB_TYPE UTensorflowLib: public RDK::ULibrary
{
public: // Методы
// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTensorflowLib(void);
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
virtual void CreateClassSamples(RDK::UStorage *storage);
// --------------------------
};

extern RDK_LIB_TYPE UTensorflowLib TensorflowLib;

}

#endif


