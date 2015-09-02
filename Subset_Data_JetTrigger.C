#include "TChain.h"
#include "TFile.h"
#include "TNtuple.h"
#include <iostream>
#include <unordered_map>
using namespace std;

struct REL {
  int run;
  int event;
  int lumiBlock;
  REL(int r, int e, int l):run(r),event(e),lumiBlock(l) {}
  bool operator==(const REL &rel) const {
    return run==rel.run && event==rel.event && lumiBlock==rel.lumiBlock;
  }
};

//template<>
//struct hash<REL>

typedef unsigned long ulong;

struct RELhash
{
  int operator()(const REL &r) const
  {
    return hash<string>()(Form("%d%d%d",r.run,r.event,r.lumiBlock));//r.run ^ r.event ^ r.lumiBlock;
  }
};

void AddFilesToChain(TChain *ch)
{
  ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/*.root");
  ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/*.root");
  ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/*.root");
  
  //ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/HiForest_10*.root");
  //ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/HiForest_10*.root");
  //ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/merged_HiForest.root");

}

/*
skimanalysis->cd()
.(Bool_t) true
root [3] .ls
TDirectoryFile*skimanalysisskimanalysis
KEY: TTreeHltTree;1
root [4] HltTree->Show(0)
======> EVENT:0
 ana_step        = 1
 phltJetHI       = 1
 pcollisionEventSelection = 0
 pHBHENoiseFilter
 */
void Subset_Data_JetTrigger()
{
  cout<<"Starting..."<<endl;

  TChain* ch = new TChain("hltobject/jetObjTree","hlt jet");
  TChain* ch2 = new TChain("hltanalysis/HltTree","hlt triggers");
  TChain* ch3 = new TChain("akPu4PFJetAnalyzer/t","jets");
  TChain* ch4 = new TChain("hiEvtAnalyzer/HiTree","hiEvent");
  TChain* ch5 = new TChain("hltMuTree/HLTMuTree","muons");
  TChain* ch6 = new TChain("skimanalysis/HltTree","skim");

  AddFilesToChain(ch);
  AddFilesToChain(ch2);
  AddFilesToChain(ch3);
  AddFilesToChain(ch4);
  AddFilesToChain(ch5);
  AddFilesToChain(ch6);
  


  int nEntries = ch->GetEntries();
  if (nEntries==ch2->GetEntries() &&
      nEntries==ch3->GetEntries() &&
      nEntries==ch4->GetEntries()) 
    cout << "# of entries: "<<nEntries<<endl;
  else
    {cout<<"non-equal # of entries in chains! exiting..."<<endl; return;}

  //  nEntries=1000;

  const int MAXSIZE = 20;

  Float_t         id;
  Float_t         pt;
  Float_t         eta;
  Float_t         phi;
  Float_t         mass;
  int             jt40, jt60, jt80, jt100;
  int             pscl40,pscl60,pscl80,pscl100;

  int             Run;
  int             Event;
  int             LumiBlock;

  int             nref;
  float           jtpt[MAXSIZE];
  float           jteta[MAXSIZE];
  float           jtphi[MAXSIZE];
  float           rawpt[MAXSIZE];
  float           discr_prob[MAXSIZE];
  float           discr_csvSimple[MAXSIZE];
  float           discr_ssvHighPur[MAXSIZE];
  float           discr_ssvHighEff[MAXSIZE];
  float           svtxm[MAXSIZE];
  float           mupt[MAXSIZE];
  float           mueta[MAXSIZE];
  float           muphi[MAXSIZE];
  float           muptrel[MAXSIZE];
  int             bin;

  int             Glb_nptl;
  float           Glb_pt[MAXSIZE];
  float           Glb_eta[MAXSIZE];
  float           Glb_phi[MAXSIZE];

  float           Glb_glbChi2_ndof[MAXSIZE];
  int             Glb_nValMuHits[MAXSIZE];
  int             Glb_nMatchedStations[MAXSIZE];
  int             Glb_nValPixHits[MAXSIZE];
  int             Glb_trkLayerWMeas[MAXSIZE];
  float           Glb_trkDxy[MAXSIZE];
  float           Glb_trkDz[MAXSIZE];

  int             nIP;
  float           ipProb0[MAXSIZE];
  int             ipJetIndex[MAXSIZE];

  int             pHBHENoiseFilter;


  ch->SetBranchAddress("id", &id);
  ch->SetBranchAddress("pt", &pt);
  ch->SetBranchAddress("eta", &eta);
  ch->SetBranchAddress("phi", &phi);
  ch->SetBranchAddress("mass", &mass);

  ch2->SetBranchAddress("HLT_PAJet40_NoJetID_v1",&jt40);
  ch2->SetBranchAddress("HLT_PAJet60_NoJetID_v1",&jt60);
  ch2->SetBranchAddress("HLT_PAJet80_NoJetID_v1",&jt80);
  ch2->SetBranchAddress("HLT_PAJet100_NoJetID_v1",&jt100);

  ch2->SetBranchAddress("HLT_PAJet40_NoJetID_v1_Prescl",&pscl40);
  ch2->SetBranchAddress("HLT_PAJet60_NoJetID_v1_Prescl",&pscl60);
  ch2->SetBranchAddress("HLT_PAJet80_NoJetID_v1_Prescl",&pscl80);
  ch2->SetBranchAddress("HLT_PAJet100_NoJetID_v1_Prescl",&pscl100);

  ch2->SetBranchAddress("Run",&Run);
  ch2->SetBranchAddress("Event",&Event);
  ch2->SetBranchAddress("LumiBlock",&LumiBlock);



  ch3->SetBranchAddress("nref", &nref);
  ch3->SetBranchAddress("jtpt", jtpt);
  ch3->SetBranchAddress("jteta", jteta);
  ch3->SetBranchAddress("jtphi", jtphi);
  ch3->SetBranchAddress("rawpt", rawpt);
  ch3->SetBranchAddress("discr_prob", discr_prob);
  ch3->SetBranchAddress("discr_csvSimple", discr_csvSimple);
  ch3->SetBranchAddress("discr_ssvHighPur", discr_ssvHighPur);
  ch3->SetBranchAddress("discr_ssvHighEff", discr_ssvHighEff);
  ch3->SetBranchAddress("svtxm", svtxm);
  ch3->SetBranchAddress("mupt", mupt);
  ch3->SetBranchAddress("mueta", mueta);
  ch3->SetBranchAddress("muphi", muphi);
  ch3->SetBranchAddress("muptrel", muptrel);

  ch3->SetBranchAddress("nIP", &nIP);
  ch3->SetBranchAddress("ipProb0", ipProb0);
  ch3->SetBranchAddress("ipJetIndex", ipJetIndex);
  
  
  ch4->SetBranchAddress("hiBin", &bin);

  ch5->SetBranchAddress("Glb_nptl",&Glb_nptl);
  ch5->SetBranchAddress("Glb_pt",Glb_pt);
  ch5->SetBranchAddress("Glb_eta",Glb_eta);
  ch5->SetBranchAddress("Glb_phi",Glb_phi);
  ch5->SetBranchAddress("Glb_glbChi2_ndof",Glb_glbChi2_ndof);
  ch5->SetBranchAddress("Glb_nValMuHits",Glb_nValMuHits);
  ch5->SetBranchAddress("Glb_nMatchedStations",Glb_nMatchedStations);
  ch5->SetBranchAddress("Glb_nValPixHits",Glb_nValPixHits);
  ch5->SetBranchAddress("Glb_trkLayerWMeas",Glb_trkLayerWMeas);
  ch5->SetBranchAddress("Glb_trkDxy",Glb_trkDxy);
  ch5->SetBranchAddress("Glb_trkDz",Glb_trkDz);

  ch6->SetBranchAddress("pHBHENoiseFilter",&pHBHENoiseFilter);

  unordered_map<REL,int,RELhash> rel;

  TFile *f = new TFile("jettrig.root","recreate");
  //TNtuple *nt = new TNtuple("nt","nt","pt:eta:phi:mass:jt40:jt60:jt80:jt100:pscl40:pscl60:pscl80:pscl100");
  TTree *nt = new TTree("jet","jet");
  nt->SetDirectory(f);
  TTree *evt = new TTree("evt","evt");
  evt->SetDirectory(f);
  TTree *muon = new TTree("muon","muon");
  muon->SetDirectory(f);

//trigger values
  nt->Branch("pt",&pt,"pt/F");
  nt->Branch("eta",&eta,"eta/F");
  nt->Branch("phi",&phi,"phi/F");
  nt->Branch("mass",&mass,"mass/F");
  nt->Branch("jt40",&jt40,"jt40/I");
  nt->Branch("jt60",&jt60,"jt60/I");
  nt->Branch("jt80",&jt80,"jt80/I");
  nt->Branch("jt100",&jt100,"jt100/I");
  nt->Branch("pscl40",&pscl40,"pscl40/I");
  nt->Branch("pscl60",&pscl60,"pscl60/I");
  nt->Branch("pscl80",&pscl80,"pscl80/I");
  nt->Branch("pscl100",&pscl100,"pscl100/I");
  nt->Branch("Run",&Run);
  nt->Branch("Event",&Event);
  nt->Branch("LumiBlock",&LumiBlock);

//jet reco values
  nt->Branch("nref", &nref, "nref/I");
  nt->Branch("jtpt",jtpt,"jtpt[nref]/F");
  nt->Branch("jteta",jteta,"jteta[nref]/F");
  nt->Branch("jtphi",jtphi,"jtphi[nref]/F");
  nt->Branch("rawpt",rawpt,"rawpt[nref]/F");
  nt->Branch("discr_prob",discr_prob,"discr_prob[nref]/F");
  nt->Branch("discr_csvSimple",discr_csvSimple,"discr_csvSimple[nref]/F");
  nt->Branch("discr_ssvHighPur",discr_ssvHighPur,"discr_ssvHighPur[nref]/F");
  nt->Branch("discr_ssvHighEff",discr_ssvHighEff,"discr_ssvHighEff[nref]/F");
  nt->Branch("svtxm",svtxm,"svtxm[nref]/F");
//
  nt->Branch("mupt", mupt, "mupt[nref]/F");
  nt->Branch("mueta", mueta, "mueta[nref]/F");
  nt->Branch("muphi", muphi, "muphi[nref]/F");
  nt->Branch("muptrel", muptrel, "muptrel[nref]/F");

  nt->Branch("nIP", &nIP, "nIP/I");
  nt->Branch("ipProb0", ipProb0, "ipProb0[nref]/F");
  nt->Branch("ipJetIndex", ipJetIndex, "ipJetIndex[nref]/I");

  //from hiEvtAnalyzer/HiTree:hiBin
  nt->Branch("bin",&bin,"bin/I");


  muon->Branch("Glb_nptl",&Glb_nptl,"Glb_nptl/I");
  muon->Branch("Glb_pt",Glb_pt, "Glb_pt[Glb_nptl]/F");
  muon->Branch("Glb_eta",Glb_eta,"Glb_eta[Glb_nptl]/F");
  muon->Branch("Glb_phi",Glb_phi,"Glb_phi[Glb_nptl]/F");

  muon->Branch("Glb_glbChi2_ndof",Glb_glbChi2_ndof,"Glb_glbChi2_ndof[Glb_nptl]/F");
  muon->Branch("Glb_nValMuHits",Glb_nValMuHits,"Glb_nValMuHits[Glb_nptl]/I");
  muon->Branch("Glb_nMatchedStations",Glb_nMatchedStations,"Glb_nMatchedStations[Glb_nptl]/I");
  muon->Branch("Glb_nValPixHits",Glb_nValPixHits,"Glb_nValPixHits[Glb_nptl]/I");
  muon->Branch("Glb_trkLayerWMeas",Glb_trkLayerWMeas,"Glb_trkLayerWMeas[Glb_nptl]/I");
  muon->Branch("Glb_trkDxy",Glb_trkDxy,"Glb_trkDxy[Glb_nptl]/F");
  muon->Branch("Glb_trkDz",Glb_trkDz,"Glb_trkDz[Glb_nptl]/F");


  int oneperc = nEntries/100;

  for (int i=0;i<nEntries;i++)
    {
      if (i % oneperc == 0) cout <<i/oneperc<<"%"<<endl;
      ch->LoadTree(i); ch2->LoadTree(i); ch3->LoadTree(i); ch4->LoadTree(i); ch5->LoadTree(i); ch6->LoadTree(i);
      ch->GetEntry(i); ch2->GetEntry(i); ch3->GetEntry(i); ch4->GetEntry(i); ch5->GetEntry(i); ch6->GetEntry(i);

      if (pHBHENoiseFilter==0) continue;

      REL r(Run,Event,LumiBlock);
      if (rel.find(r) != rel.end()) {
    	 rel[r]++;
    	 //if (rel[r]>2) cout<<r.run<<","<<r.event<<","<<r.lumiBlock<<": "<<rel[r]<<endl;
    	 continue;
      }
      else rel[r] = 1;

      nt->Fill();//pt,eta,phi,mass,jt40,jt60,jt80,jt100,pscl40,pscl60,pscl80,pscl100);
      muon->Fill();
    }

  nt->Write();
  muon->Write();
  f->Close();

}
