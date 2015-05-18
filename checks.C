#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TObjString.h"
#include "TString.h"
#include <iostream>
#include "TTimeStamp.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TCut.h"
//#include "SetupStyle.h"
#include <string.h>


const unsigned Nevents = -1;

vector<float> ptbins;

void FillSimple(float xmin, float xmax, int b)
{
  for (int i=0;i<b;i++)
  {
    float di = (xmax-xmin)/b;
    ptbins.push_back(xmin+i*di);
  }
}

void FillOnce(float xmin, float xmax, int b)
{
  ptbins.clear();
  FillSimple(xmin, xmax, b);
}

void FillXbins()
{
      FillSimple(0.4,2,10);
      FillSimple(2,5,5);
      FillSimple(5,10,5);
      FillSimple(10,20,2);
      FillSimple(20,50,3);
      FillSimple(50,140,3);
      FillSimple(140,300,1);
      ptbins.push_back(300);


    for (unsigned int i=0;i<ptbins.size();i++) std::cout<<ptbins[i]<<" ";
  }

TH1F *Select(int color, TTree *t, const char *name, const char *var, const char *selection, const char *ytitle = "Acceptance x Efficiency", const char *xtitle = "jet p_{T}, GeV/c", const char *options = "", bool normalize = false)
{

  std::cout<<"Processing "<<name<<"... ";

  vector<float> &bins = ptbins;

  TTimeStamp t1;
  TH1F *h = new TH1F(name,name,bins.size()-1,&bins[0]); h->Sumw2();
  t->Project(name,var, selection, options, Nevents);

  h->GetXaxis()->SetTitle(xtitle); 
  h->GetYaxis()->SetTitle(ytitle);

  h->SetLineColor(color);
  h->SetMarkerColor(color);

  if (normalize) h->Scale(1/h->Integral());

  TTimeStamp t2;
  std::cout<<int(t2-t1)<<" sec"<<std::endl;

  return h;
}

TH1F *Div(int color, TTree *t, const char *name, const char *x, const char *num, const char *denom, const char *ytitle = "Acceptance x Efficiency")
{

  std::cout<<"Processing "<<name<<"... ";

  vector<float> &bins = ptbins;

  TTimeStamp t1;
  TH1F *h1 = new TH1F("h1","h1",bins.size()-1,&bins[0]); h1->Sumw2();
  TH1F *h2 = new TH1F("h2","h2",bins.size()-1,&bins[0]); h2->Sumw2();
  TH1F *h = new TH1F(name,name,bins.size()-1,&bins[0]); h->Sumw2();
  t->Project("h1",x, num,"",Nevents);
  TTimeStamp t_m;
  std::cout<<int(t_m-t1)<<" sec ... ";
  t->Project("h2",x, denom, "", Nevents);
  h->Divide(h1,h2,1.,1.,"B");
  h->SetAxisRange(0.0001, 1.,"Y");

  h->GetXaxis()->SetTitle("jet p_{T}, GeV/c"); 
  h->GetYaxis()->SetTitle(ytitle);

  h->SetLineColor(color);
  h->SetMarkerColor(color);

  delete h1; delete h2;

  TTimeStamp t2;
  std::cout<<int(t2-t1)<<" sec"<<std::endl;

  return h;
}

TH1F *Load(TFile *f, TString name, int color)
{
  TH1F *h = (TH1F *)f->Get(name);
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  return h;
}


void checks(bool save)
{

  FillSimple(30,200,100);
  vector<float> jtbins = ptbins;

  TString histfile = Form("checks.root");

  gStyle->SetOptStat(0);
  //  SetupStyle();

  TFile *fin, *fout;
  TTree *t;

  if (save) {
    fin = new TFile("QCDPPb_etacut.root");
    t = (TTree *)fin->Get("t");
  }
  else fin = new TFile(histfile,"r");

  TH1F *hjes, *hjesb, *hAjinc, *hAjb, *hAjtag, *hCSVeff, *hCSVpur;

  if (save) {

    TCut safetycut = "rawpt>18";

    /*    hjes   = Select(kBlue, t, "JES","jtpt/refpt:refpt",safetycut, "Jet energy scale","ref p_{T}, GeV/c","prof");
    hjesb   = Select(kGreen, t, "JES b","jtpt/refpt:refpt",safetycut && "abs(refparton_flavorForB)==5", "Jet energy scale","ref p_{T}, GeV/c","prof");


    FillOnce(0,1,50);
    hAjinc = Select(kBlue, t, "Aj inclusive","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",safetycut, "Jet asymmetry","Aj","",true);
    hAjb = Select(kGreen, t, "Aj b","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",safetycut && "abs(refparton_flavorForB[0])==5 && abs(refparton_flavorForB[1])==5", "Jet asymmetry","Aj","",true);
    hAjtag = Select(kBlack, t, "Aj tag","(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])",safetycut && "discr_csvSimple[0]>0.7 && discr_csvSimple[1]>0.7 && abs(refparton_flavorForB[0])==5 && abs(refparton_flavorForB[1])==5", "Jet asymmetry","Aj","",true);

    ptbins = jtbins;
    */
    float csvvalue = 0.7;

    hCSVeff = Div(kGreen,t,"CSV b-jet eff","jtpt",safetycut && Form("discr_csvSimple[0]>%f && discr_csvSimple[1]>%f && abs(refparton_flavorForB[0])==5 && abs(refparton_flavorForB[1])==5", csvvalue,csvvalue),safetycut && "abs(refparton_flavorForB[0])==5 && abs(refparton_flavorForB[1])==5","CSV b-jet tagging");
    hCSVpur = Div(kBlue,t,"CSV b-jet pur","jtpt",safetycut && Form("discr_csvSimple[0]>%f && discr_csvSimple[1]>%f && abs(refparton_flavorForB[0])==5 && abs(refparton_flavorForB[1])==5",csvvalue,csvvalue),safetycut && Form("discr_csvSimple[0]>%f && discr_csvSimple[1]>%f",csvvalue,csvvalue),"CSV b-jet tagging");
    

}
else {

  hjes = Load(fin, "JES",kBlack);
  hAjinc = Load(fin, "Aj inclusive",kBlack);
  hAjb = Load(fin, "Aj b",kBlack);
  hAjtag = Load(fin, "Aj tag",kBlack);
  hCSVeff = Load(fin, "CSV b-jet eff",kBlack);
  hCSVpur = Load(fin, "CSV b-jet pur",kBlack);
}

  // effmva->SetAxisRange(0.0001, 0.2,"Y");
  //  TLatex *l = new TLatex();

  TLegend *legjes = new TLegend(0.67,0.67,0.84,0.84); //16 - 45
  legjes->SetBorderSize(0);
  //  legjes->AddEntry(hjes,"JES","P");
  // legjes->AddEntry(hjesb,"JES b","P");
  

  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  //  hjes->Draw("E1");
  //  hjesb->Draw("E1,same");
  legjes->Draw();
  
  c1->Update();

  TLegend *leg = new TLegend(0.67,0.67,0.84,0.84); //16 - 45
  leg->SetBorderSize(0);
  //  leg->AddEntry(hAjinc,"Aj inclusive","P");
  //leg->AddEntry(hAjb,"Aj b-jets","P");
  //leg->AddEntry(hAjtag,"Aj tagged","P");

  
   TCanvas *c2 = new TCanvas("c2","c2",600,600);
   //  hAjinc->Draw("E1");
   //  hAjb->Draw("E1,same");
   //  hAjtag->Draw("E1,same");
  leg->Draw();


  TLegend *leg2 = new TLegend(0.67,0.67,0.84,0.84);
  leg2->SetBorderSize(0);
  leg2->AddEntry(hCSVeff,"efficiency","P");
  leg2->AddEntry(hCSVpur,"purity","P");

  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  hCSVeff->Draw("E1");
  hCSVpur->Draw("E1,same");
  leg2->Draw();
  


  //  c1->SetLogx(true);

  //    l->DrawLatexNDC(0.6,0.75,algonames[algomin-4]);//Form("algo=%d", algomin));

  c1->  SaveAs("jec.pdf");

  if (save) {
    TFile *fout = new TFile(histfile,"recreate");

    // hjes->Write();
    // hAjinc->Write();
    // hAjb->Write();
    // hAjtag->Write();
    hCSVeff->Write();
    hCSVpur->Write();


    fout->Close();
  }
  
}
