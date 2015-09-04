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
#include <thread> 
#include <mutex>

const int NMAX = 10000;
const bool UNSAFEMAP = true;
const bool VERBOSE = false;


//so far - global veriables
vector<TString> brInt, brFloat, brVInt, brVIntCounter, brVFloat, brVFloatCounter;


class Everything {
public:
  map<TString, int> mapInt;
  map<TString, float> mapFloat;
  map<TString, vector<int> > mapVInt;
  map<TString, vector<float> > mapVFloat;

  void AddRow(Everything &ev, TString counter, int N)
  {
    for (unsigned i=0;i<brVIntCounter.size();i++)
      if (brVIntCounter[i]==counter) GetVInt(brVInt[i]).push_back(ev.GetVInt(brVInt[i])[N]);
    for (unsigned i=0;i<brVFloatCounter.size();i++)
      if (brVFloatCounter[i]==counter) GetVFloat(brVFloat[i]).push_back(ev[brVFloat[i]][N]);
  }

  void CopySingles(Everything &ev)
  {
    for (unsigned i=0;i<brInt.size();i++)
      PutInt(brInt[i],ev.GetInt(brInt[i]));
    for (unsigned i=0;i<brFloat.size();i++)
      PutFloat(brFloat[i],ev.GetFloat(brFloat[i]));

  }


  bool UpdateCounters()
  {
    vector<TString> counter; vector<unsigned> nums;
    for (unsigned i=0;i<brVIntCounter.size();i++) {
      unsigned n = std::find(counter.begin(), counter.end(), brVIntCounter[i]) - counter.begin();
      unsigned cursize = GetVInt(brVInt[i]).size();
      //cout<<brVIntCounter[i]<<" "<<n<<" "<<cursize<<" "<<brVInt[i]<<endl;
      if (n!=counter.size()) {
        if (cursize!=nums[n]) {
          cout<<"Wrong dimensions in table "<<brVIntCounter[i]
              <<" Column "<<brVInt[i]<<" has "<<cursize
              <<" elements while others have "<<nums[n]<<endl; 
          return false;
        }
      }
      else { counter.push_back(brVIntCounter[i]); nums.push_back(cursize); }
    }

    for (unsigned i=0;i<brVFloatCounter.size();i++) {
      unsigned n = std::find(counter.begin(), counter.end(), brVFloatCounter[i]) - counter.begin();
      unsigned cursize = GetVFloat(brVFloat[i]).size();
      //cout<<brVFloatCounter[i]<<" "<<n<<" "<<cursize<<" "<<brVFloat[i]<<endl;
      if (n!=counter.size()) {
        if (cursize!=nums[n]) {
          cout<<"Wrong dimensions in table "<<brVFloatCounter[i]
              <<" Column "<<brVFloat[i]<<" has "<<cursize
              <<" elements while others have "<<nums[n]<<endl; 
          return false;
        }
      }
      else {
        //cout<<"writing vector "<< brVFloat[i] <<" with counter "<<brVFloatCounter[i]<<" size "<<cursize<<endl;
        counter.push_back(brVFloatCounter[i]); nums.push_back(cursize);
      }
    }

    for (unsigned i=0;i<counter.size();i++) PutInt(counter[i], nums[i]);

    return true;
  }

  int GetInt(TString name)
  {
    if (UNSAFEMAP) return mapInt[name];
    if (mapInt.find(name) != mapInt.end())
      return mapInt[name];
    else
      cout<<"No integer with name "<<name<<" ! "<<endl;
    return -1;
  }

  float GetFloat(TString name)
  {
    if (UNSAFEMAP) return mapFloat[name];
    if (mapFloat.find(name) != mapFloat.end())
      return mapFloat[name];
    else
      cout<<"No float with name "<<name<<" ! Creating one..."<<endl;
      mapFloat[name] = 0;
    return -1;
  }

  vector<float> &GetVFloat(TString name)
  {
    if (UNSAFEMAP) return mapVFloat[name];
    if (mapVFloat.find(name) != mapVFloat.end())
      return mapVFloat[name];
    else  {
      //cout<<"No float vector with name "<<name<<" ! Creating one..."<<endl;
      vector<float> t;
      mapVFloat[name]=t;      
      return mapVFloat[name];
    }
  }

  vector<int> &GetVInt(TString name)
  {
    if (UNSAFEMAP) return mapVInt[name];
    if (mapVInt.find(name) != mapVInt.end())
      return mapVInt[name];
    else  {
      //cout<<"No int vector with name "<<name<<" ! Creating one..."<<endl;
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
    //mapInt.emplace(name,value);
  }

  void PutFloat(TString name, float value)
  {
    mapFloat[name] = value;
    //mapFloat.emplace(name,value);
  }

  void PutVFloat(TString name, vector<float> value)
  {
    mapVFloat[name] = value;
    //mapVFloat.emplace(name,value);
  }

  void PutVFloat(TString name, TString counterName, vector<float> &value)
  {
    int count = GetInt(counterName);
    vector<float> v(value.begin(), value.begin()+count);
    mapVFloat[name] = v;
    //mapVFloat.emplace(name,value);
  }

  void PutVInt(TString name, vector<int> value)
  {
    mapVInt[name] = value;
    //mapVInt.emplace(name,value);
  }

  void PutVInt(TString name, TString counterName, vector<int> &value)
  {
    int count = GetInt(counterName);
    vector<int> v(value.begin(), value.begin()+count);
    mapVInt[name] = v;
    //mapVInt.emplace(name,value);
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



typedef void (*ProcessOneToOne)(Everything &ev, Everything &evout);
typedef bool (*ProcessOneToMany)(Everything &ev, Everything &evout, int counter);

bool useOneToOne = true; //better solution will be implemented later...
ProcessOneToOne fProcessOneToOne;
ProcessOneToMany fProcessOneToMany;





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

map<TString, vector<TString> >GetCounterBranchListMap(TTree *t)
{
  map<TString, vector<TString> > m;
  TObjArray *l = t->GetListOfLeaves(); //sometimes leave can be named wrong - rely on branches
  int n = l->GetEntries();// cout<<t->GetName()<<" has "<<n<<" leaves"<<endl;
  for (int i=0;i<n;i++) {
    TLeaf * leaf = (TLeaf *)(*l)[i];
    TLeaf *lc = leaf->GetLeafCount();
    
    if (lc!=0) {
      m[lc->GetBranch()->GetName()].push_back(leaf->GetBranch()->GetName()); //get leaf's branch name
      if (VERBOSE) cout<<lc->GetBranch()->GetName()<<" _ "<<leaf->GetBranch()->GetName()<<endl;
    }
  }

  return m;
}

void AddBranchesByCounter(TTree *t, vector<TString> &branches)
{
    //add arrays if there is a counter in the list
  auto leafcounters = GetCounterBranchListMap(t); //may be not working with friends!
  vector<TString> fromcounters;
  for (auto bName:branches) 
    if (leafcounters[bName].size()!=0) 
      for (auto x:leafcounters[bName]) fromcounters.push_back(x);
  for (auto x:fromcounters) branches.push_back(x);

}

//std::mutex mtx;
void ProcessFilePar(TString fileIn, TString fileOut, TString treename,  vector<TString> friends, vector<TString> branches, vector<TString> newbranches, unsigned jobid = 0, unsigned NPAR = 1)
{
  //mtx.lock(); //for threads
  TFile *fin = new TFile(fileIn);
  TTree *tjet = (TTree*)fin->Get(treename);
  //mtx.unlock();

  vector<TTree *> friendTrees;
  vector<bool> sameFileFriend;
  for (auto f:friends) {
    auto fr = tokenize(f,":");
    if (fr.size()==1) {tjet->AddFriend(fr[0]); sameFileFriend.push_back(true);}
    else if (fr.size()==2) {tjet->AddFriend(fr[1],fr[0]); sameFileFriend.push_back(false);}

    TTree *tfriend = (TTree*)fin->Get(f);
    friendTrees.push_back(tfriend);
  }

  AddBranchesByCounter(tjet, branches);
  for (unsigned i=0;i<friendTrees.size();i++) AddBranchesByCounter(friendTrees[i],branches);

  //sort branches into categories
  for (auto bName:branches) {
    TBranch *b=tjet->GetBranch(bName);
    if (b==0) cout <<"Branch "<<bName<<" doesn't exist!"<<endl;

    //parse in case there is a tree name in the branch
    auto branchtoken = tokenize(bName,".");
    auto leafname = branchtoken[branchtoken.size()-1];

    TObjArray *bl = b->GetListOfLeaves();
    if (bl->GetEntries()>1)
      cout <<" Branch "<<b->GetTitle()<<" has more than 1 leave. Taking first..."<<endl;
    if (bl->GetEntries()==0) {
      cout <<" Branch "<<b->GetTitle()<<" has no leaves! Skipping..."<<endl;
      continue;
    }

    TLeaf * l = (TLeaf *)(*bl)[0];
    //what's the type?
    TString type = l->GetTypeName();
    if (VERBOSE) cout<<l->GetTitle()<<endl;

    //array?
    bool array = l->GetLeafCount()!=0;
    TString counter;
    if (array) counter = l->GetLeafCount()->GetBranch()->GetName();

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
  if (VERBOSE) {
    cout<<"int       : "; for (auto s:brInt) cout <<s<<", "; cout<<endl;
    cout<<"float     : "; for (auto s:brFloat) cout <<s<<", "; cout<<endl;
    cout<<"Vint      : "; for (auto s:brVInt) cout <<s<<", "; cout<<endl;
    cout<<"Vfloat    : "; for (auto s:brVFloat) cout <<s<<", "; cout<<endl;
    cout<<"Vintcnt   : "; for (auto s:brVIntCounter) cout <<s<<", "; cout<<endl;
    cout<<"Vfloatcnt : "; for (auto s:brVFloatCounter) cout <<s<<", "; cout<<endl;
  }

  tjet->SetBranchStatus("*",1);
  for (auto b:brVFloat) if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,0);
  for (auto b:brFloat)  if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,0);
  for (auto b:brVInt)   if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,0);
  for (auto b:brInt)    if (tjet->GetBranch(b)!=0) {unsigned f=0; tjet->SetBranchStatus(b,0,&f); if (VERBOSE) cout<<"turning off "<<b<<", found = "<<f<<endl;}


  TFile *fout = new TFile(fileOut,"recreate");
  TTree *tjetout;


  //in case of one-to-many event - do not copy branches automatically!
  if (useOneToOne) tjetout = tjet->CloneTree(0);
  else tjetout = new TTree(tjet->GetName(),tjet->GetTitle());


  //think about memory tree
  // tjetout->SetDirectory(0);
  tjetout->SetName(tjet->GetName());
  //TTree *tjetout = new TTree("t","t");

  //to see what is copied...
  //tjetout->Print();

  for (auto b:brVFloat) if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,1);
  for (auto b:brFloat)  if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,1);
  for (auto b:brVInt)   if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,1);
  for (auto b:brInt)    if (tjet->GetBranch(b)!=0) tjet->SetBranchStatus(b,1);

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
  int nentries1 = nentries/NPAR*jobid;
  int nentries2 = nentries/NPAR*(jobid+1);
  
  nentries = nentries2-nentries1; 
  int oneperc = nentries/100; if (oneperc==0) oneperc = 1;

  cout<<"Start processing..."<<endl;
  TStopwatch s;
  s.Start();
  TTimeStamp t0;
  double readTime = 0, processingTime = 0, copyToTime = 0, cloneTime=0, copyFromTime=0, fillTime = 0;
  

  for (Long64_t i=0; i<nentries;i++) {
    if (jobid==0 && i % oneperc == 0 && i>0) {
      std::cout << std::fixed;
      TTimeStamp t1; cout<<" \r"<<i/oneperc<<"%   "<<" est. time "<<setprecision(2) <<(t1-t0)*nentries/(i+.1)<<" s ";
      cout<<";Processing:"<<setprecision(2)<<processingTime/(t1-t0)*100<<" %";
      cout<<";Copy1:"<<setprecision(2) <<copyToTime/(t1-t0)*100<<" %";
      cout<<";Clone:"<<setprecision(2) <<cloneTime/(t1-t0)*100<<" %";
      cout<<";Copy2:"<<setprecision(2) <<copyFromTime/(t1-t0)*100<<" %";
      cout<<";Fill:"<<setprecision(2) <<fillTime/(t1-t0)*100<<" %";
      cout<<";Read:"<<setprecision(2) <<readTime/(t1-t0)*100<<" %";
      cout<<flush; 
    }
    
    TTimeStamp tsRead0;
    tjet->GetEntry(i+nentries1);
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
    //but it copies vectors, so push_back will add in the end...
    //    Everything evout = ev;
    //or even reference(in place?) (->0.2%)
    //Everything &evout = ev;
    Everything evout = ev.CloneStructure();
    TTimeStamp tsCl1;
    cloneTime+=tsCl1-tsCl0;

    bool exitEvent = false;
    int counter = 0;

    while (!exitEvent) {
    
    TTimeStamp tsPr0;
    if (useOneToOne) {
      fProcessOneToOne(ev, evout);
      evout.UpdateCounters();
      exitEvent = true;
    } else {
      exitEvent = fProcessOneToMany(ev, evout, counter); 
      //      if (!exitEvent) cout<<"event to write "<<counter<<endl;
      counter++;
    }

    TTimeStamp tsPr1;  
    processingTime+=tsPr1-tsPr0;
    
    
    //Everything evout = ev;
    TTimeStamp tsCpFrom0;
    for (unsigned j=0;j<brInt.size();j++) valIntOut[j] = evout.GetInt(brInt[j]);
    for (unsigned j=0;j<brFloat.size();j++) {valFloatOut[j] = evout.GetFloat(brFloat[j]); 
      //  cout<<brFloat[j]<<" "<<evout.GetFloat(brFloat[j])<<endl;
    }
    for (unsigned j=0;j<brVFloat.size();j++) valVFloatOut[j] = evout[brVFloat[j]];
    for (unsigned j=0;j<brVInt.size();j++) valVIntOut[j] = evout.GetVInt(brVInt[j]);
    TTimeStamp tsCpFrom1;
    copyFromTime+=tsCpFrom1-tsCpFrom0;

    TTimeStamp tsFill0;
    tjetout->Fill();
    TTimeStamp tsFill1;
    fillTime+=tsFill1-tsFill0;
  }

  }
  cout<<endl;
  s.Stop();
  cout<<"Done in ";s.Print();
  
  tjetout->FlushBaskets();
  tjetout->Write();
  
  cout<<"Copying other trees..."<<endl;

  for (unsigned i=0;i<friendTrees.size();i++) {
    TTree *t = friendTrees[i];
    if (sameFileFriend[i]) {
      //TTree *triendout = t->CloneTree(-1,"fast");
      TTree *triendout = t->CopyTree("","",nentries,nentries1);
      triendout->Write();
    }
  }
  


  fout->Close();
  friendTrees.clear();
}

//threading attempt - no speed up! looks like IO is serial in root...
// void ProcessFile(TString fileIn, TString fileOut, TString treename,  vector<TString> friends, vector<TString> branches, vector<TString> newbranches, std::function<void(Everything&,Everything&)> processFunc)
// {
//   if (NPAR<1) return;
//   //if (NPAR==1) ProcessFileJob(0, fileIn, fileOut, treename, friends, branches, newbranches, processFunc);

//   vector<std::thread> threads;
//   vector<TString> fileOutNames;
//   for (int i=0;i<NPAR;i++) {
//     TString fileName = Form("%s_job%d",fileOut.Data(),i);
//     fileOutNames.push_back(fileName);
//     threads.push_back(std::thread(ProcessFileJob,i,fileIn, fileName, treename, friends, branches, newbranches, processFunc));
//   }
    
//   for (int i=0;i<NPAR;i++) {
//     threads[i].join();
//   }
// }

TString GetFileName(unsigned jobid, TString fileOut) 
{
  return Form("%s_job%d",fileOut.Data(),jobid);
}

void ProcessFile(TString fileIn, TString fileOut, TString treename,  vector<TString> friends, vector<TString> branches, vector<TString> newbranches, ProcessOneToMany processFunc,unsigned jobid = 0, unsigned int NPAR = 1)
{
  useOneToOne = false;
  fProcessOneToMany = processFunc;


  TString fileName = NPAR>1 ? GetFileName(jobid, fileOut) : fileOut;
  ProcessFilePar(fileIn, fileName, treename, friends, branches, newbranches, jobid, NPAR);

}

void ProcessFile(TString fileIn, TString fileOut, TString treename,  vector<TString> friends, vector<TString> branches, vector<TString> newbranches, ProcessOneToOne processFunc,unsigned jobid = 0, unsigned int NPAR = 1)
{
  useOneToOne = true;
  fProcessOneToOne = processFunc;

  TString fileName = NPAR>1 ? GetFileName(jobid, fileOut) : fileOut;
  ProcessFilePar(fileIn, fileName, treename, friends, branches, newbranches, jobid, NPAR);
  
}



