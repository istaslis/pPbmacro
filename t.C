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
//#define NPARTON 7
//const int partons[7] = {0,1,2,3,4,5,21};


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
   
   /*   for (int i=0;i<NPARTON;i++) {
     int p = partons[i];
     bkgpartonmap[p] = new TH1F(Form("fparton%d",p), Form("Parton %d on the backside",p), nbins, xmin, xmax);
   }
   */
   bkgpartonB = new TH1F("bkgpartonB", "background B jets", nbins, xmin, xmax);
   bkgpartonC = new TH1F("bkgpartonC", "background C jets", nbins, xmin, xmax);
   bkgpartonUSDG = new TH1F("bkgpartonUSDG", "background USDG jets", nbins, xmin, xmax);

   diB_B = new TH1F("diB_B", "diB_B", nbins, xmin, xmax);
   diB_C = new TH1F("diB_C", "diB_C", nbins, xmin, xmax);
   diB_else = new TH1F("diB_else", "diB_else", nbins, xmin, xmax);
   diC_C = new TH1F("diC_C", "diC_C", nbins, xmin, xmax);
   diC_else = new TH1F("diC_else", "diC_else", nbins, xmin, xmax);
   dielse = new TH1F("dielse", "dielse", nbins, xmin, xmax);

   AjMCtagged = new TH1F("AjMCtagged","AjMCtagged",nbins, 0, 1);
   AjMCinc = new TH1F("AjMCinc","AjMCinc",nbins, 0, 1);
   AjMClight = new TH1F("AjMClight","AjMClight",nbins, 0, 1);
   AjMC_b = new TH1F("AjMC_b","AjMC_b",nbins, 0, 1);
   
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

  //skip the event where rawpt of two highest jets is below 18 GeV/C
  if (rawpt[0]<18 || rawpt[1]<18) return kTRUE; 

  float csvvalue = 0.9;
  float jtpt1 = 60;
  float jtpt2 = 30;

  float deltaphi = fabs(jtphi[0] - jtphi[1]);
  deltaphi = deltaphi > PI ? 2*PI-deltaphi : deltaphi;

  int p0 = fabs(refparton_flavorForB[0]);
  int p1 = fabs(refparton_flavorForB[1]);

  float aj = (jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1]);


  if (nrefout==5)
    for (int i=0;i<nrefout;i++)
      if (rawpt[i]>18 && fabs(refparton_flavorForB[i])==5) fInd->Fill(i,weight);



  if (jtpt[0]>jtpt1 && jtpt[1]>jtpt2){
    float phi1 = fabs(jtphi[0]);

    if (discr_csvSimple[0]>csvvalue && fabs(refparton_flavorForB[0])==5)
      ftaggedandB->Fill(phi1,weight);
    if (discr_csvSimple[0]>csvvalue)
      ftagged->Fill(phi1,weight);
    if (fabs(refparton_flavorForB[0])==5)
      fB->Fill(phi1,weight);
    

    if (discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue && 
	fabs(refparton_flavorForB[0])==5 && fabs(refparton_flavorForB[1])==5)
      ftaggedandB2->Fill(deltaphi,weight);
    
    if (discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue){
      ftagged2->Fill(deltaphi,weight); 
      
      if (p1==5) bkgpartonB->Fill(deltaphi, weight); else
	if (p1==4) bkgpartonC->Fill(deltaphi, weight); else
	  bkgpartonUSDG->Fill(deltaphi,weight);

      if (p0==5 && p1==5) diB_B->Fill(deltaphi,weight);
      if ((p0==5 && p1==4) || (p0==4 && p1==5)) diB_C->Fill(deltaphi,weight);
      if ((p0==5 && p1!=5 && p1!=4) || (p1==5 && p0!=5 && p0!=4) ) diB_else->Fill(deltaphi,weight);
      if (p0==4 && p1==4) diC_C->Fill(deltaphi,weight);
      if ((p0==4 && p1!=5 && p1!=4) || (p1==4 && p0!=5 && p0!=4)) diC_else->Fill(deltaphi,weight);
      if (p0!=5 && p0!=4 && p1!=5 && p1!=4) dielse->Fill(deltaphi,weight);


    }

    if (fabs(refparton_flavorForB[0])==5 && fabs(refparton_flavorForB[1])==5)
      fB2->Fill(deltaphi,weight);
    
      
    if (deltaphi>2*PI/3.) {

      AjMCinc->Fill(aj,weight);

      if (discr_csvSimple[0]>csvvalue && discr_csvSimple[1]>csvvalue)
	AjMCtagged->Fill(aj,weight);
    
      if (p0==5) AjMC_b->Fill(aj,weight);

      if (p0!=5) AjMClight->Fill(aj,weight);

    }
  
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

    bkgpartonB->Write();
    bkgpartonC->Write();
    bkgpartonUSDG->Write();
    
    diB_B->Write();
    diB_C->Write();
    diB_else->Write();
    diC_C->Write();
    diC_else->Write();
    dielse->Write();

    AjMCtagged->Write();
    AjMCinc->Write();
    AjMClight->Write();
    AjMC_b->Write();

    fB->SetDirectory(0);
    gDirectory = savedir;

    fFile->Close();

    fProofFile->Print();
    fOutput->Add(fProofFile);

  }
}

void PrepareAjhist(TH1F *aj, int color)
{
  aj->Scale(1/aj->Integral());
  aj->SetFillColor(color); 
  aj->SetFillStyle(3004); 
  aj->SetMarkerColor(color);
  aj->SetOption("HIST");
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


  //extract data:
  //Hist_Data.root/tagged2jetdata
  TFile *fdata = new TFile("Hist_Data.root");
  TH1F *tagged2jetdata = (TH1F *)fdata->Get("tagged2jetdata");
  tagged2jetdata->Sumw2();

  TH1F *Ajdata = (TH1F *)fdata->Get("Ajdata");
  Ajdata->Scale(1/Ajdata->Integral());
  TH1F *Ajdatainc = (TH1F *)fdata->Get("Ajincdata");
  Ajdatainc->Scale(1/Ajdatainc->Integral());

  fFile = new TFile("NEWSimpleNtuple.root");
  
  ftaggedandB = dynamic_cast<TH1F *>(fFile->Get("ftaggedandB"));
  ftagged = dynamic_cast<TH1F *>(fFile->Get("ftagged"));
  fB = dynamic_cast<TH1F *>(fFile->Get("fB"));
  ftaggedandB2 = dynamic_cast<TH1F *>(fFile->Get("ftaggedandB2"));
  ftagged2 = dynamic_cast<TH1F *>(fFile->Get("ftagged2"));
  fB2 = dynamic_cast<TH1F *>(fFile->Get("fB2"));
  fInd = dynamic_cast<TH1F *>(fFile->Get("fInd"));

  bkgpartonB = dynamic_cast<TH1F *>(fFile->Get("bkgpartonB"));
  bkgpartonC = dynamic_cast<TH1F *>(fFile->Get("bkgpartonC"));
  bkgpartonUSDG = dynamic_cast<TH1F *>(fFile->Get("bkgpartonUSDG"));
  bkgpartonB->SetFillColor(kRed); bkgpartonB->SetFillStyle(1001);
  bkgpartonC->SetFillColor(kGreen); bkgpartonC->SetFillStyle(1001);
  bkgpartonUSDG->SetFillColor(kBlue); bkgpartonUSDG->SetFillStyle(1001);


  diB_B = dynamic_cast<TH1F *>(fFile->Get("diB_B"));
  diB_C = dynamic_cast<TH1F *>(fFile->Get("diB_C"));
  diB_else = dynamic_cast<TH1F *>(fFile->Get("diB_else"));
  diC_C = dynamic_cast<TH1F *>(fFile->Get("diC_C"));
  diC_else = dynamic_cast<TH1F *>(fFile->Get("diC_else"));
  dielse = dynamic_cast<TH1F *>(fFile->Get("dielse"));
  diB_B->SetFillColor(kRed); diB_B->SetFillStyle(1001);
  diB_C->SetFillColor(kRed); diB_C->SetFillStyle(3001);
  diB_else->SetFillColor(kRed); diB_else->SetFillStyle(3004);
  diC_C->SetFillColor(kGreen); diC_C->SetFillStyle(1001);
  diC_else->SetFillColor(kGreen); diC_else->SetFillStyle(3004);
  dielse->SetFillColor(kBlue); dielse->SetFillStyle(3004);


  AjMCtagged = dynamic_cast<TH1F *>(fFile->Get("AjMCtagged"));
  AjMCinc = dynamic_cast<TH1F *>(fFile->Get("AjMCinc"));
  AjMClight = dynamic_cast<TH1F *>(fFile->Get("AjMClight"));
  AjMC_b = dynamic_cast<TH1F *>(fFile->Get("AjMC_b"));

  PrepareAjhist(AjMCtagged, kRed);
  PrepareAjhist(AjMCinc, kGreen);
  PrepareAjhist(AjMClight, kBlue);
  PrepareAjhist(AjMC_b,kOrange);

  eff = new TH1F("eff", "Tagging efficiency", nbins, xmin, xmax); eff->SetDirectory(0);
  pur = new TH1F("pur", "Tagging purity", nbins, xmin, xmax); pur->SetDirectory(0);
  eff2 = new TH1F("eff2", "Tagging efficiency - double", nbins, xmin, xmax); eff2->SetDirectory(0);
  pur2 = new TH1F("pur2", "Tagging purity - double", nbins, xmin, xmax); pur2->SetDirectory(0);

  bkgparton = new THStack("bkgparton","stacked background parton histogram");
  bkgparton->Add(bkgpartonB);
  bkgparton->Add(bkgpartonC);
  bkgparton->Add(bkgpartonUSDG);

  dibkg = new THStack("dibkg","stacked di-background");
  dibkg->Add(diB_B);
  dibkg->Add(diB_C);
  dibkg->Add(diB_else);
  dibkg->Add(diC_C);
  dibkg->Add(diC_else);
  dibkg->Add(dielse);

  


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
  leg->AddEntry(eff,"efficiency","P");
  leg->AddEntry(pur,"purity","P");
  TLegend *leg2 = new TLegend(0.16,0.16,0.5,0.33); leg2->SetBorderSize(0); //0.67,0.67,0.84,0.84);
  leg2->AddEntry(eff2,"efficiency","P");
  leg2->AddEntry(pur2,"purity","P");
  TLegend *leg3 = new TLegend(0.55,0.67,0.84,0.84); leg3->SetBorderSize(0);
  leg3->AddEntry(ftagged2,"tagged 2 jets","P");
  leg3->AddEntry(fB2,"di b-jets","P");
  leg3->AddEntry(ftaggedandB2,"tagged di bjets","P");
  TLegend *leg4 = new TLegend(0.55,0.67,0.84,0.84); leg4->SetBorderSize(0);
  leg4->AddEntry(fInd,"b-jet index","P");
  TLegend *leg5 = new TLegend(0.16,0.5,0.5,0.84); leg5->SetBorderSize(0);
  leg5->AddEntry(diB_B,"B+B","F");
  leg5->AddEntry(diB_C,"B+C","F");
  leg5->AddEntry(diB_else,"B+l","F");
  leg5->AddEntry(diC_C,"C+C","F");
  leg5->AddEntry(diC_else,"C+l","F");
  leg5->AddEntry(dielse,"l+l","F");
  leg5->AddEntry(tagged2jetdata,"Data","P");

  TCanvas *c1 = new TCanvas("c1", "single",600,600);
  eff->Draw("E1");
  pur->Draw("E1,same");
  leg->Draw();
  c1->SaveAs("btag_eff_pur.pdf");

  TCanvas *c2 = new TCanvas("c2", "double ",600,600);
  eff2->Draw("E1");
  pur2->Draw("E1,same");
  leg2->Draw();
  c2->SaveAs("btag_double_eff_pur.pdf");

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

  TCanvas *c5 = new TCanvas("c5", "bkgp",600,600);
  bkgparton->Draw();

  int taggeddata = tagged2jetdata->Integral();
  tagged2jetdata->Scale(ftagged2->Integral()/tagged2jetdata->Integral());
  TCanvas *c6 = new TCanvas("c6", "dibkgp",600,600);
  dibkg->Draw("hist");
  dibkg->GetHistogram()->GetXaxis()->SetTitle("#Delta #phi");
  tagged2jetdata->Draw("same");
  leg5->Draw();
  
  c6->SaveAs("di-bjets_Data_MC.pdf");

  TLegend *lAj = new TLegend(0.55,0.67,0.84,0.84); lAj->SetBorderSize(0);
  lAj->AddEntry(AjMClight,"MC light","F");
  lAj->AddEntry(AjMCtagged,"MC tagged","P");
  lAj->AddEntry(Ajdata,"data tagged","P");
  THStack *Aj = new THStack("Aj","Aj");
  //Aj->Add(AjMC,"hist");
  Aj->Add(AjMCtagged);
  //Aj->Add(AjMCinc);
  Aj->Add(AjMClight,"hist");
  //Aj->Add(AjMC_b);
  Aj->Add(Ajdata);
  TCanvas *c7 = new TCanvas("c7", "Aj",600,600);
  Aj->Draw("nostack");
  lAj->Draw();
  c7->SaveAs("Aj_MCl_MCt_dt.pdf");


  TLegend *lAjinc = new TLegend(0.55,0.67,0.84,0.84); lAjinc->SetBorderSize(0);
  lAjinc->AddEntry(Ajdatainc,"data inclusive","P");
  lAjinc->AddEntry(AjMCinc,"MC inclusive","F");
  THStack *Ajinc = new THStack("Ajinc","Ajinc");
  Ajinc->Add(Ajdatainc);
  Ajinc->Add(AjMCinc,"hist");
  TCanvas *c8 = new TCanvas("c8", "Ajinc",600,600);
  Ajinc->Draw("nostack");
  lAjinc->Draw();
  c8->SaveAs("Aj_di_MCi.pdf");

  TLegend *lAj3 = new TLegend(0.55,0.67,0.84,0.84); lAj3->SetBorderSize(0);
  lAj3->AddEntry(AjMCinc,"MC inclusive","F");
  lAj3->AddEntry(AjMClight,"MC light","P");
  lAj3->AddEntry(AjMCtagged,"MC tagged","P");
  THStack *Aj3 = new THStack("Aj3","Aj3");
  Aj3->Add(AjMCinc,"hist");
  Aj3->Add(AjMClight);
  Aj3->Add(AjMCtagged);
  TCanvas *c9 = new TCanvas("c9", "Aj3",600,600);
  Aj3->Draw("nostack");
  lAj3->Draw();
  c9->SaveAs("Aj_MCi_MCl_MCt.pdf");

  TLegend *lAj4 = new TLegend(0.55,0.67,0.84,0.84); lAj4->SetBorderSize(0);
  lAj4->AddEntry(Ajdatainc,"data inclusive","F");
  lAj4->AddEntry(Ajdata,"data tagged","P");
  THStack *Aj4 = new THStack("Aj4","Aj4");
  Aj4->Add(Ajdatainc,"hist");
  Aj4->Add(Ajdata);
  TCanvas *c10 = new TCanvas("c10", "Aj4",600,600);
  Aj4->Draw("nostack");
  lAj4->Draw();
  c10->SaveAs("Aj_di_dt.pdf");


  std::cout<<"Tagged leading jets: \t"<<ftagged->Integral()<<std::endl;
  std::cout<<"Leading b-jets: \t"<<fB->Integral()<<std::endl;
  std::cout<<"Tagged di-jets: \t"<<ftagged2->Integral()<<std::endl;
  std::cout<<"di-b-jets: \t"<<fB2->Integral()<<std::endl;
  std::cout<<"Tagged di-jets AND b-jets: \t"<<ftaggedandB2->Integral()<<std::endl;
  std::cout<<"Tagged di-jets, opposite side B: "<<bkgpartonB->Integral()<<std::endl;
  std::cout<<"Tagged di-jets, opposite side C: "<<bkgpartonC->Integral()<<std::endl;
  std::cout<<"Tagged di-jets, opposite side USDG and unmatched: "<<bkgpartonUSDG->Integral()<<std::endl;
  std::cout<<"Total 3 (=di-b-jets) :"<<bkgpartonB->Integral()+ bkgpartonC->Integral() + bkgpartonUSDG->Integral()<<std::endl;
  std::cout<<"Tagged di-jets in data: "<<taggeddata<<std::endl;

  TFile *f = new TFile("outfile.root","recreate");
  eff->Write();
  pur->Write();
  eff2->Write();
  pur2->Write();

  f->Close();
  

}
