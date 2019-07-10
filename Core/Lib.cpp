#ifndef RDK_UTensorflowLib_CPP
#define RDK_UTensorflowLib_CPP

#include "UTensorflowLib.h"

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
 RDK::UContainer *cont(0);
/*
 cont=new UStatisticMatrix<int>;
 cont->SetName("StatisticIntMatrix");
 cont->Default();
 UploadClass("UStatisticIntMatrix",cont);
*/
}
// --------------------------

}

#endif


