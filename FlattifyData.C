{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Tue Aug 11 16:05:37 2015 by ROOT version6.02/05)
//   from TTree jet/jet
//   found on file: jettrig_weight_AccCut_muons.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();
   TFile *f = new TFile("jettrig_weight_AccCut_muons.root");
   TTree *jet = (TTree *)f->Get("jet");

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
   Int_t           bin;
   Float_t         weightJet;
   Float_t         weight12003;
   Int_t           nIP;
   Int_t           nref;
   Int_t           nref2;
   Float_t         ipProb0[14];
   Float_t         jp[14];
   Int_t           ipJetIndex[14];
   Int_t           Njetmuon;
   Float_t         jtpt[14];
   Float_t         jteta[14];
   Float_t         jtphi[14];
   Float_t         rawpt[14];
   Float_t         discr_prob[14];
   Float_t         discr_csvSimple[14];
   Float_t         svtxm[14];
   Float_t         discr_ssvHighPur[14];
   Float_t         discr_ssvHighEff[14];
   Float_t         mupt[14];
   Float_t         mueta[14];
   Float_t         muphi[14];
   Float_t         muptrel[14];
   Float_t         jetmuonpt[14];
   Float_t         jetmuoneta[14];
   Float_t         jetmuonphi[14];
   Float_t         jetmuonptrel[14];
   Float_t         jetmuonip3d[14];

   // Set branch addresses.
   jet->SetBranchAddress("pt",&pt);
   jet->SetBranchAddress("eta",&eta);
   jet->SetBranchAddress("phi",&phi);
   jet->SetBranchAddress("mass",&mass);
   jet->SetBranchAddress("jt40",&jt40);
   jet->SetBranchAddress("jt60",&jt60);
   jet->SetBranchAddress("jt80",&jt80);
   jet->SetBranchAddress("jt100",&jt100);
   jet->SetBranchAddress("pscl40",&pscl40);
   jet->SetBranchAddress("pscl60",&pscl60);
   jet->SetBranchAddress("pscl80",&pscl80);
   jet->SetBranchAddress("pscl100",&pscl100);
   jet->SetBranchAddress("Run",&Run);
   jet->SetBranchAddress("Event",&Event);
   jet->SetBranchAddress("LumiBlock",&LumiBlock);
   jet->SetBranchAddress("bin",&bin);
   jet->SetBranchAddress("weightJet",&weightJet);
   jet->SetBranchAddress("weight12003",&weight12003);
   jet->SetBranchAddress("nIP",&nIP);
   jet->SetBranchAddress("nref",&nref);
   jet->SetBranchAddress("nref2",&nref2);
   jet->SetBranchAddress("ipProb0",ipProb0);
   jet->SetBranchAddress("jp",jp);
   jet->SetBranchAddress("ipJetIndex",ipJetIndex);
   jet->SetBranchAddress("Njetmuon",&Njetmuon);
   jet->SetBranchAddress("jtpt",jtpt);
   jet->SetBranchAddress("jteta",jteta);
   jet->SetBranchAddress("jtphi",jtphi);
   jet->SetBranchAddress("rawpt",rawpt);
   jet->SetBranchAddress("discr_prob",discr_prob);
   jet->SetBranchAddress("discr_csvSimple",discr_csvSimple);
   jet->SetBranchAddress("svtxm",svtxm);
   jet->SetBranchAddress("discr_ssvHighPur",discr_ssvHighPur);
   jet->SetBranchAddress("discr_ssvHighEff",discr_ssvHighEff);
   jet->SetBranchAddress("mupt",mupt);
   jet->SetBranchAddress("mueta",mueta);
   jet->SetBranchAddress("muphi",muphi);
   jet->SetBranchAddress("muptrel",muptrel);
   jet->SetBranchAddress("jetmuonpt",jetmuonpt);
   jet->SetBranchAddress("jetmuoneta",jetmuoneta);
   jet->SetBranchAddress("jetmuonphi",jetmuonphi);
   jet->SetBranchAddress("jetmuonptrel",jetmuonptrel);
   jet->SetBranchAddress("jetmuonip3d",jetmuonip3d);

   TFile *fout = new TFile("jettrig_weight_AccCut_muons_flat.root","RECREATE");
   TNtuple *n = new TNtuple("jet","jet","jtpt:jteta:jtphi:rawpt:discr_prob:discr_csvSimple:svtxm:discr_ssvHighPur:discr_ssvHighEff:jetmuonpt:jetmuonptrel:jetmuonip3d:logmuip3d:weightJet");

   Long64_t nentries = jet->GetEntries();

   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += jet->GetEntry(i);

      for (int j=0;j<nref;j++)
	n->Fill(jtpt[j],jteta[j],jtphi[j],rawpt[j],discr_prob[j],discr_csvSimple[j],svtxm[j],discr_ssvHighPur[j],discr_ssvHighEff[j],jetmuonpt[j],jetmuonptrel[j],jetmuonip3d[j],log(jetmuonip3d[j]),weightJet);

   }

   n->Write();
   fout->Close();

}
