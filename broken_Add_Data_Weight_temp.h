//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 26 15:15:06 2015 by ROOT version 6.02/05
// from TChain hltobject/jetObjTree/j
//////////////////////////////////////////////////////////

#ifndef temp_h
#define temp_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include "TProofOutputFile.h"
#include "TH1F.h"
#include "TColor.h"
#include "TCanvas.h"

// Header file for the classes stored in the TTree if any.

class temp : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   TChain         *trigChain;
   TH1F           *spectrum;


   TProofOutputFile *fProofFile;
   TFile           *fFile;
// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         id;
   Float_t         pt;
   Float_t         eta;
   Float_t         phi;
   Float_t         mass;
   int            jt40, jt60, jt80, jt100;
   int           pscl40,pscl60,pscl80,pscl100;

   // List of branches
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

   temp(TTree * /*tree*/ =0) : fChain(0) {
     trigChain = new TChain("hltanalysis/HltTree","t");
     //trigChain->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/*.root");
     //trigChain->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/*.root");
     //trigChain->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/*.root");
     
     trigChain->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb60/HiForest_10*.root");
     trigChain->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb40/HiForest_10*.root");
     trigChain->Add("/data_CMS/cms/mnguyen/pPbAna/data/pPb80100/merged_HiForest.root");


 }
   virtual ~temp() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0)   
   { 
     if (trigChain==0) cout<<"trigChain==0!!!"<<endl;
     trigChain->LoadTree(entry);
     trigChain->GetTree()->GetEntry(entry, getall);

     return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
     
   }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(temp,0);
};

#endif

#ifdef temp_cxx
void temp::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
  cout<<"try to add it!"<<endl;
  if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("id", &id, &b_id);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("mass", &mass, &b_mass);

   trigChain->SetBranchAddress("HLT_PAJet40_NoJetID_v1",&jt40,&b_HLT_PAJet40_NoJetID_v1);
   trigChain->SetBranchAddress("HLT_PAJet60_NoJetID_v1",&jt60,&b_HLT_PAJet60_NoJetID_v1);
   trigChain->SetBranchAddress("HLT_PAJet80_NoJetID_v1",&jt80,&b_HLT_PAJet80_NoJetID_v1);
   trigChain->SetBranchAddress("HLT_PAJet100_NoJetID_v1",&jt100,&b_HLT_PAJet100_NoJetID_v1);

   trigChain->SetBranchAddress("HLT_PAJet40_NoJetID_v1_Prescl",&pscl40,&b_HLT_PAJet40_NoJetID_v1_Prescl);
   trigChain->SetBranchAddress("HLT_PAJet60_NoJetID_v1_Prescl",&pscl60,&b_HLT_PAJet60_NoJetID_v1_Prescl);
   trigChain->SetBranchAddress("HLT_PAJet80_NoJetID_v1_Prescl",&pscl80,&b_HLT_PAJet80_NoJetID_v1_Prescl);
   trigChain->SetBranchAddress("HLT_PAJet100_NoJetID_v1_Prescl",&pscl100,&b_HLT_PAJet100_NoJetID_v1_Prescl);


   //    HLT_PAJet20_NoJetID_v1_Prescl



   //   cout<<"Added! it\'s fine! "<<tree->GetEntriesFast()<<endl;
}

Bool_t temp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef temp_cxx
