#ifndef RDK_UTFSQDETECTOR_H
#define RDK_UTSQFDETECTOR_H

#include "UTfComponent.h"

namespace RDK {

class UTfSqDetector: public UTfComponent
{
protected: // ���������

///�������� �� ���������� ���������� ��������� �� ������ � ������
ULProperty<bool,UTfSqDetector, ptPubParameter> UseRelativeCoords;

///�������� ��� �� �������� ���������� �����������
ULProperty<bool,UTfSqDetector, ptPubParameter> UseDebugImage;

///�������� ��� �� �������� ���������� �����������
ULProperty<std::string,UTfSqDetector, ptPubParameter> ConfigPath;
protected: // ����� � ������

/// ������� �����������
UPropertyInputData<UBitmap, UTfSqDetector> InputImage;

/// �������� ���������� �����������
UPropertyOutputData<UBitmap, UTfSqDetector> DebugImage;

/// �������� �������. ���������� ����� �� ����� ��������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, UTfSqDetector, ptOutput | ptPubState> OutputObjects;

/// �������� ������� ������ ��������������� � ��������� ������������
UPropertyOutputData<MDMatrix<double>, UTfSqDetector, ptOutput | ptPubState> OutputRects;

/// �������� ������� ��������������� �������
UPropertyOutputData<MDMatrix<int>, UTfSqDetector, ptOutput | ptPubState> OutputClasses;

/// �������� ������� ������ ��������������
UPropertyOutputData<MDMatrix<double>, UTfSqDetector, ptOutput | ptPubState> OutputReliability;

protected: // ���������� ���������

TTF::TTfSessionSqDet TfDetector;

UGraphics Graph;
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UTfSqDetector(void);
virtual ~UTfSqDetector(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------
bool SetConfigPath(const std::string &value);
// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual UTfSqDetector* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ATfDefault(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ATfBuild(void);

// ����� �������� ����� ��� ������ ��������
virtual bool ATfReset(void);

// ��������� ������ ����� �������
virtual bool ATfCalculate(void);
// --------------------------

virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
};



}
#endif // RDK_UTFSQDETECTOR_H
