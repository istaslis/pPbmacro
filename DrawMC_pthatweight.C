void DrawMC_pthatweight()
{
  
  TFile *f = new TFile("QCDPPb_etacut.root");
  TTree *t = (TTree *)f->Get("t");
  
  int nbins = 28;
  float xmin = 20;
  float xmax = 160;

  TH1F *orig = new TH1F("orig","orig",nbins, xmin, xmax);
  TH1F *weighted = new TH1F("weighted","weighted",nbins, xmin, xmax);

  
  t->Project("weighted","pthat","weight");
  t->Project("orig","pthat");

  TLegend *l = new TLegend(0.25,0.2,0.6,0.35); l->SetBorderSize(0);
  l->AddEntry(orig,"original spectrum","L");
  l->AddEntry(weighted,"weighted spectrum","P");
  THStack *hs = new THStack("s","s");
  hs->Add(orig); hs->Add(weighted);
  
  TCanvas *c = new TCanvas("c","c",600,600);
  c->SetLogy(true);
  hs->Draw("nostack");
  hs->GetHistogram()->GetXaxis()->SetTitle("#hat{p_{T}} [GeV/c]");
  l->Draw();
  c->SaveAs("pthatweight.pdf");


}
