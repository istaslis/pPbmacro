#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

void weightPtHatBins(){
    
  gROOT->Reset();


  const int Nbins=4;
  TFile *fin[Nbins], *fout[Nbins];
  TTree *tr_in[Nbins], *tr_out[Nbins];

  Int_t bounds[Nbins] = {30,50,80,120};

  /*  Float_t xSections[6] = {
    1.079e-02,
    1.021e-03,
    9.913e-05,
    1.128e-05,
    1.470e-06, 
    5.310e-07
  };
  */
  char name[100];
  
  for (Int_t it=0; it<Nbins; it++) {

    sprintf(name,"/data_CMS/cms/mnguyen/pPbAna/QCDPPb/pthat%d/merged_HiForest.root",bounds[it]);
    fin[it] = new TFile(name);
    fin[it]->cd("/akPu4PFJetAnalyzer");

    tr_in[it] = (TTree*)gDirectory->Get("t");
    cout<<"got the tree "<<it<<endl;
   
  }
  //

  float weights[Nbins] = {1.};
  float sumofweights = 1;
  for (int i=1;i<Nbins;i++)
    {
      TString nextbound = Form("pthat>%d",bounds[i]);
      cout<<tr_in[i-1]->GetEntries(nextbound)<<endl;
      cout<<tr_in[i]->GetEntries(nextbound)<<endl;
      weights[i]=(float)weights[i-1]*tr_in[i-1]->GetEntries(nextbound)/(tr_in[i]->GetEntries(nextbound)+weights[i-1]*tr_in[i-1]->GetEntries(nextbound));
      cout<<"weight "<<bounds[i]<<" : "<<weights[i]<<endl;

      sumofweights+=weights[i];
    }

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
    //    sprintf(name,"weighted_merged_jetTrackAnalyzers_IPSVInfo_Extras_pythia%d.root",bounds[it]);
    sprintf(name,"QCDPPb_HiForest.root");
    int it=0;
    fout[it] = new TFile(name,"RECREATE");
    fout[it]->mkdir("akPu4PFJetAnalyzer");
    fout[it]->cd("akPu4PFJetAnalyzer");

    tr_out[it] = new TTree("t","Jet Analyzer");

    // Set output branch addresses.
    tr_out[it]->Branch("evt",&evt,"evt/I");
    tr_out[it]->Branch("b",&b,"b/F");
    tr_out[it]->Branch("hf",&hf,"hf/F");
    tr_out[it]->Branch("bin",&bin,"bin/I");
    tr_out[it]->Branch("vx",&vx,"vx/F");
    tr_out[it]->Branch("vy",&vy,"vy/F");
    tr_out[it]->Branch("vz",&vz,"vz/F");
    tr_out[it]->Branch("nref",&nref,"nref/I");
    tr_out[it]->Branch("rawpt",rawpt,"rawpt[nref]/F");
    tr_out[it]->Branch("jtpt",jtpt,"jtpt[nref]/F");
    tr_out[it]->Branch("jteta",jteta,"jteta[nref]/F");
    tr_out[it]->Branch("jty",jty,"jty[nref]/F");
    tr_out[it]->Branch("jtphi",jtphi,"jtphi[nref]/F");
    tr_out[it]->Branch("jtpu",jtpu,"jtpu[nref]/F");
    tr_out[it]->Branch("discr_csvMva",discr_csvMva,"discr_csvMva[nref]/F");
    tr_out[it]->Branch("discr_csvSimple",discr_csvSimple,"discr_csvSimple[nref]/F");
    tr_out[it]->Branch("discr_muByIp3",discr_muByIp3,"discr_muByIp3[nref]/F");
    tr_out[it]->Branch("discr_muByPt",discr_muByPt,"discr_muByPt[nref]/F");
    tr_out[it]->Branch("discr_prob",discr_prob,"discr_prob[nref]/F");
    tr_out[it]->Branch("discr_probb",discr_probb,"discr_probb[nref]/F");
    tr_out[it]->Branch("discr_tcHighEff",discr_tcHighEff,"discr_tcHighEff[nref]/F");
    tr_out[it]->Branch("discr_tcHighPur",discr_tcHighPur,"discr_tcHighPur[nref]/F");
    tr_out[it]->Branch("nsvtx",nsvtx,"nsvtx[nref]/I");
    tr_out[it]->Branch("svtxntrk",svtxntrk,"svtxntrk[nref]/I");
    tr_out[it]->Branch("svtxdl",svtxdl,"svtxdl[nref]/F");
    tr_out[it]->Branch("svtxdls",svtxdls,"svtxdls[nref]/F");
    tr_out[it]->Branch("svtxm",svtxm,"svtxm[nref]/F");
    tr_out[it]->Branch("svtxpt",svtxpt,"svtxpt[nref]/F");
    tr_out[it]->Branch("nIPtrk",nIPtrk,"nIPtrk[nref]/I");
    tr_out[it]->Branch("nselIPtrk",nselIPtrk,"nselIPtrk[nref]/I");
    tr_out[it]->Branch("ipJetIndex",ipJetIndex,"ipJetIndex[nselIPtrk]/I");
    tr_out[it]->Branch("ipPt",ipPt,"ipPt[nselIPtrk]/F");
    tr_out[it]->Branch("ipProb0",ipProb0,"ipProb0[nselIPtrk]/F");
    tr_out[it]->Branch("ipProb1",ipProb1,"ipProb1[nselIPtrk]/F");
    tr_out[it]->Branch("ip2d",ip2d,"ip2d[nselIPtrk]/F");
    tr_out[it]->Branch("ip2dSig",ip2dSig,"ip2dSig[nselIPtrk]/F");
    tr_out[it]->Branch("ip3d",ip3d,"ip3d[nselIPtrk]/F");
    tr_out[it]->Branch("ip3dSig",ip3dSig,"ip3dSig[nselIPtrk]/F");
    tr_out[it]->Branch("ipDist2Jet",ipDist2Jet,"ipDist2Jet[nselIPtrk]/F");
    tr_out[it]->Branch("ipDist2JetSig",ipDist2JetSig,"ipDist2JetSig[nselIPtrk]/F");
    tr_out[it]->Branch("ipClosest2Jet",ipClosest2Jet,"ipClosest2Jet[nselIPtrk]/F");    
    tr_out[it]->Branch("mue",mue,"mue[nref]/F");
    tr_out[it]->Branch("mupt",mupt,"mupt[nref]/F");
    tr_out[it]->Branch("mueta",mueta,"mueta[nref]/F");
    tr_out[it]->Branch("muphi",muphi,"muphi[nref]/F");
    tr_out[it]->Branch("mudr",mudr,"mudr[nref]/F");
    tr_out[it]->Branch("muptrel",muptrel,"muptre[nref]/F");
    tr_out[it]->Branch("muchg",muchg,"muchg[nref]/I");
    tr_out[it]->Branch("pthat",&pthat,"pthat/F");
    tr_out[it]->Branch("beamId1",&beamId1,"beamId1/I");
    tr_out[it]->Branch("beamId2",&beamId1,"beamId2/I");
    tr_out[it]->Branch("refpt",refpt,"refpt[nref]/F");
    tr_out[it]->Branch("refeta",refeta,"refeta[nref]/F");
    tr_out[it]->Branch("refy",refy,"refy[nref]/F");
    tr_out[it]->Branch("refphi",refphi,"refphi[nref]/F");
    tr_out[it]->Branch("refdphijt",refdphijt,"refdphijt[nref]/F");
    tr_out[it]->Branch("refdrjt",refdrjt,"refdrjt[nref]/F");
    tr_out[it]->Branch("refparton_pt",refparton_pt,"refparton_pt[nref]/F");
    tr_out[it]->Branch("refparton_flavor",refparton_flavor,"refparton_flavor[nref]/I");
    tr_out[it]->Branch("refparton_flavorForB",refparton_flavorForB,"refparton_flavorForB[nref]/I");
    tr_out[it]->Branch("ngen",&ngen,"ngen/I");
    tr_out[it]->Branch("genmatchindex",genmatchindex,"genmatchindex[nref]/I");
    tr_out[it]->Branch("genpt",genpt,"genpt[nref]/F");
    tr_out[it]->Branch("geneta",geneta,"geneta[nref]/F");
    tr_out[it]->Branch("geny",geny,"geny[nref]/F");
    tr_out[it]->Branch("genphi",genphi,"genphi[nref]/F");
    tr_out[it]->Branch("gendphijt",gendphijt,"gendphijt[nref]/F");
    tr_out[it]->Branch("gendrjt",gendrjt,"gendrjt[nref]/F");

    tr_out[it]->Branch("weight",&weight,"weight/F");


  for (it=0; it<Nbins; it++) {
    weights[it]/=sumofweights;
      cout<<"weight "<<bounds[it]<<" : "<<weights[it]<<endl;


    weight = weights[it];//xSections[it]/(fentries/1000.);
    cout<<"  weight ("<< bounds[it] <<") : "<< weight <<endl;



    
    // Set branch addresses.
    tr_in[it]->SetBranchAddress("evt",&evt);
    tr_in[it]->SetBranchAddress("b",&b);
    //tr_in[it]->SetBranchAddress("hf",&hf);
    //tr_in[it]->SetBranchAddress("bin",&bin);
    

    /*    root [2] hiEvtAnalyzer->cd()
      (Bool_t) true
root [3] .ls
      TDirectoryFile*hiEvtAnalyzerhiEvtAnalyzer
      KEY: TTreeHiTree;1
			 root [4] HiTree->Show(0)
    */

    tr_in[it]->AddFriend("hi=hiEvtAnalyzer/HiTree");

    tr_in[it]->SetBranchAddress("hi.vx",&vx);
    tr_in[it]->SetBranchAddress("hi.vy",&vy);
    tr_in[it]->SetBranchAddress("hi.vz",&vz);

    tr_in[it]->SetBranchAddress("nref",&nref);
    tr_in[it]->SetBranchAddress("rawpt",rawpt);
    tr_in[it]->SetBranchAddress("jtpt",jtpt);
    tr_in[it]->SetBranchAddress("jteta",jteta);
    tr_in[it]->SetBranchAddress("jty",jty);
    tr_in[it]->SetBranchAddress("jtphi",jtphi);
    tr_in[it]->SetBranchAddress("jtpu",jtpu);
    tr_in[it]->SetBranchAddress("discr_csvMva",discr_csvMva);
    tr_in[it]->SetBranchAddress("discr_csvSimple",discr_csvSimple);
    tr_in[it]->SetBranchAddress("discr_muByIp3",discr_muByIp3);
    tr_in[it]->SetBranchAddress("discr_muByPt",discr_muByPt);
    tr_in[it]->SetBranchAddress("discr_prob",discr_prob);
    tr_in[it]->SetBranchAddress("discr_probb",discr_probb);
    tr_in[it]->SetBranchAddress("discr_tcHighEff",discr_tcHighEff);
    tr_in[it]->SetBranchAddress("discr_tcHighPur",discr_tcHighPur);
    tr_in[it]->SetBranchAddress("nsvtx",nsvtx);
    tr_in[it]->SetBranchAddress("svtxntrk",svtxntrk);
    tr_in[it]->SetBranchAddress("svtxdl",svtxdl);
    tr_in[it]->SetBranchAddress("svtxdls",svtxdls);
    tr_in[it]->SetBranchAddress("svtxm",svtxm);
    tr_in[it]->SetBranchAddress("svtxpt",svtxpt);
    tr_in[it]->SetBranchAddress("nIPtrk",nIPtrk);
    tr_in[it]->SetBranchAddress("nselIPtrk",nselIPtrk);
    tr_in[it]->SetBranchAddress("ipJetIndex",ipJetIndex);
    tr_in[it]->SetBranchAddress("ipPt",ipPt);
    tr_in[it]->SetBranchAddress("ipProb0",ipProb0);
    tr_in[it]->SetBranchAddress("ipProb1",ipProb1);
    tr_in[it]->SetBranchAddress("ip2d",ip2d);
    tr_in[it]->SetBranchAddress("ip2dSig",ip2dSig);
    tr_in[it]->SetBranchAddress("ip3d",ip3d);
    tr_in[it]->SetBranchAddress("ip3dSig",ip3dSig);
    tr_in[it]->SetBranchAddress("ipDist2Jet",ipDist2Jet);
    tr_in[it]->SetBranchAddress("ipDist2JetSig",ipDist2JetSig);
    tr_in[it]->SetBranchAddress("ipClosest2Jet",ipClosest2Jet);
    tr_in[it]->SetBranchAddress("mue",mue);
    tr_in[it]->SetBranchAddress("mupt",mupt);
    tr_in[it]->SetBranchAddress("mueta",mueta);
    tr_in[it]->SetBranchAddress("muphi",muphi);
    tr_in[it]->SetBranchAddress("mudr",mudr);
    tr_in[it]->SetBranchAddress("muptrel",muptrel);
    tr_in[it]->SetBranchAddress("muchg",muchg);
    tr_in[it]->SetBranchAddress("pthat",&pthat);
    tr_in[it]->SetBranchAddress("beamId1",&beamId1);
    tr_in[it]->SetBranchAddress("beamId2",&beamId2);
    tr_in[it]->SetBranchAddress("refpt",refpt);
    tr_in[it]->SetBranchAddress("refeta",refeta);
    tr_in[it]->SetBranchAddress("refy",refy);
    tr_in[it]->SetBranchAddress("refphi",refphi);
    tr_in[it]->SetBranchAddress("refdphijt",refdphijt);
    tr_in[it]->SetBranchAddress("refdrjt",refdrjt);
    tr_in[it]->SetBranchAddress("refparton_pt",refparton_pt);
    tr_in[it]->SetBranchAddress("refparton_flavor",refparton_flavor);
    tr_in[it]->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);
    tr_in[it]->SetBranchAddress("ngen",&ngen);
    tr_in[it]->SetBranchAddress("genmatchindex",genmatchindex);
    tr_in[it]->SetBranchAddress("genpt",genpt);
    tr_in[it]->SetBranchAddress("geneta",geneta);
    tr_in[it]->SetBranchAddress("geny",geny);
    tr_in[it]->SetBranchAddress("genphi",genphi);
    tr_in[it]->SetBranchAddress("gendphijt",gendphijt);
    tr_in[it]->SetBranchAddress("gendrjt",gendrjt);



    Long64_t nentries = tr_in[it]->GetEntries();
    Long64_t nbytes = 0;

    for (Long64_t i=0; i<nentries;i++) {

      nbytes += tr_in[it]->GetEntry(i);

      /*      if (it<Nbins-1){
      	if (pthat>bounds[it+1]) continue;
      }
      */
      int ptbin = Nbins-1;
      if (pthat<bounds[0]) cout<<"pthat less than "<<bounds[0]<<"!"<<endl;
      for (int i=0;i<Nbins-1;i++)
	if (pthat>bounds[i] && pthat<bounds[i+1]) ptbin = i;
      weight = weights[ptbin];

      tr_out[0]->Fill();

    }
    
  }
    tr_out[0]->Write();

    fout[0]->Close();



}

