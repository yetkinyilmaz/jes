

TH1* plot(string filename, const char* name){

  TFile* inf = new TFile(filename.data());
  //  TTree* t = (TTree*)inf->Get("akPu4PFJetsNoLimits/puTree");
  TTree* t = (TTree*)inf->Get("akPu4PFJetAnalyzer/t");
  TTree* tev = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");

  //  t->AddFriend(tjet,"jet");
  t->AddFriend(tev);


  t->SetAlias("pt1","Max$(jtpt * (abs(jteta) < 2))");
  t->SetAlias("eta1","Sum$(jteta*(jtpt == pt1))");
  t->SetAlias("mpt","Sum$(matchedPt*(jtpt == pt1))");

  t->SetAlias("raw1","Max$(rawpt * (abs(jteta) < 2))");




  TCut lead("pt1 > 100");


  TCut barrel("abs(refeta) < 1.8");
  TCut endcap("abs(refeta) > 1.8 && abs(refeta) < 2");

  TCut jtbarrel("abs(jteta) < 1.8");
  TCut jtendcap("abs(jteta) > 1.8 && abs(jteta) < 2");

  TCut ptbin("refpt > 30 && refpt < 100");
  TCut signal("subid == 0");

  TCut central("hiBin < 20");


  TH1D* h;
  int xBins = 100;
  double xMin = 0;
  double xMax = 5;

  h = new TH1D("h",";p_{T}^{reco}/p_{T}^{gen};events",xBins,xMin,xMax);

  t->Draw(Form("jtpt/refpt>>h"),central&&ptbin&&endcap);

  h->Scale(1./t->GetEntries(central));

  h->SetName(name);

  return h;
}



void plotResponseNtowers(){

  TH1::SetDefaultSumw2();

  //  string data = "/data_CMS/cms/mnguyen/bJet2015/data/PbPb_Jet6080/puTowerExclLimitV2/0000/merged_HiForestAOD.root";
  string mc = "/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/qcd30/constSubV1_csvV2/merged_HiForestAOD.root";
  string data = "/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/qcd30/puTowerExclLimitV2_moreTuples/merged_HiForestAOD.root";


  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  TH1* h0 = plot(mc,"h0");
  TH1* h1 = plot(data,"h1");

  h0->Draw("hist");
  h1->Draw("same");

  //  c1->Print(Form("towers.png"));



}


