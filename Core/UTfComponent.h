#ifndef RDK_UTFCOMPONENT_H
#define RDK_UTFCOMPONENT_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"
#include "Interface/ttfsessionSqDet.h"
namespace RDK {

class UTfComponent: virtual public RDK::UNet
{

protected: // ���������

///���� � ������ ��������� ���� (pb ������)
ULProperty<std::string,UTfComponent, ptPubParameter> PbModelPath;

///��� �������� ���� ��������� ����
ULProperty<std::string,UTfComponent, ptPubParameter> InputNodeName;

///������ ��� �������� ����� ��������� ����
ULProperty<std::vector<std::string>,UTfComponent, ptPubParameter> OutputNodeName;

///�������� ��� ������ ����������� (������������)
ULProperty<float,UTfComponent> ImgDiv;

///���������� �� ������ ����������� (�����������)
ULProperty<std::vector<float>,UTfComponent, ptPubParameter> ImgSub;

///���� ���������� �� ������ ������������� �����������
ULProperty<bool,UTfComponent, ptPubParameter> UseBGR;

///���� ������������� ������ GPU
ULProperty<double,UTfComponent, ptPubParameter> GpuFraction;

///������ ��� ������������� ������� ��������� �����
TTF::TTfSession* TfObject;
protected: // ����� � ������


protected: // ���������� ���������
ULProperty<std::string,UTfComponent,ptPubState> DebugString;

///��������� ��� ���� ��������� �����������
ULProperty<int,UTfComponent,ptPubState> ExpectedHeight;
ULProperty<int,UTfComponent,ptPubState> ExpectedWidth;
ULProperty<int,UTfComponent,ptPubState> ExpectedChannels;

///����, ���������� �� �������� ������
bool BuildDone;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UTfComponent(void);
virtual ~UTfComponent(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

bool SetPbModelPath(const std::string &value);

bool SetInputNodeName(const std::string &value);

bool SetOutputNodeName(const std::vector<std::string> &value);

bool SetImgDiv(const float &value);

bool SetImgSub(const std::vector<float> &value);

bool SetUseBGR(const bool &value);

bool SetGpuFraction(const double &value);

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault(void);
virtual bool ATfDefault(void)=0;

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild(void);
virtual bool ATfBuild(void)=0;

// ����� �������� ����� ��� ������ ��������
virtual bool AReset(void);
virtual bool ATfReset(void)=0;


// ��������� ������ ����� �������
virtual bool ABeforeCalculate(void);
// --------------------------
};


}



#endif // RDK_UTFCOMPONENT_H
