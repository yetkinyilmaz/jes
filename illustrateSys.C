#include "Analysis.h"

void illustrateSys(){


  TF1* f0 = new TF1("f0","sqrt([0]*[0]+[1]*[1]/x/x)",40,200);
  f0->SetParameters(3.3,200);

  TF1* f1 = new TF1("f1","sqrt([0]*[0]+[1]*[1]/x)",40,200);
  f1->SetParameters(2,0);


  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  TH2D* pad = new TH2D("pad",";p_{T};uncertainty (%)",100,0,200,100,0,15);

  pad->Draw();

  f0->SetLineStyle(2);

  f0->Draw("same");
  f1->Draw("same");

  TLegend* leg = makeLegend(0.3,0.6,0.8,0.9);
  leg->AddEntry(f0,"total correlated uncertainty","l");
  leg->AddEntry(f0,"(applied to both jets simultaneously)","");

  leg->AddEntry(f0," ","");
  leg->AddEntry(f1,"total uncorrelated uncertainty","l");
  leg->AddEntry(f0,"(applied to only subleading jet)","");
  leg->Draw();


  c1->Print("uncertainty_cartoon.png");
}
