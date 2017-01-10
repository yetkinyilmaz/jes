#include "quickResponse.C"

static string file = "/Users/yetkin/data/smear_v2/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_jes.root";

TH1D** load(int ipt, int ieta, int icent, int cone = 3){
  TFile* inf = new TFile(file.data());

  TH1D* h = (TH1D*)inf->Get(Form("h_%d_%d_%d",ipt,ieta,icent));
  TH1D* h0 = (TH1D*)inf->Get(Form("h0_%d_%d_%d",ipt,ieta,icent));
  TH1D* h20 = (TH1D*)inf->Get(Form("h20_%d_%d_%d",ipt,ieta,icent));
  TH1D* h20s = (TH1D*)inf->Get(Form("h20s_%d_%d_%d",ipt,ieta,icent));
  TH1D* h30 = (TH1D*)inf->Get(Form("h30_%d_%d_%d",ipt,ieta,icent));
  TH1D* hh[] = {h,h0,h20,h20s,h30};
  return hh;
}


TH1D** sophisticatedResponse(
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


  TCut etaCut(Form("abs(geneta)>%f && abs(geneta)<%f",etaMin[etaBin],etaMax[etaBin]));
  TCut signal("gensubid == 0");

  if(sample == "bfcr") signal = "gensubid == 0 && abs(genparton_flavorForB) == 5";
  TCut ptBin(Form("genpt > %d && genpt < %d",ptMin,ptMax));
  TCut recoMatch("genmatchindex >= 0");
  TCut recoCut("jtpt[genmatchindex] > 30");
  TCut algoCut("jtpt[genmatchindex] > 20");

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
  //  TFile* outf = 0;
  //  if(ptMin == 20) outf = new TFile(Form("response_profiles_etaBin%d_centBin%d.root",etaBin,centBin),"recreate");
  //  else outf = new TFile(Form("response_profiles_etaBin%d_centBin%d.root",etaBin,centBin),"update");

  TH1D *h, *h30, *h20, *h0;


  h = new TH1D("h",";p_{T}^{reco, corrected}/p_{T}^{gen};events",60,0,3);
  h0 = new TH1D("h0",";p_{T}^{reco, corrected}/p_{T}^{gen};events",60,0,3);
  h20 = new TH1D("h20",";p_{T}^{reco, corrected}/p_{T}^{gen};events",60,0,3);
  h30 = new TH1D("h30",";p_{T}^{reco, corrected}/p_{T}^{gen};events",60,0,3);

  t->Draw("jtpt[genmatchindex]/genpt>>h30",signal&&ptBin&&etaCut&&centrality&&(recoMatch&&recoCut));
  t->Draw("1.1*jtpt[genmatchindex]/genpt>>h20",signal&&ptBin&&etaCut&&centrality&&(recoMatch&&algoCut&&!recoCut));
  t->Draw("12.*genpt/genpt/genpt>>h0",signal&&ptBin&&etaCut&&centrality&&(!recoMatch || !algoCut));

  h->SetName(Form("h_%s_%d_%d",algo.data(),(int)PbPb,ptMin));
  h30->SetName(Form("h30_%s_%d_%d",algo.data(),(int)PbPb,ptMin));
  h20->SetName(Form("h20_%s_%d_%d",algo.data(),(int)PbPb,ptMin));
  h0->SetName(Form("h0_%s_%d_%d",algo.data(),(int)PbPb,ptMin));

  h->Add(h0);
  h->Add(h20);
  h->Add(h30);

  h0->Scale(1./h->Integral());
  h20->Scale(1./h->Integral());
  h30->Scale(1./h->Integral());
  h->Scale(1./h->Integral());
  
  cout<<"-------------------------"<<endl;
  cout<<"File : "<<filename.data()<<endl;
  cout<<algo.data()<<endl;
  cout<<etaMin[etaBin]<<" < |eta| < "<<etaMax[etaBin]<<endl;
  cout<<"pt ["<<ptMin<<","<<ptMax<<"]"<<endl;
  cout<<"Mean : "<<h->GetMean()<<" +- "<<h->GetMeanError()<<endl;
  cout<<"RMS  : "<<h->GetRMS()<<" +- "<<h->GetRMSError()<<endl;
  cout<<"-------------------------"<<endl;

  TLegend *t3 =new TLegend(0.58,0.63,0.82,0.94);
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);
  t3->AddEntry(h,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h->GetMean(),h->GetMeanError()),"");
  t3->AddEntry(h,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h->GetRMS(),h->GetRMSError()),"");

  //  outf->cd();
  //  h->Write();
  //  h0->Write();
  //  h20->Write();
  //  h30->Write();

  //  outf->Write();
  //  outf->Close();

  TH1D* hh[] = {h,h0,h20,h30};
  return hh;

}



