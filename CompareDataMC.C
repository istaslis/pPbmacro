TFile *fd, *fm;
TNtuple *td, *tm;


void drawText(const char *text, float xp, float yp, int color = kBlack, int size=30){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(43);
  tex->SetTextSize(size); 
  tex->SetTextColor(color); 
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw("same");
}


int counter=0;

void Compare(TString var, TString seldata, TString selmc, TString sel, int nbins, float xmin, float xmax)
{
  counter++;

  TString named = Form("%s%d",var.Data(),counter);
  TString namem = Form("%s%d",var.Data(),counter+1);
  TH1F *h1 = new TH1F(named,named,nbins, xmin, xmax);h1->Sumw2();
  td->Project(named,var,seldata);
  h1->Scale(1/h1->Integral());


  TH1F *h2 = new TH1F(namem,namem,nbins, xmin, xmax); h2->Sumw2();
  tm->Project(namem,var,selmc);
  h2->Scale(1/h2->Integral());


  TCanvas *c1 = new TCanvas(named,named,600,700);
  TPad *pad1 = new TPad("pad1","pad1",0,0.4,1,1);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  pad1->cd();
  //  pad1->SetLogy();


  h2->SetMarkerColor(kBlue);
  h1->Draw();
  h2->Draw("same");


  drawText("Data",0.7,0.6,kBlack);
  drawText("MC",0.7,0.7,kBlue);
  drawText(sel,0.18,0.8,kBlack,20);
  
  /*
    TLegend *l = new TLegend(0.65,0.75,0.84,0.84);
  l->AddEntry(h1,"Data","P");
  l->AddEntry(h2,"MC","P");
  
  l->Draw();

  */

  c1->cd();

  TPad *pad2 = new TPad("pad2","pad2",0,0.1,1,0.4);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.25);
  pad2->Draw();
  pad2->cd();

  TH1F *h3=(TH1F *)h1->Clone();//(TH1F *)h1->DrawCopy();

  //  h3->Sumw2();
  //  h3->SetStats(0);

  h3->Divide(h1,h2,1,1,"B");

  h3->SetMinimum(0);
  h3->SetMaximum(2);

  h3->GetXaxis()->SetTitle(var);
  h3->GetXaxis()->SetTitleOffset(3.5);
  //  drawText(var,0.18,0.8,kBlack,20);



  h3->SetMarkerStyle(21);
  h3->Draw("ep");
  c1->cd();

  c1->SaveAs(Form("CompareDataMC_%s.pdf",named.Data()));
  
}

void Compare(TString var, TString sel, int nbins, float xmin, float xmax)
{
  TString seldata = Form("weightJet*(%s)",sel.Data());
  TString selmc   = Form("weight*(%s)",sel.Data());

  Compare(var, seldata, selmc, sel, nbins, xmin, xmax);

}

void CompareDataMC()
{
  fm = new TFile("QCDPPb_genmuonmerged_flat.root");
  tm = (TNtuple *)fm->Get("jet");

  fd = new TFile("jettrig_weight_AccCut_muons_flat.root");
  td = (TNtuple *)fd->Get("jet");

  int nbins = 16;
  float xmin = 0, xmax=400;

  Compare("jtpt","jtpt>0 && jtpt<400",nbins, xmin, xmax);
  //  Compare("jtpt","jtpt>250 && jtpt<400",nbins, 250, 400);
  //Compare("jtpt","weightJet*(jtpt>250 && jtpt<400 && weightJet==1)","weight*(jtpt>250 && jtpt<400 && weight<5E-3)","jtpt>250 && jtpt<400 && highest bin/trigger",nbins, 250, 400);

  //Compare("discr_prob","jtpt>60 && jtpt<80 && discr_prob>0",nbins, 0, 3);
  //Compare("discr_prob","jtpt>250 && jtpt<400 && discr_prob>0",nbins, 0, 3);

  Compare("logmuip3d","jtpt>60 && jtpt<80 && jetmuonpt>7",nbins, -10, 0);
  Compare("logmuip3d","jtpt>80 && jtpt<120 && jetmuonpt>7",nbins, -10, 0);
  Compare("logmuip3d","jtpt>170 && jtpt<250 && jetmuonpt>7",nbins, -10, 0);
  Compare("logmuip3d","jtpt>250 && jtpt<400 && jetmuonpt>7",nbins, -10, 0);


  //Compare("svtxm","jtpt>60 && jtpt<80 && svtxm>0",nbins, 0, 6);
  
  //Compare("svtxm","jtpt>60 && jtpt<80 && discr_ssvHighEff>2",nbins, 0, 6);
  //Compare("svtxm","jtpt>250 && jtpt<400 && discr_ssvHighEff>2",nbins, 0, 6);
  /*
  Compare("svtxm","weightJet*(jtpt>250 && jtpt<400 && discr_ssvHighEff>2 && weightJet==1)","weight*(jtpt>250 && jtpt<400 && discr_ssvHighEff>2)","jtpt>250 && jtpt<400 && discr_ssvHighEff>2 && weightJet==1",nbins, 0, 6);
  */
  
}









/*  Compare("jtpt","weightJet*(jtpt>0 && jtpt<400)","weight*(jtpt>0 && jtpt<400)",nbins, xmin, xmax);
  Compare("jtpt","weightJet*(jtpt>250 && jtpt<400)","weight*(jtpt>250 && jtpt<400)",nbins, xmin, xmax);

  Compare("discr_prob","weightJet*(jtpt>60 && jtpt<80 && discr_prob>0)","weight*(jtpt>60 && jtpt<80 && discr_prob>0)",nbins, 0, 3);
  Compare("discr_prob","weightJet*(jtpt>250 && jtpt<400 && discr_prob>0)","weight*(jtpt>250 && jtpt<400 && discr_prob>0)",nbins, 0, 3);

  Compare("svtxm","weightJet*(jtpt>60 && jtpt<80 && svtxm>0)","weight*(jtpt>60 && jtpt<80 && svtxm>0)",nbins, 0, 6);

  Compare("svtxm","weightJet*(jtpt>60 && jtpt<80 && discr_ssvHighEff>2)","weight*(jtpt>60 && jtpt<80 && discr_ssvHighEff>2)",nbins, 0, 6);
  Compare("svtxm","weightJet*(jtpt>250 && jtpt<400 && discr_ssvHighEff>2)","weight*(jtpt>250 && jtpt<400 && discr_ssvHighEff>2)",nbins, 0, 6);

*/
