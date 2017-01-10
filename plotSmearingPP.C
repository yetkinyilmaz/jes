
#include "Analysis.h"



void plotSmearingPP(){
  string var = "resp2";
  var = "xj";

  bool pp = 0;
  int icent = 0;

  int ijet = 1;
  int ipt = 3;

  if(pp) icent = 0;
  mc = 1;

  bool qgStudy = 0;
  bool subtract = 0;

  TFile* inf0, *inf1, *inf2;

  inf0 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c6_s8_n0.root");
  inf1 = new TFile("/Users/yetkin/data/smear_v17/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c6_s8_n16_smeared.root");
  inf2 = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n16.root");

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
  TCut pthatCut(Form("pthat > 50"));

  TCut lead("pt1>100");
  TCut sublead("pt2>40");
  TCut smlead("smpt1>100");
  TCut smsublead("smpt2>40");

  TCut sclead("smpt1>100");
  TCut scsublead("smpt2>40");

  TCut quark1("abs(parton1) < 9");
  TCut gluon1("abs(parton1) == 21");

  TCut quark2("abs(parton2) < 9");
  TCut gluon2("abs(parton2) == 21");


  TCut smquark1("abs(smparton1) < 9");
  TCut smgluon1("abs(smparton1) == 21");

  TCut smquark2("abs(smparton2) < 9");
  TCut smgluon2("abs(smparton2) == 21");

  TCut quark12 = quark1 && quark2;
  TCut smquark12 = smquark1 && smquark2;

  TCut gluon12 = gluon1 && gluon2;
  TCut smgluon12 = smgluon1 && smgluon2;

  TCut near("dphi < 1.0472");
  TCut away("dphi > 2.0944");

  TCut gennear("gendphi < 1.0472");
  TCut genaway("gendphi > 2.0944");

  TCut smnear("smdphi < 1.0472");
  TCut smaway("smdphi > 2.0944");

  TCut xjbias("genpt1 == refpt1 && genpt2 == refpt2 && genxj < 0.9");
  TCut smxjbias("genpt1 == smrefpt1 && genpt2 == smrefpt2 && genxj < 0.9");

  TCut subtraction = pthatCut&&lead&&sublead&&near;
  TCut smsubtraction = pthatCut&&smlead&&smsublead&&smnear;


  TCut weight("weight");

  if(!subtract){
    subtraction = "";
    smsubtraction = "";
  }

  TH1* h0, *h1, *h2;
  TLegend* leg;
  if(var == "xj" || var == "pt1" || var == "refxj"){

      h0 = get(var,"h0",t0,pthatCut&&lead&&sublead&&near,
	       pthatCut&&lead&&sublead&&away);

      h1 = get(var,"h1",t1,centrality&&pthatCut&&lead&&sublead&&near,
               centrality&&pthatCut&&lead&&sublead&&away);

      h2 = get(var,"h2",t2,centrality&&pthatCut&&lead&&sublead&&near,
               centrality&&pthatCut&&lead&&sublead&&away);

    h0->SetMaximum(0.6);
    leg = makeLegend(0.2,0.6,0.6,0.9);

  }


  leg = makeLegend(0.5,0.6,0.9,0.9);

  h0->SetLineStyle(2);
  h1->SetMarkerStyle(24);

    h0->Draw("hist");
    h1->Draw("same");
    h2->Draw("hist same");


  leg->AddEntry(h0,Form("%d-%d%%",int(centMinCoarse[icent]*0.5),int(centMinCoarse[icent+1]*0.5)),"");
  if(var == "resp" && ijet == 0) leg->AddEntry(h0,Form("%d<p_{T}^{gen}<%d",int(ptBins[ipt]),int(ptBins[ipt+1])),"");
  if(var == "xj") leg->AddEntry(h0,"","");
  leg->AddEntry(h0,Form("Sim Reco pp"),"l");
  leg->AddEntry(h0,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h0->GetMean(),h0->GetRMS()/h0->GetMean()),"");
  leg->AddEntry(h2,Form("Sim Reco PbPb"),"l");
  leg->AddEntry(h2,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h2->GetMean(),h2->GetRMS()/h2->GetMean()),"");
  leg->AddEntry(h1,Form("Smeared Reco pp"),"p");
  leg->AddEntry(h1,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h1->GetMean(),h1->GetRMS()/h1->GetMean()),"");

  leg->Draw();

  c1->Print(Form("smear_pp_closure_%s_%s_pt%d_cent%d.png",var.data(),pp?"pp":"PbPb",ipt,icent));


}

