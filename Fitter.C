#include "TH1F.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLine.h"
#include "Math/GSLSimAnMinimizer.h"
#include "Math/Functor.h"
#include "TMatrixD.h"
#include "TObjArray.h"
#include "TFractionFitter.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "TLegend.h"
#include "RooDataSet.h"
#include "RooFitResult.h"


//using namespace Roofit;

TH1F *tmp1, *tmp2, *tmp3, *tmpdata;
TTree *tmpdatatree;
vector <double> tmpdatavector_x, tmpdatavector_weight;

void ShakeMC(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *mcout1, TH1F *mcout2, TH1F *mcout3)
{
  for (int i=1;i<=mc1->GetNbinsX();i++) {
    mcout1->SetBinContent(i,gRandom->Gaus(mc1->GetBinContent(i),0.5*mc1->GetBinError(i)));
    mcout2->SetBinContent(i,gRandom->Gaus(mc2->GetBinContent(i),0.5*mc2->GetBinError(i)));
    mcout3->SetBinContent(i,gRandom->Gaus(mc3->GetBinContent(i),0.5*mc3->GetBinError(i)));
  }


}

double sqr(double x) {return x*x;}

void ShakeMCAndData(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, TH1F *mcout1, TH1F *mcout2, TH1F *mcout3,TH1F *dataout)
{
  
    for (int i=1;i<=mc1->GetNbinsX();i++) {
    mcout1->SetBinContent(i,gRandom->Gaus(mc1->GetBinContent(i),1*mc1->GetBinError(i)));
    mcout2->SetBinContent(i,gRandom->Gaus(mc2->GetBinContent(i),1*mc2->GetBinError(i)));
    mcout3->SetBinContent(i,gRandom->Gaus(mc3->GetBinContent(i),1*mc3->GetBinError(i)));

    dataout->SetBinContent(i,gRandom->Gaus(data->GetBinContent(i),1*data->GetBinError(i)));

  }

}

void RegenerateMCAndData(TH1F *mcout1, TH1F *mcout2, TH1F *mcout3,TH1F *dataout)
{
  double s = 1;
  const int N = 10;
  double x1true=0.1, x2true=0.3, x3true=0.6;
  double x1center = 2, x2center = 4, x3center = 6;
  
   mcout1->Reset();
   mcout2->Reset();
   mcout3->Reset();
   dataout->Reset();

  for (int i=0;i<N;i++) {
    mcout1->Fill(gRandom->Gaus(x1center,s));
    mcout2->Fill(gRandom->Gaus(x2center,s));
    mcout3->Fill(gRandom->Gaus(x3center,s));

    dataout->Fill(gRandom->Gaus(x1center,s),x1true);
    dataout->Fill(gRandom->Gaus(x2center,s),x2true);
    dataout->Fill(gRandom->Gaus(x3center,s),x3true);
  }


  //normalize data for now
  dataout->Scale(1/dataout->Integral());
  mcout1->Scale(1/mcout1->Integral());
  mcout2->Scale(1/mcout2->Integral());
  mcout3->Scale(1/mcout3->Integral());


}

void RegenerateMCAndData(TH1F *mcout1, TH1F *mcout2, TH1F *mcout3,TNtuple *dataout)
{
  double s = 2;
  const int N = 100;
  double x1true=0.1, x2true=0.3, x3true=0.6;
  double x1center = 2, x2center = 4, x3center = 6;
  
   mcout1->Reset();
   mcout2->Reset();
   mcout3->Reset();
   dataout->Reset();

  for (int i=0;i<N;i++) {
    mcout1->Fill(gRandom->Gaus(x1center,s));
    mcout2->Fill(gRandom->Gaus(x2center,s));
    mcout3->Fill(gRandom->Gaus(x3center,s));

    dataout->Fill(gRandom->Gaus(x1center,s),x1true);
    dataout->Fill(gRandom->Gaus(x2center,s),x2true);
    dataout->Fill(gRandom->Gaus(x3center,s),x3true);
  }


  //normalize data for now
  //dataout->Scale(1/dataout->Integral());
  mcout1->Scale(1/mcout1->Integral());
  mcout2->Scale(1/mcout2->Integral());
  mcout3->Scale(1/mcout3->Integral());


}



void RegenerateMC(TH1F *mcout1, TH1F *mcout2, TH1F *mcout3)
{
  double s = 1;
  const int N = 1000;
double x1true=0.1, x2true=0.3, x3true=0.6;
double x1center = 2, x2center = 4, x3center = 6;

   mcout1->Reset();
   mcout2->Reset();
   mcout3->Reset();
   //dataout->Reset();

  for (int i=0;i<N;i++) {
    mcout1->Fill(gRandom->Gaus(x1center,s));
    mcout2->Fill(gRandom->Gaus(x2center,s));
    mcout3->Fill(gRandom->Gaus(x3center,s));

   // dataout->Fill(gRandom->Gaus(x1center,s),x1true);
   // dataout->Fill(gRandom->Gaus(x2center,s),x2true);
   // dataout->Fill(gRandom->Gaus(x3center,s),x3true);
  }


  //normalize data for now
  //dataout->Scale(1/dataout->Integral());
  mcout1->Scale(1/mcout1->Integral());
  mcout2->Scale(1/mcout2->Integral());
  mcout3->Scale(1/mcout3->Integral());


}

void ShakeMCAndDataBoot(TH1F *mc1, TH1F *mc2, TH1F*mc3, TTree *data, TH1F *mcout1, TH1F *mcout2, TH1F *mcout3,TH1F *dataout)
{
  tmpdatatree = data;

    for (int i=1;i<=mc1->GetNbinsX();i++) {
    mcout1->SetBinContent(i,gRandom->Gaus(mc1->GetBinContent(i),1*mc1->GetBinError(i)));
    mcout2->SetBinContent(i,gRandom->Gaus(mc2->GetBinContent(i),1*mc2->GetBinError(i)));
    mcout3->SetBinContent(i,gRandom->Gaus(mc3->GetBinContent(i),1*mc3->GetBinError(i)));

    //    dataout->SetBinContent(i,gRandom->Gaus(data->GetBinContent(i),0.5*data->GetBinError(i)));

  }

  float x, weight;
  tmpdatatree->SetBranchAddress("x",&x);
  tmpdatatree->SetBranchAddress("weight",&weight);

  tmpdatavector_x.clear(); tmpdatavector_weight.clear();
  dataout->Reset();

  //bootstrap
  int N = tmpdatatree->GetEntries();
  for (int i=0;i<N;i++) {
    tmpdatatree->GetEntry(i);//gRandom->Uniform(0,N));
    tmpdatavector_x.push_back(x);
    tmpdatavector_weight.push_back(weight);
    dataout->Fill(x,weight);
  }

}

void TransformMCAndData(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, TH1F *mcout1, TH1F *mcout2, TH1F *mcout3,TH1F *dataout)
{
  
  for (int i=1;i<=mc1->GetNbinsX();i++) {
    if (mc1->GetBinError(i)!=0)
    mcout1->SetBinContent(i,sqr(mc1->GetBinContent(i)/mc1->GetBinError(i)));
    if (mc2->GetBinError(i)!=0)
    mcout2->SetBinContent(i,sqr(mc2->GetBinContent(i)/mc2->GetBinError(i)));
    if (mc3->GetBinError(i)!=0)
    mcout3->SetBinContent(i,sqr(mc3->GetBinContent(i)/mc3->GetBinError(i)));

    if (dataout->GetBinError(i)!=0)
    dataout->SetBinContent(i,sqr(data->GetBinContent(i)/data->GetBinError(i)));

    }
}

double Chi2(const double *xx ) 
{
  const Double_t x1 = xx[0];
  const Double_t x2 = xx[1];
  const Double_t x3 = 1-x1-x2;

  bool dataOnlyError = false; //false is incorrect... probably

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

double ML(const double *xx ) 
{
  const Double_t x1 = xx[0];
  const Double_t x2 = xx[1];
  const Double_t x3 = 1-x1-x2;
  if (x3<0) return 1E6;

  double x, weight, L=0;

  for (int i=0;i<tmpdatavector_x.size();i++) {
    x = tmpdatavector_x[i];
    weight = tmpdatavector_weight[i];
    double mc1_x = tmp1->GetBinContent(tmp1->FindBin(x));
    double mc2_x = tmp2->GetBinContent(tmp2->FindBin(x));
    double mc3_x = tmp3->GetBinContent(tmp3->FindBin(x));

    double f = weight*(x1*mc1_x+x2*mc2_x+x3*mc3_x);
    if (f<=0) continue;

    double dL = -weight*log((x1*mc1_x+x2*mc2_x+x3*mc3_x)); 
    //cout<<"x1 "<<x1<<" x2 "<<x2<<" x3 "<<x3<<" x "<<x<<" w "<<weight<<" mc1_x "<<mc1_x<<" mc2_x "<<mc2_x<<" mc3_x "<<mc3_x<<" L "<<dL<<endl;
    L+=dL;

  }

//cout<<"x1 "<<x1<<" x2 "<<x2<<" x3 "<<x3<<" L "<<L<<endl;
 return L;


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
  double variable[2] = { 0.3, 0.3};
 
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
  double xx[3];

  int steps=100;

  cout<<"WTF?"<<endl;

  for (int i=0;i<steps;i++)
    for (int j=0;j<steps;j++) {
      xx[0] = (double)i/steps; xx[1] = (double)j/steps; xx[2] = 1-xx[0]-xx[1];
      double chi2v = Chi2(xx);
      
      cout<<xx[0]<<" " <<xx[1]<<" "<<chi2v<<endl;

      if (chi2v<minChi2) {NNN++;
	minChi2 = chi2v; minx1=xx[0];minx2=xx[1];}
    }
 
  x1 = minx1; x2 = minx2; x3 = 1-(x1+x2);
  xx[0] = minx1; xx[1] = minx2; xx[2] = 1-(minx1+minx2);
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

double GetFractionsRoo(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3)
{ 
  RooRealVar x("x","x",0,8) ;

  RooRealVar nb("nb","fraction of b",0.5,0,1) ;
  RooRealVar nc("nc","fraction of c",0.5,0,1) ;
  RooRealVar nl("nl","fraction of l",0.5,0,1) ;

  RooDataHist roomc1("roomc1","roomc1",RooArgList(x),mc1);
  RooDataHist roomc2("roomc2","roomc2",RooArgList(x),mc2);
  RooDataHist roomc3("roomc3","roomc3",RooArgList(x),mc3);

  RooHistPdf pdfroomc1("pdfroomc1","pdfroomc1",x,roomc1);
  RooHistPdf pdfroomc2("pdfroomc2","pdfroomc2",x,roomc2);
  RooHistPdf pdfroomc3("pdfroomc3","pdfroomc3",x,roomc3);

  RooArgList pdfs(pdfroomc1,pdfroomc2,pdfroomc3);
  RooArgList ints(nb,nc,nl);
  
  RooAddPdf* qcdEstimateModel = new RooAddPdf("QCDEstimationPdf","QCDEstimationPdf", pdfs, ints);

  // Get Data To Fit To
  RooDataHist* roodata = new RooDataHist("roodata","roodata",RooArgList(x),data);
    //fittingSetup.getRooDataHist("eebb_PreTag","DATA", "boson_Mass_Zee_1ISO_ISOcorrected");

  // Fit Model To Data
  RooFitResult* result = qcdEstimateModel->fitTo(*roodata,
             RooFit::NumCPU(1,1),
             RooFit::Save(kTRUE),
             RooFit::Extended(kTRUE),
             RooFit::SumW2Error(kFALSE),
             RooFit::PrintLevel(-1));


  x1 = nb.getVal();
  x2 = nc.getVal();
  x3 = nl.getVal();

 // result->Print();

  return 1;
}

double GetFractionsRooUnbinned(TH1F *mc1, TH1F *mc2, TH1F*mc3, TTree *data_old, double &x1, double &x2, double &x3)
{ 

  RooRealVar x("x","x",0,8) ;
  RooRealVar weight("weight","weight",0,1) ;

  TNtuple *data = (TNtuple*)data_old;//new TNtuple("datanew","datanew","x:weight");
  //for (int i=0;i<tmpdatavector_x.size();i++)
  //  data->Fill(tmpdatavector_x[i],tmpdatavector_weight[i]);


  TH1F *tmpdata = new TH1F("tmpdata","tmpdata",50,0,8);
  data->Project("tmpdata","x","weight");

  double dtot = tmpdata->Integral();

  cout<<"bc? "<<mc1->GetBinContent(3)<<endl;

  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());


  RooRealVar nb("nb","fraction of b",0.3,0,1) ;
  RooRealVar nc("nc","fraction of c",0.3,0,1) ;
  RooRealVar nl("nl","fraction of l",0.3,0,1) ;

  RooDataHist roomc1("roomc1","roomc1",RooArgList(x),mc1);
  RooDataHist roomc2("roomc2","roomc2",RooArgList(x),mc2);
  RooDataHist roomc3("roomc3","roomc3",RooArgList(x),mc3);

  RooHistPdf pdfroomc1("pdfroomc1","pdfroomc1",x,roomc1);
  RooHistPdf pdfroomc2("pdfroomc2","pdfroomc2",x,roomc2);
  RooHistPdf pdfroomc3("pdfroomc3","pdfroomc3",x,roomc3);

  RooArgList pdfs(pdfroomc1,pdfroomc2,pdfroomc3);
  RooArgList ints(nb,nc);
  
  RooAddPdf* qcdEstimateModel = new RooAddPdf("QCDEstimationPdf","QCDEstimationPdf", pdfs, ints);

  // Get Data To Fit To
  //RooDataHist* roodata = new RooDataHist("roodata","roodata",RooArgList(x),data);
  RooDataSet dataset("dataset","dataset",data,RooArgSet(x,weight),"","weight");

  /*
  RooPlot* x1frame = x.frame() ;
  //RooDataHist* dh = dataset.binnedClone() ;
  dataset.plotOn(x1frame,RooFit::Binning(50));
  TCanvas *q = new TCanvas("qwe","qwe",600,600);
  x1frame->Draw();
  q->SaveAs("dataroo.pdf");
  */

  // Fit Model To Data
  RooFitResult* result = qcdEstimateModel->fitTo(dataset,
						 RooFit::NumCPU(1,1),
						 RooFit::Save(kTRUE),
						 //						 RooFit::Extended(kTRUE),
						 RooFit::SumW2Error(kTRUE),
						 RooFit::PrintLevel(-1));

  x1 = nb.getVal();
  x2 = nc.getVal();
  x3 = 1-x1-x2;
//.getError() ;
//  result->Print();

  return 1;
}
double GetFractionsUnbinned(TH1F *mc1, TH1F *mc2, TH1F*mc3, TTree *data, double &x1, double &x2, double &x3)
{ 

  tmp1 = (TH1F *)mc1->Clone("tmp1");
  tmp2 = (TH1F *)mc2->Clone("tmp2");
  tmp3 = (TH1F *)mc3->Clone("tmp3");
  tmpdata = new TH1F("tmpdata","tmpdata",50,0,8);
  tmpdatatree = data;//(TH1F *)data->Clone("tmpdata")

  float x, weight;
  tmpdatatree->SetBranchAddress("x",&x);
  tmpdatatree->SetBranchAddress("weight",&weight);

  tmpdatavector_x.clear(); tmpdatavector_weight.clear();
  for (int i=0;i<tmpdatatree->GetEntries();i++) {
    tmpdatatree->GetEntry(i);
    tmpdatavector_x.push_back(x);
    tmpdatavector_weight.push_back(weight);
  }



  data->Project("tmpdata","x","weight");

  tmp1->Scale(1/tmp1->Integral());
  tmp2->Scale(1/tmp2->Integral());
  tmp3->Scale(1/tmp3->Integral());
  //tmpdata->Scale(1/tmpdata->Integral());

  // Choose method upon creation between:
  // kConjugateFR, kConjugatePR, kVectorBFGS,
  // kVectorBFGS2, kSteepestDescent
  //  ROOT::Math::GSLMinimizer min( ROOT::Math::kSteepestDescent);
  ROOT::Math::GSLSimAnMinimizer min;

  min.SetMaxFunctionCalls(1000000);
  min.SetMaxIterations(100000);
  min.SetTolerance(0.001);
 
  ROOT::Math::Functor f(&ML,2); 
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
    cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): "<< Chi2(xs) << endl;

  x1 = xs[0]; x2 = xs[1]; x3 = 1-(x1+x2);
  
  return Chi2(xs);


  return 1;
}



double GetFractionsFF(TH1F *mc1, TH1F *mc2, TH1F*mc3, TH1F *data, double &x1, double &x2, double &x3, double &x1e, double &x2e, double &x3e)
{
  TObjArray *mc = new TObjArray(3);
  mc->Add(mc1);
  mc->Add(mc2);
  mc->Add(mc3);
  
  TFractionFitter* fit = new TFractionFitter(data, mc, "Q");//Q = quiet, no message                                      
  fit->Constrain(0,0.0,1.0);
  fit->Constrain(1,0.0,1.0);
  fit->Constrain(2,0.0,1.0);
  fit->Fit();
  
  fit->GetResult(0,x1,x1e);
  fit->GetResult(1,x2,x2e);
  fit->GetResult(2,x3,x3e);

  return fit->GetChisquare();// / fit->GetNDF();

}
double Fit(TH1F *mc1, TH1F *mc2, TH1F*mc3, TTree *origdata,TH1F *hx1, TH1F *hx2, TH1F *hx3, TH1F *hchi2)
{
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

  TH1F *mctmp1 = (TH1F *)mc1->Clone("mctmp1");
  TH1F *mctmp2 = (TH1F *)mc2->Clone("mctmp2");
  TH1F *mctmp3 = (TH1F *)mc3->Clone("mctmp3");
  //TH1F *datatmp = (TH1F *)origdata->Clone("datatmp");
  //  TH1F *data = (TH1F *)origdata->Clone("data");

  TH1F *datatmp = (TH1F *)mc1->Clone("datatmp");
  TH1F *datatmpconst = (TH1F *)mc1->Clone("datatmpconst");  

  origdata->Project("datatmp","x","weight"); 

  origdata->Project("datatmpconst","x","weight"); 

  mctmp1->Scale(1/mctmp1->Integral());
  mctmp2->Scale(1/mctmp2->Integral());
  mctmp3->Scale(1/mctmp3->Integral());
  //datatmp->Scale(1/datatmp->Integral());

  //cout<<"mctmp1 "<<mctmp1->Integral()<<" mctmp2 "<<mctmp2->Integral()
  //    <<"mctmp3 "<<mctmp3->Integral()<<" data "<<datatmp->Integral()<<endl;
 
  ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3); 
  //ShakeMCAndData(mc1,mc2,mc3,origdata,mctmp1,mctmp2,mctmp3,datatmp); 
  //  TransformMCAndData(mctmp1,mctmp2,mctmp3,datatmp,mctmp1,mctmp2,mctmp3,datatmp); 
 
  //cout<<mctmp1->Integral()<<" "<<mctmp2->Integral()<<" "<<mctmp3->Integral()<<" "<<datatmp->Integral()<<endl;

  TCanvas *c = new TCanvas("q","q",600,600);
  datatmp->Draw();
  mctmp1->Draw("same");
  mctmp2->Draw("same");
  mctmp3->Draw("same");
  c->SaveAs("q.pdf");


  double chi2c = 0, x1c = 0, x2c = 0, x3c = 0;
  double x1e = 0, x2e = 0, x3e = 0;
  const int Ntoy = 1000;

   // TCanvas *b = new TCanvas("b","b",600,600);
  for (int i=0;i<Ntoy;i++) {
    //    ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3); 
  //ShakeMCAndData(mc1,mc2,mc3,datatmpconst,mctmp1,mctmp2,mctmp3,datatmp); 
   // ShakeMCAndDataBoot(mc1,mc2,mc3,origdata,mctmp1,mctmp2,mctmp3,datatmp); 
   // RegenerateMCAndData(mctmp1,mctmp2,mctmp3,datatmp);
  RegenerateMCAndData(mctmp1,mctmp2,mctmp3,(TNtuple *)origdata);
   //  RegenerateMC(mctmp1,mctmp2,mctmp3);datatmp->Scale(1/datatmp->Integral());
    //    mctmp2->Draw("same");
    //    b->Update();
  //  TransformMCAndData(mctmp1,mctmp2,mctmp3,datatmp,mctmp1,mctmp2,mctmp3,datatmp); 
    //    cout<<"qwerty"<<mctmp1->Integral()<<" "<<mctmp2->Integral()<<" "<<mctmp3->Integral()<<" "<<datatmp->Integral()<<endl;
    //    ShakeMCAndData(mc1,mc2,mc3,origdata,mctmp1,mctmp2,mctmp3,datatmp); 
    //    ShakeMC(mc1,mc2,mc3,mctmp1,mctmp2,mctmp3); 

    double v1,v2,v3,chi2;
    //chi2 = GetFractions(mctmp1,mctmp2,mctmp3,datatmp,v1,v2,v3);
    //chi2 = GetFractions2(mctmp1,mctmp2,mctmp3,datatmp,v1,v2,v3);
    //chi2 = GetFractionsMin(mctmp1,mctmp2,mctmp3,datatmp,v1,v2,v3);
    //chi2 = GetFractionsRoo(mctmp1,mctmp2,mctmp3,datatmp,v1,v2,v3);
    //chi2 = GetFractionsMinBrute(mctmp1,mctmp2,mctmp3,datatmp,v1,v2,v3);
    //chi2 = GetFractionsFF(mctmp1,mctmp2,mctmp3,datatmp,v1,v2,v3,x1e,x2e,x3e);
    chi2 = GetFractionsRooUnbinned(mctmp1,mctmp2,mctmp3,origdata,v1,v2,v3);
    //chi2 = GetFractionsUnbinned(mctmp1,mctmp2,mctmp3,origdata,v1,v2,v3);

    //        cout<<"qwerty"<<v1<<" "<<v2<<" "<<v3<<endl;

    if (v1<0.001 || v1>0.999 || v2<0.001 || v2>0.999 || v3<0.001 || v3>0.999) continue;

    chi2c+=chi2; x1c+=v1; x2c+=v2; x3c+=v3;
    hx1->Fill(v1); hx2->Fill(v2); hx3->Fill(v3); hchi2->Fill(chi2);
  }
  chi2c/=Ntoy;x1c/=Ntoy;x2c/=Ntoy;x3c/=Ntoy;

  cout<<"Average Fit Chi2 = "<<chi2c<<endl;

  cout<<"Average x1 = "<<x1c<<endl;
  cout<<"Average x2 = "<<x2c<<endl;
  cout<<"Average x3 = "<<x3c<<endl;

    double xx[] ={x1c,x2c,x3c};
  //  cout<<"Chi2 of average fit = "<<Chi2(xx)<<endl;


  return chi2c;

}

