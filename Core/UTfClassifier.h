#ifndef RDK_UTFCLASSIFIER_H
#define RDK_UTFCLASSIFIER_H

#include "UTfComponent.h"
#include "../../Rdk-CRLib/Core/UClassifierBase.h"
namespace RDK {

class UTfClassifier: public UClassifierBase, public UTfComponent
{
protected: // 


///     <config_path>/classification_result/ 
///      ( /)
UProperty<bool, UTfClassifier, ptPubParameter> SaveDebugResults;

protected: //   


protected: //  

TTF::TTfSession TfClassifier;

public: // 
// --------------------------
//   
// --------------------------
UTfClassifier(void);
virtual ~UTfClassifier(void);
// --------------------------

// ---------------------
//   
// ---------------------
// ---------------------
bool SetNumClasses(const int &value);
// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------
//    
// --------------------------
//         
virtual UTfClassifier* New(void);
// --------------------------

// --------------------------
//    
// --------------------------
protected:
//        
virtual bool ATfDefault(void);

//     
//   
//    Reset()   Ready  true
//    
virtual bool ATfBuild(void);

//      
virtual bool ATfReset(void);


// --------------------------
///   
virtual bool ClassifyBitmap(UBitmap &bmp, MDVector<double> &output_confidences, double conf_thresh, int &class_id, bool &is_classified);

};



}

#endif



