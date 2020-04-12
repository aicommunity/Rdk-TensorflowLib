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

protected: // ���������

    ///��������� �� ������ � ������, ���� ����� ���������
    ULProperty<bool, UTrajectoriesSaver, ptPubParameter> OnlyDefined;


protected: // ����� � ������
    /// ������ ���������
    UPropertyInputData<RTV::TAggrVector,UTrajectoriesSaver> PAllAggregates;

    /// ������ ����������
    UPropertyInputData<RTV::TAggrTrjVector,UTrajectoriesSaver> PTrajectories;

    /// �������� �����������
    UPropertyInputData<UBitmap,UTrajectoriesSaver> InputImage;

protected: // ���������� ���������


public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UTrajectoriesSaver(void);
virtual ~UTrajectoriesSaver(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------


bool SetOnlyDefined(const bool &value);


// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual UTrajectoriesSaver* New(void);
// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool AReset(void);

// ��������� ������ ����� �������
virtual bool ACalculate(void);

// --------------------------
};


}



#endif // RDK_UTRAJECTORIESSAVER_H
