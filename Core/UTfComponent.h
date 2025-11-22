#ifndef RDK_UTFCOMPONENT_H
#define RDK_UTFCOMPONENT_H

#include "../../../Rdk/Deploy/Include/rdk.h"
#include "Interface/ttfsession.h"
#include "Interface/ttfsessionSqDet.h"
namespace RDK {

class UTfComponent: virtual public RDK::UNet
{

protected: // 

///     (pb )
UProperty<std::string,UTfComponent, ptPubParameter> PbModelPath;

///    
UProperty<std::string,UTfComponent, ptPubParameter> InputNodeName;

///     
UProperty<std::vector<std::string>,UTfComponent, ptPubParameter> OutputNodeName;

///    ()
UProperty<float,UTfComponent, ptPubParameter> ImgDiv;

///    ()
UProperty<std::vector<float>,UTfComponent, ptPubParameter> ImgSub;

///     
UProperty<bool,UTfComponent, ptPubParameter> UseBGR;

///   GPU
UProperty<double,UTfComponent, ptPubParameter> GpuFraction;

///     
TTF::TTfSession* TfObject;
protected: //   


protected: //  
UProperty<std::string,UTfComponent,ptPubState> DebugString;

///    
UProperty<int,UTfComponent,ptPubState> ExpectedHeight;
UProperty<int,UTfComponent,ptPubState> ExpectedWidth;
UProperty<int,UTfComponent,ptPubState> ExpectedChannels;

///,    
bool BuildDone;

public: // 
// --------------------------
//   
// --------------------------
UTfComponent(void);
virtual ~UTfComponent(void);
// --------------------------

// ---------------------
//   
// ---------------------
// ---------------------

bool SetPbModelPath(const std::string &value);

bool SetInputNodeName(const std::string &value);

bool SetOutputNodeName(const std::vector<std::string> &value);

bool SetImgDiv(const float &value);

bool SetImgSub(const std::vector<float> &value);

bool SetUseBGR(const bool &value);

bool SetGpuFraction(const double &value);

// ---------------------
//    
// ---------------------
// ---------------------

// --------------------------

// --------------------------
//    
// --------------------------
protected:
//        
virtual bool ADefault(void);
virtual bool ATfDefault(void)=0;

//     
//   
//    Reset()   Ready  true
//    
virtual bool ABuild(void);
virtual bool ATfBuild(void)=0;

//      
virtual bool AReset(void);
virtual bool ATfReset(void)=0;


//    
virtual bool ABeforeCalculate(void);
// --------------------------
};


}



#endif // RDK_UTFCOMPONENT_H
