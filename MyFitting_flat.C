#define ROOFITTER

#include "TH1F.h"
#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TTree.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "InitStyle.h"

double GetIntegral(TH1 *h, bool full = false)
{
  TH1F *h1 = dynamic_cast<TH1F*>(h);
  //  return h1->Integral();
  if (h1!=0) return full ? h1->Integral(0,h->GetNbinsX()+1) : h->Integral();
  TH2F *h2 = dynamic_cast<TH2F*>(h);
  if (h2!=0) return full ? h2->Integral(0,h->GetNbinsX()+1,0,h->GetNbinsY()+1) : h->Integral();

  return -1;
}

//double MCBfraction, MCCfraction;

//double mcBpur, mcCpur, mctagBEff, mctagCEff;

float xmin =0, xmax =6;

float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;
//float legendx1 = 0.16, legendy1 = 0.65, legendx2 = 0.34, legendy2 = 0.84;

TString muset = "jetmuon";//"mu"

bool CLtogether = false;

int bins = 12;//16;
bool logplots = false;
TString discrname = "";
float discrvalue = -1;
float discrmax = 6;





TString variable = //"ptrel";
  //    "ip3d";
  //"jp";
  "svtxm";

TString sVar, sTitle;
  
void SetupDiscriminator()
{
  if (variable=="ptrel") {
    sVar =  TString::Format("%sptrel",muset.Data());
    sTitle = "p_{T}^{rel} [GeV/c]";
    xmin = 0; xmax = 4;
    discrmax = 20;
    }
  else if (variable=="ip3d") {
    sVar = "logmuip3d";//TString::Format("log(%sip3d)",muset.Data());
    sTitle = "log(3d IP)";
    discrmax = 20;
    xmin = -10; xmax = 0;
  } else if (variable=="jp") {
    sVar = "discr_prob";
    sTitle = "JP";
    xmin=0; xmax=3;
    bins=16;
    logplots = true;
  } else if (variable=="svtxm") {
    sVar = "svtxm";
    sTitle = "Secondary vertex mass";
    bins=12;
    logplots = true;
  }

//  const char *discrname = "discr_ssvHighPur";
//  float discrvalue = -100;//1.68;

discrname = "discr_ssvHighEff";
discrvalue = 2;

//const char *discrname = "jetmuonpt";
//float discrvalue = 7;
    
//  const char *discrname = "discr_csvSimple";
//  float discrvalue = -10;



  
}



double DataBFraction, DataBFractionError;
double DataCFraction, DataCFractionError;
double DataLFraction, DataLFractionError;
  
void DoFit(TH1F *mc1, TH1F *mc2, TH1F *mc3, TTree *td, const char *var, float pTmin, float pTmax, const char *physicscut, const char *discrData)
{

  double x1, x1e, x2, x2e, x3, x3e;


  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());

  TH2F *mc23 = (TH2F *)mc2->Clone("mc23");
  mc23->Add(mc3);
  mc23->Scale(1/mc23->Integral());




  
  #ifdef ROOFITTER
  
  cout<<"Roofit is used"<<endl;


  RooRealVar x(var,var,xmin,xmax) ;
  RooRealVar jtpt("jtpt","jtpt", pTmin, pTmax);
  //  RooRealVar discr_ssvHighPur("discr_ssvHighPur","discr_ssvHighPur", -10, 6);
  RooRealVar roodiscr(discrname,discrname,discrvalue,discrmax);
  RooRealVar weight("weight","weight",0,1) ;
  RooRealVar weightJet("weightJet","weightJet",0,200) ;

  RooRealVar nb("nb","fraction of b",0.65,0,1);


  RooDataHist roomc1("roomc1","roomc1",RooArgList(x),mc1);
  RooHistPdf pdfroomc1("pdfroomc1","pdfroomc1",x,roomc1);
  

  RooAddPdf* qcdEstimateModel;
  
  if (CLtogether) {
  RooDataHist roomc23("roomc23","roomc23",RooArgList(x),mc23);
  RooHistPdf pdfroomc23("pdfroomc23","pdfroomc23",x,roomc23);

  RooArgList pdfs2(pdfroomc1,pdfroomc23);
  RooArgList ints2(nb);

  qcdEstimateModel = new RooAddPdf("QCDEstimationPdf","QCDEstimationPdf", pdfs2, ints2);

  //  RooRealVar ncl("nc","fraction of c and l",0.35,0,1);


  // Get Data To Fit To
  //RooDataHist* roodata = new RooDataHist("roodata","roodata",RooArgList(x),data);
  RooDataSet dataset("dataset","dataset",td,RooArgSet(x,weightJet,roodiscr,jtpt),TString::Format("%s && %s",physicscut,discrData),"weightJet");

  

  // Fit Model To Data
  RooFitResult* result = qcdEstimateModel->fitTo(dataset,
             RooFit::NumCPU(1,1),
             RooFit::Save(kTRUE),
             //            RooFit::Extended(kTRUE),
             RooFit::SumW2Error(kTRUE),
             RooFit::PrintLevel(-1));

  result->Print();


  x1 = nb.getVal();
  x1e = nb.getError();

  x2 = (1-x1)/2;
  x3 = (1-x1)/2;

  x2e = x1e;
  x3e = x1e;


  } else {
  RooDataHist roomc2("roomc2","roomc2",RooArgList(x),mc2);
  RooDataHist roomc3("roomc3","roomc3",RooArgList(x),mc3);
  RooHistPdf pdfroomc2("pdfroomc2","pdfroomc2",x,roomc2);
  RooHistPdf pdfroomc3("pdfroomc3","pdfroomc3",x,roomc3);

  RooRealVar nc("nc","fraction of c",0.27,0,1);
  RooRealVar nl("nl","fraction of l",0.08,0,1);

  RooArgList pdfs(pdfroomc1,pdfroomc2,pdfroomc3);
  RooArgList ints(nb,nc);

  qcdEstimateModel = new RooAddPdf("QCDEstimationPdf","QCDEstimationPdf", pdfs, ints);
  

  // Get Data To Fit To
  //RooDataHist* roodata = new RooDataHist("roodata","roodata",RooArgList(x),data);
  RooDataSet dataset("dataset","dataset",td,RooArgSet(x,weightJet,roodiscr,jtpt),TString::Format("%s && %s",physicscut,discrData),"weightJet");

  

  // Fit Model To Data
  RooFitResult* result = qcdEstimateModel->fitTo(dataset,
             RooFit::NumCPU(1,1),
             RooFit::Save(kTRUE),
             //            RooFit::Extended(kTRUE),
             RooFit::SumW2Error(kTRUE),
             RooFit::PrintLevel(-1));

  result->Print();


  cout<<1<<endl;

  x1 = nb.getVal();
  x1e = nb.getError();

  x2 = nc.getVal();
  x3 = 1-x1-x2;

  x2e = nc.getError();
  x3e = sqrt(x1e*x1e + x2e*x2e);
  }
  
  
  
  #else



  cout<<"TFractionFitter is used!!!"<<endl;
  

   
  TObjArray *mc = new TObjArray(3);
  mc->Add(hmc_b);
  mc->Add(hmc_c);
  mc->Add(hmc_l);

  TH1F *prediction;


   TFractionFitter* fit = new TFractionFitter(hd_tag, mc, "Q"); //Q = quiet, no message
  fit->Constrain(0,0.0,1.0);  
  fit->Constrain(1,0.0,1.0);
  fit->Constrain(2,0.0,1.0);
  fit->Fit();
  prediction = (TH1F*) fit->GetPlot();
  


  cout<<fit->GetProb()<<endl;

  fit->GetResult(0,x1,x1e);
  fit->GetResult(1,x2,x2e);
  fit->GetResult(2,x3,x3e);
  
  
  #endif
  

  DataBFraction = x1; DataBFractionError = x1e;
  DataCFraction = x2; DataCFractionError = x2e;
  DataLFraction = x3; DataLFractionError = x3e;

  //  TH1F *hx1 = new TH1F("hx1","hx1",100,0,1);
  //  TH1F *hx2 = new TH1F("hx2","hx2",100,0,1);
  //  TH1F *hx3 = new TH1F("hx3","hx3",100,0,1);
  //  TH1F *hchi2 = new TH1F("hchi2","hchi2",100,0,20);

  //  double chi2 = FitDataTree(hmc_b,hmc_c,hmc_l,td,var, TString::Format("(%s && %s)",physicscut,discrData),pTmin,pTmax,"weight",x1,x2,x3,x1e,x2e,x3e);

  //  cout<<"Fit chi2 = "<<chi2<<endl;

  //  x1 = hx1->GetMean(); x1e = hx1->GetStdDev();
  //  x2 = hx2->GetMean(); x2e = hx2->GetStdDev();
  //  x3 = hx3->GetMean(); x3e = hx3->GetStdDev();

  //  prediction = (TH1F *)hmc_b->Clone("prediction");
  //  prediction->Clear();
  //  prediction->Add(hmc_b,x1);
  //  prediction->Add(hmc_c,x2);
  //  prediction->Add(hmc_l,x3);
}

void DrawMCTemplates(TH1F *h_b, TH1F *h_c, TH1F *h_l)
{
  
  TH1F *hmctemplate_b = (TH1F*)h_b->Clone("hmctemplate_b");
  TH1F *hmctemplate_c = (TH1F*)h_c->Clone("hmctemplate_c");
  TH1F *hmctemplate_l = (TH1F*)h_l->Clone("hmctemplate_l");
  hmctemplate_l->SetLineColor(kblue); hmctemplate_c->SetLineColor(kgreen); hmctemplate_b->SetLineColor(kred);
  hmctemplate_l->SetMarkerColor(kblue); hmctemplate_c->SetMarkerColor(kgreen); hmctemplate_b->SetMarkerColor(kred);
  hmctemplate_l->SetFillStyle(0); hmctemplate_c->SetFillStyle(0); hmctemplate_b->SetFillStyle(0);
  hmctemplate_l->SetLineWidth(3); hmctemplate_c->SetLineWidth(3); hmctemplate_b->SetLineWidth(3);

  hmctemplate_b->Scale(1/hmctemplate_b->Integral());
  hmctemplate_c->Scale(1/hmctemplate_c->Integral());
  hmctemplate_l->Scale(1/hmctemplate_l->Integral());

  TCanvas *ctempl = new TCanvas("ctempl","ctempl",600,600);
  THStack *tstemplate = new THStack("tstemplate",TString::Format(";%s",sTitle.Data()));
  tstemplate->Add(hmctemplate_l,"E1");
  tstemplate->Add(hmctemplate_c,"E1"); 
  tstemplate->Add(hmctemplate_b,"E1");
  tstemplate->Draw("nostack");

  TLegend *ltempl = new TLegend(legendx1, legendy1, legendx2, legendy2);
  ltempl->AddEntry(hmctemplate_b,"B","l");
  ltempl->AddEntry(hmctemplate_c,"C","l");
  ltempl->AddEntry(hmctemplate_l,"L","l");
  ltempl->Draw();

  ctempl->SetLogy(logplots);

  ctempl->SaveAs("MCTemplates.pdf");


}

void DrawMCjetcontent(TH1F *h_b, TH1F *h_c, TH1F *h_l, TH1F *h_all)
{
  
  THStack *hs =new THStack("hs",TString::Format(";%s",sTitle.Data()));
  hs->Add(h_b,"hist");
  hs->Add(h_c,"hist");
  hs->Add(h_l,"hist");
  
  TCanvas *c = new TCanvas("c","c",600,600);
  hs->Draw();
  h_all->Draw("same");

  TLegend *l = new TLegend(legendx1, legendy1, legendx2, legendy2);// l->SetFillStyle(0);
  l->AddEntry(h_b,"B","f");
  l->AddEntry(h_c,"C","f");
  l->AddEntry(h_l,"L","f");
  l->AddEntry(h_all,"all","pl");
  l->Draw();

  c->SetLogy(logplots);
  c->SaveAs("MCjetContentTagged.pdf");
  c->Update();

}

void DrawDataAllTagged(TH1F *hd_tag, TH1F *hd_untag, TH1F *hd_all)
{
  
  TCanvas *cd = new TCanvas("cd","cd",600,600);

  hd_all->Draw();
  hd_tag->Draw("same");
  hd_untag->Draw("same");

  TLegend *ld = new TLegend(legendx1, legendy1, legendx2, legendy2);// l->SetFillStyle(0);
  ld->AddEntry(hd_all,"all","P");
  ld->AddEntry(hd_tag,"tagged","P");
  ld->AddEntry(hd_untag,"untagged","P");
  ld->Draw();

  cd->SetLogy(logplots);
  cd->SaveAs("DataAllTagged.pdf");
  cd->Update();

}

void DrawTemplates(TH1F *hd_tag, TH1F *h_all, TH1F *hmc_b, TH1F *hmc_c, TH1F *hmc_l, float pTmin, float pTmax)
{

  float x1 = DataBFraction, x2 = DataCFraction, x3 = DataLFraction;
  float x1e = DataBFractionError, x2e = DataCFractionError, x3e = DataLFractionError;
    
  double total = GetIntegral(hd_tag); //prediction?
  double num_b = total*x1, num_be = total*x1e;
  double num_c = total*x2, num_ce = total*x2e;
  double num_l = total*x3, num_le = total*x3e;

  cout<<"Data : "<<endl;

  cout<<"Number of b-jets  : "<<num_b<<" ± "<<num_be<<endl;
  cout<<"Number of c-jets : "<<num_c<<" ± "<<num_ce<<endl;
  cout<<"Number of l-jets : "<<num_l<<" ± "<<num_le<<endl;

  cout<<"Fraction of b-jets : "<<x1*100<<"% ± "<<x1e*100<<endl;
  cout<<"Fraction of c-jets : "<<x2*100<<"% ± "<<x2e*100<<endl;
  cout<<"Fraction of l-jets : "<<x3*100<<"% ± "<<x3e*100<<endl;

    

  hmc_b->Scale(1/GetIntegral(hmc_b)*num_b);
  hmc_c->Scale(1/GetIntegral(hmc_c)*num_c);
  hmc_l->Scale(1/GetIntegral(hmc_l)*num_l);

  //  TCanvas *ccomp = new TCanvas("ccomp","ccomp",600,600);
  //  TH1F *pred_b = (TH1F *)hmc_b->Clone("pred_b");//(TH1F *)fit->GetMCPrediction(0);
  //  TH1F *hmc_bclone = (TH1F *)hmc_b->Clone("hmc_bclone");
  //  pred_b->Scale(1/GetIntegral(pred_b)*num_b);

  //  pred_b->SetLineColor(kBlack); pred_b->SetFillStyle(0); pred_b->SetLineWidth(3);
  //  hmc_bclone->SetLineColor(kRed); hmc_bclone->SetFillStyle(0); hmc_bclone->SetLineWidth(3);
  //  pred_b->Draw("hist");
  //  hmc_bclone->Draw("hist,same");

  //  TLegend *lcomp = new TLegend(legendx1, legendy1, legendx2, legendy2);// l->SetFillStyle(0);
  //  lcomp->AddEntry(hmc_bclone,"MC b template","l");
  //  lcomp->AddEntry(pred_b,"Prediction b template","l");
  //  lcomp->Draw();
  //  ccomp->SaveAs("btemplatecompare.pdf");


  TH1F *hmc_total = (TH1F *)hmc_b->Clone("hmc_total");
  hmc_total->Add(hmc_c);
  hmc_total->Add(hmc_l);
  hmc_total->SetLineColor(7);
  hmc_total->SetLineWidth(4);
  hmc_total->SetMarkerStyle(1);

  THStack *hsf =new THStack("hsf",TString::Format(";%s",sTitle.Data()));

  hsf->Add(hmc_b,"hist");
  hsf->Add(hmc_c,"hist");
  hsf->Add(hmc_l,"hist");
  



  //prediction->Integral() == hd_tag->Integral() so it's basically the same thing
  h_all->Scale(hd_tag->Integral()/GetIntegral(h_all));
  h_all->SetMarkerColor(kGray);
  h_all->SetLineColor(kGray);


  TCanvas *cf = new TCanvas("cf","cf",600,600);
  hd_tag->Draw("Ep");
  hsf->Draw("same");
  hmc_total->Draw("Epsame");
  hd_tag->Draw("Epsame");
  
  //  hsf->SetMaximum(1.1E8);  
  //  hsf->SetMinimum(1.);
  //  hsf->Draw();

  //  h_all->Draw("Epsame");
  TLegend *lf = new TLegend(legendx1, legendy1, legendx2, legendy2);// l->SetFillStyle(0);
  lf->AddEntry(hmc_b,"B","f");
  lf->AddEntry(hmc_c,"C","f");
  lf->AddEntry(hmc_l,"L","f");
  lf->AddEntry(hd_tag,"Data","P");
  //  lf->AddEntry(hmc_total,"MC","L");
  lf->Draw();
  cf->SetLogy(logplots);

  cf->SaveAs(TString::Format("FitTagged_%d_%d.pdf",(int)pTmin, (int)pTmax));
  cf->Update();


}

int _legendcounter = 0;
void AddLenend(vector<TH1F *>hists, vector<TString> labels, vector<TString> types, TString legendposition, TString title = "")
{
  if (hists.size()!=labels.size() || hists.size()!=types.size()) {
    cout<<"check that legend vectors are equal!"<<endl;
    return;
  }
  if (legendposition=="tr") {
    legendx1 = 0.65; legendy1 = 0.65; legendx2 = 0.84; legendy2 = 0.84;
  }
  if (legendposition=="tl") {
    legendx1 = 0.16; legendy1 = 0.65; legendx2 = 0.34; legendy2 = 0.84;
  }

  TLegend *l = new TLegend(legendx1,legendy1,legendx2,legendy2,title);
  for (unsigned i=0;i<hists.size();i++) 
    l->AddEntry(hists[i],labels[i],types[i]);
  
  l->Draw();

  _legendcounter++;
}

void SetMCStyle(TH1F *h, int color)
{
  h->SetMarkerColor(color); 
  h->SetLineColor(color);
  h->SetLineWidth(3);
  h->SetMarkerStyle(kOpenSquare);
}

void drawText(const char *text, float xp, float yp, int color = kBlack, int size=30){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(43);
  tex->SetTextSize(size);
  tex->SetTextColor(color);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw("same");
}


void DrawCompare(TH1F *h1, TH1F *h2, TString legend1 = "Data", TString legend2 = "MC", TString title = "compare")
{
  TCanvas *c1 = new TCanvas(title,title,600,700);
  TPad *pad1 = new TPad("pad1","pad1",0,0.4,1,1);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  pad1->cd();
  pad1->SetLogy();

  h1->SetMarkerColor(kBlack);
  h2->SetMarkerColor(kBlue);
  h1->Draw();
  h2->Draw("same");


  drawText(legend1.Data(),0.7,0.6,kBlack);
  drawText(legend2.Data(),0.7,0.7,kBlue);
  //drawText(sel,0.18,0.8,kBlack,20);
  c1->cd();

  TPad *pad2 = new TPad("pad2","pad2",0,0.1,1,0.4);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.25);
  pad2->Draw();
  pad2->cd();

  TH1F *h3=(TH1F *)h1->Clone();//(TH1F *)h1->DrawCopy();
  //  h3->Sumw2();                                                                                                                                                                   
  //  h3->SetStats(0);                                                                                                                                                               

  h3->Divide(h1,h2,1,1,"B");

  h3->SetMinimum(0);
  h3->SetMaximum(2.1);

  h3->GetXaxis()->SetTitle("p_T, GeV/c");
  h3->GetXaxis()->SetTitleOffset(3.5);
  //  drawText(var,0.18,0.8,kBlack,20);

  h3->SetMarkerStyle(21);
  h3->Draw("ep");
  c1->cd();

  c1->SaveAs(Form("Compare_%s.pdf",title.Data()));

}

void MyFitting_flat()
{
  InitStyle();


  variable = //"ptrel";
    //    "ip3d";
    //"jp";
    "svtxm";
  
  vector<double> pTbins = {60,80,120,170,250,400};
  //  vector<double> pTbins = {55,70,90,110,140,170,220,400};
  
  
  SetupDiscriminator();

  cout<<(TString::Format("%s>%f",discrname.Data(),discrvalue)).Data()<<endl;
  TString sdiscr = TString::Format("%s>%f",discrname.Data(),discrvalue);
  TString sdiscrData = TString::Format("%s>%f",discrname.Data(),discrvalue);
  const char *discr = sdiscr.Data();
  const char *discrData = sdiscrData.Data();

  double *ptbins = &pTbins[0];
  int Nptbins = pTbins.size()-1;
  vector<double> pt, pterrors;

  TH1F *hdataBfrac = new TH1F("hdataBfrac","hdataBfrac",Nptbins,ptbins);
  TH1F *hdataCfrac = new TH1F("hdataCfrac","hdataCfrac",Nptbins,ptbins);


  vector<double> dataB, dataBerror, dataC, dataCerror, mcB, mcBerror, mcC, mcCerror;
  vector<double> mcBpurs, mcCpurs, mctagBEffs, mctagCEffs;

  const char *datafilename = "jettrig_weight_AccCut_muons_flat.root";//"jettrig_weight_AccCut.root");//"jettrig_weight_AccCut_muons.root");//"jettrig_weight.root");

  TFile *f = new TFile("QCDPPb_genmuonmerged_flat.root"); //"QCDPPb_AccCut.root");//"QCDPPb_genmuonmerged.root");
  TTree *t = (TTree *)f->Get("jet");

  TFile *fd = new TFile(datafilename);
  TTree *td = (TTree *)fd->Get("jet");

  const char * var = sVar.Data();
  const char * title = sTitle.Data();


  TH1F *h_l   = new TH1F("h_l",TString::Format("h_l;%s",title),bins, xmin, xmax);
  TH1F *h_c   = new TH1F("h_c",TString::Format("h_c;%s",title),bins, xmin, xmax);
  TH1F *h_b   = new TH1F("h_b",TString::Format("h_b;%s",title),bins, xmin, xmax);
  TH1F *h_all = new TH1F("h_all",TString::Format("h_all;%s",title),bins, xmin, xmax);
  TH1F *h_mctotal = new TH1F("h_mctotal",TString::Format("h_mctotal;%s",title),bins, xmin, xmax);
  TH1F *h_mctotalb = new TH1F("h_mctotalb",TString::Format("h_mctotalb;%s",title),bins, xmin, xmax);
  TH1F *h_mctotalc = new TH1F("h_mctotalc",TString::Format("h_mctotalc;%s",title),bins, xmin, xmax);

  h_l->SetFillColor(kblue); h_c->SetFillColor(kgreen); h_b->SetFillColor(kred);
  h_l->SetFillStyle(1001); h_c->SetFillStyle(1001); h_b->SetFillStyle(1001);
  h_all->SetLineColor(kBlack);

  TH1F *hd_all = new TH1F("hd_all",TString::Format("hd_all;%s",title),bins, xmin, xmax);
  TH1F *hd_tag = new TH1F("hd_tag",TString::Format("hd_tag;%s",title),bins, xmin, xmax);
  TH1F *hd_untag = new TH1F("hd_untag",TString::Format("hd_untag;%s",title),bins, xmin, xmax);
  hd_all->SetMarkerColor(kblue);
  hd_untag->SetMarkerColor(kgreen);



  cout<<"MC jet content..."<<endl;

  for (int i=0;i<Nptbins;i++) 
  {
    //          int i=0;

  //    h_l->Reset(); h_c->Reset(); h_b->Reset(); h_all->Reset();

    TString sPhysicsCut = TString::Format("jtpt>%f && jtpt<%f",pTbins[i],pTbins[i+1]);

    const char * physicscut = sPhysicsCut.Data();
    cout<<physicscut<<endl;
    cout<<TString::Format("weight*(%s && abs(refparton_flavorForB)!=5 && abs(refparton_flavorForB)!=4 && %s)",physicscut,discr)<<endl;
    cout<<discr<<endl;
    t->Project("h_l", var,  TString::Format("weight*(%s && abs(refparton_flavorForB)!=5 && abs(refparton_flavorForB)!=4 && %s)",physicscut,discr));
    cout<<TString::Format("weight*(%s && abs(refparton_flavorForB)==4 && %s)",physicscut,discr)<<endl;
    t->Project("h_c", var,  TString::Format("weight*(%s && abs(refparton_flavorForB)==4 && %s)",physicscut,discr));
    cout<<TString::Format("weight*(%s && abs(refparton_flavorForB)==5 && %s)",physicscut,discr)<<endl;
    t->Project("h_b",  var, TString::Format("weight*(%s && abs(refparton_flavorForB)==5 && %s)",physicscut,discr));
    cout<<TString::Format("weight*(%s && %s)",physicscut,discr)<<endl;
    t->Project("h_all",var, TString::Format("weight*(%s && %s)",physicscut,discr));
    cout<<"mc projectionds done"<<endl;
  
    TH1F *hmc_b = (TH1F*)h_b->Clone("hmc_b");
    TH1F *hmc_c = (TH1F*)h_c->Clone("hmc_c");
    TH1F *hmc_l = (TH1F*)h_l->Clone("hmc_l");
  
    TH1F *mc1 = hmc_b;
    TH1F *mc2 = hmc_c;
    TH1F *mc3 = hmc_l;
    
    //  h_l->Smooth();
    //  h_c->Smooth();
    //  h_b->Smooth();
    //  h_all->Smooth();
    
    
    //    DrawMCjetcontent(h_b,h_c,h_l,h_all);
    DrawMCTemplates(h_b,h_c,h_l);
    
    cout<<"Data jet all and tagged..."<<endl;


    td->Project("hd_all",var,TString::Format("weightJet*(%s)",physicscut));
    td->Project("hd_tag",var,TString::Format("weightJet*(%s && %s)",physicscut,discrData));
    td->Project("hd_untag",var,TString::Format("weightJet*(%s && !(%s))",physicscut,discrData));

    DrawDataAllTagged(hd_tag, hd_untag, hd_all);

    cout<<"Fitting data jet content on tagged..."<<endl;
    
    DoFit(mc1,mc2,mc3, td, var , pTbins[i],pTbins[i+1], physicscut, discrData);

    cout<<"Fit done!"<<endl;

    //DataBFraction = 0.3; DataBFractionError=0.01;
    //DataCFraction = 0.2; DataCFractionError=0.01;
    //DataLFraction = 1-DataBFraction-DataCFraction;
    //DataLFractionError = DataBFractionError*2;

    //bin 0 is underflow bin!
    hdataBfrac->SetBinContent(i+1,DataBFraction);
    hdataBfrac->SetBinError(i+1,DataBFractionError);
    hdataCfrac->SetBinContent(i+1,DataCFraction);
    hdataCfrac->SetBinError(i+1,DataCFractionError);


    DrawTemplates(hd_tag, h_all, h_b,h_c,h_l,pTbins[i],pTbins[i+1]);


  }


  hdataBfrac->Sumw2();
  hdataCfrac->Sumw2();


  gStyle->SetErrorX(.5);
  //  TH1F::SetDefaultSumw2();


  TH1F *hmcTaggedAll = new TH1F("hmcTaggedAll","hmcTaggedAll",Nptbins,ptbins);
  TH1F *hmcTaggedC   = new TH1F("hmcTaggedC","hmcTaggedC",Nptbins,ptbins);
  TH1F *hmcTaggedB   = new TH1F("hmcTaggedB","hmcTaggedB",Nptbins,ptbins);
  t->Project("hmcTaggedAll","jtpt", TString::Format("weight*(%s)",discr));
  t->Project("hmcTaggedC",  "jtpt", TString::Format("weight*(abs(refparton_flavorForB)==4 && %s)",discr));
  t->Project("hmcTaggedB",  "jtpt", TString::Format("weight*(abs(refparton_flavorForB)==5 && %s)",discr));

  TH1F *hmcTotal    = new TH1F("hmcTotal","hmcTotal",Nptbins,ptbins);
  TH1F *hmcTotalB   = new TH1F("hmcTotalB","hmcTotalB",Nptbins,ptbins);
  TH1F *hmcTotalC   = new TH1F("hmcTotalC","hmcTotalC",Nptbins,ptbins);
  SetMCStyle(hmcTotalB,kred);SetMCStyle(hmcTotalC,kblue);

  t->Project("hmcTotal", "jtpt", "weight");
  t->Project("hmcTotalB","jtpt", "weight*(abs(refparton_flavorForB)==5)");
  t->Project("hmcTotalC","jtpt", "weight*(abs(refparton_flavorForB)==4)");

  TH1F *hmcBfraction   = new TH1F("hmcBfraction","hmcBfraction",Nptbins,ptbins);
  TH1F *hmcCfraction   = new TH1F("hmcCfraction","hmcCfraction",Nptbins,ptbins);
  SetMCStyle(hmcBfraction,kred); SetMCStyle(hmcCfraction,kblue);

  hmcBfraction->Divide(hmcTotalB,hmcTotal,1.,1.,"B");
  hmcCfraction->Divide(hmcTotalC,hmcTotal,1.,1.,"B");


  TH1F *hmctagBEff   = new TH1F("mctagBEff","mctagBEff",Nptbins,ptbins);
  TH1F *hmctagCEff   = new TH1F("mctagCEff","mctagCEff",Nptbins,ptbins);
  SetMCStyle(hmctagBEff,kred); SetMCStyle(hmctagCEff,kblue);
  //  hmctagBEff->SetMarkerColor(kred); hmctagCEff->SetMarkerColor(kblue); 

  hmctagBEff->Divide(hmcTaggedB,hmcTotalB,1.,1.,"B");
  hmctagCEff->Divide(hmcTaggedC,hmcTotalC,1.,1.,"B");

  TH1F *hmctagBPur   = new TH1F("mctagBPur","mctagBPur",Nptbins,ptbins);
  TH1F *hmctagCPur   = new TH1F("mctagCPur","mctagCPur",Nptbins,ptbins);
  SetMCStyle(hmctagBPur,kred); SetMCStyle(hmctagCPur,kblue);

  hmctagBPur->Divide(hmcTaggedB,hmcTaggedAll,1.,1.,"B");
  hmctagCPur->Divide(hmcTaggedC,hmcTaggedAll,1.,1.,"B");


  TH1F *hdataAll   = new TH1F("hdataAll","hdataAll",Nptbins,ptbins);
  TH1F *hdataTagged   = new TH1F("hdataTagged","hdataTagged",Nptbins,ptbins);
  td->Project("hdataAll","jtpt","weightJet");
  td->Project("hdataTagged","jtpt",TString::Format("weightJet*(%s)",discr));


  TH1F *htemp1   = new TH1F("htemp1","htemp1",Nptbins,ptbins);
  TH1F *htemp2   = new TH1F("htemp2","htemp2",Nptbins,ptbins);
  TH1F *hDataBFraction   = new TH1F("hDataBFraction","hDataBFraction",Nptbins,ptbins);
  TH1F *hDataCFraction   = new TH1F("hDataCFraction","hDataCFraction",Nptbins,ptbins);
  TH1F *hDataBSpectrum   = new TH1F("hDataBSpectrum","hDataBSpectrum",Nptbins,ptbins);
  TH1F *hDataCSpectrum   = new TH1F("hDataCSpectrum","hDataCSpectrum",Nptbins,ptbins);
  //mc spectrum is  hmcTotalB

  htemp1->Multiply(hdataTagged,hdataBfrac,1,1,"B");
  htemp2->Multiply(hdataAll,hmctagBEff,1,1,"B");
  hDataBFraction->Divide(htemp1, htemp2, 1,1,"B");
  hDataBSpectrum->Divide(htemp1,hmctagBEff); //WHY NOT BINOMIAL???
  //hDataBSpectrum->Multiply(hdataTagged,hdataBfrac,1,1,"B");

  htemp1->Multiply(hdataTagged,hdataCfrac,1,1,"B");
  htemp2->Multiply(hdataAll,hmctagCEff,1,1,"B");
  hDataCFraction->Divide(htemp1, htemp2, 1,1,"B");
  hDataCSpectrum->Divide(htemp1,hmctagCEff);


  TCanvas *bfrac = new TCanvas("bfractions","bfractions",600,600);
  hmcBfraction->Draw("P");
  hDataBFraction->Draw("P,same");
  hmcBfraction->SetMinimum(0); hmcBfraction->SetMaximum(0.06);
  hDataBFraction->SetMinimum(0); hDataBFraction->SetMaximum(0.06);

  AddLenend({hmcBfraction,hDataBFraction},{"MC","data"},{"P","P"},"tu","B Fractions");

  TCanvas *cfrac = new TCanvas("cfractions","cfractions",600,600);
  hmcCfraction->Draw("P");
  hDataCFraction->Draw("P,same");
  hmcCfraction->SetMinimum(0); hmcCfraction->SetMaximum(0.2);
  hDataCFraction->SetMinimum(0); hDataCFraction->SetMaximum(0.2);

  AddLenend({hmcCfraction,hDataCFraction},{"MC","data"},{"P","P"},"tu","C Fractions");


  TCanvas *cpur = new TCanvas("cpurities","cpurities",600,600);
  hmctagCPur->Draw();
  hdataCfrac->Draw("same");
  hmctagCPur->SetMinimum(0); hmctagCPur->SetMaximum(1);

  AddLenend({hmctagCPur,hdataCfrac},{"MC","data"},{"P","P"},"tu","C Tagging purity");


  TCanvas *bpur = new TCanvas("bpurities","bpurities",600,600);
  hmctagBPur->Draw();
  hdataBfrac->Draw("same");
  hmctagBPur->SetMinimum(0); hmctagBPur->SetMaximum(1);

  AddLenend({hmctagBPur,hdataBfrac},{"MC","data"},{"P","P"},"tu","B Tagging purity");


  TCanvas *ceff = new TCanvas("efficiencies","efficiencies",600,600);
  hmctagBEff->Draw();
  hmctagCEff->Draw("same");

  hmctagBEff->SetMinimum(0); hmctagBEff->SetMaximum(1);

  TCanvas *bspectrum = new TCanvas("bspectrum","bspectrum",600,600);
  hmcTotalB->Scale(1/hmcTotalB->Integral());
  hDataBSpectrum->Scale(1/hDataBSpectrum->Integral());

  hDataBSpectrum->Draw();
  hmcTotalB->Draw("same");
  AddLenend({hmcTotalB,hDataBSpectrum},{"MC","data"},{"P","P"},"tu","B spectrum");

  DrawCompare(hDataBSpectrum,hmcTotalB, "Data","MC","B spectrum");

  bspectrum->SetLogy();

  TCanvas *cspectrum = new TCanvas("cspectrum","cspectrum",600,600);
  hmcTotalC->Scale(1/hmcTotalC->Integral());
  hDataCSpectrum->Scale(1/hDataCSpectrum->Integral());

  hmcTotalC->Draw();
  hDataCSpectrum->Draw("same");
  AddLenend({hmcTotalC,hDataCSpectrum},{"MC","data"},{"P","P"},"tu","C spectrum");

  cspectrum->SetLogy();

  DrawCompare(hDataCSpectrum,hmcTotalC, "Data","MC","C spectrum");

}
