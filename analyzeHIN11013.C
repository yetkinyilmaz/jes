#include "compareToPaper.C"





void plotDataVsMC(bool do2011 = 0,
		  //		  string var = "acos(cos(dphi))",
		  string var = "xj",
		  string algo = "akPu3PF"
		  ){

  if(algo=="akPu4PF")
  leadMin = 100;
  subMin = 40;

  pp = 1;

  blue = TColor::GetColor(152,235,230);

  TH1::SetDefaultSumw2();

  if(pp) algo = "ak3PF";
  
  TChain* t = new TChain("nt");
  TChain* tmc = new TChain("nt");

  tmc->Add(Form("/data_CMS/cms/lisniak/bjet2015/mc%sqcd%s_djt.root",pp?"pp":"Pb",algo.data()));
  t->Add(Form("/data_CMS/cms/lisniak/bjet2015/dt%s%s_djt.root",pp?"ppjpf":"Pbj60",algo.data()));

  TCanvas* c1 = new TCanvas("c1","c1",600,600);  

  TH1 *h, *hmc;

  if(do2011){
    mc = 0;
    h = getPaperPlot(var);
    mc = 1;
    hmc = getPaperPlot(var);
  }else{
    mc = 0;
    h = get(var, "h", t);
    mc = 1;
    hmc = get(var, "h", tmc);
  }

  if(var == "xj"){
    hmc->SetTitle(";x_{J};Event Fraction");
    hmc->SetMaximum(0.3);
  }

  if(var == "bkg"){
    //    hmc->SetTitle(";x_{J};Event Fraction");
    hmc->SetMaximum(0.25);
  }

  hmc->Draw("hist");
  h->Draw("same");

  TLegend *t3=new TLegend(0.18,0.62,0.62,0.94);
  if(var == "bkg" || var == "aj") t3=new TLegend(0.48,0.62,0.92,0.94);

    if(do2011){
      t3->AddEntry(h,"2011 - 2.76 TeV","");
    }else{
      t3->AddEntry(h,Form("2015 - 5.02 TeV"),"");
    }
 
    if(var=="xj"){   
      if(do2011 && pp){
	t3->AddEntry(h,Form("pp data mean : %0.3f #pm %0.3f (stat)",h->GetMean(),h->GetMeanError()),"p");
	t3->AddEntry(hmc,Form("70-100%% MC mean : %0.3f #pm %0.3f (stat)",hmc->GetMean(),hmc->GetMeanError()),"l");
      }else{
	t3->AddEntry(h,Form("data mean : %0.3f #pm %0.3f (stat)",h->GetMean(),h->GetMeanError()),"p");
	t3->AddEntry(hmc,Form("MC mean : %0.3f #pm %0.3f (stat)",hmc->GetMean(),hmc->GetMeanError()),"l");
      }
      t3->AddEntry(h,"120<p_{T,1}<150","");

    }else{
      if(do2011 && pp){
        t3->AddEntry(h,Form("pp data"),"p");
        t3->AddEntry(hmc,Form("70-100%% MC"),"l");
      }else{
        t3->AddEntry(h,Form("data"),"p");
        t3->AddEntry(hmc,Form("MC"),"l");
      }
    }

    t3->AddEntry(h,algo.data(),"");

  if(pp){

  }else if(peri){
    t3->AddEntry(h,"70-100%","");
  }else if(density){
    t3->AddEntry(h,"2.76(0-20%) vs 5.02(10-30%)","");
    t3->AddEntry(h,"(similar density)","");
  }else{
    if(var=="xj") t3->AddEntry(h,"0-20%","");
  }

  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  t3->Draw();


  string sample(Form("%s_%s",pp?"pp":"PbPb",peri?"peripheral":"central"));
  if(pp) sample = "pp";
  if(density) sample = "density";

  c1->Print(Form("figure_%d_%s_%s_%s.png",do2011?2011:2015,var.data(),algo.data(),sample.data()));



}


void analyzeHIN11013(){

  //  plotDataVsMC();
  plotDataVsMC(0,"aj");
  plotDataVsMC(1,"aj");

  //  plotDataVsMC("dphi");
  //  plotDataVsMC("acos(cos(dphi))")

}
