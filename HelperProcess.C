#include <iostream>
#include <iomanip>
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

//LIMITATIONS
//int branches are for counting only
//adding: float branch only


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

bool Contains(TString what, vector<TString> &where)
{
  return std::find(where.begin(),where.end(),what)!=where.end();
}

// Helper function to avoid code mess with TString's Tokenize
inline vector<TString> tokenize(TString line, const char* delim=" ") {
  vector<TString> retvec;
  line = line.Strip();
  // Tokenize the string, use blank as delimiter
  TObjArray* tokens = line.Tokenize(delim);
  for (int i=0; i<tokens->GetSize(); i++) {
    // For some reasons Tokenize produces an awfull lot of Null-pointers  
    if (tokens->At(i)!=0) {
      // We need to explicitly cast the array contents to TObjString  
      TString t = dynamic_cast<TObjString *>(tokens->At(i))->GetString();
      retvec.push_back(t);
    }
  }
  return retvec;
}

void GetBranchNameType(TString branch, TString &brname, TString &brtype)
{
  auto b = tokenize(branch,"/");
  if (b.size()==2) {brname=b[0];brtype=b[1];}
  else cout<<"Branch "<<branch<<" cannot be parsed!"<<endl;
}

void ParseNewBranches(vector<TString> branches, vector<TString> &brInt, vector<TString> &brFloat, vector<TString> &brVInt,vector<TString> &brVIntCounter, vector<TString> &brVFloat,vector<TString> &brVFloatCounter)
{
  //{"Njetmuon/I:jetmuonpt/F,jetmuoneta/F,jetmuonphi/F","test/F"}
  for (auto b:branches)
    {
      TString brname, brtype;
      auto btab = tokenize(b,":");
      if (btab.size()==2) {//it's a table!
	TString countername, countertype;
	GetBranchNameType(btab[0],countername,countertype); //type should be int
	brInt.push_back(countername);
	//parse columns
	auto columns = tokenize(btab[1],",");
	for (auto c:columns) {
	  GetBranchNameType(c,brname,brtype);

	  if (brtype=="F") {brVFloat.push_back(brname); brVFloatCounter.push_back(countername); }
	  else if (brtype=="I") {brVInt.push_back(brname);brVIntCounter.push_back(countername); }
	  else cout<<"Branch of unknown type! "<<c<<endl;
	}

      }
      else { //not a table
	GetBranchNameType(b,brname,brtype);
	if (brtype=="F") brFloat.push_back(brname);
	else if (brtype=="I") brInt.push_back(brname);
	else cout<<"Branch of unknown type! "<<b<<endl;
      }
    }
}

//vector<TString> GetBranchesList(TTree *t)
//{
  //  l->GetEntries(); b->GetName(); b=(TBranch *)(*l)[1]; l=jet->GetListOfBranches();
//}

bool NonFriendBranch(TTree *t, TString branchName)
{
  if (t->GetBranch(branchName)==0) return true;
  TString branchtree = t->GetBranch(branchName)->GetTree()->GetName();
  TString treename = t->GetName();
  return branchtree.CompareTo(treename) == 0;
}

void ProcessFile(TString fileIn, TString fileOut, TString treename,  vector<TString> friends, vector<TString> branches, vector<TString> newbranches, std::function<void(Everything&,Everything&)> processFunc)
{
  TFile *fin = new TFile(fileIn);
  TTree *tjet = (TTree*)fin->Get(treename);

  //  auto originalBranchesList = GetBranchesList(tjet);

  vector<TTree *> friendTrees;
  vector<bool> sameFileFriend;
  for (auto f:friends) {
    auto fr = tokenize(f,":");
    if (fr.size()==1) {tjet->AddFriend(fr[0]); sameFileFriend.push_back(true);}
    else if (fr.size()==2) {tjet->AddFriend(fr[1],fr[0]); sameFileFriend.push_back(false);}
    cout<<"Wrong friend name "<<f<<endl;

    TTree *tfriend = (TTree*)fin->Get(f);
    friendTrees.push_back(tfriend);
  }

  vector<TString> brInt, brFloat, brVInt, brVIntCounter, brVFloat, brVFloatCounter;

  //sort branches into categories
  for (auto bName:branches) {
    TBranch *b=tjet->GetBranch(bName);
    if (b==0) cout <<"Branch "<<bName<<" doesn't exist!"<<endl;

    //parse in case there is a tree name in the branch
    auto branchtoken = tokenize(bName,".");
    auto leafname = branchtoken[branchtoken.size()-1];

    TLeaf *l = b->GetLeaf(leafname); //assuming one leaf in the branch!
    //what's the type?
    TString type = l->GetTypeName();
    cout<<l->GetTitle()<<endl;

    //array?
    bool array = l->GetLeafCount()!=0;
    TString counter;
    if (array) counter = l->GetLeafCount()->GetTitle();

    if (type=="Float_t")
      {  if (array) {brVFloat.push_back(bName); brVFloatCounter.push_back(counter); }else brFloat.push_back(bName);}
    else if (type=="Int_t")
      {  if (array) {brVInt.push_back(bName); brVIntCounter.push_back(counter); }else brInt.push_back(bName);}
    else cout << "Unsupported branch type "<<type<<" for branch "<<bName<<". Skipping..."<<endl;
  }

  //treat counters as ints only
  AppendToListUniquely(brVIntCounter, brInt);
  AppendToListUniquely(brVFloatCounter, brInt);

  //too keep track of old branches, which cannot be read (todo: just check it...)
  int noldbrInt = brInt.size(), noldbrFloat = brFloat.size(), noldbrVInt = brVInt.size(), noldbrVIntCounter = brVIntCounter.size(), noldbrVFloat = brVFloat.size(), noldbrVFloatCounter = brVFloatCounter.size();
  //add new branches
  ParseNewBranches(newbranches, brInt, brFloat, brVInt, brVIntCounter, brVFloat, brVFloatCounter);

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
  for (auto b:brInt) {unsigned f=0; tjet->SetBranchStatus(b,0,&f); cout<<"turning off ";cout<<b<<", found = "<<f<<endl;}


  TFile *fout = new TFile(fileOut,"recreate");
  TTree *tjetout = tjet->CloneTree(0);
  //think about memory tree
  // tjetout->SetDirectory(0);
  tjetout->SetName(tjet->GetName());
  //TTree *tjetout = new TTree("t","t");

  //to see what is copied...
  tjetout->Print();

  //here are errors from non existing branches
  for (auto b:brVFloat) tjet->SetBranchStatus(b,1);
  for (auto b:brFloat) tjet->SetBranchStatus(b,1);
  for (auto b:brVInt) tjet->SetBranchStatus(b,1);
  for (auto b:brInt) tjet->SetBranchStatus(b,1);

  vector<int> valIntIn(brInt.size()), valIntOut(brInt.size());
  vector<float> valFloatIn(brFloat.size()), valFloatOut(brFloat.size());
  vector<vector<int> >valVIntIn (brVInt.size());  vector<vector<int> >valVIntOut (brVInt.size());
  vector<vector<float> >valVFloatIn (brVFloat.size());  vector<vector<float> >valVFloatOut (brVFloat.size());

  for (unsigned i=0;i<brInt.size();i++) {
    if (tjet->GetBranch(brInt[i])!=0)
      tjet->SetBranchAddress(brInt[i],&valIntIn[i]);

    if (tjetout->GetBranch(brInt[i])!=0) {//why would it?
      tjetout->SetBranchAddress(brInt[i],&valIntOut[i]); 
      cout<<"branch "<<brInt[i]<<" already exist for some reason..."<<endl; 
    }
    else //logical...
      if (NonFriendBranch(tjet, brInt[i])) 
	tjetout->Branch(brInt[i],&valIntOut[i],Form("%s/I",brInt[i].Data()));
  }

  for (unsigned i=0;i<brFloat.size();i++) {
    if (tjet->GetBranch(brFloat[i])!=0) 
      tjet->SetBranchAddress(brFloat[i],&valFloatIn[i]);
    if (NonFriendBranch(tjet, brFloat[i]))
      tjetout->Branch(brFloat[i],&valFloatOut[i],Form("%s/F",brFloat[i].Data()));
  }
  cout<<"5"<<endl;

  for (unsigned i=0;i<brVFloat.size();i++) {
    if (tjet->GetBranch(brVFloat[i])!=0) {
      valVFloatIn[i] = vector<float>(NMAX);
      tjet->SetBranchAddress(brVFloat[i],&valVFloatIn[i][0]);
    }
    

    valVFloatOut[i] = vector<float>(NMAX);
    if (NonFriendBranch(tjet, brVFloat[i]))
      tjetout->Branch(brVFloat[i],&valVFloatOut[i][0],Form("%s[%s]/F",brVFloat[i].Data(),brVFloatCounter[i].Data()));
  }

  for (unsigned i=0;i<brVInt.size();i++) {
    if (tjet->GetBranch(brVInt[i])) {
      valVIntIn[i] = vector<int>(NMAX);
      tjet->SetBranchAddress(brVInt[i],&valVIntIn[i][0]);
    }
    valVIntOut[i] = vector<int>(NMAX);
    if (NonFriendBranch(tjet, brVInt[i]))
      tjetout->Branch(brVInt[i],&valVIntOut[i][0],Form("%s[%s]/I",brVInt[i].Data(),brVIntCounter[i].Data()));
  }

  Long64_t nentries = tjet->GetEntries();
  int oneperc = nentries/100;

  cout<<"Start processing..."<<endl;
  TStopwatch s;
  s.Start();
  TTimeStamp t0;
  double readTime = 0, processingTime = 0, copyToTime = 0, cloneTime=0, copyFromTime=0, fillTime = 0;
  

  for (Long64_t i=0; i<nentries;i++) {
    if (i % oneperc == 0 && i>0) {
      std::cout << std::fixed;
      TTimeStamp t1; cout<<"\r"<<i/oneperc<<"%   "<<" est. time "<<setprecision(2) <<(t1-t0)*nentries/(i+.1)<<" s ";
      cout<<";Processing:"<<setprecision(2)<<processingTime/(t1-t0)*100<<" %";
      cout<<";Copy1:"<<setprecision(2) <<copyToTime/(t1-t0)*100<<" %";
      cout<<";Clone:"<<setprecision(2) <<cloneTime/(t1-t0)*100<<" %";
      cout<<";Copy2:"<<setprecision(2) <<copyFromTime/(t1-t0)*100<<" %";
      cout<<";Fill:"<<setprecision(2) <<fillTime/(t1-t0)*100<<" %";
      cout<<";Read:"<<setprecision(2) <<readTime/(t1-t0)*100<<" %";
      cout<<flush; 
    }
    
    TTimeStamp tsRead0;
    tjet->GetEntry(i);
    TTimeStamp tsRead1;
    readTime+=tsRead1-tsRead0;

    Everything ev;
    TTimeStamp tsCpTo0;
    for (unsigned j=0;j<brInt.size();j++) ev.PutInt(brInt[j],valIntIn[j]);
    for (unsigned j=0;j<brFloat.size();j++) ev.PutFloat(brFloat[j],valFloatIn[j]);
    for (unsigned j=0;j<brVFloat.size();j++) ev.PutVFloat(brVFloat[j],brVFloatCounter[j],valVFloatIn[j]);
    for (unsigned j=0;j<brVInt.size();j++) ev.PutVInt(brVInt[j],brVIntCounter[j],valVIntIn[j]);
    TTimeStamp tsCpTo1;
    copyToTime+=tsCpTo1-tsCpTo0;
    

    TTimeStamp tsCl0;
    //think about: copy object (timing 10% ->3%) 
    Everything evout = ev;
    //or even reference(in place?) (->0.2%)
    //Everything &evout = ev;
    //Everything evout = ev.CloneStructure();
    TTimeStamp tsCl1;
    cloneTime+=tsCl1-tsCl0;

    //cout<<"will process..."<<endl;
    TTimeStamp tsPr0;
    processFunc(ev, evout);
    TTimeStamp tsPr1;  
    processingTime+=tsPr1-tsPr0;
    //cout<<"processed!"<<endl;
    
    //Everything evout = ev;
    
    TTimeStamp tsCpFrom0;
    for (unsigned j=0;j<brInt.size();j++) valIntOut[j] = evout.GetInt(brInt[j]);
    for (unsigned j=0;j<brFloat.size();j++) valFloatOut[j] = evout.GetFloat(brFloat[j]);
    for (unsigned j=0;j<brVFloat.size();j++) valVFloatOut[j] = evout[brVFloat[j]];
    for (unsigned j=0;j<brVInt.size();j++) valVIntOut[j] = evout.GetVInt(brVInt[j]);
    TTimeStamp tsCpFrom1;
    copyFromTime+=tsCpFrom1-tsCpFrom0;

    TTimeStamp tsFill0;
    tjetout->Fill();
    TTimeStamp tsFill1;
    fillTime+=tsFill1-tsFill0;

  }
  cout<<endl;
  s.Stop();
  cout<<"Done in ";s.Print();
  

  tjetout->Write();
  
  cout<<"Copying other trees..."<<endl;

  for (unsigned i=0;i<friendTrees.size();i++) {
    TTree *t = friendTrees[i];
    if (sameFileFriend[i]) {
      TTree *triendout = t->CloneTree(-1,"fast");
      triendout->Write();
    }
  }
  


  fout->Close();
  friendTrees.clear();
}