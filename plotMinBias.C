

{

  string algo = "akVs4PF";

  TFile* inf = TFile::Open(Form("root://xrootd.unl.edu//store/user/velicanu/Merged/Hydjet_Quenched_MinBias_5020GeV_758p2_FOREST_v20/0.root"));

  TTree* t = (TTree*)inf->Get(Form("%sJetAnalyzer/t",algo.data()));

  t->AddFriend("hiEvtAnalyzer/HiTree");

  t->Draw("Max$(jtpt*(jteta<2)):hiBin>>h(200,0,200,200,0,200)","","colz");


}



