#ifndef RDK_UTensorflowLib_H
#define RDK_UTensorflowLib_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "../../../Rtv/Deploy/Include/rtv.h"

#include "UTfClassifier.h"
#include "UTfDetector.h"
#include "UTfSqDetector.h"


namespace RDK {

class RDK_LIB_TYPE UTensorflowLib: public RDK::ULibrary
{
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UTensorflowLib(void);
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
virtual void CreateClassSamples(RDK::UStorage *storage);
// --------------------------
};

extern RDK_LIB_TYPE UTensorflowLib TensorflowLib;

}

#endif


