#ifndef RDK_UTFDETECTOR_H
#define RDK_UTFDETECTOR_H

#include "UTfComponent.h"

namespace RDK {

class UTfDetector: public UTfComponent
{
protected: // ���������

///�������� �� ���������� ���������� ��������� �� ������ � ������
ULProperty<bool,UTfDetector, ptPubParameter> UseRelativeCoords;

///�������� ��� �� �������� ���������� �����������
ULProperty<bool,UTfDetector, ptPubParameter> UseDebugImage;

protected: // ����� � ������

/// ������� �����������
UPropertyInputData<UBitmap, UTfDetector> InputImage;

/// �������� ���������� �����������
UPropertyOutputData<UBitmap, UTfDetector> DebugImage;

/// �������� �������. ���������� ����� �� ����� ��������
/// ������ �������:
/// ������ �� ���������� ��������
/// ������ 4+1=Left; Top; Right; Bottom; ClassNumber
UPropertyOutputData<MDMatrix<double>, UTfDetector, ptOutput | ptPubState> OutputObjects;

/// �������� ������� ������ ��������������� � ��������� ������������
UPropertyOutputData<MDMatrix<double>, UTfDetector, ptOutput | ptPubState> OutputRects;

/// �������� ������� ��������������� �������
UPropertyOutputData<MDMatrix<int>, UTfDetector, ptOutput | ptPubState> OutputClasses;

/// �������� ������� ������ ��������������
UPropertyOutputData<MDMatrix<double>, UTfDetector, ptOutput | ptPubState> OutputReliability;

protected: // ���������� ���������

UGraphics Graph;
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
UTfDetector(void);
virtual ~UTfDetector(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------

// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual UTfDetector* New(void);
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
#endif // RDK_UTFDETECTOR_H
