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

bool MC = true; //false = data

void ProcessEvent(Everything &ev, Everything &evout)
{
  for (int r=0;r<ev.GetInt("nref");r++) {
    if (fabs(ev["jteta"][r])<2 && ev["rawpt"][r]>22.) {
      evout.AddRow(ev,"nref",r);
    }
  }
}

void ApplyCut_MC_Acc(TString type, int id=0, int N=1){

  gROOT->Reset();

  type.ToLower();
  if (type=="mc") MC = true; else
    if (type=="data") MC = false; else
      {cout<<"Choose mc or data"<<endl; return; }


  TString sample, name, outname;

  if (MC) {
    sample = "QCDPPb";//bJetPPb
    name = Form("%s_HiForest.root",sample.Data());
    outname = Form("%s_AccCut.root",sample.Data());
  } else {
    name = "jettrig_weight.root";
    outname = "jettrig_weight_AccCut.root";
  }
  
    ProcessFile(name, outname, "jet", vector<TString>({"muon"}), vector<TString>{"nref"}, vector<TString>(), ProcessEvent, id, N);


  
}

