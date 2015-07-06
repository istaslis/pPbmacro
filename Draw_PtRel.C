
void FitB(TH1F *mc_b, TH1F *mc_cl, TH1F *data)
{

}

void Draw_PtRel()
{
  TFile *f = new TFile("QCDPPb_genmuonmerged.root");
  TTree *t = (TTree *)f->Get("jet");

  int bins = 50;
  float xmin = 0;
  float xmax = 5;

  TH1F *h_l   = new TH1F("h_l","h_l;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);
  TH1F *h_c   = new TH1F("h_c","h_c;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);
  TH1F *h_b   = new TH1F("h_b","h_b;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);
  TH1F *h_all = new TH1F("h_all","h_all;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);

//mu - discr 0.2

  float discr = 0.1;

  TString muset = "jetmuon";//"mu"
  char * physicscut = Form("jtpt>60 && %spt>5",muset.Data());// && mupt>5

  cout<<"MC jet content..."<<endl;
  
  t->Project("h_l", Form("%sptrel",muset.Data()),  Form("weight*(%sptrel<5 && %sptrel>0 && %s && abs(refparton_flavorForB)!=5 && abs(refparton_flavorForB)!=4 && discr_csvSimple>%f)",muset.Data(),muset.Data(),physicscut,discr));
  t->Project("h_c", Form("%sptrel",muset.Data()),  Form("weight*(%sptrel<5 && %sptrel>0 && %s && abs(refparton_flavorForB)==4 && discr_csvSimple>%f)",muset.Data(),muset.Data(),physicscut,discr));
  t->Project("h_b",  Form("%sptrel",muset.Data()), Form("weight*(%sptrel<5 && %sptrel>0 && %s && abs(refparton_flavorForB)==5 && discr_csvSimple>%f)",muset.Data(),muset.Data(),physicscut,discr));
  t->Project("h_all",Form("%sptrel",muset.Data()), Form("weight*(%sptrel<5 && %sptrel>0 && %s && discr_csvSimple>%f)",muset.Data(),muset.Data(),physicscut,discr));
  h_l->SetFillColor(kblue); h_c->SetFillColor(kgreen); h_b->SetFillColor(kred);
  h_l->SetFillStyle(1001); h_c->SetFillStyle(1001); h_b->SetFillStyle(1001);
  h_all->SetLineColor(kBlack);



  THStack *hs =new THStack("hs",";p_{T}^{rel} [GeV/c]");
  hs->Add(h_b,"hist");
  hs->Add(h_c,"hist");
  hs->Add(h_l,"hist");
  
  TCanvas *c = new TCanvas("c","c",600,600);
  hs->Draw();
  h_all->Draw("same");

  TLegend *l = new TLegend(0.65,0.65,0.84,0.84);// l->SetFillStyle(0);
  l->AddEntry(h_b,"B","f");
  l->AddEntry(h_c,"C","f");
  l->AddEntry(h_l,"L","f");
  l->AddEntry(h_all,"all","pl");
  l->Draw();

  c->SaveAs("MCjetContentTagged.pdf");
  c->Update();

  TH1F *hmctemplate_b = (TH1F*)h_b->Clone("hmctemplate_b");
  TH1F *hmctemplate_c = (TH1F*)h_c->Clone("hmctemplate_c");
  TH1F *hmctemplate_l = (TH1F*)h_l->Clone("hmctemplate_l");
  hmctemplate_l->SetLineColor(kblue); hmctemplate_c->SetLineColor(kgreen); hmctemplate_b->SetLineColor(kred);
  hmctemplate_l->SetFillStyle(0); hmctemplate_c->SetFillStyle(0); hmctemplate_b->SetFillStyle(0);
  hmctemplate_l->SetLineWidth(3); hmctemplate_c->SetLineWidth(3); hmctemplate_b->SetLineWidth(3);

  hmctemplate_b->Scale(1/hmctemplate_b->Integral());
  hmctemplate_c->Scale(1/hmctemplate_c->Integral());
  hmctemplate_l->Scale(1/hmctemplate_l->Integral());

  TCanvas *ctempl = new TCanvas("ctempl","ctempl",600,600);
  //hmctemplate_l->Draw("hist");
  //hmctemplate_c->Draw("hist,same"); 
  hmctemplate_b->Draw("hist,same");

  TLegend *ltempl = new TLegend(0.65,0.65,0.84,0.84);
  ltempl->AddEntry(hmctemplate_b,"B","l");
  ltempl->AddEntry(hmctemplate_c,"C","l");
  ltempl->AddEntry(hmctemplate_l,"L","l");
  ltempl->Draw();

  ctempl->SaveAs("MCTemplates.pdf");



  cout<<"Data jet all and tagged..."<<endl;

  TFile *fd = new TFile("jettrig_weight.root");
  TTree *td = (TTree *)fd->Get("nt");

  //in the data...
  muset = "mu";//"jetmuon"
  physicscut = Form("jtpt>60 && %spt>5",muset.Data());

  TH1F *hd_all = new TH1F("hd_all","hd_all;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);
  TH1F *hd_tag = new TH1F("hd_tag","hd_tag;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);
  TH1F *hd_untag = new TH1F("hd_untag","hd_untag;p_{T}^{rel} [GeV/c]",bins, xmin, xmax);

  td->Project("hd_all",Form("%sptrel",muset.Data()),Form("weightJet*(%sptrel>0 && %sptrel<5 && %s)",muset.Data(),muset.Data(),physicscut));
  td->Project("hd_tag",Form("%sptrel",muset.Data()),Form("weightJet*(%sptrel>0 && %sptrel<5 && %s && discr_csvSimple>%f)",muset.Data(),muset.Data(),physicscut,discr));
  td->Project("hd_untag",Form("%sptrel",muset.Data()),Form("weightJet*(%sptrel>0 && %sptrel<5 && %s && discr_csvSimple<%f)",muset.Data(),muset.Data(),physicscut,discr));
  hd_all->SetMarkerColor(kblue);
  hd_untag->SetMarkerColor(kgreen);

  TCanvas *cd = new TCanvas("cd","cd",600,600);

  hd_all->Draw();
  hd_tag->Draw("same");
  hd_untag->Draw("same");

  TLegend *ld = new TLegend(0.65,0.65,0.84,0.84);// l->SetFillStyle(0);
  ld->AddEntry(hd_all,"all","P");
  ld->AddEntry(hd_tag,"tagged","P");
  ld->AddEntry(hd_untag,"untagged","P");
  ld->Draw();

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

  double total = prediction->Integral();
  double num_b = total*x1, num_be = total*x1e;
  double num_c = total*x2, num_ce = total*x2e;
  double num_l = total*x3, num_le = total*x3e;

  hmc_b->Scale(1/hmc_b->Integral()*num_b);
  hmc_c->Scale(1/hmc_c->Integral()*num_c);
  hmc_l->Scale(1/hmc_l->Integral()*num_l);

  THStack *hsf =new THStack("hsf",";p_{T}^{rel} [GeV/c]");

  hsf->Add(hmc_b,"hist");
  hsf->Add(hmc_c,"hist");
  hsf->Add(hmc_l,"hist");
  

  //prediction->Integral() == hd_tag->Integral() so it's basically the same thing
  
  TCanvas *cf = new TCanvas("cf","cf",600,600);
  hsf->Draw();
  hd_tag->Draw("Epsame");
  TLegend *lf = new TLegend(0.65,0.65,0.84,0.84);// l->SetFillStyle(0);
  lf->AddEntry(hmc_b,"Fit B","f");
  lf->AddEntry(hmc_c,"Fit C","f");
  lf->AddEntry(hmc_l,"Fit L","f");
  lf->AddEntry(hd_tag,"Data tagged","P");
  lf->Draw();


  cout<<"Number of b-jets  : "<<num_b<<" ± "<<num_be<<endl;
  cout<<"Number of c-jets : "<<num_c<<" ± "<<num_ce<<endl;
  cout<<"Number of l-jets : "<<num_l<<" ± "<<num_le<<endl;

  cout<<"Fit quality : "<<fit->GetChisquare() / fit->GetNDF()<<endl;

  cf->SaveAs("FitTagged.pdf");
  cf->Update();





}
