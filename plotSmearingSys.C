
#include "Analysis.h"



void plotSmearingSys(){

  bool pp = 1;

  mc = 1;

  int ipt = 3;
  int icent = 2;
  string var = "resp";
  var = "xj";

  TFile* inf0, *inf1, *inf2;
  if(pp){
    inf0 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c6_s8_n0.root");
    inf1 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c6_s10_n0.root");
    inf2 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c4_s8_n0.root");
  }else{
    inf0 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n14.root");
    inf1 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n16.root");
    inf2 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n18.root");
  }

  TTree* t0 = (TTree*)inf0->Get("t");
  TTree* t1 = (TTree*)inf1->Get("t");
  TTree* t2 = (TTree*)inf2->Get("t");

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  defineVariables(t0);
  defineVariables(t1);
  defineVariables(t2);

  TCut etaCut("abs(geneta) < 1.5");
  TCut patCut("patc*patpt > 40");
  TCut smCut("smpt > 40");
  TCut signal("gensubid == 0");
  TCut overlapCut("genbkgsumpt < 10");
  TCut ptBin(Form("genpt > %f && genpt < %f",ptBins[ipt],ptBins[ipt+1]));

  TCut centrality(Form("bin >= %d && bin < %d",centMinCoarse[icent],centMinCoarse[icent+1]));
  if(pp){
    icent = 0;
    centrality = "bin < 999";
  }

  TCut pthatCut(Form("pthat > 50"));

  TCut lead("pt1>100");
  TCut sublead("pt2>40");
  TCut smlead("smpt1>100");
  TCut smsublead("smpt2>40");

  TCut near("dphi < 1.0472");
  TCut away("dphi > 2.0944");

  TCut gennear("gendphi < 1.0472");
  TCut genaway("gendphi > 2.0944");

  TCut smaway("smdphi > 2.0944");


  TH1* h0, *h1, *h2;
  TLegend* leg;
  if(var == "xj" || var == "pt1"){
    h0 = get("sm"+var,"h0",t0,"",centrality&&pthatCut&&smlead&&smsublead&&smaway);
    h1 = get("sm"+var,"h1",t1,"",centrality&&pthatCut&&smlead&&smsublead&&smaway);
    h2 = get("sm"+var,"h2",t2,"",centrality&&pthatCut&&smlead&&smsublead&&smaway);

    //    h0 = get("pthat","h0",t,"",centrality&&pthatCut&&lead&&sublead&&away);
    //    h1 = get("pthat","h1",t,"",centrality&&pthatCut&&smlead&&smsublead&&genaway);

    h0->SetMaximum(0.4);
    leg = makeLegend(0.2,0.6,0.6,0.9);
  }

  if(var == "resp"){
    h0 = get("sm"+var,"h0",t0,"",signal&&etaCut&&smCut&&centrality&&ptBin&&overlapCut);
    h1 = get("sm"+var,"h1",t1,"",signal&&etaCut&&smCut&&centrality&&ptBin&&overlapCut);
    h2 = get("sm"+var,"h2",t2,"",signal&&etaCut&&smCut&&centrality&&ptBin&&overlapCut);

    h0->SetMaximum(0.3);
    leg = makeLegend(0.5,0.6,0.9,0.9);

  }


  h0->SetTitle(";x_{J} (smeared);Event Fraction");

  h1->SetMarkerStyle(24);
  h2->SetLineStyle(2);

  h0->Draw("hist");
  h1->Draw("same");
  h2->Draw("hist same");


  leg->AddEntry(h0,Form("%d-%d%%",int(centMinCoarse[icent]*0.5),int(centMinCoarse[icent+1]*0.5)),"");
  if(var == "resp"){
    leg->AddEntry(h0,Form("%d<p_{T}^{gen}<%d",int(ptBins[ipt]),int(ptBins[ipt+1])),"");
    leg->AddEntry(h0,Form("N_{max} = 14 GeV"),"l");

    leg->AddEntry(h0,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h0->GetMean(),h0->GetRMS()/h0->GetMean()),"");
    leg->AddEntry(h1,Form("N_{max} = 16 GeV"),"p");
    leg->AddEntry(h1,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h1->GetMean(),h1->GetRMS()/h1->GetMean()),"");

    leg->AddEntry(h2,Form("N_{max} = 18 GeV"),"l");
    leg->AddEntry(h2,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h1->GetMean(),h1->GetRMS()/h1->GetMean()),"");

  }

  if(!pp){

    leg->AddEntry(h0,Form("N_{max} = 14 GeV"),"l");
    leg->AddEntry(h0,Form("#LTx_{J}#GT : %0.3f",h0->GetMean()),"");

    leg->AddEntry(h1,Form("N_{max} = 16 GeV"),"p");
    leg->AddEntry(h1,Form("#LTx_{J}#GT : %0.3f",h1->GetMean()),"");


    leg->AddEntry(h2,Form("N_{max} = 18 GeV"),"l");
    leg->AddEntry(h2,Form("#LTx_{J}#GT : %0.3f",h2->GetMean()),"");

  }else{

    leg->AddEntry(h0,Form("C = 0.06, S = 0.08"),"l");
    leg->AddEntry(h0,Form("#LTx_{J}#GT : %0.3f",h0->GetMean()),"");

    leg->AddEntry(h1,Form("C = 0.06, S = 0.10"),"p");
    leg->AddEntry(h1,Form("#LTx_{J}#GT : %0.3f",h1->GetMean()),"");

    leg->AddEntry(h2,Form("C = 0.04, S = 0.08"),"l");
    leg->AddEntry(h2,Form("#LTx_{J}#GT : %0.3f",h2->GetMean()),"");

  }

  leg->Draw();

  c1->Print(Form("smearing_sys_%s_%s_%d.png",pp?"pp":"PbPb",var.data(),icent));

}

