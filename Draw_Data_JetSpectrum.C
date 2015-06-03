void DrawSimpleJetSpectrum()
{
  TFile *f = new TFile("jettrig.root");
  TNtuple *nt = (TNtuple *)f->Get("nt");

  int nbins=50;
  float xmin=20;
  float xmax=150;
 
  TH1F *j40 = new TH1F("j40","j40",nbins, xmin, xmax);
  TH1F *j60 = new TH1F("j60","j60",nbins, xmin, xmax);
  TH1F *j80 = new TH1F("j80","j80",nbins, xmin, xmax);
  TH1F *j100 = new TH1F("j100","j100",nbins, xmin, xmax);

  TH1F *sp = new TH1F("sp","sp",nbins, xmin, xmax);
  
  nt->Project("j40","pt", "(jt40 && pt>40 && pt<60)*(pscl40)");
  nt->Project("j60","pt", "(jt60 && pt>60 && pt<80)*(pscl60)");
  nt->Project("j80","pt", "(jt80 && pt>80 && pt<100)*(pscl80)");
  nt->Project("j100","pt","(jt100 && pt>100)*(pscl100)");

  //nt->Project("j40","pt", "(jt40 && !jt60 && !jt80 && !jt100)*(pscl40)");
  //nt->Project("j60","pt", "(jt60 && !jt80 && !jt100)");
  //nt->Project("j80","pt", "(jt80 && !jt100)");
  //nt->Project("j100","pt","(jt100)");

  sp->Add(j40,j60,1.,1.);
  sp->Add(j80);
  sp->Add(j100);

  THStack *hs = new THStack("s","stacked");
  j40->SetFillColor(kRed); j40->SetFillStyle(1001);
  j60->SetFillColor(kGreen); j60->SetFillStyle(1001);
  j80->SetFillColor(kBlue); j80->SetFillStyle(1001);
  j100->SetFillColor(kOrange);j100->SetFillStyle(1001);

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

  TCanvas *c = new TCanvas("c","c",600,600);
  c->SetLogy(true);
  hs->Draw();
  sp->Draw("same");


  
}
