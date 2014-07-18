//Add includes for your classes here
#include "DataFormats/Common/interface/Wrapper.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANeventVars.h"
#include "ttH-LeptonPlusJets/AnalysisCode/interface/BEANeventVars_NLO.h"
#include <vector>

namespace {
   struct ttH_LeptonPlusJets_AnalysisCode{
  
     //add 'dummy' Wrapper variable for each class type you put into the Event
     eventVars eventVarsDummy0;
     std::vector<eventVars> eventVarsDummy1;  
     edm::Wrapper<eventVars> eventVarsDumm2;
     edm::Wrapper<std::vector<eventVars> > eventVarsDummy4;

     BEANeventVars_NLO BEANeventVars_NLODummy0;
     std::vector<BEANeventVars_NLO> BEANeventVars_NLODummy1;  
     edm::Wrapper<BEANeventVars_NLO> BEANeventVars_NLODumm2;
     edm::Wrapper<std::vector<BEANeventVars_NLO> > BEANeventVars_NLODummy4;

   };
}
