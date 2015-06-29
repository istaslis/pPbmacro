#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>

void MergeQCD_B()
{
//Declaration of leaves types
   Int_t           evt;
   Float_t         weight;
   Float_t         pthat;
   Int_t           nrefin;
   Int_t           nrefout;
   Float_t         jtpt[100];
   Float_t         jteta[100];
   Float_t         jtphi[100];
   Float_t         rawpt[100];
   Float_t         refpt[100];
   Int_t           refparton_flavorForB[100];
   Float_t         discr_prob[100];
   Float_t         discr_csvSimple[100];
   Float_t         svtxm[100];
   Int_t           bin;

   int             sampleNum; //0-qcd, 1-bjet

   TFile *fout = new TFile("BjetMerged.root","recreate");
   TTree *tout = new TTree("t","t");
   tout->Branch("evt",&evt,"evt/I");
   tout->Branch("weight",&weight,"weight/F");
   tout->Branch("pthat",&pthat,"pthat/F");
   tout->Branch("nrefin",&nrefin,"nrefin/I");
   tout->Branch("nrefout",&nrefout,"nrefout/I");
   tout->Branch("jtpt",jtpt,"jtpt[nrefout]/F");
   tout->Branch("jteta",jteta,"jteta[nrefout]/F");
   tout->Branch("jtphi",jtphi,"jtphi[nrefout]/F");
   tout->Branch("rawpt",rawpt,"rawpt[nrefout]/F");
   tout->Branch("refpt",refpt,"refpt[nrefout]/F");
   tout->Branch("refparton_flavorForB",refparton_flavorForB,"refparton_flavorForB[nrefout]/I");
   tout->Branch("discr_prob",discr_prob,"discr_prob[nrefout]/F");
   tout->Branch("discr_csvSimple",discr_csvSimple,"discr_csvSimple[nrefout]/F");
   tout->Branch("svtxm",svtxm,"svtxm[nrefout]/F");
   tout->Branch("bin",&bin,"bin/I");
   tout->Branch("sampleNum",&sampleNum,"sampleNum/I");


   TString files[2] = {"QCDPPb_AccCut.root","bJetPPb_AccCut.root"};
   for (sampleNum =0; sampleNum<2;sampleNum++) {
  TFile *f = new TFile(files[sampleNum]);
  TTree * t = (TTree *)f->Get("t");

   // Set branch addresses.
   t->SetBranchAddress("evt",&evt);
   t->SetBranchAddress("weight",&weight);
   t->SetBranchAddress("pthat",&pthat);
   t->SetBranchAddress("nrefin",&nrefin);
   t->SetBranchAddress("nrefout",&nrefout);
   t->SetBranchAddress("jtpt",jtpt);
   t->SetBranchAddress("jteta",jteta);
   t->SetBranchAddress("jtphi",jtphi);
   t->SetBranchAddress("rawpt",rawpt);
   t->SetBranchAddress("refpt",refpt);
   t->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);
   t->SetBranchAddress("discr_prob",discr_prob);
   t->SetBranchAddress("discr_csvSimple",discr_csvSimple);
   t->SetBranchAddress("svtxm",svtxm);
   t->SetBranchAddress("bin",&bin);

   Long64_t nentries = t->GetEntries();
   cout<<nentries<<endl;
   int oneperc = nentries / 100;
   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += t->GetEntry(i);
      if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;

      tout->Fill();
   }
   cout<<endl;
   f->Close();
   }

   fout->cd();
   tout->Write();
   fout->Close();

}
