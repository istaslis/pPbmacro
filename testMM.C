#include "Fitter.C"

int bins = 10;
double x1 = 0;
double x2 = 8;
double s = 2;

const int N = 100;

double x1true=0.1, x2true=0.3, x3true=0.6;

double x1center = 2, x2center = 4, x3center = 6;

void GenerateData(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data)
{
  data->SetMarkerColor(kRed);
  mc1->SetMarkerColor(kBlue);  mc1->SetLineColor(kBlue);
  mc2->SetMarkerColor(kGreen); mc2->SetLineColor(kGreen);
  mc3->SetMarkerColor(kBlack); mc3->SetLineColor(kBlack);

  
  
  for (int i=0;i<N;i++) {
    mc1->Fill(gRandom->Gaus(3.5,s));
    mc2->Fill(gRandom->Gaus(4,s));
    mc3->Fill(gRandom->Gaus(4.5,s));

    data->Fill(gRandom->Gaus(3.5,s),x1true);
    data->Fill(gRandom->Gaus(4,s),x2true);
    data->Fill(gRandom->Gaus(4.5,s),x3true);
  }

  //normalize data for now
  data->Scale(1/data->Integral());
  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());

}

void GenerateDataUnbinned(TH1F *mc1, TH1F *mc2, TH1F*mc3, TNtuple *data)
{
  data->SetMarkerColor(kRed);
  mc1->SetMarkerColor(kBlue);  mc1->SetLineColor(kBlue);
  mc2->SetMarkerColor(kGreen); mc2->SetLineColor(kGreen);
  mc3->SetMarkerColor(kBlack); mc3->SetLineColor(kBlack);

  
  
  for (int i=0;i<N;i++) {
    mc1->Fill(gRandom->Gaus(x1center,s));
    mc2->Fill(gRandom->Gaus(x2center,s));
    mc3->Fill(gRandom->Gaus(x3center,s));

    data->Fill(gRandom->Gaus(x1center,s),x1true);
    data->Fill(gRandom->Gaus(x2center,s),x2true);
    data->Fill(gRandom->Gaus(x3center,s),x3true);
  }

  //normalize data for now
  //data->Scale(1/data->Integral());
  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());

}

Double_t median1(TH1 *h1) { 
  //compute the median for 1-d histogram h1 
  Int_t nbins = h1->GetXaxis()->GetNbins(); 
  Double_t *x = new Double_t[nbins]; 
  Double_t *y = new Double_t[nbins]; 
  for (Int_t i=0;i<nbins;i++) {
    x[i] = h1->GetXaxis()->GetBinCenter(i+1); 
    y[i] = h1->GetBinContent(i+1); 
  } 
  Double_t median = TMath::Median(nbins,x,y); 
  delete [] x; 
  delete [] y; 
  return median; 
} 

void testMM()
{
  //TFile *fout = new TFile("fout.root","recreate");
  TH1F *hx1 = new TH1F("hx1","hx1",100,0,1);
  TH1F *hx2 = new TH1F("hx2","hx2",100,0,1);
  TH1F *hx3 = new TH1F("hx3","hx3",100,0,1);
  TH1F *hchi2 = new TH1F("hchi2","hchi2",1000,0,2000);

  TH1F *mc1, *mc2, *mc3, *data, *tmp;

  mc1 = new TH1F("mc1","mc1",bins,x1,x2);
  mc2 = new TH1F("mc2","mc2",bins,x1,x2);
  mc3 = new TH1F("mc3","mc3",bins,x1,x2);

  TNtuple *d = new TNtuple("d","d","x:weight");

  TH1F *pred = new TH1F("pred","pred",bins,x1,x2);
  TH1F *truth = new TH1F("truth","truth",bins,x1,x2);

  data = new TH1F("data","data",bins,x1,x2);
  tmp = new TH1F("tmp","tmp",bins,x1,x2);

  mc1->Sumw2(); mc2->Sumw2(); mc3->Sumw2(); data->Sumw2();



  //GenerateData(mc1,mc2,mc3,data);
  GenerateDataUnbinned(mc1,mc2,mc3,d);

  d->Project("data","x","weight");
 
  TCanvas *cc = new TCanvas ("temp","temp",600,600);
  data->Draw();
  cc->SaveAs("data.pdf");
  cout<<"???"<<data->Integral()<<endl;



  double chi2 = Fit(mc1,mc2,mc3,d,hx1,hx2,hx3,hchi2);

  cout<<"Fit chi2 = "<<chi2<<endl;

  TH1F *mctmp1, *mctmp2, *mctmp3;
  mctmp1 = (TH1F *)mc1->Clone("mctmp1x");
  mctmp2 = (TH1F *)mc2->Clone("mctmp2x");
  mctmp3 = (TH1F *)mc3->Clone("mctmp3x");

  ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3);
  
  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  mc1->Draw();
  mctmp1->Draw("same");

  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  hx1->Draw();
  hx2->Draw("same");
  hx3->Draw("same");
  double ymax = hx1->GetMaximum();
  TLine *l1 = new TLine(x1true,0,x1true,ymax); l1->SetLineWidth(2.0);l1->SetLineColor(kRed); l1->Draw();
  TLine *l2 = new TLine(x2true,0,x2true,ymax); l2->SetLineWidth(2.0);l2->SetLineColor(kRed); l2->Draw();
  TLine *l3 = new TLine(x3true,0,x3true,ymax); l3->SetLineWidth(2.0);l3->SetLineColor(kRed); l3->Draw();

  TCanvas *cchi2 = new TCanvas("cchi2","cchi2",600,600);
  hchi2->Draw();


  cout<<"x1 = "<<hx1->GetMean()<<"±"<<hx1->GetStdDev()<<endl;
  cout<<"x2 = "<<hx2->GetMean()<<"±"<<hx2->GetStdDev()<<endl;
  cout<<"x3 = "<<hx3->GetMean()<<"±"<<hx3->GetStdDev()<<endl;

  cout<<"Median 1 "<<median1(hx1)<<endl;
  cout<<"Median 2 "<<median1(hx2)<<endl;
  cout<<"Median 3 "<<median1(hx3)<<endl;

  double xx[]={x1true,x2true,x3true};
  //cout<<"Chi2 of truth = "<<Chi2(xx)<<endl;

  mc1->Scale(hx1->GetMean());
  mc2->Scale(hx2->GetMean());
  mc3->Scale(hx3->GetMean());

  pred->Add(mc1);  pred->Add(mc2);  pred->Add(mc3);
  pred->SetMarkerColor(kBlue);

  truth->Add(mc1,x1true/mc1->Integral());
  truth->Add(mc2,x2true/mc2->Integral());
  truth->Add(mc3,x3true/mc3->Integral());
  truth->SetMarkerColor(kBlack);


  data->Scale(1/data->Integral());
  data->SetMarkerColor(kRed);
  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  data->Draw();
  mc1->Draw("same,hist");
  mc2->Draw("same,hist");
  mc3->Draw("same,hist");

  pred->Draw("same");
  data->Draw("same");
  truth->Draw("same");

  float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;
  TLegend *l = new TLegend(legendx1,legendy1,legendx2,legendy2);
  l->AddEntry(pred,"Fit","P");
  l->AddEntry(data,"Data","P");
  l->AddEntry(truth,"Truth","P");

  l->Draw();




}
/*
void testMMUnbinnedTODO()
{
  TH1F *hx1 = new TH1F("hx1","hx1",100,0,1);
  TH1F *hx2 = new TH1F("hx2","hx2",100,0,1);
  TH1F *hx3 = new TH1F("hx3","hx3",100,0,1);
  TH1F *hchi2 = new TH1F("hchi2","hchi2",1000,0,2000);

  TH1F *mc1, *mc2, *mc3, *data, *tmp;

  mc1 = new TH1F("mc1","mc1",bins,x1,x2);
  mc2 = new TH1F("mc2","mc2",bins,x1,x2);
  mc3 = new TH1F("mc3","mc3",bins,x1,x2);

  TH1F *pred = new TH1F("pred","pred",bins,x1,x2);
  TH1F *truth = new TH1F("truth","truth",bins,x1,x2);

  data = new TH1F("data","data",bins,x1,x2);
  tmp = new TH1F("tmp","tmp",bins,x1,x2);

  mc1->Sumw2(); mc2->Sumw2(); mc3->Sumw2(); data->Sumw2();



  GenerateData(mc1,mc2,mc3,data);

  double chi2 = Fit(mc1,mc2,mc3,data,hx1,hx2,hx3,hchi2);

  cout<<"Fit chi2 = "<<chi2<<endl;

  TH1F *mctmp1, *mctmp2, *mctmp3;
  mctmp1 = (TH1F *)mc1->Clone("mctmp1x");
  mctmp2 = (TH1F *)mc2->Clone("mctmp2x");
  mctmp3 = (TH1F *)mc3->Clone("mctmp3x");

  ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3);
  
  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  mc1->Draw();
  mctmp1->Draw("same");

  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  hx1->Draw();
  hx2->Draw("same");
  hx3->Draw("same");
  double ymax = hx1->GetMaximum();
  TLine *l1 = new TLine(x1true,0,x1true,ymax); l1->SetLineWidth(2.0);l1->SetLineColor(kRed); l1->Draw();
  TLine *l2 = new TLine(x2true,0,x2true,ymax); l2->SetLineWidth(2.0);l2->SetLineColor(kRed); l2->Draw();
  TLine *l3 = new TLine(x3true,0,x3true,ymax); l3->SetLineWidth(2.0);l3->SetLineColor(kRed); l3->Draw();

  TCanvas *cchi2 = new TCanvas("cchi2","cchi2",600,600);
  hchi2->Draw();


  cout<<"x1 = "<<hx1->GetMean()<<"±"<<hx1->GetStdDev()<<endl;
  cout<<"x2 = "<<hx2->GetMean()<<"±"<<hx2->GetStdDev()<<endl;
  cout<<"x3 = "<<hx3->GetMean()<<"±"<<hx3->GetStdDev()<<endl;



//  double xx[]={x1true,x2true,x3true};
//  cout<<"Chi2 of truth = "<<Chi2(xx)<<endl;

  mc1->Scale(hx1->GetMean());
  mc2->Scale(hx2->GetMean());
  mc3->Scale(hx3->GetMean());

  pred->Add(mc1);  pred->Add(mc2);  pred->Add(mc3);
  pred->SetMarkerColor(kBlue);

  truth->Add(mc1,x1true/mc1->Integral());
  truth->Add(mc2,x2true/mc2->Integral());
  truth->Add(mc3,x3true/mc3->Integral());
  truth->SetMarkerColor(kBlack);


  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  data->Draw();
  mc1->Draw("same,hist");
  mc2->Draw("same,hist");
  mc3->Draw("same,hist");

  pred->Draw("same");
  data->Draw("same");
  truth->Draw("same");


}
*/
