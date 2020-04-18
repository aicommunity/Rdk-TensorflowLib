#ifndef RDK_UTensorflowLib_CPP
#define RDK_UTensorflowLib_CPP

#include "Lib.h"

namespace RDK {

UTensorflowLib TensorflowLib;



// --------------------------
// ������������ � �����������
// --------------------------
UTensorflowLib::UTensorflowLib(void)
 : RDK::ULibrary("TensorflowLib","1.0")
{
}
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
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


