#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include <algorithm>
#include "TStopwatch.h"
#include "TTimeStamp.h"
#include <functional>
#include "HelperProcess.C"

double dR(float phi1, float eta1, float phi2, float eta2)
{
  return sqrt((phi2-phi1)*(phi2-phi1) + (eta2-eta1)*(eta2-eta1));
}

void getP3(float pt, float eta, float phi, float &px, float &py, float &pz)
{
  px=pt*cos(phi);
  py=pt*sin(phi);
  pz=pt*sinh(eta);
}

//look here for details: https://github.com/CmsHI/cmssw/blob/forest_CMSSW_7_5_0_pre5/HeavyIonsAnalysis/JetAnalysis/src/HiInclusiveJetAnalyzer.cc#L1279
double getPtRel(float jetpt, float jeteta, float jetphi, float muonpt, float muoneta, float muonphi)
{
  float jetpx = jetpt*cos(jetphi);
  float jetpy = jetpt*sin(jetphi);
  float jetpz = jetpt*sinh(jeteta);

  float muonpx = muonpt*cos(muonphi);
  float muonpy = muonpt*sin(muonphi);
  float muonpz = muonpt*sinh(muoneta);

  float jetp2 = jetpx*jetpx+jetpy*jetpy+jetpz*jetpz;
  float muonp2 = muonpx*muonpx+muonpy*muonpy+muonpz*muonpz;

  float jetdotmuon = jetpx*muonpx+jetpy*muonpy+jetpz*muonpz;

  float pLrel2 = jetdotmuon*jetdotmuon/jetp2;
  float pTrel2 = muonp2-pLrel2;

  return (pTrel2 > 0) ? sqrt(pTrel2) : 0.0;
}

void ProcessEvent(Everything &ev, Everything &evout)
{
  for (int j=0;j<ev.GetInt("nref");j++) {

    float jeteta = ev["jteta"][j], jetpt = ev["jtpt"][j], jetphi = ev["jtphi"][j];
    int indexClosest = -1; double dRclosest = 999; 
    double jetmuonpt=0, jetmuonphi=0, jetmuoneta=0, jetmuonptrel = 0;

    for (int m=0;m<ev.GetInt("Gen_nptl");m++) {
      double dR_jm = dR(ev["Gen_phi"][m], ev["Gen_eta"][m], jetphi,jeteta);
      if (dR_jm<dRclosest) { indexClosest = m; dRclosest = dR_jm; }
    }
    
    if (indexClosest>=0) {
      jetmuonpt = ev["Gen_pt"][indexClosest]; 
      jetmuonphi = ev["Gen_phi"][indexClosest]; 
      jetmuoneta = ev["Gen_eta"][indexClosest];
      jetmuonptrel=getPtRel(jetpt, jeteta, jetphi, jetmuonpt, jetmuoneta, jetmuonphi);
    }

    evout["jetmuonpt"].push_back(jetmuonpt);
    evout["jetmuonphi"].push_back(jetmuonphi);
    evout["jetmuoneta"].push_back(jetmuoneta);
    evout["jetmuonptrel"].push_back(jetmuonptrel);

    
    //copy other jet stuff

    evout["jtpt"].push_back(ev["jtpt"][j]);
    evout["jteta"].push_back(ev["jteta"][j]);
    evout["jtphi"].push_back(ev["jtphi"][j]);
    evout["rawpt"].push_back(ev["rawpt"][j]);
    evout["refpt"].push_back(ev["refpt"][j]);
    evout.GetVInt("refparton_flavorForB").push_back(ev.GetVInt("refparton_flavorForB")[j]);
    evout["discr_prob"].push_back(ev["discr_prob"][j]);
    evout["discr_csvSimple"].push_back(ev["discr_csvSimple"][j]);
    evout["svtxm"].push_back(ev["svtxm"][j]);

    evout["jty"].push_back(ev["jty"][j]);
    evout["refeta"].push_back(ev["refeta"][j]);
    evout["refy"].push_back(ev["refy"][j]);
    evout["refphi"].push_back(ev["refphi"][j]);
    evout["refparton_pt"].push_back(ev["refparton_pt"][j]);


    evout["mue"].push_back(ev["mue"][j]);
    evout["mupt"].push_back(ev["mupt"][j]);
    evout["mueta"].push_back(ev["mueta"][j]);
    evout["muphi"].push_back(ev["muphi"][j]);
    evout["mudr"].push_back(ev["mudr"][j]);
    evout["muptrel"].push_back(ev["muptrel"][j]);
    evout.GetVInt("muchg").push_back(ev.GetVInt("muchg")[j]);


  }

    evout.PutInt("nref", evout["jtpt"].size());
    evout.PutInt("Njetmuon", evout["jetmuonpt"].size());

}

void Merge_MuonJet(){

  gROOT->Reset();

  TString sample = "QCDPPb";//bJetPPb
  TString name = Form("%s_AccCut.root",sample.Data());
  TString outname = "QCDPPb_genmuonmerged.root";//Form("%s_temp.root",sample.Data());
  
  
  vector<TString> jetbranches = {
    "jtpt", "jteta", "jtphi", "rawpt", "refpt", "refparton_flavorForB", "discr_prob", "discr_csvSimple","svtxm",
     "jty", "refeta","refy","refphi","refparton_pt",
    "mue", "mupt", "mueta", "muphi", "mudr", "muptrel", "muchg","Gen_nptl","Gen_eta","Gen_phi","Gen_pt"
  };

  vector<TString> newbranches = {"Njetmuon/I:jetmuonpt/F,jetmuoneta/F,jetmuonphi/F,jetmuonptrel/F"};

  ProcessFile(name, outname, "jet", vector<TString> ({"muon","evt"}), jetbranches, newbranches, ProcessEvent);



  //  ProcessFile(name, outname, "jet", vector<TString> ({"QCDPPb_AccCut.root:m=muon","evt"}), jetbranches, newbranches, ProcessEvent);


  
}

