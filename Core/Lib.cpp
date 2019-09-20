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
 UploadClass<UTfClassifier>("TClassifier", "Classifier");
}
// --------------------------

}

#endif


