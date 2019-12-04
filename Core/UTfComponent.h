#ifndef RDK_UTFCOMPONENT_H
#define RDK_UTFCOMPONENT_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"
namespace RDK {

class UTfComponent:public RDK::UNet
{

protected: // ���������
///���� ���������� �� ��� ����������� ������ (true - pb ������, false - meta+ckpt ������)
ULProperty<bool,UTfComponent, ptPubParameter> UsePb;

///���� � ������ ��������� ���� (pb ������)
ULProperty<std::string,UTfComponent, ptPubParameter> PbModelPath;

///���� � ������ ��������� ���� (meta+ckpt ������)
ULProperty<std::string,UTfComponent, ptPubParameter> MetaModelPath;

ULProperty<std::string,UTfComponent, ptPubParameter> CkptPath;

///��� �������� ���� ��������� ����
ULProperty<std::string,UTfComponent, ptPubParameter> InputNodeName;

///������ ��� �������� ����� ��������� ����
ULProperty<std::vector<std::string>,UTfComponent, ptPubParameter> OutputNodeName;

///�������� ��� ������ ����������� (������������)
ULProperty<float,UTfComponent> ImgDiv;

///���������� �� ������ ����������� (�����������)
ULProperty<std::vector<float>,UTfComponent, ptPubParameter> ImgSub;

///���� ���������� �� ��� ����������� ������ (true - pb ������, false - meta+ckpt ������)
ULProperty<bool,UTfComponent, ptPubParameter> UseBGR;

///���� ������������� ������ GPU
ULProperty<double,UTfComponent, ptPubParameter> GpuFraction;

///���� ���������� �� ��������� ������ GPU �� ���� �������������
ULProperty<bool,UTfComponent, ptPubParameter> GpuGrow;

///������ ��� ������������� ������� ��������� �����
TTF::TTfSession TfObject;
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


bool SetUsePb(const bool &value);

bool SetMetaModelPath(const std::string &value);

bool SetCkptPath(const std::string &value);

bool SetPbModelPath(const std::string &value);

bool SetInputNodeName(const std::string &value);

bool SetOutputNodeName(const std::vector<std::string> &value);

bool SetImgDiv(const float &value);

bool SetImgSub(const std::vector<float> &value);

bool SetUseBGR(const bool &value);

bool SetGpuFraction(const double &value);

bool SetGpuGrow(const bool &value);

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
virtual bool ACalculate(void);
virtual bool ATfCalculate(void)=0;
// --------------------------
};


}



#endif // RDK_UTFCOMPONENT_H
