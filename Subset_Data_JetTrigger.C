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
  //bool operator()(const REL &rel1, const REL &rel2) const {
  //  return rel1.Run==rel2.Run && rel1.Event==rel2.Event && rel3.LumiBlock==rel3.LumiBlock;
  //}
  bool operator==(const REL &rel) const {
    return run==rel.run && event==rel.event && lumiBlock==rel.lumiBlock;
  }
  //bool operator!=(const REL &rel) const {
  //  return Run!=rel.Run || Event!=rel.Event || LumiBlock!=rel.LumiBlock;
  //}
  //bool operator<(const REL &rel) const {
  //  return Run+Event+LumiBlock<rel.Run+rel.Event+rel.LumiBlock;
  // }
};

//template<>
//struct hash<REL>
struct RELhash
{
  int operator()(const REL &r) const
  {
    return r.run ^ r.event ^ r.lumiBlock;
  }
};

void CopyTrigger()
{
  TChain* ch = new TChain("hltobject/jetObjTree","t");
  ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/*.root");
  ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/*.root");
  ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/*.root");

  //ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/HiForest_10*.root");
  //ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/HiForest_10*.root");
  //ch->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/merged_HiForest.root");

  TChain* ch2 = new TChain("hltanalysis/HltTree","t");
  ch2->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/*.root");
  ch2->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/*.root");
  ch2->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/*.root");
  //  ch2->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/HiForest_10*.root");
  //ch2->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/HiForest_10*.root");
  //ch2->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/merged_HiForest.root");

  int nEntries = ch->GetEntries();
  int n2 = ch2->GetEntries();
  cout<<"chain 1 has "<<nEntries<<" entries"<<endl;
  cout<<"chain 2 has "<<n2<<" entries"<<endl;
  if (nEntries!=n2) {cout<<"not equal! exiting..."<<endl; return;}

  //  nEntries=10000;

  Float_t         id;
  Float_t         pt;
  Float_t         eta;
  Float_t         phi;
  Float_t         mass;
  int            jt40, jt60, jt80, jt100;
  int           pscl40,pscl60,pscl80,pscl100;

  int             Run;
  int             Event;
  int             LumiBlock;

  TBranch        *b_id;   //!                                                                                              
  TBranch        *b_pt;   //!                                                                                              
  TBranch        *b_eta;   //!                                                                                             
  TBranch        *b_phi;   //!                                                                                             
  TBranch        *b_mass;   //!                                                                                            


  TBranch        *b_HLT_PAJet40_NoJetID_v1;
  TBranch        *b_HLT_PAJet60_NoJetID_v1;
  TBranch        *b_HLT_PAJet80_NoJetID_v1;
  TBranch        *b_HLT_PAJet100_NoJetID_v1;

  TBranch        *b_HLT_PAJet40_NoJetID_v1_Prescl;
  TBranch        *b_HLT_PAJet60_NoJetID_v1_Prescl;
  TBranch        *b_HLT_PAJet80_NoJetID_v1_Prescl;
  TBranch        *b_HLT_PAJet100_NoJetID_v1_Prescl;

  TBranch        *b_Run;
  TBranch        *b_Event;
  TBranch        *b_LumiBlock;


  ch->SetBranchAddress("id", &id, &b_id);
  ch->SetBranchAddress("pt", &pt, &b_pt);
  ch->SetBranchAddress("eta", &eta, &b_eta);
  ch->SetBranchAddress("phi", &phi, &b_phi);
  ch->SetBranchAddress("mass", &mass, &b_mass);

  ch2->SetBranchAddress("HLT_PAJet40_NoJetID_v1",&jt40,&b_HLT_PAJet40_NoJetID_v1);
  ch2->SetBranchAddress("HLT_PAJet60_NoJetID_v1",&jt60,&b_HLT_PAJet60_NoJetID_v1);
  ch2->SetBranchAddress("HLT_PAJet80_NoJetID_v1",&jt80,&b_HLT_PAJet80_NoJetID_v1);
  ch2->SetBranchAddress("HLT_PAJet100_NoJetID_v1",&jt100,&b_HLT_PAJet100_NoJetID_v1);

  ch2->SetBranchAddress("HLT_PAJet40_NoJetID_v1_Prescl",&pscl40,&b_HLT_PAJet40_NoJetID_v1_Prescl);
  ch2->SetBranchAddress("HLT_PAJet60_NoJetID_v1_Prescl",&pscl60,&b_HLT_PAJet60_NoJetID_v1_Prescl);
  ch2->SetBranchAddress("HLT_PAJet80_NoJetID_v1_Prescl",&pscl80,&b_HLT_PAJet80_NoJetID_v1_Prescl);
  ch2->SetBranchAddress("HLT_PAJet100_NoJetID_v1_Prescl",&pscl100,&b_HLT_PAJet100_NoJetID_v1_Prescl);

  ch2->SetBranchAddress("Run",&Run,&b_Run);
  ch2->SetBranchAddress("Event",&Event,&b_Event);
  ch2->SetBranchAddress("LumiBlock",&LumiBlock,&b_LumiBlock);

  REL r(1,2,3);
  cout<<r.run<<","<<r.event<<","<<r.lumiBlock<<endl;

  unordered_map<REL,int,RELhash> rel;

  TFile *f = new TFile("jettrig.root","recreate");
  TNtuple *nt = new TNtuple("nt","nt","pt:eta:phi:mass:jt40:jt60:jt80:jt100:pscl40:pscl60:pscl80:pscl100");
  nt->SetDirectory(f);

  int oneperc = nEntries/100;

  for (int i=0;i<nEntries;i++)
    {
      if (i % oneperc == 0) cout <<i/oneperc<<"%"<<endl;
      ch->LoadTree(i);
      ch2->LoadTree(i);
      ch->GetEntry(i);
      ch2->GetEntry(i);

      REL r(Run,Event,LumiBlock);
      if (rel.find(r) != rel.end()) {
	rel[r]++;
	cout<<r.run<<","<<r.event<<","<<r.lumiBlock<<": "<<rel[r]<<endl;
	continue;
      } 
      else rel[r] = 1;

      nt->Fill(pt,eta,phi,mass,jt40,jt60,jt80,jt100,pscl40,pscl60,pscl80,pscl100);
    }

  nt->Write();
  f->Close();

}
