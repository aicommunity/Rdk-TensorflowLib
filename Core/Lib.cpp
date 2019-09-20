#ifndef RDK_UTensorflowLib_CPP
#define RDK_UTensorflowLib_CPP

#include "Lib.h"

namespace RDK {

UTensorflowLib TensorflowLib;



// --------------------------
// Конструкторы и деструкторы
// --------------------------
UTensorflowLib::UTensorflowLib(void)
 : RDK::ULibrary("TensorflowLib","1.0")
{
}
// --------------------------

// --------------------------
// Методы заполенения бибилиотеки
// --------------------------
// Заполняет массив ClassSamples готовыми экземплярами образцов и их именами.
// Не требуется предварительная очистка массива и уборка памяти.
void UTensorflowLib::CreateClassSamples(RDK::UStorage *storage)
{
 UploadClass<UTfClassifier>("TClassifier", "Classifier");
}
// --------------------------

}

#endif


