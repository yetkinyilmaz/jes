#include "analyzeResponseEta.C"

void plotPuUnderestimation(){

  TCanvas* can2 = new TCanvas("can2","can2",600,600);

  shiftToReco = 0;
  TGraph* g0 = analyzeEtaBin(0,0);
  g0->SetName("g0");
  shiftToReco = 1;
  TGraph* g1 = analyzeEtaBin(0,0);
  g1->SetName("g1");

  g0->SetMaximum(1.3);
  g0->SetMinimum(0.9);

  g0->SetMarkerStyle(24);
  g1->SetMarkerSize(0.8);

  can2->cd();
  g0->Draw("A p");
  //  g1->Draw("p same");

  TLine* line = new TLine(0,1,500,1);
  line->SetLineStyle(2);
  line->Draw();

  TF1* f = new TF1("f","1.-[0]*exp(-[1]*x)",30,500);
  f->SetParameters(0.08,0.01);
  f->SetLineColor(4);
  f->SetLineStyle(2);
  f->Draw("same");

  TF1* f1 = new TF1("f1","1.-[0]*exp(-[1]*x)+[2]/(x-[3])/(x-[3])",30,500);
  f1->SetParameters(0.08,0.01,230,15);
  f1->SetLineColor(3);
  f1->SetLineStyle(2);
  f1->Draw("same");


  TLegend *leg0 =new TLegend(0.58,0.43,0.82,0.64);
  leg0->SetFillColor(0);
  leg0->SetBorderSize(0);
  leg0->SetFillStyle(0);
  leg0->SetTextFont(43);
  leg0->SetTextSize(18);
  leg0->AddEntry(g0,"x axis not scaled","p");
  leg0->AddEntry(g1,"x axis scaled","p");

  //  leg0->Draw();
  can2->cd();
  can2->Print("axis_scaling_central.png");


}
