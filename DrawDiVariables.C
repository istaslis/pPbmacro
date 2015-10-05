#include "InitStyle.h"

#define PI 3.1415926

const int MAXREFPARTONS = 6;
const int MAXREFPARTON = 3;
const int NCUTS = 5;

const int colors[] = {kBlue,kYellow,kGreen,kMagenta,28,kRed};
const int colors3[] = {kBlue,kGreen,kRed};

const char * canvassuffix = "_lj0_9";

//map<TString, TH1F *> m[MAXREFPARTONS];

vector<TH1F *> Ajs(MAXREFPARTONS);
vector<TH1F *> Phis(MAXREFPARTONS);
vector<TH1F *> Etas(MAXREFPARTONS);
vector<TH1F *> hSsvhe(MAXREFPARTONS);
vector<TH1F *> hCSV(MAXREFPARTONS);
vector<TH1F *> hMuon(MAXREFPARTONS);
vector<TH1F *> hsvtxm0(MAXREFPARTON);
vector<TH1F *> hsvtxm1true(MAXREFPARTON);
vector<TH1F *> hsvtxm1tag(MAXREFPARTON);


vector<TH1F *> hDiB(NCUTS);
vector<TH1F *> hAll(NCUTS);
vector<TH1F *> hTag(NCUTS);

vector<TH1F *> hEff(NCUTS);
vector<TH1F *> hPur(NCUTS);


void Init(vector <TH1F *> &v, TString histname,int bins, float xmin, float xmax)
{
  for (unsigned i=0;i<v.size();i++){
    TString name = TString::Format("%s_rp%d",histname.Data(),i);
    v[i] = new TH1F(name,name,bins,xmin,xmax);
    v[i]->SetFillColor(v.size()==3 ? colors3[i] : colors[i]);
    v[i]->SetFillStyle(1001);
  }
}

void InitPt(vector<TH1F *> &v, TString histname)
{
  vector<double> pTbins = {60,80,120,170,250,400};
  double *ptbins = &pTbins[0];
  int Nptbins = pTbins.size()-1;

  for (unsigned i=0;i<v.size();i++) {
    TString name = TString::Format("%s_%d",histname.Data(),i);
    if (v.size()==NCUTS) {
      float discrValue = 0.5+0.5*(float)i/NCUTS;
      name = TString::Format("%s_%.1f",histname.Data(),discrValue);
    }

    v[i] = new TH1F(name,name,Nptbins,ptbins);
    v[i]->SetLineColor(colors[i]); v[i]->SetMarkerColor(colors[i]); v[i]->SetLineWidth(2.0);

  }
  

}

void FillAjs(float jtpt0, float jtpt1, int refpartons, float weight)
{
  if (jtpt0>60 && jtpt1>30) {
    float Aj = (jtpt0-jtpt1)/(jtpt0+jtpt1);
    Ajs[refpartons]->Fill(Aj,weight);
  }
}

void FillPhis(float jtpt0, float jtpt1, int refpartons, float weight, float jtphi0, float jtphi1)
{    
  if (jtpt0>60 && jtpt1>30) {
    float dPhi = abs(jtphi0-jtphi1);
    if (dPhi>PI) dPhi=2*PI-dPhi;
    Phis[refpartons]->Fill(dPhi,weight);
  }
}

void FillEtas(float jtpt0, float jtpt1, int refpartons, float weight, float jteta0, float jteta1)
{
  if (jtpt0>60 && jtpt1>30) {
    float dEta = jteta0-jteta1;
    Etas[refpartons]->Fill(dEta,weight);
  }
}

void Fillssvhe(float jtpt0, float jtpt1, int refpartons, float weight, float ssvhe0, float ssvhe1)
{
  if (jtpt0>60 && jtpt1>30) {
    float minssvhe = min(ssvhe0,ssvhe1);
    hSsvhe[refpartons]->Fill(minssvhe,weight);
  }
}

void FillCSV(float jtpt0, float jtpt1, int refpartons, float weight, float csv0, float csv1)
{
  if (jtpt0>60 && jtpt1>30) {
    float mincsv = min(csv0,csv1);
    hCSV[refpartons]->Fill(mincsv,weight);
  }
}

void FillMuon(float jtpt0, float jtpt1, int refpartons, float weight, float muonpt0, float muonpt1)
{
  if (jtpt0>60 && jtpt1>30) {
    float minmuonpt = min(muonpt0,muonpt1);
    hMuon[refpartons]->Fill(minmuonpt,weight);
  }
}

int refpartonmap(int refparton_flavorForB)
{
  if (refparton_flavorForB==5) return 2;
  if (refparton_flavorForB==4) return 1;
  return 0;
}

void FillSVTXM01(float jtpt0, float jtpt1, float weight, int refparton_flavorForB0, float svtxm0, int refparton_flavorForB1, float svtxm1, float discr_ssvHighEff0)
{
  if (jtpt0>60 && jtpt1>30) {

    if (discr_ssvHighEff0>0)
      hsvtxm0[refpartonmap(refparton_flavorForB0)]->Fill(svtxm0,weight);

    if (refparton_flavorForB0==5)
      hsvtxm1true[refpartonmap(refparton_flavorForB1)]->Fill(svtxm1,weight);
    if (discr_ssvHighEff0>0 && svtxm0>3.5)
      hsvtxm1tag[refpartonmap(refparton_flavorForB1)]->Fill(svtxm1,weight);

  }
}

void FillEffPur(float jtpt0, float jtpt1, float weight, int refparton_flavorForB0, int refparton_flavorForB1, 
                float discr_csvSimple0, float discr_csvSimple1)
{
  if (jtpt0>60 && jtpt1>30) {

    for (int i=0;i<NCUTS;i++) {
      float discrValue = 0.5+0.5*(float)i/NCUTS;

      if (discr_csvSimple0>0.9 && discr_csvSimple1>discrValue)
        hAll[i]->Fill(jtpt0,weight);

      if (refparton_flavorForB0==5 && refparton_flavorForB1==5)
        hDiB[i]->Fill(jtpt0,weight);

      if (discr_csvSimple0>0.9 && refparton_flavorForB0==5 && discr_csvSimple1>discrValue && refparton_flavorForB1==5)
        hTag[i]->Fill(jtpt0,weight);


    }
  }
}

void CalcEffPur()
{
  for (int i=0;i<NCUTS;i++){
    hEff[i]->Divide(hTag[i],hDiB[i],1,1,"B");
    hPur[i]->Divide(hTag[i],hAll[i],1,1,"B");
    hEff[i]->SetMaximum(0.6);
    hPur[i]->SetMaximum(1.0);
  }

}


void NormalizeAllBins(vector<TH1F *> &hs)
{
  TH1F *hsum = (TH1F *)hs[0]->Clone("tempsum");
  hsum->Reset();
  for (auto h:hs)
    hsum->Add(h);

  for (auto h:hs)
    h->Divide(h,hsum,1,1,"B");
}

void DrawHists(vector<TH1F *> &hs, TString name, TString histopt = "hist", TString stackopt = "")
{
  float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;  


  THStack *h = new THStack(name,name);
  TLegend *l = new TLegend(legendx1, legendy1, legendx2, legendy2,name);
  //  for (auto hist:hs) {
  for (int i=hs.size()-1;i>=0;i--) {
    h->Add(hs[i], histopt);
    l->AddEntry(hs[i],hs[i]->GetTitle());
    cout<<"histogram "<<hs[i]->GetTitle()<<" has "<<hs[i]->Integral()<<" values"<<endl;
  }


  TCanvas *c = new TCanvas(name,name,600,600);
  h->Draw(stackopt);
  l->Draw();
  c->SaveAs(TString::Format("%s%s.pdf",name.Data(),canvassuffix));
}


void DrawDiVariables() {
  InitStyle();

  TFile *_file0=new TFile("QCDPPb_dijet_flat.root");
  
  TTreeReader reader("jet", _file0);
  TTreeReaderValue<float> weight(reader, "weight");
  TTreeReaderValue<int> refpartons(reader, "refpartons");
  TTreeReaderValue<int> refparton_flavorForB0(reader, "refparton_flavorForB0");
  TTreeReaderValue<int> refparton_flavorForB1(reader, "refparton_flavorForB1");
  TTreeReaderValue<float> jtpt0(reader, "jtpt0");
  TTreeReaderValue<float> jtpt1(reader, "jtpt1");
  TTreeReaderValue<float> jtphi0(reader, "jtphi0");
  TTreeReaderValue<float> jtphi1(reader, "jtphi1");
  TTreeReaderValue<float> jteta0(reader, "jteta0");
  TTreeReaderValue<float> jteta1(reader, "jteta1");
  TTreeReaderValue<float> discr_ssvHighEff0(reader, "discr_ssvHighEff0");
  TTreeReaderValue<float> discr_ssvHighEff1(reader, "discr_ssvHighEff1");
  TTreeReaderValue<float> discr_csvSimple0(reader, "discr_csvSimple0");
  TTreeReaderValue<float> discr_csvSimple1(reader, "discr_csvSimple1");
  TTreeReaderValue<float> jetmuonpt0(reader, "jetmuonpt0");
  TTreeReaderValue<float> jetmuonpt1(reader, "jetmuonpt1");
  TTreeReaderValue<float> svtxm0(reader, "svtxm0");
  TTreeReaderValue<float> svtxm1(reader, "svtxm1");

  
  Init(Ajs, "Aj", 10,0,1);
  Init(Phis,"Phi",10,0,PI);
  Init(Etas,"Eta",10,-5,5);
  Init(hSsvhe,"SSVHE",10,0,6);
  Init(hCSV,"CSV",20,0,1);
  Init(hMuon,"Muon",10,0,20);
  Init(hsvtxm0,"SVTXM0",10,0,6);
  Init(hsvtxm1true,"SVTXM1true",10,0,6);
  Init(hsvtxm1tag,"SVTXM1tag",10,0,6);

  InitPt(hDiB,"hDiB");
  InitPt(hAll,"hAll");
  InitPt(hTag,"hTag");

  InitPt(hEff,"Efficiency");
  InitPt(hPur,"Purity");





  while(reader.Next()) {
    float w = *weight;
    
    if (*discr_ssvHighEff0>0 && *discr_ssvHighEff1>0) { 
    //    if (*jetmuonpt0>0 && *jetmuonpt1>0) {
      FillAjs(*jtpt0, *jtpt1, *refpartons, *weight);
      FillPhis(*jtpt0, *jtpt1, *refpartons, *weight, *jtphi0, *jtphi1);
      FillEtas(*jtpt0, *jtpt1, *refpartons, *weight, *jteta0, *jteta1);
    }
    Fillssvhe(*jtpt0, *jtpt1, *refpartons, *weight, *discr_ssvHighEff0, *discr_ssvHighEff1);
    FillCSV(*jtpt0, *jtpt1, *refpartons, *weight, *discr_csvSimple0, *discr_csvSimple1);
 
    // FillMuon(*jtpt0, *jtpt1, *refpartons, *weight, *jetmuonpt0, *jetmuonpt1);

    // FillSVTXM01(*jtpt0, *jtpt1, *weight, *refparton_flavorForB0, *svtxm0, *refparton_flavorForB1, *svtxm1, *discr_ssvHighEff0);

    FillEffPur(*jtpt0, *jtpt1, *weight, *refparton_flavorForB0, *refparton_flavorForB1, *discr_csvSimple0, *discr_csvSimple1);

  }

  CalcEffPur();

  NormalizeAllBins(hSsvhe);
  NormalizeAllBins(hCSV);
  NormalizeAllBins(hMuon);
  
  DrawHists(Ajs,"Aj");
  DrawHists(Phis,"delta Phi");
  DrawHists(Etas,"delta Eta");
  DrawHists(hSsvhe,"sshve");
  DrawHists(hCSV,"CSV");
  // DrawHists(hMuon,"Muon");
  // DrawHists(hsvtxm0,"svtxm0");
  // DrawHists(hsvtxm1true,"hsvtxm1true");
  // DrawHists(hsvtxm1tag,"hsvtxm1tag");

  DrawHists(hEff, "Efficiency", "","nostack");
  DrawHists(hPur, "Purity", "","nostack");

}
/*
refpartons
jtpt0
jteta0
jtphi0
rawpt0
discr_prob0
discr_csvSimple0
svtxm0
discr_ssvHighPur0
discr_ssvHighEff0
jetmuonpt0
jetmuonptrel0
jetmuonip3d0
logmuip3d0
jtpt1
jteta1
jtphi1
rawpt1
discr_prob1
discr_csvSimple1
svtxm1
discr_ssvHighPur1
discr_ssvHighEff1
jetmuonpt1
jetmuonptrel1
jetmuonip3d1
logmuip3d1
weight
*/
