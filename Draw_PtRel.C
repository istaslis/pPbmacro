double GetIntegral(TH1F *h, bool full = false)
{
  return full ? h->Integral(0,h->GetNbinsX()+1) : h->Integral();
}

void Draw_PtRel()
{
  const char *datafilename = "jettrig_weight_AccCut_muons.root";//"jettrig_weight_AccCut.root");//"jettrig_weight_AccCut_muons.root");//"jettrig_weight.root");

  TFile *f = new TFile("QCDPPb_genmuonmerged.root"); //"QCDPPb_AccCut.root");//"QCDPPb_genmuonmerged.root");
  TTree *t = (TTree *)f->Get("jet");

  float xmin =0, xmax =3;

  float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;
  //float legendx1 = 0.16, legendy1 = 0.65, legendx2 = 0.34, legendy2 = 0.84;

  TString muset = "jetmuon";//"mu"


  const char *discr = "jetmuonpt>7";
  const char *discrData = "jetmuonpt>7";

  //  const char *discr = "discr_csvSimple>0.5";
  //  const char *discrData = "discr_csvSimple>0.5";

  //  const char *discr = "discr_ssvHighPur>1.68";
  //  const char *discrData = "discr_ssvHighPur>1.68";
  
  int bins = 10;//16;
  bool logplots = false;

  TString variable = "ptrel";
                      //"ip3d";
                     //"jp";

  TString sVar, sTitle, sPhysicsCut;

  if (variable=="ptrel") {
    sVar =  Form("%sptrel",muset.Data());
    sTitle = "p_{T}^{rel} [GeV/c]";
    sPhysicsCut = "jtpt>60 && jtpt<100";//Form("jtpt>60 && jtpt<100 && %spt>7",muset.Data());
    xmin = 0; xmax = 4;
    }
  else if (variable=="ip3d") {
    sVar = Form("log(%sip3d)",muset.Data());
    sTitle = "log(3d IP)";
    sPhysicsCut = "jtpt>60 && jtpt<100";//Form("jtpt>60 && jtpt<100 && %spt>7",muset.Data());
    xmin = -10; xmax = 0;
  } else if (variable=="jp") {
    sVar = "discr_prob";
    sTitle = "JP";
    sPhysicsCut = "jtpt>60 && jtpt<100";
    bins=16;
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
  
  t->Project("h_l", var,  Form("100000*weight*(%s && abs(refparton_flavorForB)!=5 && abs(refparton_flavorForB)!=4 && %s)",physicscut,discr));
  t->Project("h_c", var,  Form("100000*weight*(%s && abs(refparton_flavorForB)==4 && %s)",physicscut,discr));
  t->Project("h_b",  var, Form("100000*weight*(%s && abs(refparton_flavorForB)==5 && %s)",physicscut,discr));
  t->Project("h_all",var, Form("100000*weight*(%s && %s)",physicscut,discr));

  t->Project("h_mctotal",var, Form("100000*weight*(%s)",physicscut));
  t->Project("h_mctotalb",var, Form("100000*weight*(%s && abs(refparton_flavorForB)==5)",physicscut));
  t->Project("h_mctotalc",var, Form("100000*weight*(%s && abs(refparton_flavorForB)==4)",physicscut));

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

  double mcBfraction = mctotalb/mctotal;
  double mcCfraction = mctotalc/mctotal;

  double mctagBEff = mctaggedb/mctotalb;
  double mctagCEff = mctaggedc/mctotalc;





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


  TObjArray *mc = new TObjArray(3);
  mc->Add(hmc_b);
  mc->Add(hmc_c);
  mc->Add(hmc_l);

  TFractionFitter* fit = new TFractionFitter(hd_tag, mc);
  fit->Constrain(0,0.0,1.0);  
  fit->Constrain(1,0.0,1.0);
  fit->Constrain(2,0.0,1.0);
  fit->Fit();
  TH1F* prediction = (TH1F*) fit->GetPlot();
  


  cout<<fit->GetProb()<<endl;

  double x1, x1e, x2, x2e, x3, x3e;
  fit->GetResult(0,x1,x1e);
  fit->GetResult(1,x2,x2e);
  fit->GetResult(2,x3,x3e);

  //  cout<<fit->GetResult(0,x1,x1e)<<endl;

  double total = GetIntegral(prediction);
  double num_b = total*x1, num_be = total*x1e;
  double num_c = total*x2, num_ce = total*x2e;
  double num_l = total*x3, num_le = total*x3e;

  hmc_b->Scale(1/GetIntegral(hmc_b)*num_b);
  hmc_c->Scale(1/GetIntegral(hmc_c)*num_c);
  hmc_l->Scale(1/GetIntegral(hmc_l)*num_l);

  TCanvas *ccomp = new TCanvas("ccomp","ccomp",600,600);
  TH1F *pred_b = (TH1F *)fit->GetMCPrediction(0);
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

  cf->SaveAs("FitTagged.pdf");
  cf->Update();


  cout<<setprecision(2);

  cout<<"MC : "<<endl;
  
  cout<<"Fraction of b-jets : "<<mctaggedb/mctaggedall*100<<"%"<<endl;
  cout<<"Fraction of c-jets : "<<mctaggedc/mctaggedall*100<<"%"<<endl;
  cout<<"Fraction of l-jets : "<<mctaggedl/mctaggedall*100<<"%"<<endl;

  cout<<(mctaggedb+mctaggedc+mctaggedl)/mctaggedall<<endl;


  cout<<"Data : "<<endl;

  cout<<"Number of b-jets  : "<<num_b<<" ± "<<num_be<<endl;
  cout<<"Number of c-jets : "<<num_c<<" ± "<<num_ce<<endl;
  cout<<"Number of l-jets : "<<num_l<<" ± "<<num_le<<endl;

  //  cout<<setprecision(2);

  cout<<"Fraction of b-jets : "<<x1*100<<"% ± "<<x1e*100<<endl;
  cout<<"Fraction of c-jets : "<<x2*100<<"% ± "<<x2e*100<<endl;
  cout<<"Fraction of l-jets : "<<x3*100<<"% ± "<<x3e*100<<endl;

  cout<<"Fit quality : "<<fit->GetChisquare() / fit->GetNDF()<<endl;


  double tagPurityB = x1;
  double tagPurityC = x2;

  double bjetfraction = datatagged*tagPurityB/mctagBEff/dataall;
  double cjetfraction = datatagged*tagPurityC/mctagCEff/dataall;

  cout<<"datatagged:"<<datatagged<<endl;
  cout<<"dataall:"<<dataall<<endl;

  cout<<"MC b-tagging efficiency : "<<mctagBEff<<endl;
  cout<<"MC c-tagging efficiency : "<<mctagCEff<<endl;

  std::cout.unsetf ( std::ios::floatfield ); 

  cout <<"b-jet fraction in MC: "<<mcBfraction<<endl;
  cout <<"c-jet fraction in MC: "<<mcCfraction<<endl;

  cout <<"b-jet fraction in data: "<<bjetfraction<<endl;
  cout <<"c-jet fraction in data: "<<cjetfraction<<endl;



}
