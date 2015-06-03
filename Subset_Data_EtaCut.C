#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include <algorithm>

void Subset_Data_EtaCut()
{
   TFile *f = new TFile("jettrig_weight.root");
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
   Int_t           nref;
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
   nt->SetBranchAddress("nref",&nref);
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

   Int_t           nrefout;
   Float_t         jtptout[14];
   Float_t         jtetaout[14];
   Float_t         jtphiout[14];
   Float_t         rawptout[14];
   Float_t         discr_probout[14];
   Float_t         discr_csvSimpleout[14];
   Float_t         svtxmout[14];
   Float_t         muptout[14];
   Float_t         muetaout[14];
   Float_t         muphiout[14];
   Float_t         muptrelout[14];

   TFile *fout = new TFile("jettrig_weight_etacut.root","recreate");
   TTree *ntout = new  TTree("nt","nt etacut");

   ntout->Branch("pt",&pt,"pt/F");
   ntout->Branch("eta",&eta,"eta/F");
   ntout->Branch("phi",&phi,"phi/F");
   ntout->Branch("mass",&mass,"mass/F");
   ntout->Branch("jt40",&jt40,"jt40/I");
   ntout->Branch("jt60",&jt60,"jt60/I");
   ntout->Branch("jt80",&jt80,"jt80/I");
   ntout->Branch("jt100",&jt100,"jt100/I");
   ntout->Branch("pscl40",&pscl40,"pscl40/I");
   ntout->Branch("pscl60",&pscl60,"pscl60/I");
   ntout->Branch("pscl80",&pscl80,"pscl80/I");
   ntout->Branch("pscl100",&pscl100,"pscl100/I");
   ntout->Branch("Run",&Run,"Run/I");
   ntout->Branch("Event",&Event,"Event/I");
   ntout->Branch("LumiBlock",&LumiBlock,"LumiBlock/I");

   ntout->Branch("nrefout",&nrefout,"nrefout/I");
   ntout->Branch("jtpt",jtptout,"jtpt[nrefout]/F");
   ntout->Branch("jteta",jtetaout,"jteta[nrefout]/F");
   ntout->Branch("jtphi",jtphiout,"jtphi[nrefout]/F");
   ntout->Branch("rawpt",rawptout,"rawpt[nrefout]/F");
   ntout->Branch("discr_prob",discr_probout,"discr_prob[nrefout]/F");
   ntout->Branch("discr_csvSimple",discr_csvSimpleout,"discr_csvSimple[nrefout]/F");
   ntout->Branch("svtxm",svtxmout,"svtxm[nrefout]/F");
   ntout->Branch("mupt",muptout,"mupt[nrefout]/F");
   ntout->Branch("mueta",muetaout,"mueta[nrefout]/F");
   ntout->Branch("muphi",muphiout,"muphi[nrefout]/F");
   ntout->Branch("muptrel",muptrelout,"muptrel[nrefout]/F");
   
   ntout->Branch("bin",&bin,"bin/I");
   ntout->Branch("weightJet",&weightJet,"weightJet/F");
   ntout->Branch("weight12003",&weight12003,"weight12003/F");

   Long64_t nentries = nt->GetEntries();

   Long64_t nbytes = 0;
   int oneperc = nentries/100;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += nt->GetEntry(i);

      if (i % oneperc == 0) cout<<"\r"<<i/oneperc<<"%   "<<flush;
      nrefout = 0;
      for (int r = 0;r<nref;r++)
         if (fabs(jteta[r])<2) 
         {
          jtptout[nrefout]=jtpt[r];
          jtetaout[nrefout]=jteta[r];
          jtphiout[nrefout]=jtphi[r];
          rawptout[nrefout]=rawpt[r];
          discr_probout[nrefout]=discr_prob[r];
          discr_csvSimpleout[nrefout]=discr_csvSimple[r];
          svtxmout[nrefout]=svtxm[r];

          muptout[nrefout] = mupt[r];
          muetaout[nrefout] = mueta[r];
          muphiout[nrefout] = muphi[r];
          muptrelout[nrefout] = muptrel[r];
          nrefout++;
       }

       ntout->Fill();

    }
    cout<<endl;
    fout->Close();
    f->Close();
 }
