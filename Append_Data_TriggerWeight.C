#include "TFile.h"
#include "TNtuple.h"
#include <iostream>
#include "TSystem.h"
#include "HelperProcess.C"


void ProcessEvent(Everything &ev, Everything &evout)
{
  auto jt40 = ev.GetInt("jt40");
  auto jt60 = ev.GetInt("jt60");
  auto jt80 = ev.GetInt("jt80");    
  auto jt100 = ev.GetInt("jt100");
  auto pt = ev.GetFloat("pt");
  auto pscl40 = ev.GetInt("pscl40");
  auto pscl60 = ev.GetInt("pscl60");
  auto pscl80 = ev.GetInt("pscl80");  
  auto pscl100 = ev.GetInt("pscl100");



  float weightJet = 0, weight12003 = 0;

  if (jt40 && !jt60 && !jt80 && !jt100) weight12003 = 1/pscl40;
  if (jt60 && !jt80 && !jt100) weight12003 = 1;
  if (jt80 && !jt100) weight12003 = 1;
  if (jt100) weight12003 = 1;

  if (jt40 && pt>40 && pt<60) weightJet = pscl40;
  if (jt60 && pt>60 && pt<80) weightJet = pscl60;
  if (jt80 && pt>80 && pt<100) weightJet = pscl80;
  if (jt100 && pt>100) weightJet = pscl100;

  if (pt>200 and weightJet!=1)
    cout<<jt40<<","<<pscl40<<" "<<jt60<<","<<pscl60<<" "<<jt80<<","<<pscl80<<" "<<jt100<<","<<pscl100<<" : "<<pt<<","<<weightJet<<endl;

  evout.CopySingles(ev);

  evout.PutFloat("weightJet",weightJet);
  evout.PutFloat("weight12003",weight12003);

}

void Append_Data_TriggerWeight(int id=0, int N=1){

  gROOT->Reset();

  TString name = "jettrig.root";
  TString outname = "jettrig_weight.root";

  ProcessFile(name, outname, "jet", vector<TString>({"muon"}), 
    vector<TString>{"pt","jt40","jt60","jt80","jt100","pscl40","pscl60","pscl80","pscl100"}, 
    vector<TString>{"weightJet/F","weight12003/F"}, ProcessEvent, id, N);


}



//old update version
/*
//#ifndef __CINT__
//#include <omp.h>
//#endif

void Append_Data_TriggerWeight()
{
  cout<<"Copying file..."<<endl;
  gSystem->CopyFile("jettrig.root", "jettrig_weight.root",true);
  cout<<"Done!"<<endl;

  TFile *f = new TFile("jettrig_weight.root","update");
  TTree *nt = (TTree *)f->Get("jet");

//Declaration of leaves types
   Float_t         pt;
   Int_t         jt40;
   Int_t         jt60;
   Int_t         jt80;
   Int_t         jt100;
   Int_t         pscl40;
   Int_t         pscl60;
   Int_t         pscl80;
   Int_t         pscl100;
  

   // Set branch addresses.
   nt->SetBranchAddress("pt",&pt);
   nt->SetBranchAddress("jt40",&jt40);
   nt->SetBranchAddress("jt60",&jt60);
   nt->SetBranchAddress("jt80",&jt80);
   nt->SetBranchAddress("jt100",&jt100);
   nt->SetBranchAddress("pscl40",&pscl40);
   nt->SetBranchAddress("pscl60",&pscl60);
   nt->SetBranchAddress("pscl80",&pscl80);
   nt->SetBranchAddress("pscl100",&pscl100);
  
   float weightJet = 0, weight12003 = 0;

   TBranch *b_weightJet = nt->Branch("weightJet",&weightJet,"weightJet/F");//,b_weightJet);
   TBranch *b_weight12003 = nt->Branch("weight12003",&weight12003,"weight12003/F");//,b_weight12003);

   Long64_t nentries = nt->GetEntries();
   cout<<nentries<<endl;

   int oneperc = nentries/100;
   Long64_t nbytes = 0;

   for (Long64_t i=0; i<nentries;i++) {
           nbytes += nt->GetEntry(i);

      if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;

      weightJet = weight12003 = 0;

      if (jt40 && !jt60 && !jt80 && !jt100) weight12003 = 1/pscl40;
      if (jt60 && !jt80 && !jt100) weight12003 = 1;
      if (jt80 && !jt100) weight12003 = 1;
      if (jt100) weight12003 = 1;

      if (jt40 && pt>40 && pt<60) weightJet = pscl40;
      if (jt60 && pt>60 && pt<80) weightJet = pscl60;
      if (jt80 && pt>80 && pt<100) weightJet = pscl80;
      if (jt100 && pt>100) weightJet = pscl100;

      if (pt>200 and weightJet!=1)
  cout<<jt40<<","<<pscl40<<" "<<jt60<<","<<pscl60<<" "<<jt80<<","<<pscl80<<" "<<jt100<<","<<pscl100<<" : "<<pt<<","<<weightJet<<endl;
      //nt->Fill();
      b_weightJet->Fill();
      b_weight12003->Fill();
   }
   cout<<endl;

   nt->Write("", TObject::kOverwrite);

   //   TTree *muonout = muon->CloneTree(-1,"fast");
   //muonout->Write();


   f->Close();

   f = new TFile("jettrig_weight.root");
   nt = (TTree *)f->Get("jet");
   cout<<nt->GetEntries()<<endl;
   f->Close();

}
*/









