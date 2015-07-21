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

bool MatchedMuons(float pt1, float phi1, float eta1, float pt2, float phi2, float eta2)
{
  double dR12 = dR(phi1, eta1, phi2, eta2);
  double dPoP = fabs(pt1-pt2)/(pt1+pt2);
  return dR12<0.1 && dPoP<0.1;
}

void ProcessEvent(Everything &ev, Everything &evout)
{
  int Nmuons = ev.GetInt("Glb_nptl");
  vector<float> & muonpT = ev["Glb_pt"];
  vector<float>& muonphi = ev["Glb_phi"];
  vector<float>& muoneta = ev["Glb_eta"];

  int Ngenmuons = ev.GetInt("Gen_nptl");
  vector<float> & genmuonpT = ev["Gen_pt"];
  vector<float>& genmuonphi = ev["Gen_phi"];
  vector<float>& genmuoneta = ev["Gen_eta"];

  vector<int> &Gen_glbInd = evout.GetVInt("Gen_glbInd");
  vector<int> &Glb_genInd = evout.GetVInt("Glb_genInd");

  vector<float> &Glb_gendR = evout["Glb_gendR"];
  vector<float> &Glb_gendPoP = evout["Glb_gendPoP"];


  vector<int> &Gen_Nmatched = evout.GetVInt("Gen_Nmatched");
  vector<int> &Glb_Nmatched = evout.GetVInt("Glb_Nmatched");

  Gen_glbInd = vector<int>(Ngenmuons, -1);
  Glb_genInd = vector<int>(Nmuons, -1);

  Glb_gendR = vector<float>(Nmuons, 999);
  Glb_gendPoP = vector<float>(Nmuons, 999);


  Gen_Nmatched = vector<int>(Ngenmuons, 0);
  Glb_Nmatched = vector<int>(Nmuons, 0);

  for (int m=0;m<Nmuons;m++) {
    double dRc = 999, dPoPc = 999; int G = -1;
    for (int g=0;g<Ngenmuons;g++) {
      double dr = dR(muonphi[m], muoneta[m], genmuonphi[g],genmuoneta[g]);
      double dpop = fabs(muonpT[m]-genmuonpT[g])/(muonpT[m]+genmuonpT[g]);

      if (dr<dRc && dpop<dPoPc) { dRc = dr; dPoPc = dpop; G=g;}
    }
    
    Glb_genInd[m] = G;
    Glb_gendR[m]=dRc;
    Glb_gendPoP[m]=dPoPc;
    /*
	//	Glb_Nmatched[m]++;
	
      if (MatchedMuons(muonpT[m],muonphi[m],muoneta[m],genmuonpT[g],genmuonphi[g],genmuoneta[g]))
	{
	  Gen_glbInd[g] = m;
	  Glb_genInd[m] = g;
	  Gen_Nmatched[g]++;
	  Glb_Nmatched[m]++;
	}
	}*/
  }

  evout.PutInt("Gen_nptl2",Ngenmuons);
  evout.PutInt("Glb_nptl2",Nmuons);
}

void MatchMuons(){

  gROOT->Reset();

  TString sample = "QCDPPb";//bJetPPb
  TString name = Form("%s_AccCut.root",sample.Data());
  TString outname = "QCDPPb_muonmatched.root";//Form("%s_temp.root",sample.Data());
  
  
  vector<TString> usebranches = {
    "Glb_nptl","Glb_eta","Glb_phi","Glb_pt", 
    "Gen_nptl","Gen_eta","Gen_phi","Gen_pt", 
    //    "Glb_glbChi2_ndof","Glb_nValMuHits","Glb_nMatchedStations","Glb_nValPixHits"
  };


  vector<TString> newbranches = {"Gen_nptl2/I:Gen_glbInd/I,Gen_Nmatched/I","Glb_nptl2/I:Glb_genInd/I,Glb_Nmatched/I,Glb_gendPoP/F,Glb_gendR/F"};

  ProcessFile(name, outname, "muon", vector<TString> ({"jet","evt"}), usebranches, newbranches, ProcessEvent);



  //  ProcessFile(name, outname, "jet", vector<TString> ({"QCDPPb_AccCut.root:m=muon","evt"}), jetbranches, newbranches, ProcessEvent);


  
}

