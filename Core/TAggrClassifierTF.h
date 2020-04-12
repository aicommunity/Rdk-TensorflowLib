#ifndef RDK_TAGGRCLASSIFIERTF_H
#define RDK_TAGGRCLASSIFIERTF_H



#include "../../Rtv-ObjectClassificationLib/Core/TAggrClassifierBase.h"
#include "UTfClassifier.h"

namespace RDK {

class TAggrClassifierTF: public RTV::TAggrClassifierBase
{
protected: // ���������

public: // ���������������� ������

/// �������� �����������
UPropertyInputData<MDMatrix<int>,TAggrClassifierTF> InputClassifierResults;

/// ������� ������ � ������������� ���� ������������ �����������
UPropertyInputData<MDMatrix<double>, TAggrClassifierTF> InputConfidences;

protected: // ������

int NumClasses;
UTfClassifier UBitmapClassifier;

public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
TAggrClassifierTF(void);
virtual ~TAggrClassifierTF(void);
// --------------------------

// ---------------------
// ������ ���������� �����������
// ---------------------
// ---------------------
bool SetNumClasses(const int& num);
const int &GetNumClasses(void) const;
// ---------------------
// ������ ���������� ����������� ���������
// ---------------------
// ---------------------

// --------------------------
// ��������� ������ ���������� ��������
// --------------------------
// �������� ������ ��� ����� ������ ����� ������� ����� ������
virtual TAggrClassifierTF* New(void);
// --------------------------

// --------------------------
// ������� ������ ���������� ������
// --------------------------
protected:
// �������������� �������� �� ��������� � ����� �������� �����
virtual bool ADefault2(void);

// ������������ ������ ���������� ��������� �������
// ����� ��������� ����������
// ������������� �������� ����� Reset() � ���������� Ready � true
// � ������ �������� ������
virtual bool ABuild2(void);

// ����� �������� �����.
virtual bool AReset2(void);

// ������ ����������
virtual bool ACalculate2(void);

// --------------------------
/// ������������ ������������� ������� �� �����������
/// � ������� result ���������� ������ �������������� ������� ������� �� �������
virtual bool Recognize(MDVector<double> &result);
};



}
#endif // RDK_TAGGRCLASSIFIERTF_H
