void Draw_MC_BjetTagging()
{
  //serial version
  TChain* ch = new TChain("t","t");
  ch->AddFile("QCDPPb_etacut.root");
  ch->Process("t.C+");
    

  //parallel version
  //TProof *plite= TProof::Open("");//workers=6") 
  //TChain* ch = new TChain("t","t");
  //ch->AddFile("QCDPPb_etacut.root");
  //ch->SetProof();
  //ch->Process("t.C+");

  //don't calculate - run this in console 
    /*
      .L t.C+
      t h;
      h.Terminate();
    */
}
