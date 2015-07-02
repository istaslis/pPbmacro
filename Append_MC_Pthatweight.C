#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include <iostream>

const int Nbins=4;
vector<int> bounds = {30,50,80,120};

vector<float> CalculatePthatWeights()
{
  vector<int> eventsCurrBound(Nbins);
  vector<int> eventsNextBound(Nbins);
  
  vector<float> weights (Nbins, 1.); //Nbins values of 1.

  for (int ind = 0;ind<Nbins;ind++) {
    TString finname = Form("tempout%d.root",bounds[ind]);
    TFile *f = new TFile(finname);
    TTree *t = (TTree *)f->Get("jet");
  
    eventsCurrBound[ind] = t->GetEntries(Form("pthat>%d",bounds[ind]));
    if (ind<Nbins-1) eventsNextBound[ind] = t->GetEntries(Form("pthat>%d",bounds[ind+1])); 
    
    delete t;
    delete f;
  }


  float sumofweights = 1;
  for (int i=1;i<Nbins;i++)
  {
    weights[i]=(float)weights[i-1]*eventsNextBound[i-1]/(eventsCurrBound[i]+weights[i-1]*eventsNextBound[i-1]);
    sumofweights+=weights[i];
  }
  for (int it=0;it<Nbins;it++)
    weights[it]/=sumofweights;

  return weights;
}

void Append_MC_Pthatweight(int ind) {

  TString finname = Form("tempout%d.root",bounds[ind]);

  vector<float> weights = CalculatePthatWeights();
  for (auto w:weights) cout<<w<<" "; cout<<endl;

  TFile *f = new TFile(finname,"update");
  TTree *t = (TTree *)f->Get("jet");

  float pthat, weight;
  t->SetBranchAddress("pthat",&pthat);
  TBranch *b_weight = t->Branch("weight",&weight,"weight/F");

  Long64_t nentries = t->GetEntries();
  int oneperc = nentries/100;
  
  for (Long64_t i=0; i<nentries;i++) {
    if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;

    t->GetEntry(i);

    int ptbin = Nbins-1;
    if (pthat<bounds[0]) cout<<"pthat less than "<<bounds[0]<<":"<<pthat<<"!"<<endl;
    
    for (int i=0;i<Nbins-1;i++)
      if (pthat>bounds[i] && pthat<bounds[i+1]) ptbin = i;
    
    weight = weights[ptbin];

    b_weight->Fill();
  }

  t->Write("", TObject::kOverwrite);
  f->Close();

}