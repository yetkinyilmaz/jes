
#include "Analysis.h"



void plotEfficiency(){

  mc = 1;
  bool pp = 0;

  int ipt = 3;
  int icent = 0;

  TFile* inf;
  if(pp){
    inf = new TFile("/Users/yetkin/data/smear_v9/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c4_s9_n0.root");
  }else{
    inf = new TFile("/Users/yetkin/data/smear_v14/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n16.root");
  }
  TTree* t = (TTree*)inf->Get("t");
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  defineVariables(t);
  string var = "genpt";


  TCut etaCut("abs(geneta) < 1.5");
  TCut patCut("patc*patpt > 30");
  TCut smCut("smpt > 30");
  TCut recoCut("patc*recopt > 30");

  TCut signal("gensubid == 0");
  TCut overlapCut("genbkgsumpt < 10");
  TCut ptBin(Form("genpt > %f && genpt < %f",ptBins[ipt],ptBins[ipt+1]));

  TCut centrality(Form("bin >= %d && bin < %d",centMinCoarse[icent],centMinCoarse[icent+1]));

  if(pp) centrality = "bin < 2000";
  TCut pthatCut(Form("pthat > 50"));

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

  Xbins = 48;
  Xmax = 120;
  Xmin = 0;

  TH1* h0, *h1, *hs0, *hs1, *h2;
  TLegend* leg;

  //  patCut = recoCut;

  h0 = get(var,"h0",t,"",signal&&etaCut&&centrality&&overlapCut);
  h1 = get(var,"h1",t,"",signal&&etaCut&&patCut&&centrality&&overlapCut);
  h2 = get(var,"h2",t,"",signal&&etaCut&&recoCut&&centrality&&overlapCut);

  hs0 = get(var,"hs0",t,"",signal&&etaCut&&centrality&&overlapCut);
  hs1 = get(var,"hs1",t,"",signal&&etaCut&&smCut&&centrality&&overlapCut);


  h2->Divide(h2,h0,1,1,"B");
  h1->Divide(h1,h0,1,1,"B");
  hs1->Divide(hs1,hs0,1,1,"B");
  h0->SetMaximum(0.3);
  leg = makeLegend(0.5,0.6,0.9,0.9);

  hs1->SetMarkerStyle(24);

  h1->Draw("hist");
  hs1->Draw("same");
  //  h2->Draw("same");


  drawLine(120);

  leg->AddEntry(h0,Form("%d-%d%%",int(centMinCoarse[icent]*0.5),int(centMinCoarse[icent+1]*0.5)),"");

  if(var == "resp") leg->AddEntry(h0,Form("%d<p_{T}^{gen}<%d",int(ptBins[ipt]),int(ptBins[ipt+1])),"");
  if(var == "xj") leg->AddEntry(h0,"","");
  leg->AddEntry(h1,Form("Full Simulation"),"l");
  //  leg->AddEntry(h0,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h0->GetMean(),h0->GetRMS()/h0->GetMean()),"");
  leg->AddEntry(hs1,Form("Smearing"),"p");
  //  leg->AddEntry(h1,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h1->GetMean(),h1->GetRMS()/h1->GetMean()),"");
  leg->Draw();

  c1->Print(Form("jet_efficiency_%s_%d.png",pp?"pp":"PbPb",icent));

}

