#include "Analysis.h"

static bool shiftToReco = 0;
static bool useFits = 0;


TGraphErrors* analyzeEtaBin(int etaBin = 3, int centBin = 0, string sample = "qcd", bool resolution = 0){

  TF1* fGaus = new TF1("fGaus","gaus",0.8,1.3);
  fGaus->SetParLimits(1,0.95,1.15);

  int cone = 4;  int Nbins = 20;
  double ptMin = 0;
  double ptMax = 300;

  TGraphErrors* g = new TGraphErrors(Nbins);

  g->GetXaxis()->SetTitle("p_{T}^{gen}");
  g->GetYaxis()->SetTitle("#LTp_{T}^{reco}/p_{T}^{gen}#GT");

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  string dir0 = "puTowerExclLimitV3";

  TH1* h, *h0, *h20, *h20s, *h30;
  TH1** hh;

  bool superQuick = 1;

  TFile* inf = new TFile(file.data());
  TTree* t = (TTree*)inf->Get("t");
  defineVariables(t);

  TFile* outf = 0;
  if(!superQuick){
    outf = new TFile(Form("response_profiles_etaBin%d_centBin%d.root",etaBin,centBin),"recreate");
  }

  int ig = 0;
  for(int i = 0; i < Nbins; ++i){

    if(superQuick){
      hh = load(i,etaBin,centBin);
      h = hh[0];
      h0 = hh[1];
      h20 = hh[2];
      h20s = hh[3];
      h30 = hh[4];

    }else{
      hh[0] = get("resp",Form("hh_%d_%d_%d",i,etaBin,centBin),t,"",Form("weight*(gensubid == 0 && abs(geneta) < 1.5 && genpt > %f && genpt < %f && bin >= %d && bin < %d)",ptBins[i],ptBins[i+1],centBins[centBin],centBins[centBin+1]));

      h = hh[0];
      h0 = hh[0];
      h20 = hh[0];
      h20s = hh[0];
      h30 = hh[0];
    }

    double x = (ptBins[i] + ptBins[i+1])/2.;
    double ex = (ptBins[i+1] - ptBins[i])/2.;

    h = h30;

    //    h->Reset();
    //    h->Add(h20s);
    //    h->Add(h30);

    double mu = h->GetMean();
    double emu = h->GetMeanError();

    if(useFits){

      fGaus->SetParameters(h->GetMaximum(),h->GetMean(),h->GetRMS());

      h->Fit(fGaus,"R");
      mu = fGaus->GetParameter(1);
      emu = fGaus->GetParError(1);
    }

    if(shiftToReco) x = mu*x;

    double rescale = mu;
    rescale = 1;

    if(resolution){

      if(useFits){
	emu = fGaus->GetParError(2)/rescale;
	mu = fGaus->GetParameter(2)/rescale;
      }else{
	emu = h->GetRMSError()/rescale;
        mu = h->GetRMS()/rescale;
      }
    }

    c1->cd();
    h->Draw();
    fGaus->Draw("same");
    c1->Print(Form("fit_debug_cent%d_pt%d.png",centBin, i));


    if(h->Integral() > 0){
      g->SetPoint(ig,x,mu);
      g->SetPointError(ig,0,emu);
      ig++;
    }

    if(!superQuick){
      outf->cd();
      h->Write();
      h0->Write();
      h20->Write();
      h30->Write();
    }


  }

  g->Draw("Ap");

  TLegend *t3=new TLegend(0.44,0.62,0.92,0.94);
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  t3->AddEntry(g,"pp, pp-PF non-PU","l");

  if(1){
  if(centBin == 0) t3->AddEntry(g,"PbPb 0-10%, HI-PF PU","p");
  if(centBin == 1) t3->AddEntry(g,"PbPb 10-30%, HI-PF PU","p");
  if(centBin == 2) t3->AddEntry(g,"PbPb 30-100%, HI-PF PU","p");
  }
  //  t3->AddEntry(h0,"exclude R = 0.4","p");


  //  t3->AddEntry(g,Form("%0.1f<|#eta|<%0.1f",etaMinFine[etaBin],etaMaxFine[etaBin]),"");
  //  t3->Draw();

  c1->Print(Form("figure_response_%s_centBin%d_etaBin%d.png",sample.data(),centBin,etaBin));

  TCut centrality("hiBin < 200");

  if(1){
    centrality = Form("hiBin >= %d && hiBin < %d",centBin*10,(centBin+1)*10);
    if(centBin == 6) centrality = Form("hiBin >= %d && hiBin < %d",6*10,10*10);
    if(centBin == 7) centrality = Form("hiBin >= %d && hiBin < %d",10*10,14*10);
    if(centBin == 8) centrality = Form("hiBin >= %d && hiBin < %d",14*10,20*10);
  }else{
    if(centBin == 0) centrality = "hiBin < 20";
    if(centBin == 1) centrality = "hiBin >= 20 && hiBin < 60";
    if(centBin == 2) centrality = "hiBin >= 60";
  }


  cout<<"Centrality cut : "<<(const char*)centrality<<endl;

  if(!superQuick){
    outf->Write();
    outf->Close();
  }


  g->GetXaxis()->SetTitle("p_{T}^{gen}");
  g->GetYaxis()->SetTitle("#LTp_{T}^{reco}/p_{T}^{gen}#GT");

  return g;

}

void analyzeResponseEta(){

  if(0){

    analyzeEtaBin(0,8);

  }else{
    string sample = "qcd";
    for(int ieta = 0; ieta < 5; ++ieta){
    for(int icent = 9; icent >= 0; --icent){
	analyzeEtaBin(ieta,icent,sample);
      }
    }
  }


}
