#include "Fitter.C"

int bins = 10;
double x1 = 0;
double x2 = 8;
double s = 1;

const int N = 1000;

double x1true=0.2, x2true=0.3, x3true=0.5;

void GenerateData(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data)
{
  data->SetMarkerColor(kRed);
  mc1->SetMarkerColor(kBlue);  mc1->SetLineColor(kBlue);
  mc2->SetMarkerColor(kGreen); mc2->SetLineColor(kGreen);
  mc3->SetMarkerColor(kBlack); mc3->SetLineColor(kBlack);

  
  
  for (int i=0;i<N;i++) {
    mc1->Fill(gRandom->Gaus(2,s));
    mc2->Fill(gRandom->Gaus(3,s));
    mc3->Fill(gRandom->Gaus(4,s));

    data->Fill(gRandom->Gaus(2,s),x1true);
    data->Fill(gRandom->Gaus(3,s),x2true);
    data->Fill(gRandom->Gaus(4,s),x3true);
  }

  //normalize data for now
  data->Scale(1/data->Integral());
  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());

}

void testMM()
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

  double xx[]={x1true,x2true,x3true};
  cout<<"Chi2 of truth = "<<Chi2(xx)<<endl;

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
