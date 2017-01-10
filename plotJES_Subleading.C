#include "Analysis.h"



void plotJES_Subleading(double cc = 1.02, double ss = 0, double nn = 0, string var = "xj", bool doMC = 0, bool doPP = 0){


  double c = cc; 

  double s = ss;
  double n = nn;

  mc = doMC;
  pp = doPP;

  leadMin = 100;
  subMin = 40;

  string algo = "akPu4PF";
  if(pp) algo = "ak4PF";

  TCut dijet(Form("(pt1> %d && pt2 > %d && acos(cos(dphi)) > 2.0944)",leadMin,subMin));
  TCut subtraction(Form("(pt1> %d && pt2 > %d && acos(cos(dphi)) < 1.0472)",leadMin,subMin));



  TChain* t = new TChain("nt");
  if(mc){
    t->Add(Form("%s/mc%sqcd%s_djt.root",dir.data(),pp?"pp":"Pb",algo.data()));
  }else{
    t->Add(Form("%s/dt%s%s_djt.root",dir.data(),pp?"ppjpf":"Pbjcl",algo.data()));
  }


  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  
  defineVariables(t);
  //  rescalePt(t,1,1,0,0);
  h0 = get(var, "h0", t, subtraction, dijet);

  rescalePt(t,2,c,s,n);
  h1 = get(var, "h1", t, subtraction, dijet);

  rescalePt(t,2,2.-c,-s,-n);
  h2 = get(var, "h2", t, subtraction, dijet);

  //  h1->SetMarkerStyle(24);
  h1->SetMarkerStyle(25);
  

  TLegend *t3;

  if(var == "xj"){
    t3 =new TLegend(0.22,0.63,0.52,0.94);
    h0->SetMaximum(0.42);
  }else{
    t3 =new TLegend(0.58,0.63,0.82,0.94);
  }

  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  t3->AddEntry(h0,Form("%s",mc?"MC":"Data"),"");
  t3->AddEntry(h0,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h0->GetMean(),h0->GetMeanError()),"l");
  t3->AddEntry(h1,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h1->GetMean(),h1->GetMeanError()),"p");
  t3->AddEntry(h2,Form("#LTx_{J}#GT: %0.3f #pm %0.3f",h2->GetMean(),h2->GetMeanError()),"p");

  h0->Draw("hist");
  h1->Draw("same");
  h2->Draw("same");


  t3 = makeLegend(0.22,0.54,0.47,0.9);
  t3->AddEntry(h0,Form("%s",mc?"MC":"Data 0-10%"),"");

  t3->AddEntry(h0,Form("Scale factor : %0.0f%%",-(1.-c)*100.),"");
  t3->AddEntry(h0,Form("not applied"),"l");
  t3->AddEntry(h0,Form("#LTx_{J}#GT: %0.3f",h0->GetMean()),"");
  t3->AddEntry(h1,Form("upward"),"p");
  t3->AddEntry(h1,Form("#LTx_{J}#GT: %0.3f",h1->GetMean()),"");
  t3->AddEntry(h0,Form("downward"),"p");
  t3->AddEntry(h2,Form("#LTx_{J}#GT: %0.3f",h2->GetMean()),"");
  t3->Draw();



  string label = "sub";

  c1->Print(Form("figure_sys_jes_pt_%s_%s_%s_%s_c%d_s%d_n%d.png",var.data(),label.data(),pp?"pp":"PbPb",mc?"mc":"data",int(c*100),abs(int(s)),abs(int(n))));

}

