#include "Analysis.h"

static double etaMinCoarse[] = {0,0,0.5,1.5};
static double etaMaxCoarse[] = {2,0.5,1.5,2};

//static double etaMinFine[] = {0.0,1.5,1.6,1.8};
static double etaMaxFine[] = {1.5,1.6,1.8,2.0};



TH1D* quickResponse(
		    string dir = "puTowerExclLimitV3",
		    string sample = "qcd",
		    int ptMin = 40,
		    int ptMax = 50,
		    bool PbPb  = 1,
		    int etaBin = 0,
		    int centBin = 0,
		    int cone = 4,
		    bool fineBinned = 1,
		    bool evenQuicker = 0
		    ){

  string algo = Form("ak%s%dPF",PbPb?"Pu":"",cone);

  if(evenQuicker){
    TFile* inf = new TFile(Form("response_profiles_etaBin%d_centBin%d.root",etaBin,centBin));
    TH1D* hh = (TH1D*)inf->Get(Form("h_%s_%d_%d",algo.data(),(int)PbPb,ptMin));
    return hh;
  }

  TH1::SetDefaultSumw2();

  int pthat = 30;
  if(ptMin > 70) pthat = 50;
  //  if(ptMin > 90) pthat = 65;
  if(ptMin > 110) pthat = 80;

  //Force
  //  pthat = 30;

  string mc;
  // = "/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/qcd80/puTowerExclLimitV2_moreTuples/merged_HiForestAOD.root";

  if(PbPb){
    mc = Form("/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/%s%d/%s/merged_*.root",sample.data(),pthat,dir.data());
  }else{
    mc = Form("/data_CMS/cms/mnguyen/bJet2015/mc/pp/pythia6/%s%d/constSubV1_csvV2/merged_HiForestAOD*.root",sample.data(),pthat);
  }

  double *etaMin, *etaMax;

  if(fineBinned){
    etaMin = etaMinFine;
    etaMax = etaMaxFine;
  }else{
    etaMin = etaMinCoarse;
    etaMax = etaMaxCoarse;
  }


  string filename = mc;

  TChain* t = new TChain(Form("%sJetAnalyzer/t",algo.data()));
  TChain* tev = new TChain("hiEvtAnalyzer/HiTree");

  t->Add(filename.data());
  tev->Add(filename.data());

  t->AddFriend(tev);


  /*
  t->SetAlias("pt1","Max$(jtpt * (abs(jteta) < 2))");
  t->SetAlias("eta1","Sum$(refeta*(jtpt == pt1))");
  t->SetAlias("mpt","Sum$(matchedPt*(jtpt == pt1))");
  t->SetAlias("raw1","Max$(rawpt * (abs(jteta) < 2))");
  t->SetAlias("raw0","Max$(jtpt * (abs(jteta) < 2))");
  */


  TCut etaCut(Form("abs(refeta)>%f && abs(refeta)<%f",etaMin[etaBin],etaMax[etaBin]));
  TCut signal("subid == 0");

  if(sample == "bfcr") signal = "subid == 0 && abs(refparton_flavorForB) == 5";
  TCut ptBin(Form("refpt > %d && refpt < %d",ptMin,ptMax));
  TCut recoCut("jtpt > 30");
  TCut centrality("hiBin < 200");

  if(fineBinned){
    centrality = Form("hiBin >= %d && hiBin < %d",centBin*10,(centBin+1)*10);
    if(centBin == 6) centrality = Form("hiBin >= %d && hiBin < %d",6*10,10*10);
    if(centBin == 7) centrality = Form("hiBin >= %d && hiBin < %d",10*10,14*10);
    if(centBin == 8) centrality = Form("hiBin >= %d && hiBin < %d",14*10,20*10);
  }else{
    if(centBin == 0) centrality = "hiBin < 20";
    if(centBin == 1) centrality = "hiBin >= 20 && hiBin < 60";
    if(centBin == 2) centrality = "hiBin >= 60";
  }

  if(!PbPb) centrality = "jtpt > 0";

  TH1D *h, *h0,*h1;

  h = new TH1D("h",";p_{T}^{reco, corrected}/p_{T}^{gen};events",60,0,3);


  t->Draw("jtpt/refpt>>h",signal&&ptBin&&etaCut&&recoCut&&centrality);

  h->Scale(1./h->Integral());
  
  cout<<"-------------------------"<<endl;
  cout<<"File : "<<filename.data()<<endl;
  cout<<algo.data()<<endl;
  cout<<etaMin[etaBin]<<" < |eta| < "<<etaMax[etaBin]<<endl;
  cout<<"pt ["<<ptMin<<","<<ptMax<<"]"<<endl;
  cout<<"Mean : "<<h->GetMean()<<" +- "<<h->GetMeanError()<<endl;
  cout<<"RMS  : "<<h->GetRMS()<<" +- "<<h->GetRMSError()<<endl;
  cout<<"-------------------------"<<endl;

  h->SetName(Form("h_%s_%d_%d",algo.data(),(int)PbPb,ptMin));

  TLegend *t3 =new TLegend(0.58,0.63,0.82,0.94);
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);
  t3->AddEntry(h,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h->GetMean(),h->GetMeanError()),"");
  t3->AddEntry(h,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h->GetRMS(),h->GetRMSError()),"");



  return h;



}



