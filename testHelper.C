#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include <vector>
#include "HelperProcess.C"

void makeFile(char *fname)
{
  TFile *fout = new TFile(fname,"recreate");
  TTree *t = new TTree("t","t");
 
  int i; float f;
  int n, k = 10;
  vector<int> vi(1000); vector<Float_t> vf(1000); vector<Float_t> vk(1000); //must be allocated static anyway

  cout<<&(vi[0])<<" : "<<&(vf[0])<<endl;
  t->Branch("i",&i,"i/I");
  t->Branch("f",&f,"f/F");
  t->Branch("n",&n,"n/I");
  t->Branch("k",&k,"k/I");

  t->Branch("vi",vi.data(),"vi[n]/I");
  t->Branch("vf",vf.data(),"vf[n]/F");
  t->Branch("vk",vk.data(),"vk[k]/F");

  for (i=0;i<10000;i++){
    f=gRandom->Gaus();
    n=gRandom->Integer(1000);
   
    vi.clear(); vf.clear(); vk.clear();
    for (int l=0;l<k;l++) vk.push_back(gRandom->Gaus());
    for (int j=0;j<n;j++){
      vi.push_back(j);
      float t = gRandom->Gaus();
      vf.push_back(t);
    }
    t->Fill();

    }
  t->Write();

  fout->Close();


}

void ProcessEvent(Everything &ev, Everything &evout)
{
  //auto i=ev.GetInt("i");
  auto n=ev.GetInt("n");
  //auto f=ev.GetFloat("f");
  //auto vi=ev.GetVInt("vi");
  //auto vf=ev["vf"];
  

  for (int j=1;j<n;j++) {
    evout.AddRow(ev,"n",j);

    //evout["newVI"].push_back(vf[j]);
  }
}


void testHelper()
{
  cout<<"Making file..."<<endl;
  makeFile("testHelper1.root");

  cout<<"Processing..."<<endl;
  ProcessFile("testHelper1.root","testHelper2.root","t",vector<TString>(),vector<TString>({"i","f","vf"}),
	      vector<TString>({"newN/I:newVI/F"}),ProcessEvent);










}



















