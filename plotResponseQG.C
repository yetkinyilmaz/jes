
#include "Analysis.h"



void plotResponseQG(){

  bool pp = 0;
  int icent = 0;
  if(pp) icent = 0;

  int ijet = 2;
  int ipt = 3;

  mc = 1;


  TFile* inf;

  if(pp){
    inf = new TFile("/Users/yetkin/data/smear_v12/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c2_s6_n0.root");
  }else{
    inf = new TFile("/Users/yetkin/data/smear_v12/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n14.root");
  }

  TTree* t = (TTree*)inf->Get("t");
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  defineVariables(t);

  //  t->SetAlias("weight","pthatweight");
  string var = "jtresp";
  //  var = "pt1";
  //  var = "xj";


  TCut etaCut("abs(geneta) < 1.5");
  TCut refetaCut("abs(refeta) < 1.5");
  TCut patCut("patc*patpt > 30");
  TCut smCut("smpt > 30");
  TCut jtCut("jtc*jtpt > 30");

  TCut signal("gensubid == 0");
  TCut jtsignal("subid == 0");
  TCut overlapCut("genbkgsumpt < 10");
  TCut ptBin(Form("genpt > %f && genpt < %f",ptBins[ipt],ptBins[ipt+1]));
  TCut refptBin(Form("refpt > %f && refpt < %f",ptBins[ipt],ptBins[ipt+1]));

  TCut centrality(Form("bin >= %d && bin < %d",centMinCoarse[icent],centMinCoarse[icent+1]));
  TCut pthatCut(Form("pthat > 50"));

  TCut ijtlead("geneta == refeta1");
  TCut ijtsublead("geneta == refeta2");

  TCut ismlead("geneta == smeta1");
  TCut ismsublead("geneta == smeta2");


  TCut lead("pt1>100");
  TCut sublead("pt2>40");
  TCut smlead("smpt1>100");
  TCut smsublead("smpt2>40");

  TCut near("dphi < 1.0472");
  TCut away("dphi > 2.0944");

  TCut gennear("gendphi < 1.0472");
  TCut genaway("gendphi > 2.0944");

  TCut smnear("smdphi < 1.0472");
  TCut smaway("smdphi > 2.0944");

  TCut quark("abs(parton) < 9");
  TCut gluon("abs(parton) == 21");



  TH1* h0, *h1, *h2;
  TLegend* leg;


  Xbins = 60;
  Xmin = 0;
  Xmax = 3;


  h0 = get("jtresp","h0",t,"",jtsignal&&refetaCut&&jtCut&&refptBin&&centrality);
  h1 = get("jtresp","h1",t,"",jtsignal&&refetaCut&&jtCut&&refptBin&&centrality&&quark);
  h2 = get("jtresp","h2",t,"",jtsignal&&refetaCut&&jtCut&&refptBin&&centrality&&gluon);
  
  h0->SetMaximum(0.3);
  leg = makeLegend(0.5,0.6,0.9,0.9);





  h2->SetMarkerStyle(24);

  h0->Draw("hist");
  h1->Draw("same");
  h2->Draw("same");


  leg->AddEntry(h0,Form("%d-%d%%",int(centMinCoarse[icent]*0.5),int(centMinCoarse[icent+1]*0.5)),"");
  //  if(var == "resp" && ijet == 0) 
  leg->AddEntry(h0,Form("%d<p_{T}^{gen}<%d",int(ptBins[ipt]),int(ptBins[ipt+1])),"");
  if(var == "xj") leg->AddEntry(h0,"","");
  leg->AddEntry(h0,Form("all jets"),"l");
  leg->AddEntry(h0,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h0->GetMean(),h0->GetRMS()/h0->GetMean()),"");
  leg->AddEntry(h2,Form("gluon jets"),"p");
  leg->AddEntry(h2,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h2->GetMean(),h2->GetRMS()/h2->GetMean()),"");
  leg->AddEntry(h1,Form("quark jets"),"p");
  leg->AddEntry(h1,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h1->GetMean(),h1->GetRMS()/h1->GetMean()),"");
  leg->Draw();

  c1->Print(Form("quarkjets_closure_%s_%s_pt%d_cent%d.png",var.data(),pp?"pp":"PbPb",ipt,icent));


}

