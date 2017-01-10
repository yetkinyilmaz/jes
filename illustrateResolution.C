#include "Analysis.h"


void illustrateResolution(){

  TFile* inf = new TFile("/Users/yetkin/data/smear_v8/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_n2.root");
  TTree* t = (TTree*)inf->Get("t");

  defineVariables(t);

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  Xbins = 48;
  Xmin = 0;
  Xmax = 1.2;


  TH1* h0 = get("genxj","h0",t,"","genpt1> 100 && genpt2 > 40 && gendphi > 2.0944");
  TH1* h1 = get("genxj","h1",t,"","pt1> 100 && pt2 > 40 && dphi > 2.0944");
  TH1* h2 = get("refxj","h1",t,"","pt1> 100 && pt2 > 40 && dphi > 2.0944");
  TH1* h3 = get("xj","h1",t,"","pt1> 100 && pt2 > 40 && dphi > 2.0944");


  //    TH1* h1 = get("genxj","h1",t,"","pt1> 100 && pt2 > 40 && dphi > 2.0944");

  //  TH1* h1 = get("xj","h1",t,"pt1> 100 && pt2 > 40 && dphi < 1.0472","pt1> 100 && pt2 > 40 && dphi > 2.0944");



  h0->SetMaximum(0.1);
  h1->SetMaximum(0.1);
  h2->SetMaximum(0.1);
  h3->SetMaximum(0.1);

  h0->Draw("hist");
  h1->Draw("same");

  TLegend *leg = makeLegend(0.26,0.63,0.52,0.94);
  //  leg->AddEntry(h0,Form("#LTx_{J}#GT: %0.3f",h0->GetMean()),"l");
  //  leg->AddEntry(h1,Form("#LTx_{J}#GT: %0.3f",h1->GetMean()),"p");
  leg->AddEntry(h0,"Events selected by gen-jets","l");
  leg->AddEntry(h1,"Events selected by reco-jets","p");
  leg->Draw();

  c1->Print("sketch_resolution_selection.png");

  h1->Draw("hist");
  h2->Draw("same");

  leg = makeLegend(0.26,0.63,0.52,0.94);
  leg->AddEntry(h0,"Jets sorted by gen-level p_{T}","l");
  leg->AddEntry(h1,"Jets sorted by reco-level p_{T}","p");
  leg->Draw();

  c1->Print("sketch_resolution_sorting.png");

  h2->Draw("hist");
  h3->Draw("same");

  leg = makeLegend(0.26,0.63,0.52,0.94);
  leg->AddEntry(h0,"gen-level p_{T} in final dijets","l");
  leg->AddEntry(h1,"reco-level p_{T} in final dijets","p");
  leg->Draw();

  c1->Print("sketch_resolution_pt.png");

  h0->Draw("hist");
  h3->Draw("same");

  leg = makeLegend(0.26,0.63,0.52,0.94);
  leg->AddEntry(h0,"fully gen-level","l");
  leg->AddEntry(h1,"fully reco-level","p");
  leg->Draw();

  c1->Print("sketch_resolution_full.png");



}



