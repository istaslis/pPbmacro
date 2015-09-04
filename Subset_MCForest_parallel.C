#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "Helpers.h"

vector<TString> branchesMuon = {
  "Gen_nptl","Gen_pt","Gen_eta","Gen_phi",
  "Glb_nptl","Glb_pt","Glb_eta","Glb_phi", 
  "Glb_glbChi2_ndof","Glb_nValMuHits","Glb_nMatchedStations","Glb_nValPixHits",
  "Glb_trkLayerWMeas","Glb_trkDxy","Glb_trkDz","Glb_nValTrkHits","Glb_dxy","Glb_dz"
};

vector<TString> branchesEvtAna = {
  "vx","vy","vz"
};

vector<TString> branchesJet = {
  "evt", "b", 
  
  "nref",
  "rawpt",
  "jtpt", "jteta", "jty", "jtphi",
  "discr_csvSimple","discr_prob","svtxm", "discr_ssvHighPur","discr_ssvHighEff",
  
  "nIP","ipProb0","ipJetIndex",

  "mue", "mupt", "mueta", "muphi", "mudr", "muptrel", "muchg",
  
  "pthat",
  
  "refpt", "refeta", "refy", "refphi", "refparton_pt", "refparton_flavorForB",
  "ngen", "genmatchindex", "genpt", "geneta", "geny", "genphi"
};


void Subset_MCForest_parallel(int ind) {
  const int Nbins=4;
  Int_t bounds[Nbins] = {30,50,80,120};

  TString sample = "QCDPPb";
  TString finname = Form("/data_CMS/cms/mnguyen/pPbAna/%s/pthat%d/merged_HiForest.root",sample.Data(), bounds[ind]);
  TString foutname = Form("tempout%d.root",bounds[ind]);

  vector<TString> treeout = {"jet","muon","evt"};
  vector<TString> treein  = {"akPu4PFJetAnalyzer/t","hltMuTree/HLTMuTree","hiEvtAnalyzer/HiTree"};
  auto br = {branchesJet,branchesMuon,branchesEvtAna};


  SubsetMultipleTrees(foutname, treeout, finname, treein, br);


}

