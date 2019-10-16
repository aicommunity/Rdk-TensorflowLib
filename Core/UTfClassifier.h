#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"

namespace RDK {

class UTfClassifier: public RDK::UNet
{
protected: // ���������
ULProperty<std::string,UTfClassifier> ModelPath;

ULProperty<std::string,UTfClassifier> InputNodeName;

ULProperty<std::vector<std::string>,UTfClassifier> OutputNodeName;

ULProperty<float,UTfClassifier> ImgDiv;

ULProperty<float,UTfClassifier> ImgSub;

ULProperty<double,UTfClassifier> GpuFraction;

ULProperty<bool,UTfClassifier> GpuGrow;

TTF::TTfSession TfObject;
protected: // ����� � ������
UPropertyInputData<UBitmap,UTfClassifier, ptPubInput> InputImage;

UPropertyOutputData<UBitmap,UTfClassifier> DebugImage;

protected: // ���������� ���������
ULProperty<int,UTfClassifier,ptPubState> NumberOfClass;
ULProperty<float,UTfClassifier,ptPubState> DebugFloat;
ULProperty<std::string,UTfClassifier,ptPubState> DebugString;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UTfClassifier(void);
virtual ~UTfClassifier(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------
bool SetModelPath(const std::string &value);

bool SetInputNodeName(const std::string &value);

bool SetOutputNodeName(const std::vector<std::string> &value);

bool SetImgDiv(const float &value);

bool SetImgSub(const float &value);

bool SetGpuFraction(const double &value);

bool SetGpuGrow(const bool &value);

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual UTfClassifier* New(void);
// --------------------------

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

#endif



