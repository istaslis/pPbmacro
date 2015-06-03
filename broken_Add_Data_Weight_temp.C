#define temp_cxx
// The class definition in temp.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("temp.C")
// root> T->Process("temp.C","some options")
// root> T->Process("temp.C+")
//

#include "temp.h"
#include <TH2.h>
#include <TStyle.h>

int tot = 0;
void temp::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   cout<<"total entries : "<<tot<<endl;

}

const int nbins = 50;
const float xmin=20;
const float xmax=120;


void temp::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
  TDirectory *savedir = gDirectory;

   TString option = GetOption();

   fProofFile = new TProofOutputFile("trigoutput.root");
  // Open the file
  fFile = fProofFile->OpenFile("RECREATE");


  savedir->cd();

  spectrum = new TH1F("spectrum", "weighted spectrum", nbins, xmin, xmax);
  spectrum->Sumw2();

}

Bool_t temp::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either temp::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  int readentry = fChain->GetReadEntry();//76096
  if (entry % 1000 == 0) cout<<readentry<<endl;//cout<<entry<<"/"<<tot<<endl;

    GetEntry(entry);
  
    float weight = 0;

    float pscl20 = 0;//1/4388.;
    /*    float pscl40 = 1/72.;
    float pscl60 = 1/9.;
    float pscl80 = 1/2.;
    float pscl100= 1/1.;
    float pscl120= 1/1.;
    */

    //12-003
    /*    if (jt40 && !jt60 && !jt80 && !jt100) weight = 1/pscl40;
    if (jt60 && !jt80 && !jt100) weight = 1;
    if (jt80 && !jt100) weight = 1;
    if (jt100) weight = 1;
    */

    if (jt40 && pt>40 && pt<60)  weight = 1/pscl40;
    if (jt60 && pt>60 && pt<80)  weight = 1/pscl60;
    if (jt80 && pt>80 && pt<100) weight = 1/pscl80;
    if (jt100 && pt>100) weight = 1/pscl100;


    /*    if (pt>20 && pt<40)  weight = 1/pscl20;
    if (pt>40 && pt<60)  weight = 1/(1-(1-pscl20)*(1-pscl40));
    if (pt>60 && pt<80)  weight = 1/(1-(1-pscl20)*(1-pscl40)*(1-pscl60));
    if (pt>80 && pt<100) weight = 1/(1-(1-pscl20)*(1-pscl40)*(1-pscl60)*(1-pscl80));
    if (pt>100)          weight = 1/(1-(1-pscl20)*(1-pscl40)*(1-pscl60)*(1-pscl80)*(1-pscl100));
    */



    spectrum->Fill(pt,weight);


    //    cout<<entry<<"/"<<fChain->GetTree()->GetEntries()<<endl;
    //    cout<<(long)fChain->GetFriend("hltanalysis/HltTree")<<endl;
  //  std::cout<<HLT_PAJet100_NoJetID_v1<<" : "<<pt<<std::endl;

   return kTRUE;
}

void temp::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
  TDirectory *savedir = gDirectory;

  if(fFile) {
    fFile->cd();
    bool ok = false;
    spectrum->Write();

    spectrum->SetDirectory(0);
    gDirectory = savedir;

    fFile->Close();

    fProofFile->Print();
    fOutput->Add(fProofFile);

  }
}


void temp::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
  TColor *pal = new TColor();

  int kblue2    = pal->GetColor(  9,  0,200);
  int kred2     = pal->GetColor(190,  0,  3);

  int kblueLight2    = pal->GetColor(178,185,254);
  int kredLight2     = pal->GetColor(253,169,179);



  fFile = new TFile("trigoutput.root");
  
  spectrum = dynamic_cast<TH1F *>(fFile->Get("spectrum"));
  //  bkgpartonC->SetFillColor(kGreen); bkgpartonC->SetFillStyle(1001);

  //  eff = new TH1F("eff", "Tagging efficiency", nbins, xmin, xmax); eff->SetDirectory(0);
  //  TLegend *leg = new TLegend(0.16,0.16,0.5,0.33); leg->SetBorderSize(0);
  //leg->AddEntry(eff,"efficiency 1 jet","P");
  //leg->AddEntry(pur,"putiry 1 jet","P");

  TCanvas *c1 = new TCanvas("c1", "spectrum",600,600);
  spectrum->Draw("E1");
  //  leg->Draw();

  /*  TFile *f = new TFile("outfile.root","recreate");
  eff->Write();
  pur->Write();
  eff2->Write();
  pur2->Write();

  f->Close();
  */

}

