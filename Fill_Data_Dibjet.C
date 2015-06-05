#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"

const float PI = 3.1416;

void Fill_Data_Dibjet()
{
  TFile *f = new TFile("jettrig_weight_etacut.root");
  TTree *nt = (TTree *)f->Get("nt");

//Declaration of leaves types
   Float_t         pt;
   Float_t         eta;
   Float_t         phi;
   Float_t         mass;
   Int_t           jt40;
   Int_t           jt60;
   Int_t           jt80;
   Int_t           jt100;
   Int_t           pscl40;
   Int_t           pscl60;
   Int_t           pscl80;
   Int_t           pscl100;
   Int_t           Run;
   Int_t           Event;
   Int_t           LumiBlock;
   Int_t           nrefout;
   Float_t         jtpt[14];
   Float_t         jteta[14];
   Float_t         jtphi[14];
   Float_t         rawpt[14];
   Float_t         discr_prob[14];
   Float_t         discr_csvSimple[14];
   Float_t         svtxm[14];
   Float_t         mupt[14];
   Float_t         mueta[14];
   Float_t         muphi[14];
   Float_t         muptrel[14];
   Int_t           bin;
   Float_t         weightJet;
   Float_t         weight12003;

   // Set branch addresses.
   nt->SetBranchAddress("pt",&pt);
   nt->SetBranchAddress("eta",&eta);
   nt->SetBranchAddress("phi",&phi);
   nt->SetBranchAddress("mass",&mass);
   nt->SetBranchAddress("jt40",&jt40);
   nt->SetBranchAddress("jt60",&jt60);
   nt->SetBranchAddress("jt80",&jt80);
   nt->SetBranchAddress("jt100",&jt100);
   nt->SetBranchAddress("pscl40",&pscl40);
   nt->SetBranchAddress("pscl60",&pscl60);
   nt->SetBranchAddress("pscl80",&pscl80);
   nt->SetBranchAddress("pscl100",&pscl100);
   nt->SetBranchAddress("Run",&Run);
   nt->SetBranchAddress("Event",&Event);
   nt->SetBranchAddress("LumiBlock",&LumiBlock);
   nt->SetBranchAddress("nrefout",&nrefout);
   nt->SetBranchAddress("jtpt",jtpt);
   nt->SetBranchAddress("jteta",jteta);
   nt->SetBranchAddress("jtphi",jtphi);
   nt->SetBranchAddress("rawpt",rawpt);
   nt->SetBranchAddress("discr_prob",discr_prob);
   nt->SetBranchAddress("discr_csvSimple",discr_csvSimple);
   nt->SetBranchAddress("svtxm",svtxm);
   nt->SetBranchAddress("mupt",mupt);
   nt->SetBranchAddress("mueta",mueta);
   nt->SetBranchAddress("muphi",muphi);
   nt->SetBranchAddress("muptrel",muptrel);
   nt->SetBranchAddress("bin",&bin);
   nt->SetBranchAddress("weightJet",&weightJet);
   nt->SetBranchAddress("weight12003",&weight12003);

   Long64_t nentries = nt->GetEntries();

   int nbins = 20;
   float xmin = 0;
   float xmax = PI;

   TFile *fout = new TFile("Hist_Data.root","recreate");
   TH1F *tagged2jetdata = new TH1F("tagged2jetdata", "Tagged di-jets", nbins, xmin, xmax);
   float csvvalue = 0.9;
   float jtpt1 = 60;
   float jtpt2 = 30;


   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += nt->GetEntry(i);

      float deltaphi = fabs(jtphi[0] - jtphi[1]);
      float valueToDraw = deltaphi > PI ? 2*PI-deltaphi : deltaphi;
      if (rawpt[0]>18 && rawpt[1]>18  && jtpt[0]>jtpt1 && jtpt[1]>jtpt2 &&
	  discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue)
	tagged2jetdata->Fill(valueToDraw, weightJet);

   }

   tagged2jetdata->Write();
   fout->Close();
}
