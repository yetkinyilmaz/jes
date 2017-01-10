#include "Analysis.h"
#include "Corrections.h"

void plotCorrection(){

  Corrections corr;


  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  TH2D* pad = new TH2D("pad",";centrality % #times 2;correction parameter c_{3} [GeV^{2}]",100,0,200,100,0,300);

  pad->Draw();
  corr.fcent->Draw("same");


  c1->Print("pu_correction_centrality.png");

}
