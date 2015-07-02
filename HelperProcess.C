#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include <algorithm>
#include "TStopwatch.h"
#include "TTimeStamp.h"
#include <functional>
#include "TBranch.h"
#include "TLeaf.h"

const int NMAX = 10000;



class Everything {
public:
  map<TString, int> mapInt;
  map<TString, float> mapFloat;
  map<TString, vector<int> > mapVInt;
  map<TString, vector<float> > mapVFloat;

  int GetInt(TString name)
  {
    if (mapInt.find(name) != mapInt.end())
      return mapInt[name];
    else
      cout<<"No integer with name "<<name<<" ! "<<endl;
    return -1;
  }

  float GetFloat(TString name)
  {
    if (mapFloat.find(name) != mapFloat.end())
      return mapFloat[name];
    else
      cout<<"No float with name "<<name<<" ! "<<endl;
    return -1;
  }

  vector<float> &GetVFloat(TString name)
  {
    if (mapVFloat.find(name) != mapVFloat.end())
      return mapVFloat[name];
          else  {
      cout<<"No float vector with name "<<name<<" ! Creating one..."<<endl;
      vector<float> t;
      mapVFloat[name]=t;      
      return mapVFloat[name];
      }

  //  unsafe way of the same thing
  //  return mapVFloat[name]; 
  }
  vector<int> &GetVInt(TString name)
  {
    if (mapVInt.find(name) != mapVInt.end())
      return mapVInt[name];
    else  {
      cout<<"No int vector with name "<<name<<" ! Creating one..."<<endl;
      vector<int> t;
      mapVInt[name]=t;      
      return mapVInt[name];
    }
  }
  
  //only one can be so simple...
  vector<float> &operator[](TString name)
  {
    return GetVFloat(name);
  }

  void PutInt(TString name, int value)
  {
    mapInt[name] = value;
  }

  void PutFloat(TString name, int value)
  {
    mapFloat[name] = value;
  }

  void PutVFloat(TString name, vector<float> value)
  {
    mapVFloat[name] = value;
  }

  void PutVFloat(TString name, TString counterName, vector<float> &value)
  {
    int count = GetInt(counterName);
    vector<float> v(value.begin(), value.begin()+count);
    mapVFloat[name] = v;
  }

  void PutVInt(TString name, vector<int> value)
  {
    mapVInt[name] = value;
  }

  void PutVInt(TString name, TString counterName, vector<int> &value)
  {
    int count = GetInt(counterName);
    vector<int> v(value.begin(), value.begin()+count);
    mapVInt[name] = v;
  }

  void PutArray(TString name, int len, float *arr)
  {
    vector<float> vec(arr,arr+len);
    mapVFloat[name] = vec;
  }

  void GetArray(TString name, float *arr)
  {
    if (mapVFloat.find(name) != mapVFloat.end())
      {
	for (unsigned i=0;i<mapVFloat[name].size();i++) arr[i] = mapVFloat[name][i];
      }
    else  cout<<"No vector with name "<<name<<" ! "<<endl;
    
  }

  Everything CloneStructure()
  {
    Everything e;

    for (auto it=mapInt.begin();it!=mapInt.end();it++) e.PutInt(it->first, 0);
    for (auto it=mapFloat.begin();it!=mapFloat.end();it++) e.PutFloat(it->first, 0);
    for (auto it=mapVInt.begin();it!=mapVInt.end();it++) e.PutVInt(it->first, vector<int>(0));
    for (auto it=mapVFloat.begin();it!=mapVFloat.end();it++) e.PutVFloat(it->first, vector<float>(0));

    return e;
  }

};

void testEverything()
{
  Everything ev;
  ev.PutInt("myint",10);
  ev.PutVFloat("myvec",vector<float>({1.,2.,3.}));
  cout<<ev.GetInt("myint")<<endl;
  cout<<ev.GetVFloat("myvec").size()<<endl;
  cout<<ev.GetInt("wrong")<<endl;
  cout<<ev.GetVFloat("wrong").size()<<endl;
}

void AppendToListUniquely(vector<TString> &from, vector<TString> &to)
{
  for (auto s:from)
    if (std::find(to.begin(),to.end(),s)==to.end()) to.push_back(s);
}

void ProcessFile(TString fileIn, TString fileOut, TString treename,  vector<TString> branches, std::function<void(Everything&,Everything&)> processFunc)
{
  TFile *fin = new TFile(fileIn);
  TTree *tjet = (TTree*)fin->Get("jet");
  TTree *tmuon = (TTree*)fin->Get("muon");
  TTree *tevt = (TTree*)fin->Get("evt");

  //  vector<TString> brInt , vector<TString> brVFloat;
  vector<TString> brInt, brFloat, brVInt, brVIntCounter, brVFloat, brVFloatCounter;

  //sort branches into categories
  //  for (int i=0;i<brVFloat.size();i++) {
  
  for (auto bName:branches) {
    TBranch *b=tjet->GetBranch(bName);
    TLeaf *l = b->GetLeaf(bName); //assuming one leaf in the branch!

    //what's the type?
    TString type = l->GetTypeName();
    //array?
    bool array = l->GetLeafCount()!=0;
    TString counter;
    if (array) counter = l->GetLeafCount()->GetTitle();

    //todo: add l->GetLeafCount()->GetTitle() to the list of branches 

    if (type=="Float_t")
      {  if (array) {brVFloat.push_back(bName); brVFloatCounter.push_back(counter); }else brFloat.push_back(bName);}
    else if (type=="Int_t")
      {  if (array) {brVInt.push_back(bName); brVIntCounter.push_back(counter); }else brInt.push_back(bName);}
    else cout << "Unsupported branch type "<<type<<" for branch "<<bName<<". Skipping..."<<endl;
  }

  //treat counters as ints only
  AppendToListUniquely(brVIntCounter, brInt);
  AppendToListUniquely(brVFloatCounter, brInt);

  //print for debugging
  cout<<"int       : "; for (auto s:brInt) cout <<s<<", "; cout<<endl;
  cout<<"float     : "; for (auto s:brFloat) cout <<s<<", "; cout<<endl;
  cout<<"Vint      : "; for (auto s:brVInt) cout <<s<<", "; cout<<endl;
  cout<<"Vfloat    : "; for (auto s:brVFloat) cout <<s<<", "; cout<<endl;
  cout<<"Vintcnt   : "; for (auto s:brVIntCounter) cout <<s<<", "; cout<<endl;
  cout<<"Vfloatcnt : "; for (auto s:brVFloatCounter) cout <<s<<", "; cout<<endl;
 
  tjet->SetBranchStatus("*",1);
  for (auto b:brVFloat) tjet->SetBranchStatus(b,0);
  for (auto b:brFloat) tjet->SetBranchStatus(b,0);
  for (auto b:brVInt) tjet->SetBranchStatus(b,0);
  for (auto b:brInt) tjet->SetBranchStatus(b,0);



  TFile *fout = new TFile(fileOut,"recreate");
  TTree *tjetout = tjet->CloneTree(0);

  for (auto b:brVFloat) tjet->SetBranchStatus(b,1);
  for (auto b:brFloat) tjet->SetBranchStatus(b,1);
  for (auto b:brVInt) tjet->SetBranchStatus(b,1);
  for (auto b:brInt) tjet->SetBranchStatus(b,1);

  vector<int> valIntIn(brInt.size()), valIntOut(brInt.size());
  vector<float> valFloatIn(brFloat.size()), valFloatOut(brFloat.size());
  vector<vector<int> >valVIntIn (brVInt.size());  vector<vector<int> >valVIntOut (brVInt.size());
  vector<vector<float> >valVFloatIn (brVFloat.size());  vector<vector<float> >valVFloatOut (brVFloat.size());

  for (unsigned i=0;i<brInt.size();i++) {
    tjet->SetBranchAddress(brInt[i],&valIntIn[i]);
    //save both in output tree to check how many we had
    tjetout->Branch(Form("%sin",brInt[i].Data()),&valIntIn[i],Form("%sin/I",brInt[i].Data()));
    tjetout->Branch(Form("%sout",brInt[i].Data()),&valIntOut[i],Form("%sout/I",brInt[i].Data()));
  }

  for (unsigned i=0;i<brFloat.size();i++) {
    tjet->SetBranchAddress(brFloat[i],&valFloatIn[i]);
    tjetout->Branch(brFloat[i],&valFloatOut[i],Form("%s/F",brFloat[i].Data()));
  }

  for (unsigned i=0;i<brVFloat.size();i++) {
    valVFloatIn[i] = vector<float>(NMAX);
    valVFloatOut[i] = vector<float>(NMAX);
    tjet->SetBranchAddress(brVFloat[i],&valVFloatIn[i][0]);
    tjetout->Branch(brVFloat[i],&valVFloatOut[i][0],Form("%s[%sout]/F",brVFloat[i].Data(),brVFloatCounter[i].Data()));
  }

  for (unsigned i=0;i<brVInt.size();i++) {
    valVIntIn[i] = vector<int>(NMAX);
    valVIntOut[i] = vector<int>(NMAX);
    tjet->SetBranchAddress(brVInt[i],&valVIntIn[i][0]);
    tjetout->Branch(brVInt[i],&valVIntOut[i][0],Form("%s[%sout]/F",brVInt[i].Data(),brVIntCounter[i].Data()));
  }

  Long64_t nentries = tjet->GetEntries();
  int oneperc = nentries/100;

  cout<<"Start processing..."<<endl;
  TStopwatch s;
  s.Start();
  TTimeStamp t0;
  double processingTime = 0;
  

  for (Long64_t i=0; i<nentries;i++) {
    if (i % oneperc == 0)
      { TTimeStamp t1; cout<<"\r"<<i/oneperc<<"%   "<<" est. time "<<(t1-t0)*nentries/(i+.1)<<" s ";
        cout<<"; Processing time is "<<processingTime/(t1-t0)*100<<" % "<<flush; }
    tjet->GetEntry(i);
    
    Everything ev;
    for (unsigned j=0;j<brInt.size();j++) ev.PutInt(brInt[j],valIntIn[j]);
    for (unsigned j=0;j<brFloat.size();j++) ev.PutFloat(brFloat[j],valFloatIn[j]);
    for (unsigned j=0;j<brVFloat.size();j++) ev.PutVFloat(brVFloat[j],brVFloatCounter[j],valVFloatIn[j]);
    for (unsigned j=0;j<brVInt.size();j++) ev.PutVInt(brVInt[j],brVIntCounter[j],valVIntIn[j]);
    
    Everything evout = ev.CloneStructure();

    //cout<<"will process..."<<endl;
    TTimeStamp tsPr0;
    processFunc(ev, evout);
    TTimeStamp tsPr1;  
    processingTime+=tsPr1-tsPr0;
    //cout<<"processed!"<<endl;
    
    //Everything evout = ev;
    
    for (unsigned j=0;j<brInt.size();j++) valIntOut[j] = evout.GetInt(brInt[j]);
    for (unsigned j=0;j<brFloat.size();j++) valFloatOut[j] = evout.GetFloat(brFloat[j]);
    for (unsigned j=0;j<brVFloat.size();j++) valVFloatOut[j] = evout[brVFloat[j]];
    for (unsigned j=0;j<brVInt.size();j++) valVIntOut[j] = evout.GetVInt(brVInt[j]);
      
    tjetout->Fill();
  }
  cout<<endl;
  s.Stop();
  cout<<"Done in ";s.Print();
  

  tjetout->Write();
  
  cout<<"Copying other trees..."<<endl;
  TTree *tmuonout = tmuon->CloneTree(-1,"fast");
  tmuonout->Write();
  TTree *tevtout = tevt->CloneTree(-1,"fast");
  tevtout->Write();
  


    fout->Close();

}
