

void plotLeading(bool mc = 0, bool s2760 = 0){

  string algo = "akVs4PF";

  TFile* inf;
  
  if(s2760){
    inf = TFile::Open(Form("root://xrootd.unl.edu//store/user/dgulhan/HIMinBiasUPC-HIRun2011-14Mar2014-v2_tag_HI_MatchEqR_DatabaseJEC_merged2/HIMinBiasUPC-HIRun2011-14Mar2014-v2_tag_HI_MatchEqR_DatabaseJEC.root"));
  }else{
    inf = TFile::Open(Form("root://xrootd.unl.edu//store/group/phys_heavyions/jisun/ForestRun2015/PbPbMB/PbPb_HIMinimumBias2_FullForestAOD_goldenjson_Run263233-263293_02162016/HIMinimumBias2/crab_PbPb_HIMinimumBias2_FullForestAOD_goldenjson_Run263233-263293_02162016/160215_222950/0000/HiForestAOD_975.root"));
  }
  
  TTree* t = (TTree*)inf->Get(Form("%sJetAnalyzer/t",algo.data()));
  t->AddFriend("skimanalysis/HltTree");
  t->AddFriend("hiEvtAnalyzer/HiTree");

  TCut selection("pcollisionEventSelection");

  TH2D* h = new TH2D("h",";centrality;leading jet p_{T}",200,0,200,200,0,200);




  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  t->Draw("Max$(jtpt*(abs(jteta)<2)):hiBin>>h",selection,"colz");

  c1->Print(Form("figure_fluctuation_minbias_%s_%dGeV_%s.png",mc?"mc":"data",s2760?2760:5020,algo.data()));

  t->Draw("Max$(jtpt*(abs(jteta)<2)):hiBin>>h",selection,"prof same");

  c1->Print(Form("figure_fluctuation_minbias_%s_%dGeV_%s_prof.png",mc?"mc":"data",s2760?2760:5020,algo.data()));



}

