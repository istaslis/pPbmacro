void DrawJetSpectrum()
{
  TFile *f = new TFile("jettrig_withweight.root");
  TNtuple *nt = (TNtuple *)f->Get("ntweight");

  int nbins=50;
  float xmin=20;
  float xmax=150;
 
  TH1F *j40 = new TH1F("j40","j40",nbins, xmin, xmax);
  TH1F *j60 = new TH1F("j60","j60",nbins, xmin, xmax);
  TH1F *j80 = new TH1F("j80","j80",nbins, xmin, xmax);
  TH1F *j100 = new TH1F("j100","j100",nbins, xmin, xmax);

  TH1F *sp = new TH1F("sp","sp",nbins, xmin, xmax);
  
  nt->Project("j40","pt", "jt40*weightJet");
  nt->Project("j60","pt", "jt60*weightJet");
  nt->Project("j80","pt", "jt80*weightJet");
  nt->Project("j100","pt","jt100*weightJet");

  nt->Project("sp","pt","weightJet");

    THStack *hs = new THStack("s","stacked");
    j40->SetFillColorAlpha(kRed,0.3); j40->SetFillStyle(1001);//3001);
    j60->SetFillColorAlpha(kGreen,0.3); j60->SetFillStyle(1001);//3002);
    j80->SetFillColorAlpha(kBlue,0.3); j80->SetFillStyle(1001);//3003);
    j100->SetFillColorAlpha(kOrange,0.3);j100->SetFillStyle(1001);//3004);
  j40->SetMarkerColor(kRed);
  j60->SetMarkerColor(kGreen);
  j80->SetMarkerColor(kBlue);
  j100->SetMarkerColor(kOrange);


  
  //does the trick!
  j40->GetSumw2()->Set(0);
  j60->GetSumw2()->Set(0);
  j80->GetSumw2()->Set(0);
  j100->GetSumw2()->Set(0);


  hs->Add(j40);
  hs->Add(j60);
  hs->Add(j80);
  hs->Add(j100);
  

  cout<<sp->Integral()<<endl;

  TLegend *leg= new TLegend(0.6,0.7,0.84,0.84); leg->SetBorderSize(0);
  leg->AddEntry(j40,"jet 40 fired");
  leg->AddEntry(j60,"jet 60 fired");
  leg->AddEntry(j80,"jet 80 fired");
  leg->AddEntry(j100,"jet 100 fired");

  TCanvas *c = new TCanvas("c","c",600,600);
  c->SetLogy(true);
  //  hs->Draw();
  //sp->Draw("same");

  sp->Draw();
  j40->Draw("same");
  j60->Draw("same");
  j80->Draw("same");
  j100->Draw("same");

  leg->Draw();

  //  hs->Draw("same");
  c->SaveAs("jetspectrum.pdf");
}
