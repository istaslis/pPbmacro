#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include <iostream>

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
   TH1F *Ajdata = new TH1F("Ajdata", "Aj data", nbins, 0, 1);
   TH1F *Ajincdata = new TH1F("Ajincdata", "Aj non b-jet data", nbins, 0, 1);
   TH1F *Ajbkgdata = new TH1F("Ajbkgdata", "Aj bkg data", nbins, 0, 1);

   float csvvalue = 0.9;
   float jtpt1 = 60;
   float jtpt2 = 30;

   int oneperc = nentries/100;
   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += nt->GetEntry(i);
      if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;

      float deltaphi = fabs(jtphi[0] - jtphi[1]);
      deltaphi = deltaphi > PI ? 2*PI-deltaphi : deltaphi;
      if (rawpt[0]>18 && rawpt[1]>18) { //safety cut
	
	if (jtpt[0]>jtpt1 && jtpt[1]>jtpt2 &&
	  discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue)
	tagged2jetdata->Fill(deltaphi, weightJet);

	if (jtpt[0]>jtpt1 && jtpt[1]>jtpt2 && deltaphi>2*PI/3. && discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue)
	  Ajdata->Fill((jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1]), weightJet);

	if (jtpt[0]>jtpt1 && jtpt[1]>jtpt2 && deltaphi>2*PI/3.)
	  Ajincdata->Fill((jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1]),weightJet);

	if (jtpt[0]>jtpt1 && jtpt[1]>jtpt2 && deltaphi<PI/3. && discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue)
	  Ajbkgdata->Fill((jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1]),weightJet);

      }
   }
   cout<<endl;

   tagged2jetdata->Write();
   Ajdata->Write();
   Ajincdata->Write();
   Ajbkgdata->Write();
   fout->Close();
}
