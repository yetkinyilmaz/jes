

TH1* plot(string filename, const char* name){

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


  TCut barrel("abs(ieta) <= 20");
  TCut endcap("abs(ieta) > 20 && abs(eta) < 2");
  TCut central("hiBin < 20");


  TH1D* h;
  int xBins = 100;
  double xMin = 0;
  double xMax = 100;

  h = new TH1D("h","",xBins,xMin,xMax);

  t->Draw(Form("ntow>>h"),lead&&central&&barrel);

  h->Scale(1./t->GetEntries(lead&&central));

  h->SetName(name);

  return h;
}



void plotNtowers(){

  TH1::SetDefaultSumw2();

  string data = "/data_CMS/cms/mnguyen/bJet2015/data/PbPb_Jet6080/puTowerExclLimitV2/0000/merged_HiForestAOD.root";
  string mc = "/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/qcd80/puTowerExclLimitV2_moreTuples/merged_HiForestAOD.root";


  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  TH1* h0 = plot(mc,"h0");
  TH1* h1 = plot(data,"h1");

  h0->Draw("hist");
  h1->Draw("same");

  c1->Print(Form("towers.png"));



}


