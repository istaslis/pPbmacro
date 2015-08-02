#include "TH1F.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLine.h"
#include "Math/GSLSimAnMinimizer.h"
#include "Math/Functor.h"
#include "TMatrixD.h"

TH1F *tmp1, *tmp2, *tmp3, *tmpdata;

void ShakeMC(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *mcout1, TH1F *mcout2, TH1F *mcout3)
{
  for (int i=1;i<=mc1->GetNbinsX();i++) {
    mcout1->SetBinContent(i,gRandom->Gaus(mc1->GetBinContent(i),0.5*mc1->GetBinError(i)));
    mcout2->SetBinContent(i,gRandom->Gaus(mc2->GetBinContent(i),0.5*mc2->GetBinError(i)));
    mcout3->SetBinContent(i,gRandom->Gaus(mc3->GetBinContent(i),0.5*mc3->GetBinError(i)));
  }


}

double Chi2(const double *xx ) 
{
  const Double_t x1 = xx[0];
  const Double_t x2 = xx[1];
  const Double_t x3 = 1-x1-x2;

  bool dataOnlyError = true; //false is incorrect... probably

  double chi2;

  for (int i=1;i<=tmpdata->GetNbinsX();i++) {
    double num = tmpdata->GetBinContent(i)-(x1*tmp1->GetBinContent(i)+x2*tmp2->GetBinContent(i)+x3*tmp3->GetBinContent(i));
    double s1 = tmpdata->GetBinError(i);
    if (dataOnlyError)  {
      if (s1*s1!=0) 
	chi2+=num*num/(s1*s1); 	//    else chi2+=1000;
      //      cout<<x1<<" "<<tmp1->GetBinContent(i)<<" "<<x2<<" "<<tmp2->GetBinContent(i)<<" "<<x3<<" "<<tmp3->GetBinContent(i)<<": "  <<s1<<" ? "<<num<<"_"<<1/s1/s1<<endl;

      //      cout<<"bin "<<i<<" chi2 = "<<num*num/(s1*s1)<<endl;
    } else {
      
      double s2sq = x1*x1*tmp1->GetBinError(i)*tmp1->GetBinError(i)+
	x2*x2*tmp2->GetBinError(i)*tmp2->GetBinError(i)+
	x3*x3*tmp3->GetBinError(i)*tmp3->GetBinError(i);

      //      cout<<x1<<" "<<tmp1->GetBinError(i)<<" "<<x2<<" "<<tmp2->GetBinError(i)<<" "<<x3<<" "<<tmp3->GetBinError(i)<<": "
      //	  <<s1<<"_"<<sqrt(s2sq)<<" ? "<<s1+sqrt(s2sq)<<endl;
      /*if (s1!=0 && s2sq!=0) {
	double oneovererror = 1/s1/s1+1/s2sq;
	chi2+=num*num*oneovererror;
	cout<<"bin "<<i<<" chi2 = "<<num*num*oneovererror<<endl;

        } //else chi2+=1000;
      */
      s1+=sqrt(s2sq);
      if (s1*s1!=0)
	chi2+=num*num/(s1*s1);  
      

 }
    
  }

  //  cout<<x1<<" "<<x2<<" "<<x3<<" "<<chi2/(tmpdata->GetNbinsX()-2)<<" : "<<tmpdata->Integral()<<" "<<tmp1->Integral()<<endl;

  //ndof = n points - n params;
  return chi2/(tmpdata->GetNbinsX()-2);


}

//NOT YET IMPLEMENTED
double ML(const double *xx ) 
{
  const Double_t x1 = xx[0];
  const Double_t x2 = xx[1];
  const Double_t x3 = 1-x1-x2;

  double chi2;

  for (int i=1;i<=tmpdata->GetNbinsX();i++) {

    double num = tmpdata->GetBinContent(i)-(x1*tmp1->GetBinContent(i)+x2*tmp2->GetBinContent(i)+x3*tmp3->GetBinContent(i));
    double s1 = tmpdata->GetBinError(i);
    //    double s2 = x1*tmp1->GetBinError(i)+x2*tmp2->GetBinError(i)+x3*tmp3->GetBinError(i);
    //    if (s1!=0 && s2!=0) {
    //      double error = s1*s1/sqrt(s1*s1+s2*s2);
    //      chi2+=num*num/error;
    //    } else chi2+=1000;

    if (s1>0.001) chi2+=num*num/s1;
    else chi2+=1000;


  }

  //  cout<<x1<<" "<<x2<<" "<<x3<<" "<<chi2<<" : "<<tmpdata->Integral()<<" "<<tmp1->Integral()<<endl;

  return chi2;


}

double GetFractionsMin(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  tmp1 = (TH1F *)mc1->Clone("tmp1");
  tmp2 = (TH1F *)mc2->Clone("tmp2");
  tmp3 = (TH1F *)mc3->Clone("tmp3");
  tmpdata = (TH1F *)data->Clone("tmpdata");

  tmp1->Scale(1/tmp1->Integral());
  tmp2->Scale(1/tmp2->Integral());
  tmp3->Scale(1/tmp3->Integral());
  tmpdata->Scale(1/tmpdata->Integral());

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
  
  return Chi2(xs);

}
double GetFractionsMinBrute(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  tmp1 = (TH1F *)mc1->Clone("tmp1");
  tmp2 = (TH1F *)mc2->Clone("tmp2");
  tmp3 = (TH1F *)mc3->Clone("tmp3");
  tmpdata = (TH1F *)data->Clone("tmpdata");

  tmp1->Scale(1/tmp1->Integral());
  tmp2->Scale(1/tmp2->Integral());
  tmp3->Scale(1/tmp3->Integral());
  tmpdata->Scale(1/tmpdata->Integral());

  double minChi2 = 9999, minx1=0, minx2=0; int NNN = 0;
  for (double ix1=0;ix1<1;ix1+=0.01)
    for (double ix2=0;ix2<1;ix2+=0.01) {
      double xx[] = {ix1,ix2,1-ix1-ix2};
      double chi2 = Chi2(xx);
      
      cout<<chi2<<endl;

      if (chi2<minChi2) {NNN++;
	minChi2 = chi2; minx1=ix1;minx2=ix2;}
      delete xx;
    }
 
  x1 = minx1; x2 = minx2; x3 = 1-(x1+x2);
  double xx[] = {x1,x2,1-x1-x2};
  cout<<"NNN "<<NNN<<endl;
  cout << "Minimum: f(" << x1 << "," << x2 << "): "<< Chi2(xx) << endl;

  return Chi2(xx);

}


double GetFractions(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  tmp1 = (TH1F *)mc1->Clone("tmp1");
  tmp2 = (TH1F *)mc2->Clone("tmp2");
  tmp3 = (TH1F *)mc3->Clone("tmp3");
  tmpdata = (TH1F *)data->Clone("tmpdata");

  TMatrixD A(3,3);
  TMatrixD E(3,1);
  TMatrixD X(3,1);

  for (int i=2;i<=data->GetNbinsX();i++) {
    double x = data->GetBinCenter(i);
    double f = data->GetBinContent(i);

    cout<<"x"<<x<<" f "<<f<<endl;

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

  double xx[] ={x1,x2,x3};
  return Chi2(xx);

}
double GetFractions2(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{
  tmp1 = (TH1F *)mc1->Clone("tmp1");
  tmp2 = (TH1F *)mc2->Clone("tmp2");
  tmp3 = (TH1F *)mc3->Clone("tmp3");
  tmpdata = (TH1F *)data->Clone("tmpdata");

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

  double xx[] ={x1,x2,x3};
  return Chi2(xx);

}

double Fit(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *origdata,TH1F *hx1, TH1F *hx2, TH1F *hx3, TH1F *hchi2)
{
  TH1F *mctmp1 = (TH1F *)mc1->Clone("mctmp1");
  TH1F *mctmp2 = (TH1F *)mc2->Clone("mctmp2");
  TH1F *mctmp3 = (TH1F *)mc3->Clone("mctmp3");
  TH1F *data = (TH1F *)origdata->Clone("data");

  mctmp1->Scale(1/mctmp1->Integral());
  mctmp2->Scale(1/mctmp2->Integral());
  mctmp3->Scale(1/mctmp3->Integral());
  data->Scale(1/data->Integral());

  cout<<"mctmp1 "<<mctmp1->Integral()<<" mctmp2 "<<mctmp2->Integral()
      <<"mctmp3 "<<mctmp3->Integral()<<" data "<<data->Integral()<<endl;
  
  TCanvas *c = new TCanvas("q","q",600,600);
  data->Draw();
  mctmp1->Draw("same");
  mctmp2->Draw("same");
  mctmp3->Draw("same");
  c->SaveAs("q.pdf");

  double chi2c = 0, x1c = 0, x2c = 0, x3c = 0;

  const int Ntoy = 1;

  for (int i=0;i<Ntoy;i++) {
    ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3); 

    double v1,v2,v3,chi2;
    //chi2 = GetFractions(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);
    //chi2 = GetFractions2(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);
    //chi2 = GetFractionsMin(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);
    chi2 = GetFractionsMinBrute(mctmp1,mctmp2,mctmp3,data,v1,v2,v3);

    chi2c+=chi2; x1c+=v1; x2c+=v2; x3c+=v3;
    hx1->Fill(v1); hx2->Fill(v2); hx3->Fill(v3); hchi2->Fill(chi2);
  }
  chi2c/=Ntoy;x1c/=Ntoy;x2c/=Ntoy;x3c/=Ntoy;

  cout<<"Average Fit Chi2 = "<<chi2c<<endl;

  cout<<"Average x1 = "<<x1c<<endl;
  cout<<"Average x2 = "<<x2c<<endl;
  cout<<"Average x3 = "<<x3c<<endl;

  double xx[] ={x1c,x2c,x3c};
  cout<<"Chi2 of average fit = "<<Chi2(xx)<<endl;


  return chi2c;

}

