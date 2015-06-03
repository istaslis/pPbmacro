{
  TFile f("jettrig_weight_etacut.root");
  cout<<nt->Draw("abs(jtphi[0]-jtphi[1])","weightJet*(rawpt[0]>18 && rawpt[1]>18 && jtpt[0]>60 && jtpt[1]>30 &&discr_csvSimple[0]>0.9 && discr_csvSimple[1]>0.9)")<<endl;

}
