#define ROOFITTER

double GetIntegral(TH1 *h, bool full = false)
{
  TH1F *h1 = dynamic_cast<TH1F*>(h);
  //  return h1->Integral();
  if (h1!=0) return full ? h1->Integral(0,h->GetNbinsX()+1) : h->Integral();
  TH2F *h2 = dynamic_cast<TH2F*>(h);
  if (h2!=0) return full ? h2->Integral(0,h->GetNbinsX()+1,0,h->GetNbinsY()+1) : h->Integral();

  return -1;
}

double MCBfraction, MCCfraction;
double DataBFraction, DataBFractionError;
double DataCFraction, DataCFractionError;

double mcBpur, mcCpur, mctagBEff, mctagCEff;



void DoFit(double pTmin, double pTmax)
{
  cout<<"Perform fit in pT bin : ["<<pTmin<<","<<pTmax<<"]"<<endl;

  const char *datafilename = "jettrig_weight_AccCut_muons_flat.root";//"jettrig_weight_AccCut.root");//"jettrig_weight_AccCut_muons.root");//"jettrig_weight.root");

  TFile *f = new TFile("QCDPPb_genmuonmerged_flat.root"); //"QCDPPb_AccCut.root");//"QCDPPb_genmuonmerged.root");
  TTree *t = (TTree *)f->Get("jet");

  float xmin =0, xmax =6;

  float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;
  //float legendx1 = 0.16, legendy1 = 0.65, legendx2 = 0.34, legendy2 = 0.84;

  TString muset = "jetmuon";//"mu"
  float discrmax = 6;


  //  const char *discrname = "discr_ssvHighPur";
  //  float discrvalue = -100;//1.68;

  //const char *discrname = "discr_ssvHighEff";
  //float discrvalue = 2;

  const char *discrname = "jetmuonpt";
  float discrvalue = 7;
    
  //  const char *discrname = "discr_csvSimple";
  //  float discrvalue = -10;


  const char *discr = Form("%s>%f",discrname,discrvalue);
  const char *discrData = Form("%s>%f",discrname,discrvalue);
  
  int bins = 12;//16;
  bool logplots = false;

  TString variable = "ptrel";
    //"ip3d";
    //"jp";
    //"svtxm";

  TString sVar, sTitle, sPhysicsCut;

  if (variable=="ptrel") {
    sVar =  Form("%sptrel",muset.Data());
    sTitle = "p_{T}^{rel} [GeV/c]";
    sPhysicsCut = Form("jtpt>%f && jtpt<%f",pTmin, pTmax);//Form("jtpt>60 && jtpt<100 && %spt>7",muset.Data());
    xmin = 0; xmax = 4;
    discrmax = 20;
    }
  else if (variable=="ip3d") {
    sVar = "logmuip3d";//Form("log(%sip3d)",muset.Data());
    sTitle = "log(3d IP)";
    sPhysicsCut =  Form("jtpt>%f && jtpt<%f",pTmin, pTmax);//Form("jtpt>60 && jtpt<100 && %spt>7",muset.Data());
    discrmax = 20;
    xmin = -10; xmax = 0;
  } else if (variable=="jp") {
    sVar = "discr_prob";
    sTitle = "JP";
    xmin=0; xmax=3;
    sPhysicsCut =  Form("jtpt>%f && jtpt<%f",pTmin, pTmax);
    bins=16;
    logplots = true;
  } else if (variable=="svtxm") {
    sVar = "svtxm";
    sTitle = "Secondary vertex mass";
    sPhysicsCut =  Form("jtpt>%f && jtpt<%f",pTmin, pTmax);
    bins=12;
    logplots = true;
  }

  const char * var = sVar.Data();
  const char * title = sTitle.Data();
  const char * physicscut = sPhysicsCut.Data();


  TH1F *h_l   = new TH1F("h_l",Form("h_l;%s",title),bins, xmin, xmax);
  TH1F *h_c   = new TH1F("h_c",Form("h_c;%s",title),bins, xmin, xmax);
  TH1F *h_b   = new TH1F("h_b",Form("h_b;%s",title),bins, xmin, xmax);
  TH1F *h_all = new TH1F("h_all",Form("h_all;%s",title),bins, xmin, xmax);
  TH1F *h_mctotal = new TH1F("h_mctotal",Form("h_mctotal;%s",title),bins, xmin, xmax);
  TH1F *h_mctotalb = new TH1F("h_mctotalb",Form("h_mctotalb;%s",title),bins, xmin, xmax);
  TH1F *h_mctotalc = new TH1F("h_mctotalc",Form("h_mctotalc;%s",title),bins, xmin, xmax);


  cout<<"MC jet content..."<<endl;
  
  t->Project("h_l", var,  Form("weight*(%s && abs(refparton_flavorForB)!=5 && abs(refparton_flavorForB)!=4 && %s)",physicscut,discr));
  t->Project("h_c", var,  Form("weight*(%s && abs(refparton_flavorForB)==4 && %s)",physicscut,discr));
  t->Project("h_b",  var, Form("weight*(%s && abs(refparton_flavorForB)==5 && %s)",physicscut,discr));
  t->Project("h_all",var, Form("weight*(%s && %s)",physicscut,discr));

  t->Project("h_mctotal",var, Form("weight*(%s)",physicscut));
  t->Project("h_mctotalb",var, Form("weight*(%s && abs(refparton_flavorForB)==5)",physicscut));
  t->Project("h_mctotalc",var, Form("weight*(%s && abs(refparton_flavorForB)==4)",physicscut));

  h_l->SetFillColor(kblue); h_c->SetFillColor(kgreen); h_b->SetFillColor(kred);
  h_l->SetFillStyle(1001); h_c->SetFillStyle(1001); h_b->SetFillStyle(1001);
  h_all->SetLineColor(kBlack);


  double mctaggedb = GetIntegral(h_b,true);
  double mctaggedc = GetIntegral(h_c,true);
  double mctaggedl = GetIntegral(h_l,true);
  double mctaggedall = GetIntegral(h_all,true);



  double mctotal = GetIntegral(h_mctotal,true);
  double mctotalb = GetIntegral(h_mctotalb,true);
  double mctotalc = GetIntegral(h_mctotalc,true);

  cout<<"mctotalb "<<mctotalb<<" mctotal "<<mctotal<<endl;

  mcBfraction = mctotalb/mctotal;
  mcCfraction = mctotalc/mctotal;

  mctagBEff = mctaggedb/mctotalb;
  mctagCEff = mctaggedc/mctotalc;

  double mctagEff = mctaggedall/mctotal;



  //  h_l->Smooth();
  //  h_c->Smooth();
  //  h_b->Smooth();
  //  h_all->Smooth();



  THStack *hs =new THStack("hs",Form(";%s",title));
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
  THStack *tstemplate = new THStack("tstemplate",Form(";%s",title));
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




  cout<<"Data jet all and tagged..."<<endl;

  TFile *fd = new TFile(datafilename);
  TTree *td = (TTree *)fd->Get("jet");

  //in the data...
  //  muset = "mu";//"jetmuon"
  //physicscut = "jtpt>60 && jtpt<100";//Form("jtpt>60 && jtpt<100 %spt>5",muset.Data());

  TH1F *hd_all = new TH1F("hd_all",Form("hd_all;%s",title),bins, xmin, xmax);
  TH1F *hd_tag = new TH1F("hd_tag",Form("hd_tag;%s",title),bins, xmin, xmax);
  TH1F *hd_untag = new TH1F("hd_untag",Form("hd_untag;%s",title),bins, xmin, xmax);

  td->Project("hd_all",var,Form("weightJet*(%s)",physicscut));
  td->Project("hd_tag",var,Form("weightJet*(%s && %s)",physicscut,discrData));
  td->Project("hd_untag",var,Form("weightJet*(%s && !(%s))",physicscut,discrData));
  hd_all->SetMarkerColor(kblue);
  hd_untag->SetMarkerColor(kgreen);

  double datatagged = GetIntegral(hd_tag,true);
  double dataall = GetIntegral(hd_all,true);

  cout<<"?"<<GetIntegral(hd_tag)<<endl;


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

  cout<<"Fitting data jet content on tagged..."<<endl;

  //Try the fit

  TH1F *hmc_b = (TH1F*)h_b->Clone("hmc_b");
  TH1F *hmc_c = (TH1F*)h_c->Clone("hmc_c");
  TH1F *hmc_l = (TH1F*)h_l->Clone("hmc_l");

  TH1F *mc1 = hmc_b;
  TH1F *mc2 = hmc_c;
  TH1F *mc3 = hmc_l;


  //TNtuple *data = (TNtuple*)data_old;//new TNtuple("datanew","datanew","x:weight");
  //for (int i=0;i<tmpdatavector_x.size();i++)
  //  data->Fill(tmpdatavector_x[i],tmpdatavector_weight[i]);

     
  mc1->Scale(1/mc1->Integral());
  mc2->Scale(1/mc2->Integral());
  mc3->Scale(1/mc3->Integral());

  
  #ifdef ROOFITTER
  
  cout<<"Roofit is used"<<endl;

  double x1, x1e, x2, x2e, x3, x3e;

  RooRealVar x(var,var,xmin,xmax) ;
  RooRealVar jtpt("jtpt","jtpt", pTmin, pTmax);
  //  RooRealVar discr_ssvHighPur("discr_ssvHighPur","discr_ssvHighPur", -10, 6);
  RooRealVar roodiscr(discrname,discrname,discrvalue,discrmax);
  RooRealVar weight("weight","weight",0,1) ;
  RooRealVar weightJet("weightJet","weightJet",0,200) ;



  RooRealVar nb("nb","fraction of b",0.65,0,1);
  RooRealVar nc("nc","fraction of c",0.27,0,1);
  RooRealVar nl("nl","fraction of l",0.08,0,1);

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
  RooDataSet dataset("dataset","dataset",td,RooArgSet(x,weightJet,roodiscr,jtpt),Form("%s && %s",physicscut,discrData),"weightJet");

  // Fit Model To Data
  RooFitResult* result = qcdEstimateModel->fitTo(dataset,
             RooFit::NumCPU(1,1),
             RooFit::Save(kTRUE),
             //            RooFit::Extended(kTRUE),
             RooFit::SumW2Error(kTRUE),
             RooFit::PrintLevel(-1));

  x1 = nb.getVal();
  x2 = nc.getVal();
  x3 = 1-x1-x2;

  x1e = nb.getError();
  x2e = nc.getError();
  x3e = x1e + x2e;

  result->Print();
  
  
  #else



  cout<<"TFractionFitter is used!!!"<<endl;
  

   
  TObjArray *mc = new TObjArray(3);
  mc->Add(hmc_b);
  mc->Add(hmc_c);
  mc->Add(hmc_l);

  TH1F *prediction;
  double x1, x1e, x2, x2e, x3, x3e;


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
  
  TH1F *hx1 = new TH1F("hx1","hx1",100,0,1);
  TH1F *hx2 = new TH1F("hx2","hx2",100,0,1);
  TH1F *hx3 = new TH1F("hx3","hx3",100,0,1);
  TH1F *hchi2 = new TH1F("hchi2","hchi2",100,0,20);


  //  double chi2 = FitDataTree(hmc_b,hmc_c,hmc_l,td,var, Form("(%s && %s)",physicscut,discrData),pTmin,pTmax,"weight",x1,x2,x3,x1e,x2e,x3e);

  //  cout<<"Fit chi2 = "<<chi2<<endl;

//  x1 = hx1->GetMean(); x1e = hx1->GetStdDev();
//  x2 = hx2->GetMean(); x2e = hx2->GetStdDev();
//  x3 = hx3->GetMean(); x3e = hx3->GetStdDev();

  prediction = (TH1F *)hmc_b->Clone("prediction");
  prediction->Clear();
  prediction->Add(hmc_b,x1);
  prediction->Add(hmc_c,x2);
  prediction->Add(hmc_l,x3);
  



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

  TCanvas *ccomp = new TCanvas("ccomp","ccomp",600,600);
  TH1F *pred_b = (TH1F *)hmc_b->Clone("pred_b");//(TH1F *)fit->GetMCPrediction(0);
  TH1F *hmc_bclone = (TH1F *)hmc_b->Clone("hmc_bclone");
  pred_b->Scale(1/GetIntegral(pred_b)*num_b);

  pred_b->SetLineColor(kBlack); pred_b->SetFillStyle(0); pred_b->SetLineWidth(3);
  hmc_bclone->SetLineColor(kRed); hmc_bclone->SetFillStyle(0); hmc_bclone->SetLineWidth(3);
  pred_b->Draw("hist");
  hmc_bclone->Draw("hist,same");

  TLegend *lcomp = new TLegend(legendx1, legendy1, legendx2, legendy2);// l->SetFillStyle(0);
  lcomp->AddEntry(hmc_bclone,"MC b template","l");
  lcomp->AddEntry(pred_b,"Prediction b template","l");
  lcomp->Draw();
  ccomp->SaveAs("btemplatecompare.pdf");


  TH1F *hmc_total = (TH1F *)hmc_b->Clone("hmc_total");
  hmc_total->Add(hmc_c);
  hmc_total->Add(hmc_l);
  hmc_total->SetLineColor(7);
  hmc_total->SetLineWidth(4);
  hmc_total->SetMarkerStyle(1);

  THStack *hsf =new THStack("hsf",Form(";%s",title));

  hsf->Add(hmc_b,"hist");
  hsf->Add(hmc_c,"hist");
  hsf->Add(hmc_l,"hist");
  



  //prediction->Integral() == hd_tag->Integral() so it's basically the same thing
  h_all->Scale(GetIntegral(prediction)/GetIntegral(h_all));
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

  cf->SaveAs(Form("FitTagged_%d_%d.pdf",(int)pTmin, (int)pTmax));
  cf->Update();


  cout<<setprecision(2);

  MCBfraction = mcBfraction;
  MCCfraction = mcCfraction;
  
  mcBpur = mctaggedb/mctaggedall;
  mcCpur = mctaggedc/mctaggedall;


  cout<<"MC : "<<endl;
  
  cout<<"Fraction of b-jets : "<<mctaggedb/mctaggedall*100<<"%"<<endl;
  cout<<"Fraction of c-jets : "<<mctaggedc/mctaggedall*100<<"%"<<endl;
  cout<<"Fraction of l-jets : "<<mctaggedl/mctaggedall*100<<"%"<<endl;

  cout<<(mctaggedb+mctaggedc+mctaggedl)/mctaggedall<<endl;



  double tagPurityB = x1;
  double tagPurityC = x2;

  DataBFraction = datatagged*tagPurityB/mctagBEff/dataall;
  DataCFraction = datatagged*tagPurityC/mctagCEff/dataall;

  DataBFractionError = datatagged*x1e/mctagBEff/dataall;
  DataCFractionError = datatagged*x2e/mctagCEff/dataall;

  cout<<"datatagged:"<<datatagged<<endl;
  cout<<"dataall:"<<dataall<<endl;

  cout<<"MC b-tagging efficiency : "<<mctagBEff<<endl;
  cout<<"MC c-tagging efficiency : "<<mctagCEff<<endl;
  cout<<"MC tagging efficiency   : "<<mctagEff<<endl;

  std::cout.unsetf ( std::ios::floatfield ); 

  cout <<"b-jet fraction in MC: "<<mcBfraction<<endl;
  cout <<"c-jet fraction in MC: "<<mcCfraction<<endl;

  cout <<"b-jet fraction in data: "<<DataBFraction<<" ± "<<DataBFractionError<<endl;
  cout <<"c-jet fraction in data: "<<DataCFraction<<" ± "<<DataCFractionError<<endl;



}

void MyFitting_flat()
{
  vector<double> pTbins = {60,80,120,170,250,400};
  //  vector<double> pTbins = {70,90,110,140,170,220,400};
  int Nptbins = pTbins.size();
  vector<double> pt, pterrors;

  vector<double> dataB, dataBerror, dataC, dataCerror, mcB, mcBerror, mcC, mcCerror;
  vector<double> mcBpurs, mcCpurs, mctagBEffs, mctagCEffs;
  
  //TH1F *MCtaggingB = new TH1F("MCB","MCB",Nptbins-1,pTbins); MCtaggingB->SetMarkerColor(kRed);
  //TH1F *DatataggingB = new TH1F("DataB","DataB",Nptbins-1,pTbins); DatataggingB->SetMarkerColor(kBlack); DatataggingB->SetFillColor(kYellow);
  //DatataggingB->SetFillStyle(1001);
  //MCtaggingB->SetMinimum(0);  MCtaggingB->SetMaximum(0.2);

  //TH1F *MCtaggingC = new TH1F("MCC","MCC",Nptbins-1,pTbins);MCtaggingC->SetMarkerColor(kBlue);
  //TH1F *DatataggingC = new TH1F("DataC","DataC",Nptbins-1,pTbins);DatataggingC->SetMarkerColor(kBlack);DatataggingC->SetFillColor(kYellow);
  //DatataggingC->SetFillStyle(1001);
  //MCtaggingC->SetMinimum(0);  MCtaggingC->SetMaximum(0.2);

  //  for (int i=0;i<Nptbins-1;i++) 
  {
        int i=1;
    DoFit(pTbins[i],pTbins[i+1]);
    pt.push_back((pTbins[i+1]+pTbins[i])/2);
    pterrors.push_back((pTbins[i+1]-pTbins[i])/2);
    //MCBfraction = 0.1;DataBFraction = 0.11; DataBFractionError=0.05;
    //MCCfraction = 0.1;DataCFraction = 0.11; DataCFractionError=gRandom->Uniform(0.01,0.08);
    dataB.push_back(DataBFraction); dataBerror.push_back(DataBFractionError);
    dataC.push_back(DataCFraction); dataCerror.push_back(DataCFractionError);
    mcB.push_back(MCBfraction);mcBerror.push_back(0.0001);
    mcC.push_back(MCCfraction);mcCerror.push_back(0.0001);

    mcBpurs.push_back(mcBpur);
    mcCpurs.push_back(mcCpur);
    mctagBEffs.push_back(mctagBEff);
    mctagCEffs.push_back(mctagCEff);


  }


  TGraphErrors* gb =   new TGraphErrors(pt.size(), &pt[0], &dataB[0], &pterrors[0], &dataBerror[0]); gb->SetMarkerColor(kBlack); //gb->SetFillColor(kYellow); gb->SetFillStyle(3001);
  TGraphErrors* gc =   new TGraphErrors(pt.size(), &pt[0], &dataC[0], &pterrors[0], &dataCerror[0]); gc->SetMarkerColor(kBlack); //gc->SetFillColor(kYellow); gc->SetFillStyle(3001);
  TGraphErrors* gmcb = new TGraphErrors(pt.size(), &pt[0], &mcB[0], &pterrors[0], &mcBerror[0]); gmcb->SetMarkerColor(kBlue);
  TGraphErrors* gmcc = new TGraphErrors(pt.size(), &pt[0], &mcC[0], &pterrors[0], &mcCerror[0]); gmcc->SetMarkerColor(kBlue);
  gmcb->SetLineColor(kBlue);gmcb->SetLineWidth(3);
  gmcc->SetLineColor(kBlue);gmcc->SetLineWidth(3);


  gb->GetXaxis()->SetRangeUser(0,400);
  gc->GetXaxis()->SetRangeUser(0,400);
  gmcb->GetXaxis()->SetRangeUser(0,400);
  gmcc->GetXaxis()->SetRangeUser(0,400);

  gb->GetYaxis()->SetRangeUser(0,0.09);
  gc->GetYaxis()->SetRangeUser(0,0.2);
  gmcb->GetYaxis()->SetRangeUser(0,0.09);
  gmcc->GetYaxis()->SetRangeUser(0,0.2);




  TCanvas *c1 = new TCanvas("resultB","resultB",600,600);
  gb->Draw("ap");//("a2p");//gb->Draw("pX");
  gmcb->Draw("p");


  c1->SaveAs("MCvsDataB.pdf");

  TCanvas *c2 = new TCanvas("resultC","resultC",600,600);
  gc->Draw("ap");//"a2p" gc->Draw("pX");
  gmcc->Draw("p");

  c2->SaveAs("MCvsDataC.pdf");
  
  
  TGraph *gmcBpurs = new TGraph(mcBpurs.size(),&pt[0],&mcBpurs[0]); gmcBpurs->SetMarkerColor(kRed);
  TGraph *gmcCpurs = new TGraph(mcCpurs.size(),&pt[0],&mcCpurs[0]); gmcCpurs->SetMarkerColor(kBlue);
  TGraph *gmctagBEffs = new TGraph(mctagBEffs.size(),&pt[0],&mctagBEffs[0]); 
  gmctagBEffs->SetMarkerColor(kRed);
  TGraph *gmctagCEffs = new TGraph(mctagCEffs.size(),&pt[0],&mctagCEffs[0]); 
  gmctagCEffs->SetMarkerColor(kBlue);
  
  
  gmcBpurs->GetHistogram()->SetMinimum(0.);  gmcBpurs->GetHistogram()->SetMaximum(1.);
  gmcCpurs->GetHistogram()->SetMinimum(0.);  gmcCpurs->GetHistogram()->SetMaximum(1.);
  gmctagBEffs->GetHistogram()->SetMinimum(0.);  gmctagBEffs->GetHistogram()->SetMaximum(1.);
  gmctagCEffs->GetHistogram()->SetMinimum(0.);  gmctagCEffs->GetHistogram()->SetMaximum(1.);

  /*  gmcBpurs->GetXaxis()->SetLimits(0,1);
  gmcCpurs->GetXaxis()->SetLimits(0,1);
  gmctagBEffs->GetXaxis()->SetLimits(0,1);
  gmctagBEffs->GetXaxis()->SetLimits(0,1);
  */

  TMultiGraph *mg1 = new TMultiGraph(); mg1->Add(gmcBpurs); mg1->Add(gmcCpurs);
  TMultiGraph *mg2 = new TMultiGraph(); mg2->Add(gmctagBEffs); mg2->Add(gmctagCEffs);

  TCanvas *c3 = new TCanvas("purities","purities",600,600);
  mg1->Draw("AP");

  TCanvas *c4 = new TCanvas("efficiencies","efficiencies",600,600);
  mg2->Draw("AP");

  c3->Modified(); c3->Update();
  c4->Modified(); c4->Update();

  mg1->GetXaxis()->SetRangeUser(0,400);
  mg2->GetXaxis()->SetRangeUser(0,400);
  mg1->GetYaxis()->SetRangeUser(0,1);
  mg2->GetYaxis()->SetRangeUser(0,1);

  c3->Modified(); c3->Update();
  c4->Modified(); c4->Update();


}
