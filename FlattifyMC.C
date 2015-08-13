{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Tue Aug 11 15:51:31 2015 by ROOT version6.02/05)
//   from TTree jet/akPu4PFpatJetsWithBtagging Jet Analysis Tree
//   found on file: QCDPPb_genmuonmerged.root
//////////////////////////////////////////////////////////

//Reset ROOT and connect tree file
   gROOT->Reset();
   f = new TFile("QCDPPb_genmuonmerged.root");
   
   TTree *jet = (TTree *)f->Get("jet");

//Declaration of leaves types
   Int_t           evt;
   Float_t         b;
   Int_t           nref;
   Float_t         mue[68];
   Float_t         mudr[68];
   Int_t           muchg[68];
   Float_t         pthat;
   Int_t           ngen;
   Int_t           genmatchindex[57];
   Float_t         genpt[57];
   Float_t         geneta[57];
   Float_t         geny[57];
   Float_t         genphi[57];
   Float_t         weight;
   Int_t           nIP;
   Int_t           nref2;
   Float_t         ipProb0[0];
   Float_t         jp[10];
   Int_t           ipJetIndex[0];
   Int_t           Njetmuon;
   Float_t         jtpt[68];
   Float_t         jteta[68];
   Float_t         jtphi[68];
   Float_t         rawpt[68];
   Float_t         discr_prob[68];
   Float_t         discr_csvSimple[68];
   Float_t         svtxm[68];
   Float_t         discr_ssvHighPur[68];
   Float_t         discr_ssvHighEff[68];
   Float_t         mupt[68];
   Float_t         mueta[68];
   Float_t         muphi[68];
   Float_t         muptrel[68];
   Float_t         jty[68];
   Float_t         refeta[68];
   Float_t         refy[68];
   Float_t         refphi[68];
   Float_t         refparton_pt[68];
   Float_t         refpt[68];
   Float_t         jetmuonpt[10];
   Float_t         jetmuoneta[10];
   Float_t         jetmuonphi[10];
   Float_t         jetmuonptrel[10];
   Float_t         jetmuonip3d[10];
   Int_t           refparton_flavorForB[68];

   // Set branch addresses.
   jet->SetBranchAddress("evt",&evt);
   jet->SetBranchAddress("b",&b);
   jet->SetBranchAddress("nref",&nref);
   jet->SetBranchAddress("mue",mue);
   jet->SetBranchAddress("mudr",mudr);
   jet->SetBranchAddress("muchg",muchg);
   jet->SetBranchAddress("pthat",&pthat);
   jet->SetBranchAddress("ngen",&ngen);
   jet->SetBranchAddress("genmatchindex",genmatchindex);
   jet->SetBranchAddress("genpt",genpt);
   jet->SetBranchAddress("geneta",geneta);
   jet->SetBranchAddress("geny",geny);
   jet->SetBranchAddress("genphi",genphi);
   jet->SetBranchAddress("weight",&weight);
   jet->SetBranchAddress("nIP",&nIP);
   jet->SetBranchAddress("nref2",&nref2);
   jet->SetBranchAddress("ipProb0",&ipProb0);
   jet->SetBranchAddress("jp",jp);
   jet->SetBranchAddress("ipJetIndex",&ipJetIndex);
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
   jet->SetBranchAddress("jty",jty);
   jet->SetBranchAddress("refeta",refeta);
   jet->SetBranchAddress("refy",refy);
   jet->SetBranchAddress("refphi",refphi);
   jet->SetBranchAddress("refparton_pt",refparton_pt);
   jet->SetBranchAddress("refpt",refpt);
   jet->SetBranchAddress("jetmuonpt",jetmuonpt);
   jet->SetBranchAddress("jetmuoneta",jetmuoneta);
   jet->SetBranchAddress("jetmuonphi",jetmuonphi);
   jet->SetBranchAddress("jetmuonptrel",jetmuonptrel);
   jet->SetBranchAddress("jetmuonip3d",jetmuonip3d);
   jet->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// jet->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   TFile *fout = new TFile("QCDPPb_genmuonmerged_flat.root","RECREATE");
   TNtuple *n = new TNtuple("jet","jet","jtpt:jteta:jtphi:rawpt:discr_prob:discr_csvSimple:svtxm:discr_ssvHighPur:discr_ssvHighEff:refparton_flavorForB:jetmuonpt:jetmuonptrel:jetmuonip3d:logmuip3d:weight");

   Long64_t nentries = jet->GetEntries();

   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += jet->GetEntry(i);

      for (int j=0;j<nref;j++)
	if (jtpt[j]-pthat<80) //if pthat>jtpt - fine...
	  n->Fill(jtpt[j],jteta[j],jtphi[j],rawpt[j],discr_prob[j],discr_csvSimple[j],svtxm[j],discr_ssvHighPur[j],discr_ssvHighEff[j],refparton_flavorForB[j],jetmuonpt[j],jetmuonptrel[j],jetmuonip3d[j],log(jetmuonip3d[j]),weight);

   }

   n->Write();
   fout->Close();
}
