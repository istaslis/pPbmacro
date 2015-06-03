#include "TFile.h"
#include "TNtuple.h"
#include <iostream>
//#ifndef __CINT__
//#include <omp.h>
//#endif

void jettrigger()
{
  TFile *f = new TFile("jettrig.root");
  TNtuple *nt = (TNtuple *)f->Get("nt");

//Declaration of leaves types
   Float_t         pt;
   Float_t         eta;
   Float_t         phi;
   Float_t         mass;
   Float_t         jt40;
   Float_t         jt60;
   Float_t         jt80;
   Float_t         jt100;
   Float_t         pscl40;
   Float_t         pscl60;
   Float_t         pscl80;
   Float_t         pscl100;
  

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
  
   TFile *fout = new TFile("jettrig_withweight.root","recreate");
   TNtuple *ntout = new TNtuple("ntweight","ntweight","pt:eta:phi:mass:jt40:jt60:jt80:jt100:pscl40:pscl60:pscl80:weightJet:weight12003");
   //TTree *ntout=new TTree("ntweight","ntweight");
   ntout->SetDirectory(fout);

   /*   ntout->Branch("pt",&pt);
   ntout->Branch("eta",&eta);
   ntout->Branch("phi",&phi);
   ntout->Branch("mass",&mass);
   ntout->Branch("jt40",&jt40);
   ntout->Branch("jt60",&jt60);
   ntout->Branch("jt80",&jt80);
   ntout->Branch("jt100",&jt100);
   ntout->Branch("pscl40",&pscl40);
   ntout->Branch("pscl60",&pscl60);
   ntout->Branch("pscl80",&pscl80);
   ntout->Branch("pscl100",&pscl100);
   ntout->Branch("weight",&weight);
   */
   Long64_t nentries = nt->GetEntries();
   cout<<nentries<<endl;

   int oneperc = nentries/100;
   Long64_t nbytes = 0;

   //#ifndef __CINT__
   //#pragma omp parallel for ordered schedule(dynamic)
   //#endif
   for (Long64_t i=0; i<nentries;i++) {
           nbytes += nt->GetEntry(i);

      if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;

      float weightJet = 0, weight12003 = 0;

      if (jt40 && !jt60 && !jt80 && !jt100) weight12003 = 1/pscl40;
      if (jt60 && !jt80 && !jt100) weight12003 = 1;
      if (jt80 && !jt100) weight12003 = 1;
      if (jt100) weight12003 = 1;

      if (jt40 && pt>40 && pt<60) weightJet = pscl40;
      if (jt60 && pt>60 && pt<80) weightJet = pscl60;
      if (jt80 && pt>80 && pt<100) weightJet = pscl80;
      if (jt100 && pt>100) weightJet = pscl100;

      //#ifndef __CINT__
      //#pragma omp ordered
      //#endif
	ntout->Fill(pt,eta,phi,mass,jt40,jt60,jt80,jt100,pscl40,pscl60,pscl80,weightJet,weight12003);

   }
   cout<<endl;
   cout<<ntout->GetEntries()<<endl;
   ntout->Write();
   fout->Close();
   f->Close();

   f = new TFile("jettrig_withweight.root");
   nt = (TNtuple *)f->Get("ntweight");
   cout<<nt->GetEntries()<<endl;
   f->Close();

}
