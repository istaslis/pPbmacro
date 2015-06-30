#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

void Subset_MCForest_Add_PthatWeight_parallel(int ind){

  gROOT->Reset();


  const int Nbins=4;
  TFile *fin[Nbins], *fout, *f_outtemp[Nbins];
  TTree *tr_in[Nbins], *tr_out[Nbins];


  Int_t bounds[Nbins] = {30,50,80,120};

  char *name;

  TString sample = "QCDPPb";//"bJetPPb"; //QCDPPb
  
  for (Int_t it=0; it<Nbins; it++) 
  {
    name = Form("/data_CMS/cms/mnguyen/pPbAna/%s/pthat%d/merged_HiForest.root",sample.Data(), bounds[it]);
    fin[it] = new TFile(name);
    fin[it]->cd("/akPu4PFJetAnalyzer");

    tr_in[it] = (TTree*)gDirectory->Get("t");
    cout<<"got the tree "<<it<<endl;

  }
  //

  sprintf(name,"%s_HiForest_p.root",sample.Data());


  float weights[Nbins] = {1.};
  float sumofweights = 1;
  for (int i=1;i<Nbins;i++)
  {
    TString nextbound = Form("pthat>%d",bounds[i]);
    cout<<tr_in[i-1]->GetEntries(nextbound)<<endl;
    cout<<tr_in[i]->GetEntries(nextbound)<<endl;
    weights[i]=(float)weights[i-1]*tr_in[i-1]->GetEntries(nextbound)/(tr_in[i]->GetEntries(nextbound)+weights[i-1]*tr_in[i-1]->GetEntries(nextbound));

    sumofweights+=weights[i];
  }
  for (int i=1;i<Nbins;i++) {
    weights[it]/=sumofweights;
    cout<<"weight "<<bounds[it]<<" : "<<weights[it]<<endl;
  }

  
  vector<TString> Branches = {
    "evt", "b", 

    "nref",
    "rawpt",
    "jtpt",
    "jteta",
    "jty",
    "jtphi",
    "discr_csvSimple",

    "mue", "mupt", "mueta", "muphi", "mudr", "muptrel", "muchg",

    "pthat",
    
    "refpt", "refeta", "refy", "refphi", "refparton_pt", "refparton_flavorForB",
    "ngen", "genmatchindex", "genpt", "geneta", "geny", "genphi"
  };

  Float_t         vx, vy, vz, pthat, weight;

  bool firstpass = true;

   

  //for (int it = 0;it<Nbins;it++) 
  {
    int it=ind;

    tr_in[it]->SetBranchStatus("*",0);
    for (auto b:Branches) tr_in[it]->SetBranchStatus(b,1);
    tr_in[it]->SetBranchAddress("pthat",&pthat);

   

    cout <<"copy address";

    f_outtemp[it] = new TFile(Form("tempfile%d.root",it),"RECREATE");

    tr_out[it] = tr_in[it]->CloneTree(0,"fast");
    tr_out[it]->SetName("t");//Form("t%d",it));
    //tr_out[it]->SetDirectory(0);

    //Add Friend only after Clone Tree! Otherwise some useless info 
    //is copied which slows down further processing (like Draw command)
    tr_in[it]->AddFriend("hi=hiEvtAnalyzer/HiTree");

    tr_in[it]->SetBranchAddress("hi.vx",&vx);
    tr_in[it]->SetBranchAddress("hi.vy",&vy);
    tr_in[it]->SetBranchAddress("hi.vz",&vz);

    tr_out[it]->Branch("vx",&vx,"vx/F");
    tr_out[it]->Branch("vy",&vy,"vy/F");
    tr_out[it]->Branch("vz",&vz,"vz/F");
    tr_out[it]->Branch("weight",&weight,"weight/F");
    cout <<"done"<<endl;




  weight = weights[it];
  cout<<"  weight ("<< bounds[it] <<") : "<< weight <<endl;

  Long64_t nentries = tr_in[it]->GetEntries();
  Long64_t nbytes = 0;

  for (Long64_t i=0; i<nentries;i++) {

    nbytes += tr_in[it]->GetEntry(i);

      /*      if (it<Nbins-1){
      	if (pthat>bounds[it+1]) continue;
      } */

        int ptbin = Nbins-1;
        if (pthat<bounds[0]) cout<<"pthat less than "<<bounds[0]<<"!"<<endl;
        for (int i=0;i<Nbins-1;i++)
         if (pthat>bounds[i] && pthat<bounds[i+1]) ptbin = i;
       weight = weights[ptbin];

       tr_out[it]->Fill();

     }
     tr_out[it]->Write();

   }
/*
  fout = new TFile(name,"RECREATE");
  TList *list = new TList;
  for (auto t:tr_out) list->Add(t);
    cout<<"Merging..."<<endl;
  TTree *tr_fullout = TTree::MergeTrees(list,"fast");
  tr_fullout->SetName("newtree");
  cout<<"Writing..."<<endl;
  tr_fullout->Write();
  fout->Close();
*/


}


