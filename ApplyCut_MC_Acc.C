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

void ProcessEvent(Everything &ev, Everything &evout)
{
  int rout = 0;
  for (int r=0;r<ev.GetInt("nref");r++)
    if (fabs(ev["jteta"][r])<2 && ev["rawpt"][r]>18.) {
      evout["jtpt"].push_back(ev["jtpt"][r]);
      evout["jteta"].push_back(ev["jteta"][r]);
      evout["jtphi"].push_back(ev["jtphi"][r]);
      evout["rawpt"].push_back(ev["rawpt"][r]);
      evout["refpt"].push_back(ev["refpt"][r]);
      evout.GetVInt("refparton_flavorForB").push_back(ev.GetVInt("refparton_flavorForB")[r]);
      evout["discr_prob"].push_back(ev["discr_prob"][r]);
      evout["discr_csvSimple"].push_back(ev["discr_csvSimple"][r]);
      evout["svtxm"].push_back(ev["svtxm"][r]);

      evout["mue"].push_back(ev["mue"][r]);
      evout["mupt"].push_back(ev["mupt"][r]);
      evout["mueta"].push_back(ev["mueta"][r]);
      evout["muphi"].push_back(ev["muphi"][r]);
      evout["mudr"].push_back(ev["mudr"][r]);
      evout["muptrel"].push_back(ev["muptrel"][r]);
      evout.GetVInt("muchg").push_back(ev.GetVInt("muchg")[r]);
      rout++;

    }

    evout.PutInt("nref", evout["jtpt"].size());

}

void ApplyCut_MC_Acc(){

  gROOT->Reset();

  TString sample = "QCDPPb";//bJetPPb
  TString name = Form("%s_HiForest.root",sample.Data());
  TString outname = Form("%s_temp.root",sample.Data());
  
  
  vector<TString> jetbranches = {
    "jtpt", "jteta", "jtphi", "rawpt", "refpt", "refparton_flavorForB", "discr_prob", "discr_csvSimple","svtxm",
    "mue", "mupt", "mueta", "muphi", "mudr", "muptrel", "muchg"
  };

  ProcessFile(name, outname, "jet", jetbranches, ProcessEvent);


  
}

