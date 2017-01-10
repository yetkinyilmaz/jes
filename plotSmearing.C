
#include "Analysis.h"



void plotSmearingSample(
		  bool pp = 0,
		  int icent = 2,
		  string var = "xj",
		  int ipt = 3
			){

  int ijet = 1;

  if(pp) icent = 0;
  mc = 1;

  bool qgStudy = 0;
  bool subtract = 0;

  TFile* inf;

  if(pp){
    inf = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_pp_ak4PF_corpt_c6_s8_n0.root");
  }else{
    inf = new TFile("/Users/yetkin/data/smear_v15/fullsim_qcd_5020GeV_dijets_pythia6_pthat0_PbPb_akPu4PF_corpt_c6_s10_n16.root");
  }

  TTree* t = (TTree*)inf->Get("t");
  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  defineVariables(t);

  //  t->SetAlias("weight","pthatweight");

  //  var = "respcorr";

  //  var = "pt1";
  //  var = "xj";


  TCut etaCut("abs(geneta) < 1.5");
  TCut patCut("patc*patpt > 40");
  TCut smCut("smpt > 40");
  TCut signal("gensubid == 0");
  TCut overlapCut("genbkgsumpt < 10");
  TCut ptBin(Form("genpt > %f && genpt < %f",ptBins[ipt],ptBins[ipt+1]));

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

  TCut subtraction = centrality&&pthatCut&&lead&&sublead&&near;
  TCut smsubtraction = centrality&&pthatCut&&smlead&&smsublead&&smnear;


  TCut weight("weight");

  if(!subtract){
    subtraction = "";
    smsubtraction = "";
  }

  TH1* h0, *h1;
  TLegend* leg;
  if(var == "xj" || var == "pt1" || var == "refxj"){

    Xbins = 12;
    Xmin = 0;
    Xmax = 1.2;



    if(qgStudy){

      h0 = get(var,"h0",t,"",centrality&&pthatCut&&lead&&sublead&&away);
      h1 = get(var,"h1",t,"",centrality&&pthatCut&&lead&&sublead&&away&&gluon2);

    }else{


      h0 = get(var,"h0",t,subtraction,
	       centrality&&pthatCut&&lead&&sublead&&away);

      h1 = get("sm"+var,"h1",t,smsubtraction,
	       centrality&&pthatCut&&smlead&&smsublead&&smaway);

      //      h0 = get(var,"h0",t,"",
      //               centrality&&pthatCut&&lead&&sublead&&away&&"subid2 == 0");

      //      h1 = get("sm"+var,"h1",t,"",
      //               centrality&&pthatCut&&smlead&&smsublead&&smaway);


      //      check if JES can explain this
      //      h1 = get("1.-abs(1.-smxj*0.98)","h1",t,smsubtraction,
      //	       centrality&&pthatCut&&smlead&&smsublead&&smaway);

      //      rescalePt(t,2,20,0,0);
      //      h0 = get("sc"+var,"h0",t,subtraction,
      //	   centrality&&pthatCut&&sclead&&scsublead&&away);

    }

    h0->SetMaximum(0.6);
    leg = makeLegend(0.2,0.6,0.6,0.9);

  }

  if(var == "pthat" || var == "pthat"){

    h0 = get("pthat","h0",t,"",centrality&&pthatCut&&lead&&sublead&&away);
    h1 = get("pthat","h1",t,"",centrality&&pthatCut&&smlead&&smsublead&&genaway);


    h0->SetMaximum(0.6);
    leg = makeLegend(0.2,0.6,0.6,0.9);

  }


  if(var == "resp" || var == "resp1" || var == "resp2" || var == "resp12" || var == "resp21"){

    Xbins = 60;
    Xmin = 0;
    Xmax = 3;

  }

  if(var == "respcorr"){

    h0 = new TH2D("h0","",40,0,2,40,0,2);
    h1 = new TH2D("h1","",40,0,2,40,0,2);

    h0 = new TProfile("h0","",40,0,2);
    h1 = new TProfile("h1","",40,0,2);


    t->Draw("resp2:resp1>>h0",weight*(centrality&&lead&&sublead));
    t->Draw("smresp2:smresp1>>h1",weight*(centrality&&smlead&&smsublead));

    //    h0->Scale(1./h0->Integral());
    //    h1->Scale(1./h1->Integral());

    h1->SetLineColor(2);

  }


   if(var == "resp"){
    h0 = get(var,"h0",t,"",signal&&etaCut&&patCut&&centrality&&ptBin&&overlapCut);
    h1 = get("sm"+var,"h1",t,"",signal&&etaCut&&smCut&&centrality&&ptBin&&overlapCut);
  }

  if(var == "resp1"){
    h0 = get(var,"h0",t,"",centrality&&lead);
    h1 = get("sm"+var,"h1",t,"",centrality&&smlead);
  }

  if(var == "resp2"){
    h0 = get(var,"h0",t,"",centrality&&lead&&sublead);
    h1 = get("sm"+var,"h1",t,"",centrality&&smlead&&smsublead);
  }

  if(var == "resp12" || var == "resp21"){


    if(0){
    h0 = get(var,"h0",t,centrality&&lead&&sublead&&near,
	     centrality&&lead&&sublead&&away);
    h1 = get("sm"+var,"h1",t,centrality&&smlead&&smsublead&&near,
	     centrality&&smlead&&smsublead&&away);
    }

    h0 = get(var,"h0",t,"",
             centrality&&lead&&sublead&&away&&"subid2 == 0");
    h1 = get("sm"+var,"h1",t,"",
             centrality&&smlead&&smsublead&&away);

  }


  if(var == "resp" || var == "resp1" || var == "resp2" || var == "resp12" || var == "resp21"){
    h0->SetMaximum(0.2);
    if(pp) h0->SetMaximum(0.3);
  }


  leg = makeLegend(0.5,0.6,0.9,0.9);

  h1->SetMarkerStyle(24);

  if(0 && var == "respcorr"){
    //    c1->SetLogz();
    h0->SetMaximum(0.6);
    h0->SetMinimum(0.005);
    h1->SetMaximum(0.6);
    h1->SetMinimum(0.005);

    h0->Draw("box");
    h1->Draw("box SAME");
  }else{
    h0->Draw("hist");
    h1->Draw("same");
  }

  leg->AddEntry(h0,Form("%d-%d%%",int(centMinCoarse[icent]*0.5),int(centMinCoarse[icent+1]*0.5)),"");
  if(var == "resp") leg->AddEntry(h0,Form("%d<p_{T}^{gen}<%d",int(ptBins[ipt]),int(ptBins[ipt+1])),"");
  if(var == "resp1" ||  var == "resp12") leg->AddEntry(h0,"Leading jet (p_{T} > 100)","");
  if(var == "resp2" ||  var == "resp21") leg->AddEntry(h0,"Subeading jet (p_{T} > 40)","");

  if(var == "xj") leg->AddEntry(h0,"","");
  leg->AddEntry(h0,Form("Full Simulation"),"l");
  leg->AddEntry(h0,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h0->GetMean(),h0->GetRMS()/h0->GetMean()),"");
  leg->AddEntry(h1,Form("Smearing"),"p");
  leg->AddEntry(h1,Form("#mu  : %0.3f  #sigma/#mu : %0.3f",h1->GetMean(),h1->GetRMS()/h1->GetMean()),"");
  leg->Draw();

  c1->Print(Form("smearing_closure_%s_%s_pt%d_cent%d.png",var.data(),pp?"pp":"PbPb",ipt,icent));


}


void plotSmearing(){

  // plotSmearingSample(pp,icent,var,ipt);
  plotSmearingSample(1,0,"xj",0);
  plotSmearingSample(0,0,"xj",0);
  plotSmearingSample(0,1,"xj",0);
  plotSmearingSample(0,2,"xj",0);

  if(1){
  plotSmearingSample(1,0,"resp",3);
  plotSmearingSample(1,0,"resp",9);
  plotSmearingSample(1,0,"resp12",0);
  plotSmearingSample(1,0,"resp21",0);


  plotSmearingSample(0,0,"resp",3);
  plotSmearingSample(0,0,"resp",9);
  plotSmearingSample(0,0,"resp12",0);
  plotSmearingSample(0,0,"resp21",0);

  plotSmearingSample(0,1,"resp",3);
  plotSmearingSample(0,1,"resp",9);
  plotSmearingSample(0,1,"resp12",0);
  plotSmearingSample(0,1,"resp21",0);

  plotSmearingSample(0,2,"resp",3);
  plotSmearingSample(0,2,"resp",9);
  plotSmearingSample(0,2,"resp12",0);
  plotSmearingSample(0,2,"resp21",0);
  }

}
