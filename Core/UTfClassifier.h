#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "UTfComponent.h"

namespace RDK {

class UTfClassifier: public UTfComponent
{
protected: // ���������

///����� �����������: ���� ����� �� ��������� ������ �����������, �� �� ������������ � 0, ��� ������ ������������ � 0.
///TODO: ��� ������ �� OneHot'�. � ���������, ����� ����� �������� � ���� '-1', � ����������� ��� 0
ULProperty<double,UTfClassifier, ptPubParameter> ConfidenceThreshold;

/// ���������� ������� �������� (����� ������ ����� � �������)
ULProperty<int,UTfClassifier, ptPubParameter> NumClasses;

protected: // ����� � ������

/// ������� �����������
/// ������������, ���� ��������� ��������� ���� InputImages
UPropertyInputData<UBitmap,UTfClassifier, ptPubInput> InputImage;

/// ������ ������� �����������
UPropertyInputData<std::vector<UBitmap>, UTfClassifier, ptPubInput> InputImages;

/// �������� ������� � �������� ��������
UPropertyOutputData<MDMatrix<int>,UTfClassifier, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������� - ����������� ������
UPropertyOutputData<MDMatrix<double>, UTfClassifier, ptPubOutput> OutputConfidences;

UPropertyOutputData<UBitmap,UTfClassifier, ptPubOutput> DebugImage;

protected: // ���������� ���������
/// �����, ����������� �� �������������, �������
ULProperty<double,UTfClassifier, ptPubState> ClassificationTime;

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
/// ������������ ���� �����������
virtual bool ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified);

};



}

#endif



