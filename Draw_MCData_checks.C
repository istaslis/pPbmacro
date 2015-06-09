void Draw_MCData_checks()
{
  TFile *fdt = new TFile("jettrig_weight_etacut.root");
  TFile *fmc = new TFile("QCDPPb_etacut.root");

  TTree *dt = (TTree *)fdt->Get("nt");
  TTree *mc = (TTree *)fmc->Get("t");

  int nbins = 50;
  float xmin = 10;
  float xmax = 150;

  TProfile *jecmc = new TProfile("jecmc","jecmc",nbins, xmin, xmax);
  TProfile *jecdt = new TProfile("jecdt","jecdt",nbins, xmin, xmax);
  jecmc->SetAxisRange(1.05,1.2,"Y");
  jecdt->SetMarkerColor(kRed);

  dt->Project("jecdt","jtpt/rawpt:rawpt","rawpt<150","prof");
  mc->Project("jecmc","jtpt/rawpt:rawpt","rawpt<150","prof");

  TLegend *l = new TLegend(0.6,0.7,0.84,0.84); l->SetBorderSize(0);
  l->SetHeader("JEC");
  l->AddEntry(jecmc,"MC","P");
  l->AddEntry(jecdt,"Data","P");


  TCanvas *c1 = new TCanvas("c1","c1", 600,600);
  jecmc->Draw();
  jecdt->Draw("same");
  l->Draw();
  c1->SaveAs("JEC_MC_Data.pdf");


  //the reason JEC are different at profile plot is that JEC=f(eta), and eta distribution in MC and Data is different
  //take a small slice in eta 
  TProfile *jecmceta = new TProfile("jecmceta","jecmceta",nbins, xmin, xmax);
  TProfile *jecdteta = new TProfile("jecdteta","jecdteta",nbins, xmin, xmax);
  jecmceta->SetAxisRange(1.05,1.2,"Y");  jecdteta->SetMarkerColor(kRed);

  dt->Project("jecdteta","jtpt/rawpt:rawpt","rawpt<150 && jteta>-1.15 && jteta<-0.95","prof");
  mc->Project("jecmceta","jtpt/rawpt:rawpt","rawpt<150 && jteta>-1.15 && jteta<-0.95","prof");

  TLegend *l2 = new TLegend(0.6,0.7,0.84,0.84); l2->SetBorderSize(0);
  l2->SetHeader("JEC, eta slice");
  l2->AddEntry(jecmceta,"MC","P");
  l2->AddEntry(jecdteta,"Data","P");

  TCanvas *c2 = new TCanvas("c2","c2", 600,600);
  jecmceta->Draw();
  jecdteta->Draw("same");
  l2->Draw();

  c2->SaveAs("JEC_MC_Data_etaslice.pdf");

}
