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
 UploadClass<UTfClassifier>("UTfClassifier", "ClassifierTF");
 UploadClass<UTfDetector>("UTfDetector", "DetectorTF");
 UploadClass<UTfSqDetector>("UTfSqDetector", "DetectorTFSq");
 //UploadClass<TAggrClassifierTF>("TAggrClassifierTF","AggrClassifierTF");
 UploadClass<UTrajectoriesSaver>("UTrajectoriesSaver","TrajectoriesSaver");

}
// --------------------------

}

#endif


