void DrawJetSpectrum()
{
  TFile *f = new TFile("jettrig_weight.root");
  TTree *nt = (TTree *)f->Get("nt");

  int nbins=28;
  float xmin=20;
  float xmax=160;
 
  TH1F *j40 = new TH1F("j40","j40",nbins, xmin, xmax);
  TH1F *j60 = new TH1F("j60","j60",nbins, xmin, xmax);
  TH1F *j80 = new TH1F("j80","j80",nbins, xmin, xmax);
  TH1F *j100 = new TH1F("j100","j100",nbins, xmin, xmax);

  TH1F *sp = new TH1F("sp","sp",nbins, xmin, xmax);
  
  nt->Project("j40","jtpt", "(jt40 && pt>40 && pt<60 && rawpt>22)*weightJet");
  nt->Project("j60","jtpt", "(jt60 && pt>60 && pt<80 && rawpt>22)*weightJet");
  nt->Project("j80","jtpt", "(jt80 && pt>80 && pt<100 && rawpt>22)*weightJet");
  nt->Project("j100","jtpt","(jt100 && pt>100 && rawpt>22)*weightJet");

  nt->Project("sp","jtpt","weightJet*(rawpt>22)");

    THStack *hs = new THStack("s","stacked");
    j40->SetFillColorAlpha(kRed,1); j40->SetFillStyle(1001);//3001);
    j60->SetFillColorAlpha(kGreen,1); j60->SetFillStyle(1001);//3002);
    j80->SetFillColorAlpha(kBlue,1); j80->SetFillStyle(1001);//3003);
    j100->SetFillColorAlpha(kOrange,1);j100->SetFillStyle(1001);//3004);
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
  //sp->Draw("same");

  hs->Draw();
  sp->Draw("same");
  //  j40->Draw("same");
  //j60->Draw("same");
  //j80->Draw("same");
  //j100->Draw("same");

  leg->Draw();

  hs->GetHistogram()->GetXaxis()->SetTitle("jet p_{T} [GeV/c]");

  //  hs->Draw("same");
  c->SaveAs("jetspectrum.pdf");
}
