int bins = 10;
double x1 = 0;
double x2 = 8;
double s = 1;

const int N = 1000;

double x1true=0.2, x2true=0.3, x3true=0.5;

void GenerateData(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data)
{
  data->SetMarkerColor(kRed);
  mc1->SetMarkerColor(kBlue);
  mc2->SetMarkerColor(kGreen);
  mc3->SetMarkerColor(kBlack);

  
  
  for (int i=0;i<N;i++) {
    mc1->Fill(gRandom->Gaus(2,s));
    mc2->Fill(gRandom->Gaus(4,s));
    mc3->Fill(gRandom->Gaus(6,s));

    data->Fill(gRandom->Gaus(2,s),x1true);
    data->Fill(gRandom->Gaus(4,s),x2true);
    data->Fill(gRandom->Gaus(6,s),x3true);
  }

  //normalize data for now
  data->Scale(1/data->Integral());
  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());

}

void ShakeMC(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *mcout1, TH1F *mcout2, TH1F *mcout3)
{
  for (int i=1;i<=mc1->GetNbinsX();i++) {
    mcout1->SetBinContent(i,gRandom->Gaus(mc1->GetBinContent(i),mc1->GetBinError(i)));
    mcout2->SetBinContent(i,gRandom->Gaus(mc2->GetBinContent(i),mc2->GetBinError(i)));
    mcout3->SetBinContent(i,gRandom->Gaus(mc3->GetBinContent(i),mc3->GetBinError(i)));
  }


}

void GetFractions(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  TMatrixD A(3,3);
  TMatrixD E(3,1);
  TMatrixD X(3,1);

  for (int i=1;i<=data->GetNbinsX();i++) {
    double x = data->GetBinCenter(i);
    double f = data->GetBinContent(i);

    double f1 = mc1->GetBinContent(i);
    double f2 = mc2->GetBinContent(i);
    double f3 = mc3->GetBinContent(i);


    A(0,0)+=f1*x;     A(0,1)+=f2*x;     A(0,2)+=f3*x;
    A(1,0)+=f1*x*x;   A(1,1)+=f2*x*x;   A(1,2)+=f3*x*x;
    A(2,0)+=f1*x*x*x; A(2,1)+=f2*x*x*x; A(2,2)+=f3*x*x*x;

    E(0,0)+=f*x; E(1,0)+=f*x*x; E(2,0)+=f*x*x*x; 

  }

  X.Mult(A.Invert(),E);

  x1 = X(0,0); x2 = X(1,0); x3 = X(2,0);

  //  cout<<"X[0]  = "<<X(0,0)<<", X[1]  = "<<X(1,0)<<", X[2]  = "<<X(2,0)<<endl;


}
void GetFractions2(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  TMatrixD A(2,2);
  TMatrixD E(2,1);
  TMatrixD X(2,1);

  for (int i=1;i<=data->GetNbinsX();i++) {
    double x = data->GetBinCenter(i);
    double f = data->GetBinContent(i);

    double f1 = mc1->GetBinContent(i);
    double f2 = mc2->GetBinContent(i);
     double f3 = mc3->GetBinContent(i);


     A(0,0)+=(f1-f3)*x;     A(0,1)+=(f2-f3)*x;
     A(1,0)+=(f1-f3)*x*x*x;   A(1,1)+=(f2-f3)*x*x*x;

    E(0,0)+=(f-f3)*x; E(1,0)+=(f-f3)*x*x*x;

  }

  X.Mult(A.Invert(),E);

  x1 = X(0,0); x2 = X(1,0); x3 = 1-x1-x2;//X(2,0);

  //  cout<<"X[0]  = "<<x1<<", X[1]  = "<<x2<<", X[2]  = "<<x3<<endl;


}

TH1F *tmp1, *tmp2, *tmp3, *tmpdata;

double Chi2(const double *xx ) 
{
  const Double_t x1 = xx[0];
  const Double_t x2 = xx[1];
  const Double_t x3 = 1-x1-x2;

  double chi2;

  for (int i=1;i<=tmpdata->GetNbinsX();i++) {
    double num = tmpdata->GetBinContent(i)-(x1*tmp1->GetBinContent(i)+x2*tmp2->GetBinContent(i)+x3*tmp3->GetBinContent(i));
    if (tmpdata->GetBinError(i)!=0)
      chi2+=num*num/tmpdata->GetBinError(i);
  }

  //  cout<<x1<<" "<<x2<<" "<<x3<<" "<<chi2<<" : "<<tmpdata->Integral()<<" "<<tmp1->Integral()<<endl;

  return chi2;


}

void GetFractionsMin(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  tmp1 = (TH1F *)mc1->Clone("tmp1");
  tmp2 = (TH1F *)mc2->Clone("tmp2");
  tmp3 = (TH1F *)mc3->Clone("tmp3");
  tmpdata = (TH1F *)data->Clone("tmpdata");

  // Choose method upon creation between:
  // kConjugateFR, kConjugatePR, kVectorBFGS,
  // kVectorBFGS2, kSteepestDescent
  //  ROOT::Math::GSLMinimizer min( ROOT::Math::kSteepestDescent);
  ROOT::Math::GSLSimAnMinimizer min;

  min.SetMaxFunctionCalls(1000000);
  min.SetMaxIterations(100000);
  min.SetTolerance(0.001);
 
  ROOT::Math::Functor f(&Chi2,2); 
  double step[2] = {0.01,0.01};
  double variable[2] = { 0.5, 0.5};
 
  min.SetFunction(f);
 
  // Set the free variables to be minimized!
  min.SetVariable(0,"x",variable[0], step[0]);
  min.SetVariable(1,"y",variable[1], step[1]);

  min.SetVariableLimits(0,0,1);
  min.SetVariableLimits(1,0,1);
 
  min.Minimize(); 
 
  const double *xs = min.X();
  //  cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): "<< Chi2(xs) << endl;

  x1 = xs[0]; x2 = xs[1]; x3 = 1-(x1+x2);
  

}

void testMM()
{
  TH1F *hx1 = new TH1F("hx1","hx1",100,0,1);
  TH1F *hx2 = new TH1F("hx2","hx2",100,0,1);
  TH1F *hx3 = new TH1F("hx3","hx3",100,0,1);

  TH1F *mc1, *mc2, *mc3, *data, *tmp;
  TH1F *mctmp1, *mctmp2, *mctmp3;

  mc1 = new TH1F("mc1","mc1",bins,x1,x2);
  mc2 = new TH1F("mc2","mc2",bins,x1,x2);
  mc3 = new TH1F("mc3","mc3",bins,x1,x2);

  data = new TH1F("data","data",bins,x1,x2);
  tmp = new TH1F("tmp","tmp",bins,x1,x2);

  mc1->Sumw2(); mc2->Sumw2(); mc3->Sumw2(); data->Sumw2();



  GenerateData(mc1,mc2,mc3,data);




  mctmp1 = (TH1F *)mc1->Clone("mctmp1");
  mctmp2 = (TH1F *)mc2->Clone("mctmp2");
  mctmp3 = (TH1F *)mc3->Clone("mctmp3");

  for (int i=0;i<1000;i++) {
    ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3);
    //    GenerateData(mc1,mc2,mc3,tmp);

    double v1,v2,v3;
    //GetFractions(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);
    //GetFractions2(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);
    GetFractionsMin(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);
    hx1->Fill(v1); hx2->Fill(v2); hx3->Fill(v3);
  }

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


  cout<<"x1 = "<<hx1->GetMean()<<"±"<<hx1->GetStdDev()<<endl;
  cout<<"x2 = "<<hx2->GetMean()<<"±"<<hx2->GetStdDev()<<endl;
  cout<<"x3 = "<<hx3->GetMean()<<"±"<<hx3->GetStdDev()<<endl;

  mc1->Scale(hx1->GetMean());
  mc2->Scale(hx2->GetMean());
  mc3->Scale(hx3->GetMean());

  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  data->Draw();
  mc1->Draw("same");
  mc2->Draw("same");
  mc3->Draw("same");
  data->Draw("same");



}
