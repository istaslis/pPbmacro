#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include <algorithm>
#include "TStopwatch.h"

void Subset_MC_AccCut(){
    
  gROOT->Reset();

  TString sample = "QCDPPb";//"QCDPPb";//bJetPPb
  TString name = Form("%s_HiForest.root",sample.Data());
  TString outname = Form("%s_AccCut.root",sample.Data());
  TFile *fin = new TFile(name);
  TTree *tjet = (TTree*)fin->Get("jet");
  TTree *tmuon = (TTree*)fin->Get("muon");
  TTree *tevt = (TTree*)fin->Get("evt");
  
  
  vector<TString> jetbranches = {
    "jtpt", "jteta", "jtphi", "rawpt", "refpt", "refparton_flavorForB", "discr_prob", "discr_csvSimple","svtxm",
    "mue", "mupt", "mueta", "muphi", "mudr", "muptrel", "muchg"
  };

  tjet->SetBranchStatus("*",1);
  for (auto b:jetbranches) tjet->SetBranchStatus(b,0);

  TFile *fout = new TFile(outname,"recreate");
  TTree *tjetout = tjet->CloneTree(0,"fast");

  for (auto b:jetbranches) tjet->SetBranchStatus(b,1);

    Int_t           nref;
    Float_t         rawpt[1000];
    Float_t         refpt[1000];
    Int_t           refparton_flavorForB[1000];  
    Float_t         jtpt[1000];
    Float_t         jteta[1000];
    Float_t         jtphi[1000];
    Float_t         discr_csvSimple[1000];
    Float_t         discr_prob[1000];
    Float_t         svtxm[1000];

    Float_t         mue[1000];
    Float_t         mupt[1000];
    Float_t         mueta[1000];
    Float_t         muphi[1000];
    Float_t         mudr[1000];
    Float_t         muptrel[1000];
    Int_t           muchg[1000];


    tjet->SetBranchAddress("nref",&nref);
    tjet->SetBranchAddress("rawpt",rawpt);
    tjet->SetBranchAddress("refpt",refpt);
    tjet->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);
    tjet->SetBranchAddress("jtpt",jtpt);
    tjet->SetBranchAddress("jteta",jteta);
    tjet->SetBranchAddress("jtphi",jtphi);
    tjet->SetBranchAddress("discr_csvSimple",discr_csvSimple);
    tjet->SetBranchAddress("discr_prob",discr_prob);
    tjet->SetBranchAddress("svtxm",svtxm);    

    tjet->SetBranchAddress("mue",mue);
    tjet->SetBranchAddress("mupt",mupt);
    tjet->SetBranchAddress("mueta",mueta);
    tjet->SetBranchAddress("muphi",muphi);
    tjet->SetBranchAddress("mudr",mudr);
    tjet->SetBranchAddress("muptrel",muptrel);
    tjet->SetBranchAddress("muchg",muchg);

    Int_t           nrefout;
    Float_t         jtptout[1000];
    Float_t         jtetaout[1000];
    Float_t         jtphiout[1000];
    Float_t         rawptout[1000];
    Float_t         refptout[1000];
    Int_t           refparton_flavorForBout[1000];
    Float_t         discr_probout[1000];
    Float_t         discr_csvSimpleout[1000];
    Float_t         svtxmout[1000];

    Float_t         mueout[1000];
    Float_t         muptout[1000];
    Float_t         muetaout[1000];
    Float_t         muphiout[1000];
    Float_t         mudrout[1000];
    Float_t         muptrelout[1000];
    Int_t           muchgout[1000];


  
    tjetout->Branch("nrefin",&nref,"nrefin/I");
    tjetout->Branch("nrefout",&nrefout,"nrefout/I");
    
    tjetout->Branch("jtpt",jtptout,"jtpt[nrefout]/F");
    tjetout->Branch("jteta",jtetaout,"jteta[nrefout]/F");
    tjetout->Branch("jtphi",jtphiout,"jtphi[nrefout]/F");
    tjetout->Branch("rawpt",rawptout,"rawpt[nrefout]/F");
    tjetout->Branch("refpt",refptout,"refpt[nrefout]/F");
    tjetout->Branch("refparton_flavorForB",refparton_flavorForBout,"refparton_flavorForB[nrefout]/I");
    tjetout->Branch("discr_prob",discr_probout,"discr_prob[nrefout]/F");
    tjetout->Branch("discr_csvSimple",discr_csvSimpleout,"discr_csvSimple[nrefout]/F");
    tjetout->Branch("svtxm",svtxmout,"svtxm[nrefout]/F");
    
    tjetout->Branch("mue",mue,"mue[nrefout]/F");
    tjetout->Branch("mupt",mupt,"mupt[nrefout]/F");
    tjetout->Branch("mueta",mueta,"mueta[nrefout]/F");
    tjetout->Branch("muphi",muphi,"muphi[nrefout]/F");
    tjetout->Branch("mudr",mudr,"mudr[nrefout]/F");
    tjetout->Branch("muptrel",muptrel,"muptrel[nrefout]/F");
    tjetout->Branch("muchg",muchg,"muchg[nrefout]/F");


    Long64_t nentries = tjet->GetEntries();
    int oneperc = nentries/100;
    TStopwatch s;
    s.Start();

    for (Long64_t i=0; i<nentries;i++) {
      if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;
      tjet->GetEntry(i);

      nrefout = 0;
      for (int r = 0;r<nref;r++)
       	if (fabs(jteta[r])<2 && rawpt[r]>18.) {
	       
	      jtptout[nrefout]=jtpt[r];
	      jtetaout[nrefout]=jteta[r];
	      jtphiout[nrefout]=jtphi[r];
	      rawptout[nrefout]=rawpt[r];
	      refptout[nrefout]=refpt[r];
	      refparton_flavorForBout[nrefout]=refparton_flavorForB[r];
	      discr_probout[nrefout]=discr_prob[r];
	      discr_csvSimpleout[nrefout]=discr_csvSimple[r];
	      svtxmout[nrefout]=svtxm[r];
            
          mue[nrefout]=mue[r];
          mupt[nrefout]=mupt[r];
          mueta[nrefout]=mueta[r];
          muphi[nrefout]=muphi[r];
          mudr[nrefout]=mudr[r];
          muptrel[nrefout]=muptrel[r];
          muchg[nrefout]=muchg[r];

	      nrefout++;
	    }
      

      tjetout->Fill();
      
    }
    cout<<endl;
    s.Print();  

    tjetout->Write();

    cout<<"Copying other trees..."<<endl;
    TTree *tmuonout = tmuon->CloneTree(-1,"fast");
    tmuonout->Write();
    TTree *tevtout = tevt->CloneTree(-1,"fast");
    tevtout->Write();
    
    fout->Close();


}

