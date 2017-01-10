#include "Analysis.h"


void plotJES_Correction(double cc = 0.99, double ss = -0.16, double nn = -1.8, string var = "xj", bool doMC = 0, bool doPP = 0){

  // most central shifts by 0.02 for double cc = 1.05, double ss = -1.8, double nn = 15.
  // shift by 0.01 for double cc = 0.99, double ss = -0.16, double nn = -1.8.

  double c = cc; 

  double s = ss;
  double n = nn;

  mc = doMC;
  pp = doPP;

  leadMin = 100;
  subMin = 30;

  string algo = "akPu4PF";
  if(pp) algo = "ak4PF";

  TChain* t = new TChain("nt");
  if(mc){
    t->Add(Form("%s/mc%sqcd%s_djt.root",dir.data(),pp?"pp":"Pb",algo.data()));
  }else{
    t->Add(Form("%s/dt%s%s_djt.root",dir.data(),pp?"ppjpf":"Pbj60",algo.data()));
  }


  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  
  defineVariables(t);
  h0 = get(var, "h0", t);

  correctPt(t,1,c,s,n);
  correctPt(t,2,c,s,n);
  h1 = get(var, "h1", t);

  //  rescalePt(t,2,2.-c,-s,-n);
  //  h2 = get(var, "h2", t);

  h1->SetMarkerStyle(24);
  //  h1->SetMarkerStyle(25);
  

  TLegend *t3;

  if(var == "xj"){
    t3 =new TLegend(0.22,0.69,0.52,0.94);
    h0->SetMaximum(0.35);
  }else{
    t3 =new TLegend(0.58,0.63,0.82,0.94);
  }

  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  t3->AddEntry(h0,Form("%s",mc?"MC":"Data"),"");
  t3->AddEntry(h0,Form("correction : %0.2f %s %0.2f/#sqrt{p_{T}} %s %0.2f/p_{T}",c,s>0?"+":"-",fabs(s),n>0?"+":"-",fabs(n)),"");
  t3->AddEntry(h0,Form("not applied #LTx_{J}#GT: %0.3f #pm %0.3f",h0->GetMean(),h0->GetMeanError()),"l");
  t3->AddEntry(h1,Form("applied      #LTx_{J}#GT: %0.3f #pm %0.3f",h1->GetMean(),h1->GetMeanError()),"p");
  //  t3->AddEntry(h2,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h2->GetMean(),h2->GetMeanError()),"p");

  h0->Draw("hist");
  h1->Draw("same");
  //  h2->Draw("same");
  t3->Draw();

  string label = "sub";

  c1->Print(Form("figure_sys_jes_corr_%s_%s_%s_%s_c%d_s%d_n%d.png",var.data(),label.data(),pp?"pp":"PbPb",mc?"mc":"data",int(c*100),abs(int(s)),abs(int(n))));

  //  TCanvas* c2 = new TCanvas("c2","c2",400,400);
  t3 =new TLegend(0.12,0.13,0.92,0.94);
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  //  t3->AddEntry(h0,Form("Scale factor : %0.2f",c),"");
  //  t3->AddEntry(h0,Form("Res. Corr. not applied"),"l");
  //  t3->AddEntry(h1,Form("Res. Corr. applied"),"p");
  //  t3->AddEntry(h0,Form("JES shift downward"),"p");

  //  t3->AddEntry(h2,Form("JES shift applied to both jets"),"p");

  // t3->Draw();

  c1->Print(Form("figure_sys_jes_corr_%s_%s_%s_%s_c%d_s%d_n%d.png",var.data(),label.data(),pp?"pp":"PbPb",mc?"mc":"data",int(c*100),abs(int(s)),abs(int(n))));

  //  c2->Print(Form("figure_sys_jes_corr_legend_c%d.png",int(c*100)));

}

