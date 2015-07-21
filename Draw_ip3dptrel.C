void Draw_ip3dptrel()
{
  TFile *f = new TFile("QCDPPb_genmuonmerged.root");
  TTree *jet = (TTree *)f->Get("jet");
  
  TH2F *h2B = new TH2F("h2B","h2B",50,-10,0,50,0,5);
  TH2F *h2C = new TH2F("h2C","h2C",50,-10,0,50,0,5);
  TH2F *h2L = new TH2F("h2L","h2L",50,-10,0,50,0,5);
  TH2F *h2i = new TH2F("h2i","h2i",50,-10,0,50,0,5);

  TCanvas *c1 = new TCanvas("c1","c1",600,600); TLegend *l1 = new TLegend(0.7,0.7,0.85,0.85,"B");
  jet->Project("h2B","jetmuonptrel:log(jetmuonip3d)","jetmuonptrel>0 && jetmuonptrel<5 && jetmuonpt>5 && abs(refparton_flavorForB)==5","colz");
  h2B->Draw("colz"); 
  l1->Draw();  c1->SaveAs("muon_ptrel_ip3d_B.pdf");
  TCanvas *c2 = new TCanvas("c2","c2",600,600); TLegend *l2 = new TLegend(0.7,0.7,0.85,0.85,"C");
  jet->Project("h2C","jetmuonptrel:log(jetmuonip3d)","jetmuonptrel>0 && jetmuonptrel<5 && jetmuonpt>5 && abs(refparton_flavorForB)==4","colz");
  h2C->Draw("colz");
  l2->Draw();  c2->SaveAs("muon_ptrel_ip3d_C.pdf");
  TCanvas *c3 = new TCanvas("c3","c3",600,600); TLegend *l3 = new TLegend(0.7,0.7,0.85,0.85,"L");
  jet->Project("h2L","jetmuonptrel:log(jetmuonip3d)","jetmuonptrel>0 && jetmuonptrel<5 && jetmuonpt>5 && abs(refparton_flavorForB)!=5 && abs(refparton_flavorForB)!=4","colz");
  h2L->Draw("colz");
  l3->Draw();  c3->SaveAs("muon_ptrel_ip3d_L.pdf");
  TCanvas *c4 = new TCanvas("c4","c4",600,600); TLegend *l4 = new TLegend(0.7,0.7,0.85,0.85,"all");
  jet->Project("h2i","jetmuonptrel:log(jetmuonip3d)","jetmuonptrel>0 && jetmuonptrel<5 && jetmuonpt>5","colz");
  h2i->Draw("colz");
  l4->Draw();  c4->SaveAs("muon_ptrel_ip3d_all.pdf");




  TObjArray *mc = new TObjArray(3);
  mc->Add(h2B);
  mc->Add(h2C);
  mc->Add(h2L);



  TFractionFitter* fit = new TFractionFitter(h2i, mc);
  fit->Constrain(0,0.0,1.0);
  fit->Constrain(1,0.0,1.0);
  fit->Constrain(2,0.0,1.0);
  fit->Fit();
  TH2F* prediction = (TH2F*) fit->GetPlot();



  cout<<fit->GetProb()<<endl;

  double x1, x1e, x2, x2e, x3, x3e;
  fit->GetResult(0,x1,x1e);
  fit->GetResult(1,x2,x2e);
  fit->GetResult(2,x3,x3e);

  TCanvas *c5 = new TCanvas("c5","c5",600,600); TLegend *l5 = new TLegend(0.7,0.7,0.85,0.85,"pred");
  prediction->Draw("colz");
  l5->Draw(); c5->SaveAs("muon_ptrel_ip3d_pred.pdf");



}
