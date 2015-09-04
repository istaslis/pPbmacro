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

bool MC = true;

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
  int Nmuons = ev.GetInt("Glb_nptl");
  auto muonpT = ev["Glb_pt"];
  auto muonphi = ev["Glb_phi"];
  auto muoneta = ev["Glb_eta"];
  auto muonchi2ndof = ev["Glb_glbChi2_ndof"];
  auto muonnhits = ev.GetVInt("Glb_nValMuHits");
  auto muonpxhits = ev.GetVInt("Glb_nValPixHits");
  auto muonnMatchedStations = ev.GetVInt("Glb_nMatchedStations");
  auto muontrkDxy = ev["Glb_trkDxy"];
  auto muontrkDz = ev["Glb_trkDz"];
  auto muontrkLayerWM = ev.GetVInt("Glb_trkLayerWMeas");

  //Glb_pt>7 && fabs(Glb_eta)<2 && Glb_nMatchedStations>1  && Glb_glbChi2_ndof<10 && Glb_nValMuHits>0 && Glb_nValPixHits>2 && Glb_trkDxy<0.2 && Glb_trkDz<0.5 && Glb_trkLayerWMeas>5)

  int njets = ev.GetInt("nref");
  auto jtpt = ev["jtpt"];
  auto jtphi = ev["jtphi"];
  auto jteta = ev["jteta"];



  for (int j=0;j<njets;j++) {

    float jeteta = jteta[j], jetpt = jtpt[j], jetphi = jtphi[j];
    int indexClosest = -1; double dRclosest = 999; 
    double jetmuonpt=0, jetmuonphi=0, jetmuoneta=0, jetmuonptrel = 0, jetmuonip3d = 0;

    for (int m=0;m<Nmuons;m++)
      if (fabs(muoneta[m])<2 && muonnMatchedStations[m]>1 && muonchi2ndof[m]<10 
          && muonnhits[m]>0 && muonpxhits[m]>2 && muontrkDxy[m]<0.2 && muontrkDz[m]<0.5)
      {
       double dR_jm = dR(muonphi[m], muoneta[m], jetphi,jeteta);
       if (dR_jm<0.4 && dR_jm<dRclosest) { indexClosest = m; dRclosest = dR_jm; }
      }

     if (indexClosest>=0) {
      jetmuonpt = muonpT[indexClosest]; 
      jetmuonphi = muonphi[indexClosest]; 
      jetmuoneta = muoneta[indexClosest];
      jetmuonptrel=getPtRel(jetpt, jeteta, jetphi, jetmuonpt, jetmuoneta, jetmuonphi);
      jetmuonip3d = sqrt(muontrkDxy[indexClosest]*muontrkDxy[indexClosest] + muontrkDz[indexClosest]*muontrkDz[indexClosest]);
    }

    evout["jetmuonpt"].push_back(jetmuonpt);
    evout["jetmuonphi"].push_back(jetmuonphi);
    evout["jetmuoneta"].push_back(jetmuoneta);
    evout["jetmuonptrel"].push_back(jetmuonptrel);
    evout["jetmuonip3d"].push_back(jetmuonip3d);
    
    //copy other jet stuff
    evout.AddRow(ev,"nref",j);
  }
}

void Merge_MuonJet(TString type, int id = 0, int N = 1){

  gROOT->Reset();
  
  type.ToLower();
  if (type=="mc") MC = true; else
    if (type=="data") MC = false; else
      {cout<<"Choose mc or data"<<endl; return; }

  TString sample = "QCDPPb";//bJetPPb
  TString name,outname;
  
  if (MC) {name = Form("%s_AccCut.root",sample.Data());outname = "QCDPPb_genmuonmerged.root"; }
  else {name =  "jettrig_weight_AccCut.root"; outname = "jettrig_weight_AccCut_muons.root"; }


  vector<TString> newbranches = {"Njetmuon/I:jetmuonpt/F,jetmuoneta/F,jetmuonphi/F,jetmuonptrel/F,jetmuonip3d/F"};

  ProcessFile(name, outname, "jet", vector<TString> ({"muon"}), vector<TString> {"nref","Glb_nptl"}, newbranches, ProcessEvent, id, N);



  //  ProcessFile(name, outname, "jet", vector<TString> ({"QCDPPb_AccCut.root:m=muon","evt"}), jetbranches, newbranches, ProcessEvent);  
}

