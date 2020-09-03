#ifndef RDK_UTFDETECTOR_H
#define RDK_UTFDETECTOR_H

#include "UTfComponent.h"
#include "../../Rdk-CRLib/Core/UDetectorBase.h"
namespace RDK {

class UTfDetector: public UDetectorBase, public UTfComponent
{
protected: // ���������



protected: // ����� � ������


protected: // ���������� ���������
TTF::TTfSession TfDetector;

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

// --------------------------

virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
};



}
#endif // RDK_UTFDETECTOR_H
