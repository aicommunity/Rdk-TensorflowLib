#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "UTfComponent.h"

namespace RDK {

class UTfClassifier: public UTfComponent
{
protected: // ���������

protected: // ����� � ������

/// ������� �����������
/// ������������, ���� ��������� ��������� ���� InputImages
UPropertyInputData<UBitmap,UTfClassifier, ptPubInput> InputImage;

/// ������ ������� �����������
UPropertyInputData<std::vector<UBitmap>, UTfClassifier, ptPubInput> InputImages;

/// ���������� ������� �������� (����� ������ ����� � �������)
ULProperty<int,UTfClassifier, ptPubParameter> NumClasses;

///����� �����������: ���� ����� �� ��������� ������ �����������, �� �� ������������ � 0, ��� ������ ������������ � 0.
///TODO: ��� ������ �� OneHot'�. � ���������, ����� ����� �������� � ���� '-1', � ����������� ��� 0
ULProperty<double,UTfClassifier, ptPubParameter> ConfidenceThreshold;

/// �������� ������� � �������� ��������
UPropertyOutputData<MDMatrix<int>,UTfClassifier, ptPubOutput> OutputClasses;

/// �������� �������. ���������� �������� �� ����� ��������, ���������� ����� � ������� �� ����� �������
/// ������ �������� - ����������� ������
UPropertyOutputData<MDMatrix<double>, UTfClassifier> OutputConfidences;

/// �����, ����������� �� �������������, �������
ULProperty<double,UTfClassifier, ptPubState> ClassificationTime;

UPropertyOutputData<UBitmap,UTfComponent, ptPubOutput> DebugImage;

protected: // ���������� ���������


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



