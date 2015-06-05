//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri May 15 00:29:59 2015 by ROOT version 6.03/05
// from TTree t/tree after selection
// found on file: QCDPPb_etacut.root
//////////////////////////////////////////////////////////

#ifndef t_h
#define t_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "TCanvas.h"
#include <TProofOutputFile.h>
#include "TSystem.h"
#include <map>
#include "THStack.h"

// Header file for the classes stored in the TTree if any.

class t : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TFile            *fFile;
   TH1F *ftaggedandB,*ftagged, *fB;
   TH1F *ftaggedandB2,*ftagged2, *fB2;
   TH1F *fInd;
   TH1F *eff,*pur,*eff2,*pur2;

   //   std::map<int, TH1F*> bkgpartonmap;

   TH1F *bkgpartonB,*bkgpartonC, *bkgpartonUSDG;
   THStack *bkgparton;

   TH1F *diB_B, *diB_C,*diB_else,*diC_C,*diC_else,*dielse;
   THStack *dibkg;

   TH1F *AjMC;


   TProofOutputFile *fProofFile;
// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           evt;
   Float_t         weight;
   Float_t         pthat;
   Int_t           nrefin;
   Int_t           nrefout;
   Float_t         jtpt[51];   //[nrefout]
   Float_t         jteta[51];   //[nrefout]
   Float_t         jtphi[51];   //[nrefout]
   Float_t         rawpt[51];   //[nrefout]
   Float_t         refpt[51];   //[nrefout]
   Int_t           refparton_flavorForB[51];   //[nrefout]
   Float_t         discr_prob[51];   //[nrefout]
   Float_t         discr_csvSimple[51];   //[nrefout]
   Float_t         svtxm[51];   //[nrefout]
   Int_t           bin;

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_nrefin;   //!
   TBranch        *b_nrefout;   //!
   TBranch        *b_jtpt;   //!
   TBranch        *b_jteta;   //!
   TBranch        *b_jtphi;   //!
   TBranch        *b_rawpt;   //!
   TBranch        *b_refpt;   //!
   TBranch        *b_refparton_flavorForB;   //!
   TBranch        *b_discr_prob;   //!
   TBranch        *b_discr_csvSimple;   //!
   TBranch        *b_svtxm;   //!
   TBranch        *b_bin;   //!

   t(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~t() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(t,0);
};

#endif

#ifdef t_cxx
void t::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("pthat", &pthat, &b_pthat);
   fChain->SetBranchAddress("nrefin", &nrefin, &b_nrefin);
   fChain->SetBranchAddress("nrefout", &nrefout, &b_nrefout);
   fChain->SetBranchAddress("jtpt", jtpt, &b_jtpt);
   fChain->SetBranchAddress("jteta", jteta, &b_jteta);
   fChain->SetBranchAddress("jtphi", jtphi, &b_jtphi);
   fChain->SetBranchAddress("rawpt", rawpt, &b_rawpt);
   fChain->SetBranchAddress("refpt", refpt, &b_refpt);
   fChain->SetBranchAddress("refparton_flavorForB", refparton_flavorForB, &b_refparton_flavorForB);
   fChain->SetBranchAddress("discr_prob", discr_prob, &b_discr_prob);
   fChain->SetBranchAddress("discr_csvSimple", discr_csvSimple, &b_discr_csvSimple);
   fChain->SetBranchAddress("svtxm", svtxm, &b_svtxm);
   fChain->SetBranchAddress("bin", &bin, &b_bin);
}

Bool_t t::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef t_cxx
