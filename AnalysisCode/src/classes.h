//Add includes for your classes here
#include "DataFormats/Common/interface/Wrapper.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANeventVars.h"
#include <vector>

namespace {
   struct ttH_LeptonPlusJets_AnalysisCode{
  
     //add 'dummy' Wrapper variable for each class type you put into the Event
     eventVars eventVarsDummy0;
     std::vector<eventVars> eventVarsDummy1;  
     edm::Wrapper<eventVars> eventVarsDumm2;
     edm::Wrapper<std::vector<eventVars> > eventVarsDummy4;

   };
}
