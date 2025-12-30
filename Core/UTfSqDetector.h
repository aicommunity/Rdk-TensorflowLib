#ifndef RDK_UTFSQDETECTOR_H
#define RDK_UTSQFDETECTOR_H

#include "UTfComponent.h"
#include "../../Rdk-CRLib/Core/UDetectorBase.h"

namespace RDK {

class UTfSqDetector:  public UDetectorBase, public UTfComponent
{
protected: // 


///     
UProperty<std::string,UTfSqDetector, ptPubParameter> ConfigPath;
protected: //   



protected: //  

TTF::TTfSessionSqDet TfDetector;

public: // 
// --------------------------
//   
// --------------------------
UTfSqDetector(void);
virtual ~UTfSqDetector(void);
// --------------------------

// ---------------------
//   
// ---------------------
// ---------------------
bool SetConfigPath(const std::string &value);
// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------
//    
// --------------------------
//         
virtual UTfSqDetector* New(void);
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

virtual bool Detect(UBitmap &bmp, MDMatrix<double> &output_rects, MDMatrix<int> &output_classes, MDMatrix<double> &reliabilities);
};



}
#endif // RDK_UTFSQDETECTOR_H
