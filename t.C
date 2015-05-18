#define t_cxx
// The class definition in t.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("t.C")
// root> T->Process("t.C","some options")
// root> T->Process("t.C+")
//

#include "t.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include "TColor.h"
#include "TLegend.h"
//#include "/grid_mnt/vol__vol_U__u/llr/cms/lisniak/rootlogon.C"

#define PI 3.14159

void t::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

int nbins = 20;
float xmin = 0;
float xmax = PI; //2pi

void t::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

  TDirectory *savedir = gDirectory;

   TString option = GetOption();

   fProofFile = new TProofOutputFile("NEWSimpleNtuple.root");
  // Open the file
  fFile = fProofFile->OpenFile("RECREATE");


  savedir->cd();



   ftaggedandB = new TH1F("ftaggedandB", "Tagged and B jets", nbins, xmin, xmax);
   ftagged = new TH1F("ftagged", "Tagged jets", nbins, xmin, xmax);
   fB = new TH1F("fB", "B jets", nbins, xmin, xmax);
   ftaggedandB2 = new TH1F("ftaggedandB2", "Tagged and B jets - double", nbins, xmin, xmax);
   ftagged2 = new TH1F("ftagged2", "Tagged jets - double", nbins, xmin, xmax);
   fB2 = new TH1F("fB2", "B jets - double", nbins, xmin, xmax);

   fInd = new TH1F("fInd","b-jet index",5,0,5);

   ftaggedandB->Sumw2(); ftagged->Sumw2(); fB->Sumw2();
   ftaggedandB2->Sumw2(); ftagged2->Sumw2(); fB2->Sumw2();

  // fOutput->Add(fH1F);

  // fH1F->SetDirectory(fFile);

}

Bool_t t::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either t::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  //    Double_t x = fRandom->Gaus(0.,1.);
  GetEntry(entry);

  float csvvalue = 0.9;

  float valueToDraw = fabs(jtphi[0]);

  if (rawpt[0]>18 && jtpt[0]>60 && jtpt[1]>30){
    if (discr_csvSimple[0]>csvvalue && fabs(refparton_flavorForB[0])==5)
      ftaggedandB->Fill(valueToDraw);

    if (discr_csvSimple[0]>csvvalue)
      ftagged->Fill(valueToDraw);

    if (fabs(refparton_flavorForB[0])==5)
      fB->Fill(valueToDraw);
    

  }
  if (nrefout==5)
    for (int i=0;i<nrefout;i++)
      if (rawpt[i]>18 && fabs(refparton_flavorForB[i])==5) fInd->Fill(i);

  float deltaphi = fabs(jtphi[0] - jtphi[1]);
  valueToDraw = deltaphi > PI ? 2*PI-deltaphi : deltaphi;

  if (rawpt[0]>18 && rawpt[1]>18  && jtpt[0]>60 && jtpt[1]>30){
    if (discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue && 
	       fabs(refparton_flavorForB[0])==5 && fabs(refparton_flavorForB[1])==5)
      ftaggedandB2->Fill(valueToDraw);

    if (discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue)
      ftagged2->Fill(valueToDraw); 

    if (fabs(refparton_flavorForB[0])==5 && fabs(refparton_flavorForB[1])==5)
      fB2->Fill(valueToDraw);
    
  }




   return kTRUE;
}

void t::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

  TDirectory *savedir = gDirectory;

  if(fFile) {
    fFile->cd();
    bool ok = false;
    ftaggedandB->Write();
    ftagged->Write();
    fB->Write();
    ftaggedandB2->Write();
    ftagged2->Write();
    fB2->Write();
    fInd->Write();

    fB->SetDirectory(0);
    gDirectory = savedir;

    fFile->Close();

    fProofFile->Print();
    fOutput->Add(fProofFile);

  }
}

void t::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  TColor *pal = new TColor();

  int kblue2    = pal->GetColor(  9,  0,200);
  int kred2     = pal->GetColor(190,  0,  3);

  int kblueLight2    = pal->GetColor(178,185,254);
  int kredLight2     = pal->GetColor(253,169,179);



  fFile = new TFile("NEWSimpleNtuple.root");
  
  ftaggedandB = dynamic_cast<TH1F *>(fFile->Get("ftaggedandB"));
  ftagged = dynamic_cast<TH1F *>(fFile->Get("ftagged"));
  fB = dynamic_cast<TH1F *>(fFile->Get("fB"));
  ftaggedandB2 = dynamic_cast<TH1F *>(fFile->Get("ftaggedandB2"));
  ftagged2 = dynamic_cast<TH1F *>(fFile->Get("ftagged2"));
  fB2 = dynamic_cast<TH1F *>(fFile->Get("fB2"));
  fInd = dynamic_cast<TH1F *>(fFile->Get("fInd"));

  eff = new TH1F("eff", "Tagging efficiency", nbins, xmin, xmax); eff->SetDirectory(0);
  pur = new TH1F("pur", "Tagging purity", nbins, xmin, xmax); pur->SetDirectory(0);
  eff2 = new TH1F("eff2", "Tagging efficiency - double", nbins, xmin, xmax); eff2->SetDirectory(0);
  pur2 = new TH1F("pur2", "Tagging purity - double", nbins, xmin, xmax); pur2->SetDirectory(0);



  //TH1F *tagged2vs1 = new TH1F("tagged2vs1", "Double tagged / once tagged", nbins, xmin, xmax); tagged2vs1->SetDirectory(0);
  //TH1F *bjet2vs1 = new TH1F("bjet2vs1", "dijet / jt", nbins, xmin, xmax); bjet2vs1->SetDirectory(0);

  eff->Divide(ftaggedandB,fB,1,1,"B"); eff->SetMarkerColor(kred2); eff->SetLineColor(kredLight2);
  pur->Divide(ftaggedandB,ftagged,1,1,"B"); pur->SetMarkerColor(kblue2); pur->SetLineColor(kblueLight2);
  eff2->Divide(ftaggedandB2,fB2,1,1,"B"); eff2->SetMarkerColor(kred2); eff2->SetLineColor(kredLight2);
  pur2->Divide(ftaggedandB2,ftagged2,1,1,"B"); pur2->SetMarkerColor(kblue2); pur2->SetLineColor(kblueLight2);

  //  tagged2vs1->Divide(ftagged2,ftagged,1,1,"B"); tagged2vs1->SetMarkerColor(kblue2); tagged2vs1->SetLineColor(kblueLight2);
  //bjet2vs1->Divide(fB2,fB,1,1,"B"); bjet2vs1->SetMarkerColor(kblue2); bjet2vs1->SetLineColor(kblueLight2);

  eff->GetXaxis()->SetTitle("#phi");
  pur->GetXaxis()->SetTitle("#phi");
  eff2->GetXaxis()->SetTitle("#Delta #phi");
  pur2->GetXaxis()->SetTitle("#Delta #phi");
  eff->SetAxisRange(0.0, 1.,"Y");  pur->SetAxisRange(0.0, 1.,"Y");  eff2->SetAxisRange(0.0, 1.,"Y");
  pur2->SetAxisRange(0.0, 1.,"Y");


  TLegend *leg = new TLegend(0.16,0.16,0.5,0.33); leg->SetBorderSize(0);
  leg->AddEntry(eff,"efficiency 1 jet","P");
  leg->AddEntry(pur,"putiry 1 jet","P");
  TLegend *leg2 = new TLegend(0.16,0.16,0.5,0.33); leg2->SetBorderSize(0); //0.67,0.67,0.84,0.84);
  leg2->AddEntry(eff2,"efficiency 2 jets","P");
  leg2->AddEntry(pur2,"putiry 2 jets","P");
  TLegend *leg3 = new TLegend(0.55,0.67,0.84,0.84); leg3->SetBorderSize(0);
  leg3->AddEntry(ftagged2,"tagged 2 jets","P");
  leg3->AddEntry(fB2,"di b-jets","P");
  leg3->AddEntry(ftaggedandB2,"tagged di bjets","P");
  TLegend *leg4 = new TLegend(0.55,0.67,0.84,0.84); leg4->SetBorderSize(0);
  leg4->AddEntry(fInd,"b-jet index","P");


  TCanvas *c1 = new TCanvas("c1", "single",600,600);
  eff->Draw("E1");
  pur->Draw("E1,same");
  leg->Draw();
  TCanvas *c2 = new TCanvas("c2", "double ",600,600);
  eff2->Draw("E1");
  pur2->Draw("E1,same");
  leg2->Draw();

  fB2->SetMarkerColor(kred2); fB2->SetLineColor(kredLight2);
  ftagged2->SetMarkerColor(kblue2); ftagged2->SetLineColor(kblueLight2);

  TCanvas *c3 = new TCanvas("c3", "di",600,600);
  fB2->Draw("E1");
  ftagged2->Draw("E1,same");
  ftaggedandB2->Draw("E1,same");
  leg3->Draw();

  TCanvas *c4 = new TCanvas("c4", "ind",600,600);
  fInd->Draw("E1");
  leg4->Draw();


  //if (fH1F) fH1F->Draw("AP");
  //c1->Update();


  TFile *f = new TFile("outfile.root","recreate");
  eff->Write();
  pur->Write();
  eff2->Write();
  pur2->Write();

  f->Close();
  

}
