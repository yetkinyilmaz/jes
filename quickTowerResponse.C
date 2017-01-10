

void quickResponse(string file = "/data_CMS/cms/mnguyen/bJet2015/mc/pp/pythia6/qcd80/merged_HiForestAOD_part1.root"){

  //  string mc = "/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/qcd80/puTowerExclLimitV2_moreTuples/merged_HiForestAOD.root";
  //  mc = "/data_CMS/cms/mnguyen/bJet2015/mc/pp/pythia6/qcd80/merged_HiForestAOD_part1.root";
  string mc = file;

  string filename = mc;

  TFile* inf = new TFile(filename.data());
  TTree* t = (TTree*)inf->Get("akPu4PFJetsNoLimits/puTree");
  TTree* tjet = (TTree*)inf->Get("akPu4PFJetAnalyzer/t");
  TTree* tev = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");

  t->AddFriend(tjet,"jet");
  t->AddFriend(tev);


  t->SetAlias("pt1","Max$(jet.jtpt * (abs(jet.jteta) < 2))");
  t->SetAlias("eta1","Sum$(jet.jteta*(jet.jtpt == pt1))");
  t->SetAlias("mpt","Sum$(jet.matchedPt*(jet.jtpt == pt1))");

  t->SetAlias("raw1","Max$(jet.rawpt * (abs(jet.jteta) < 2))");
  t->SetAlias("raw0","Max$(jtpt * (abs(jteta) < 2))");




  TCut lead("pt1 > 100");


  TCut barrel("abs(jteta)<1.5");
  TCut endcap("abs(jteta) > 1.5 && abs(jteta) < 2");
  TCut jetbarrel("abs(jet.jteta)<1.5");
  TCut jetendcap("abs(jet.jteta) > 1.5 && abs(jet.jteta) < 2");

  TCut central("hiBin < 20");


  TH1D* h0,*h1;

  h0 = new TH1D("h0",";p_{T}^{reco, uncorrected}/p_{T}^{gen};events",300,0,3);
  h1 = new TH1D("h1",";p_{T}^{reco, uncorrected}/p_{T}^{gen};events",300,0,3);




}



