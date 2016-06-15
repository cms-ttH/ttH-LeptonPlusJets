ttH-LeptonPlusJets
==================

+++++++++++++++++
+++  Install  +++
+++++++++++++++++


cmsrel CMSSW_8_0_8_patch1
cd CMSSW_7_6_3/src/
cmsenv

#git clone git@github.com:hsatoshi/MiniAOD.git  -b satoshi_80x
#git clone git@github.com:hsatoshi/ttH-LeptonPlusJets.git -b satoshi_cleanup_80x
#git clone git@github.com:hsatoshi/GenParticleTopOriginChargedleptonFilter.git ttHAnalysisSubprogram/GenParticleTopOriginChargedleptonFilter

git clone https://github.com/hsatoshi/MiniAOD.git -b satoshi_80x
git clone https://github.com/hsatoshi/ttH-LeptonPlusJets.git -b satoshi_cleanup_80x
git clone https://github.com/hsatoshi/GenParticleTopOriginChargedleptonFilter.git ttHAnalysisSubprogram/GenParticleTopOriginChargedleptonFilter
scram b 


+++++++++++++++++
+++ Selection +++
+++++++++++++++++


Selection of electron
 - slimmedElectrons from miniAOD
 - No selection.

Selection of muon
 - slimmedMuons from miniAOD
 - No selection

Selection of jets
 - slimmedJets from iniAOD 
 - Pt > 20 GeV after JEC.
 - |Eta| < 5.


Selection of gen jets
 - this is independent from reco jet.
 - Input can be set with option "genjet", but usually 
 - PT cut 8 GeV.
 - no Eta cut


+++++++++++++++++
+++ Variables +++
+++++++++++++++++

Event

 run_  : run number
 lumi_ : lumi block
 evt_  : event number
 numTruePV_ : 
 numGenPV_  :

 GoodFirstPV_ :
 numPVs_ :
 numSys_ :

 additionalJetEventId_;

 higgsDecayType_;
 ttbarDecayType_;


Trigger 

passHLT_XXXX : trigger for the path. 1=pass



Lepton.

lepton_isMuon_  : If muon, 1. If electron, 0. 
lepton_isTight_ : POG Tight ID. 1=pass. [*1] 
lepton_isLoose_ : POG Loose ID. 1=pass. [*1]
lepton_pt_      : pt
lepton_eta_     : eta
lepton_phi_      : phi
lepton_relIso_   : delta_beta corrected relative isolation. cone 04.


Reconstructed ak4 jet.
 
jet_pt_  : pt
jet_phi_ : phi
jet_eta_ : eta
jet_m_   : mass
jet_combinedMVABJetTags_ : "pfCombinedMVAV2BJetTags"
jet_combinedInclusiveSecondaryVertexV2BJetTags_ : "pfCombinedInclusiveSecondaryVertexV2BJetTags"

jet_partonflavour_ : result of jet->partonFlavour()
jet_flavour_       : result of jet->hadronFlavour()

jet_AssociatedGenJet_pt_ : genJet associated with the reco jet
jet_AssociatedGenJet_eta_: genJet associated with the reco jet
jet_AssociatedGenJet_phi_: genJet associated with the reco jet
jet_AssociatedGenJet_m_  : genJet associated with the reco jet

jet_genId_
jet_genParentId_
jet_genGrandParentId_

jet_vtxMass_
jet_vtxNtracks_
jet_vtx3DVal_
jet_vtx3DSig_


GenJet ak4 (independent from reco jet):

genjet_pt_ : pt
genjet_eta_: eta
genjet_phi_: phi
genjet_m_  : mass
genjet_BhadronMatch_ : =1 if this gen jet is regarded as a B-hadron with by GenHFHadronMatcher --[*2]


[*1] POG lepton ID : 
 Muon POG loose ID is not used at the moment. Always 1.
 Electron POG tight=MVA80. Loose=MVA90.

[*2] GenHFHadronMatcher in PhysicsTools.JetMCAlgos.GenHFHadronMatcher_cff
