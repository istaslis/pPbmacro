#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include <algorithm>

void Subset_MC_AccCut(){
    
  gROOT->Reset();


  TFile *fin, *fout;
  TTree *tr_in, *tr_out;
  //  TNtuple *nt_out;

  char *sample = "QCDPPb";//"QCDPPb";//bJetPPb
  char *name = Form("%s_HiForest.root",sample);
  char *outname = Form("%s_AccCut.root",sample);
  fin = new TFile(name);
  fin->cd("/akPu4PFJetAnalyzer");

  tr_in = (TTree*)gDirectory->Get("t");
   
  
  //Declaration of leaves types
    Int_t           evt;
    Float_t         b;
    Float_t         hf;
    Int_t           bin;
    Int_t           nref;
    Float_t         vx, vy, vz;
    Float_t         rawpt[1000];
    Float_t         jtpt[1000];
    Float_t         jteta[1000];
    Float_t         jty[1000];
    Float_t         jtphi[1000];
    Float_t         jtpu[1000];
    Float_t         discr_csvMva[1000];
    Float_t         discr_csvSimple[1000];
    Float_t         discr_muByIp3[1000];
    Float_t         discr_muByPt[1000];
    Float_t         discr_prob[1000];
    Float_t         discr_probb[1000];
    Float_t         discr_tcHighEff[1000];
    Float_t         discr_tcHighPur[1000];
    Int_t         nsvtx[1000];
    Int_t         svtxntrk[1000];
    Float_t         svtxdl[1000];
    Float_t         svtxdls[1000];
    Float_t         svtxm[1000];
    Float_t         svtxpt[1000];
    Int_t         nIPtrk[1000];
    Int_t         nselIPtrk[1000];
    Int_t ipJetIndex[1000];
    Float_t ipPt[1000];
    Float_t ipProb0[1000];
    Float_t ipProb1[1000];
    Float_t ip2d[1000];
    Float_t ip2dSig[1000];
    Float_t ip3d[1000];
    Float_t ip3dSig[1000];
    Float_t ipDist2Jet[1000];
    Float_t ipDist2JetSig[1000];
    Float_t ipClosest2Jet[1000];

    Float_t         mue[1000];
    Float_t         mupt[1000];
    Float_t         mueta[1000];
    Float_t         muphi[1000];
    Float_t         mudr[1000];
    Float_t         muptrel[1000];
    Int_t           muchg[1000];
    Float_t         pthat;
    Int_t         beamId1;
    Int_t         beamId2;
    Float_t         refpt[1000];
    Float_t         refeta[1000];
    Float_t         refy[1000];
    Float_t         refphi[1000];
    Float_t         refdphijt[1000];
    Float_t         refdrjt[1000];
    Float_t         refparton_pt[1000];
    Int_t           refparton_flavor[1000];
    Int_t           refparton_flavorForB[1000];
    Int_t           ngen;
    Int_t           genmatchindex[1000];
    Float_t         genpt[1000];
    Float_t         geneta[1000];
    Float_t         geny[1000];
    Float_t         genphi[1000];
    Float_t         gendphijt[1000];
    Float_t         gendrjt[1000];

    Float_t weight;

    // Set branch addresses.
    tr_in->SetBranchAddress("evt",&evt);
    tr_in->SetBranchAddress("b",&b);
    //tr_in->SetBranchAddress("hf",&hf);
    //tr_in->SetBranchAddress("bin",&bin);
    
    tr_in->SetBranchAddress("weight",&weight);


    tr_in->SetBranchAddress("vx",&vx);
    tr_in->SetBranchAddress("vy",&vy);
    tr_in->SetBranchAddress("vz",&vz);

    tr_in->SetBranchAddress("nref",&nref);
    tr_in->SetBranchAddress("rawpt",rawpt);
    tr_in->SetBranchAddress("jtpt",jtpt);
    tr_in->SetBranchAddress("jteta",jteta);
    tr_in->SetBranchAddress("jty",jty);
    tr_in->SetBranchAddress("jtphi",jtphi);
    tr_in->SetBranchAddress("jtpu",jtpu);
    tr_in->SetBranchAddress("discr_csvMva",discr_csvMva);
    tr_in->SetBranchAddress("discr_csvSimple",discr_csvSimple);
    tr_in->SetBranchAddress("discr_muByIp3",discr_muByIp3);
    tr_in->SetBranchAddress("discr_muByPt",discr_muByPt);
    tr_in->SetBranchAddress("discr_prob",discr_prob);
    tr_in->SetBranchAddress("discr_probb",discr_probb);
    tr_in->SetBranchAddress("discr_tcHighEff",discr_tcHighEff);
    tr_in->SetBranchAddress("discr_tcHighPur",discr_tcHighPur);
    tr_in->SetBranchAddress("nsvtx",nsvtx);
    tr_in->SetBranchAddress("svtxntrk",svtxntrk);
    tr_in->SetBranchAddress("svtxdl",svtxdl);
    tr_in->SetBranchAddress("svtxdls",svtxdls);
    tr_in->SetBranchAddress("svtxm",svtxm);
    tr_in->SetBranchAddress("svtxpt",svtxpt);
    tr_in->SetBranchAddress("nIPtrk",nIPtrk);
    tr_in->SetBranchAddress("nselIPtrk",nselIPtrk);
    tr_in->SetBranchAddress("ipJetIndex",ipJetIndex);
    tr_in->SetBranchAddress("ipPt",ipPt);
    tr_in->SetBranchAddress("ipProb0",ipProb0);
    tr_in->SetBranchAddress("ipProb1",ipProb1);
    tr_in->SetBranchAddress("ip2d",ip2d);
    tr_in->SetBranchAddress("ip2dSig",ip2dSig);
    tr_in->SetBranchAddress("ip3d",ip3d);
    tr_in->SetBranchAddress("ip3dSig",ip3dSig);
    tr_in->SetBranchAddress("ipDist2Jet",ipDist2Jet);
    tr_in->SetBranchAddress("ipDist2JetSig",ipDist2JetSig);
    tr_in->SetBranchAddress("ipClosest2Jet",ipClosest2Jet);
    tr_in->SetBranchAddress("mue",mue);
    tr_in->SetBranchAddress("mupt",mupt);
    tr_in->SetBranchAddress("mueta",mueta);
    tr_in->SetBranchAddress("muphi",muphi);
    tr_in->SetBranchAddress("mudr",mudr);
    tr_in->SetBranchAddress("muptrel",muptrel);
    tr_in->SetBranchAddress("muchg",muchg);
    tr_in->SetBranchAddress("pthat",&pthat);
    tr_in->SetBranchAddress("beamId1",&beamId1);
    tr_in->SetBranchAddress("beamId2",&beamId2);
    tr_in->SetBranchAddress("refpt",refpt);
    tr_in->SetBranchAddress("refeta",refeta);
    tr_in->SetBranchAddress("refy",refy);
    tr_in->SetBranchAddress("refphi",refphi);
    tr_in->SetBranchAddress("refdphijt",refdphijt);
    tr_in->SetBranchAddress("refdrjt",refdrjt);
    tr_in->SetBranchAddress("refparton_pt",refparton_pt);
    tr_in->SetBranchAddress("refparton_flavor",refparton_flavor);
    tr_in->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);
    tr_in->SetBranchAddress("ngen",&ngen);
    tr_in->SetBranchAddress("genmatchindex",genmatchindex);
    tr_in->SetBranchAddress("genpt",genpt);
    tr_in->SetBranchAddress("geneta",geneta);
    tr_in->SetBranchAddress("geny",geny);
    tr_in->SetBranchAddress("genphi",genphi);
    tr_in->SetBranchAddress("gendphijt",gendphijt);
    tr_in->SetBranchAddress("gendrjt",gendrjt);


    Int_t           nrefout;

    Float_t         jtptout[1000];
    Float_t         jtetaout[1000];
    Float_t         jtphiout[1000];
    Float_t         rawptout[1000];
    Float_t         refptout[1000];
    Int_t         refparton_flavorForBout[1000];
    Float_t         discr_probout[1000];
    Float_t         discr_csvSimpleout[1000];
    Float_t         svtxmout[1000];


    fout = new TFile(outname,"recreate");
    //    nt_out = new TNtuple("jtasymmetry","jtasymmetry","jtpt1:jtpt2:weight:pthat");

    tr_out = new TTree("t","tree after selection");

    tr_out->Branch("evt",&evt,"evt/I");

    tr_out->Branch("weight",&weight,"weight/F");
    tr_out->Branch("pthat",&pthat,"pthat/F");

    tr_out->Branch("nrefin",&nref,"nrefin/I");
    tr_out->Branch("nrefout",&nrefout,"nrefout/I");
    
    tr_out->Branch("jtpt",jtptout,"jtpt[nrefout]/F");
    tr_out->Branch("jteta",jtetaout,"jteta[nrefout]/F");
    tr_out->Branch("jtphi",jtphiout,"jtphi[nrefout]/F");
    tr_out->Branch("rawpt",rawptout,"rawpt[nrefout]/F");
    tr_out->Branch("refpt",refptout,"refpt[nrefout]/F");
    tr_out->Branch("refparton_flavorForB",refparton_flavorForBout,"refparton_flavorForB[nrefout]/I");
    tr_out->Branch("discr_prob",discr_probout,"discr_prob[nrefout]/F");
    tr_out->Branch("discr_csvSimple",discr_csvSimpleout,"discr_csvSimple[nrefout]/F");
    tr_out->Branch("svtxm",svtxmout,"svtxm[nrefout]/F");
    tr_out->Branch("bin",&bin,"bin/I");
    
    tr_out->Branch("mue",mue,"mue[nrefout]/F");
    tr_out->Branch("mupt",mupt,"mupt[nrefout]/F");
    tr_out->Branch("mueta",mueta,"mueta[nrefout]/F");
    tr_out->Branch("muphi",muphi,"muphi[nrefout]/F");
    tr_out->Branch("mudr",mudr,"mudr[nrefout]/F");
    tr_out->Branch("muptrel",muptrel,"muptrel[nrefout]/F");
    tr_out->Branch("muchg",muchg,"muchg[nrefout]/F");




    Long64_t nentries = tr_in->GetEntries();
    Long64_t nbytes = 0;

    for (Long64_t i=0; i<nentries;i++) {
      nbytes += tr_in->GetEntry(i);

           nrefout = 0;
      for (int r = 0;r<nref;r++)
       	if (fabs(jteta[r])<2 && rawpt[r]>18.) 
	  {
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
      

      tr_out->Fill();
      


    if (i%1000 == 0) std::cout<<i<<"/"<<nentries<<"  "<<nrefout<<std::endl;

    }
    

    tr_out->Write();


    fout->Close();


}

