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

bool ProcessEvent(Everything &ev, Everything &evout, int counter)
{

  int nref = ev.GetInt("nref");
  //  if (nref>0)
  //    cout<<counter<<" "<<nref<<endl;;

  if (counter>=nref) return true;



  auto jtpt = ev["jtpt"];
  auto jteta = ev["jteta"];
  auto jtphi = ev["jtphi"];
  auto rawpt = ev["rawpt"];
  auto discr_prob = ev["discr_prob"];
  auto discr_csvSimple = ev["discr_csvSimple"];
  auto svtxm = ev["svtxm"];
  auto discr_ssvHighPur = ev["discr_ssvHighPur"];
  auto discr_ssvHighEff = ev["discr_ssvHighEff"];
  auto jetmuonpt = ev["jetmuonpt"];
  auto jetmuonptrel = ev["jetmuonptrel"];
  auto jetmuonip3d = ev["jetmuonip3d"];
  auto refparton_flavorForB = ev.GetVInt("refparton_flavorForB");
  auto weight = ev.GetFloat("weight");
  auto weightJet = ev.GetFloat("weightJet");
    
  evout.PutFloat("jtpt",jtpt[counter]);
  evout.PutFloat("jteta",jteta[counter]);
  evout.PutFloat("jtphi",jtphi[counter]);
  evout.PutFloat("rawpt",rawpt[counter]);
  evout.PutFloat("discr_prob",discr_prob[counter]);
  evout.PutFloat("discr_csvSimple",discr_csvSimple[counter]);
  evout.PutFloat("svtxm",svtxm[counter]);
  evout.PutFloat("discr_ssvHighPur",discr_ssvHighPur[counter]);
  evout.PutFloat("discr_ssvHighEff",discr_ssvHighEff[counter]);
  evout.PutFloat("jetmuonpt",jetmuonpt[counter]);
  evout.PutFloat("jetmuonptrel",jetmuonptrel[counter]);
  evout.PutFloat("jetmuonip3d",jetmuonip3d[counter]);
  evout.PutFloat("logmuip3d",log(jetmuonip3d[counter]));
  if (MC) {
    evout.PutInt("refparton_flavorForB",refparton_flavorForB[counter]);
    evout.PutFloat("weight",weight);
  } else {
    evout.PutFloat("weightJet",weightJet);
  }
  
  return false;
}

void Flattify(TString type, int id=0, int N=1){

  gROOT->Reset();

  type.ToLower();
  if (type=="mc") MC = true; else
    if (type=="data") MC = false; else
      {cout<<"Choose mc or data"<<endl; return; }

  TString sample = "QCDPPb";//bJetPPb
  TString name,outname;
  
  if (MC) {name = Form("%s_genmuonmerged.root",sample.Data());outname = "QCDPPb_genmuonmerged_flat.root"; }
  else {name =  "jettrig_weight_AccCut_muons.root"; outname = "jettrig_weight_AccCut_muons_flat.root"; }

  vector<TString> newbranches = {"jtpt/F","jteta/F","jtphi/F","rawpt/F","discr_prob/F","discr_csvSimple/F","svtxm/F",
  "discr_ssvHighPur/F","discr_ssvHighEff/F","jetmuonpt/F","jetmuonptrel/F","jetmuonip3d/F","logmuip3d/F"};

  if (MC) {
    newbranches.push_back("refparton_flavorForB/I");
    newbranches.push_back("weight/F");
  } else newbranches.push_back("weightJet/F");
  
  //technically nref and Njetmuon are different but they are equal, so good to use counter
  ProcessFile(name, outname, "jet", {}, {"nref","Njetmuon"}, newbranches, ProcessEvent, id, N);
  
}

