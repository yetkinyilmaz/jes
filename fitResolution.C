#include "Analysis.h"
#include "Response.h"
#include <iostream>
using namespace std;

static double centCenters[] = {4.5, 14.5, 24.5, 34.5, 44.5, 54.5, 69.5, 89.5, 119.5, 169.5};

void fitResolution(){
  bool pp = 0;

  file = "/Users/yetkin/data/smear_v10/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c5_s11_n16.root";
  if(pp) file = "/Users/yetkin/data/smear_v8/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_n2.root";

  TH1::SetDefaultSumw2();
  useFits = 1;

  string dummy;
  shiftToReco = 0;

  int plotPar = 2;

  TGraphErrors* g[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  TF1* f[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  TF1* fex[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  int N = 10;
  if(pp) N = 1;

  int etaBin = 0;

  TGraphErrors* gFit = new TGraphErrors(N);

  TCanvas* cc1 = new TCanvas("cc1","cc1",600,600);
  TH2D* pad = new TH2D("pad","",60,0,600,200,0,0.35);
  pad->SetTitle(";p_{T}^{gen};#sigma/#mu(p_{T}^{reco}/p_{T}^{gen})");

  pad->Draw();

  string formula = "sqrt(pow([0],2)+pow([1]/sqrt(x),2)+pow([2]/x,2))";

  for(int i = N-1; i >= 0; --i){

    g[i] = analyzeEtaBin(etaBin,i,"qcd",1);
    g[i]->SetName(Form("g_%d",i));
    g[i]->SetMarkerStyle(24);

    g[i]->SetMarkerColor(int(i/3.)+1);
    g[i]->SetLineColor(int(i/3.)+1);
    if(i == 9){
      g[i]->SetMarkerColor(12);
      g[i]->SetLineColor(12);
    }
    double xMin,xMax,yMin,yMax;
    xMin = 50;
    xMax = 400;

    if(i != N-1){
      xMax = 150;
    }

    f[i] = new TF1(Form("f_%d",i),formula.data(),xMin,xMax);
    fex[i] = new TF1(Form("fex_%d",i),formula.data(),0,600);
    fex[i]->SetLineStyle(2);

    f[i]->SetParLimits(0,0.03,0.1);
    f[i]->SetParLimits(1,0.8,2.1);
    f[i]->SetParLimits(2,0,60);


    double C = 0.045;
    double S = 0.95;
    double N = 0;

    // pp reco
    // 2016.07.04
    C = 0.045;
    S = 0.95;
    N = 0;

    // 2016.07.05
    C = 0.06;
    S = 0.8;
    N = 0;

    //    f[i]->FixParameter(0,C);
    //    f[i]->FixParameter(1,S);
    //    f[i]->FixParameter(2,N);


    if(!pp){
      f[i]->SetParLimits(0,C,0.1);
      f[i]->SetParLimits(1,S,2.1);
      f[i]->SetParLimits(2,0,30);

      // HI reco
      // 2016.07.04
      C = 0.05;
      S = 1.1;

      // 2016.07.05
      C = 0.06;
      S = 1.0;

      f[i]->FixParameter(0,C);
      f[i]->FixParameter(1,S);
    }

    if(i != N-1){
      //      f[i]->FixParameter(0,C);
      //      f[i]->FixParameter(1,S);
    }

    f[i]->SetLineColor(int(i/3.)+1);
    fex[i]->SetLineColor(int(i/3.)+1);

    if(i == 9){
      f[i]->SetLineColor(12);
      fex[i]->SetLineColor(12);
    }

    cc1->cd();
    pad->Draw();
    g[i]->Draw("p same");
    g[i]->Fit(f[i],"R0");
    g[i]->Fit(f[i],"R0");

    fex[i]->SetParameters(f[i]->GetParameters());

    f[i]->Draw("same");
    fex[i]->Draw("same");

    gFit->SetPoint(i,centCenters[i],f[i]->GetParameter(plotPar));
    gFit->SetPointError(i,0,f[i]->GetParError(plotPar));

    if(!pp) cc1->Print(Form("resolution_%d.png",i));
    //    cin>>dummy;
  }


  pad->Draw();

  TLegend* leg = makeLegend();
  for(int i = N-1; i >= 0; i -= 3){
    g[i]->Draw("p same");
    f[i]->Draw("same");
    fex[i]->Draw("same");
    if(!pp)leg->AddEntry(g[i],Form("%d - %d%%",centBins[i]/2,centBins[i+1]/2),"p");
  }

  leg->Draw();
  cc1->Print(Form("resolution_%s_all.png",pp?"pp":"PbPb"));


  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  gFit->SetTitle("");
  gFit->GetXaxis()->SetTitle("centrality % #times 2");
  gFit->GetYaxis()->SetTitle("N_{fit}");

  gFit->Draw("Ap");

  TF1* fPar;


  fPar = new TF1("fPar","pol1",0,120);
  fPar1 = new TF1("fPar1","pol1",0,120);
  fPar2 = new TF1("fPar2","pol1",0,120);
  fPar->SetParameters(15,-0.1);
  fPar1->SetParameters(15,-0.1);
  fPar2->SetParameters(15,-0.1);


  gFit->Fit("fPar","R");
  gFit->SetName("gFit");

  //  fPar1->SetParameter(1,fPar->GetParameter(1));
  //  fPar2->SetParameter(1,fPar->GetParameter(1));

  fPar1->SetParameter(0,fPar->GetParameter(0)-2.);
  fPar2->SetParameter(0,fPar->GetParameter(0)+2.);
  fPar1->SetLineStyle(7);
  fPar2->SetLineStyle(7);

  fPar1->SetLineWidth(1);
  fPar2->SetLineWidth(1);

  //  fPar1->Draw("same");
  //  fPar2->Draw("same");

  gFit->Write();
  if(!pp) c2->Print("jec_vs_cent.png");

}
