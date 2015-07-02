#include "TStopwatch.h"

void SubsetTree(TString fileout, TString treenameout, TString filein, TString treenamein, vector<TString> branches)
{
  TStopwatch s;
  s.Start();

  TFile *f = new TFile(filein);
  TTree *t = (TTree *)f->Get(treenamein);

  t->SetBranchStatus("*",0);
  for (auto b:branches) t->SetBranchStatus(b,1);
  
  TFile *fout = new TFile(fileout,"RECREATE");
  TTree *tout = t->CloneTree(-1,"fast");
  tout->SetName(treenameout);

  tout->Write();
  fout->Close();

  s.Stop();
  cout<<"Finished in "<<s.RealTime()<<" s"<<endl;
  //s.Print();

}

void SubsetMultipleTrees(TString fileout, vector<TString> treenameout, TString filein, vector<TString> treenamein, vector<vector<TString> > branches)
{
  TStopwatch s;
  s.Start();

  TFile *f = new TFile(filein);
  vector<TTree *>ts;
  for (unsigned i=0;i<treenamein.size();i++) {
    TTree *t = (TTree *)f->Get(treenamein[i]);
    t->SetBranchStatus("*",0);
    for (auto b:branches[i]) t->SetBranchStatus(b,1);
    ts.push_back(t);
  }

  TFile *fout = new TFile(fileout,"RECREATE");
  for (unsigned i=0;i<ts.size();i++) {

    TTree *tout = ts[i]->CloneTree(-1,"fast");

    //shows progress, but much slower
    // TTree *tout = ts[i]->CloneTree(0,"fast");
    // int nentries = ts[i]->GetEntries();
    // int oneperc = nentries/100;
    // for (int j=0;j<nentries;j++)
    // {
    //   ts[i]->GetEntry(j); tout->Fill();
    //   if (j % oneperc == 0) cout<<"\r"<<j/oneperc<<"%   "<<flush;
    // }
    // cout<<endl;

    tout->SetName(treenameout[i]);
    tout->Write();
    delete tout;
    delete ts[i];
  }

  fout->Close();

  s.Stop();
  cout<<"Finished in "<<s.RealTime()<<" s"<<endl;
}
