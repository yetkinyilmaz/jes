
static string algo = "akVs4PF";


TH1* plot(bool mc = 0, bool s2760 = 1, int mode = 0){


  TFile* inf;
  
  if(s2760){
    if(mc){
      inf = TFile::Open(Form("/data_CMS/cms/yilmaz/dijet/test3/minbias_mc_2760GeV_dijets_%s_file0.root",algo.data()));
    }else{
      inf = TFile::Open(Form("/data_CMS/cms/yilmaz/dijet/test7/minbias_data_2760GeV_dijets_%s_merged.root",algo.data()));
    }
  }else{
    if(mc){
      inf = TFile::Open(Form("/data_CMS/cms/yilmaz/dijet/test3/minbias_mc_5020GeV_dijets_%s.root",algo.data()));
    }else{
      inf = TFile::Open(Form("/data_CMS/cms/yilmaz/dijet/test4/minbias_data_5020GeV_dijets_%s_merged.root",algo.data()));
      //      inf = TFile::Open(Form("/data_CMS/cms/yilmaz/dijet/test3/minbias_data_5020GeV_dijets_%s_merged.root",algo.data()));

    }
  }
  
  TTree* t = (TTree*)inf->Get(Form("t"));

  TCut selection("");

  TH1* h;

  if(mode == 1) h = new TH2D("h",";centrality;leading fluctuation p_{T}",200,0,200,150,0,150);
  else h = new TH1D("h",";leading fluctuation p_{T};event fraction",200,0,200);

  TProfile* p = new TProfile("p",";centrality;leading jet p_{T}",200,0,200);


  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  string plotname = "centrality";
  if(mode == 1){
    t->Draw("pt1:bin>>h",selection,"colz");
    plotname = "scatter";
  }

  if(mode == 0){
    t->Draw("pt1>>h",selection&&"bin < 20","");
    plotname = "projection";
    h->Scale(1./h->Integral());
  }

  TLegend *leg=new TLegend(0.3,0.78,0.87,0.85);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(28);
  leg->AddEntry(h,Form("%s %s TeV",mc?"Hydjet Drum":"Data",s2760?"2.76":"5.02"),"");
  leg->Draw();

  c1->Print(Form("figure_fluctuation_%s_minbias_%s_%dGeV_%s.png",plotname.data(),mc?"mc":"data",s2760?2760:5020,algo.data()));
  c1->SetLogz();

  c1->Print(Form("figure_fluctuation_%s_minbias_%s_%dGeV_%s_log.png",plotname.data(),mc?"mc":"data",s2760?2760:5020,algo.data()));


  if(mode == 2){
    t->Draw("pt1:bin>>p",selection,"prof same");

    leg->Draw();
    c1->Print(Form("figure_fluctuation_%s_minbias_%s_%dGeV_%s_prof.png",plotname.data(),mc?"mc":"data",s2760?2760:5020,algo.data()));

  }


  if(mode == 0){
    h->SetMaximum(0.055);
    //    h->SetMinimum(0.);
  }

  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  return h;

}


void compare(bool sDep = 1, bool s2760 = 1, bool mc = 0){

  TH1::SetDefaultSumw2();

  //  plot(mc,s2760,mode)

  TH1* h0, *h1;

  if(sDep){
    h0 = plot(mc,1);
    h1 = plot(mc,0);
  }else{
    h0 = plot(1,s2760);
    h1 = plot(0,s2760);
  }

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  h0->Draw("hist");
  h1->Draw("same");

  TLegend *leg=new TLegend(0.6,0.62,0.87,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);

  if(sDep){

    if(mc) leg->AddEntry(h1,Form("Hydjet Drum"),"");
    else leg->AddEntry(h1,Form("Data"),"");

    leg->AddEntry(h0,Form("Min bias %s TeV",1?"2.76":"5.02"),"l");
    leg->AddEntry(h1,Form("Min bias %s TeV",0?"2.76":"5.02"),"p");

  }else{
    leg->AddEntry(h0,Form("Min bias %s TeV",s2760?"2.76":"5.02"),"");
    leg->AddEntry(h0,Form("Hydjet Drum"),"l");
    leg->AddEntry(h1,Form("Data"),"p");
  }

  leg->Draw();

  if(sDep){
    c1->Print(Form("figure_leading_fluctuation_%s_%s.png",mc?"mc":"data",algo.data()));
  }else{
    c1->Print(Form("figure_leading_fluctuation_%dGeV_%s.png",s2760?2760:5020,algo.data()));
  }


  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  h0->SetMaximum(1.);
  h0->SetMinimum(0.00001);

  h0->Draw("hist");
  h1->Draw("same");
  c2->SetLogy();

  leg->Draw();

  if(sDep){
    c2->Print(Form("figure_leading_fluctuation_%s_%s_log.png",mc?"mc":"data",algo.data()));
  }else{
    c2->Print(Form("figure_leading_fluctuation_%dGeV_%s_log.png",s2760?2760:5020,algo.data()));
  }


}

void plotLeading(){

  //  compare(sDep, s2760, mc)
  //  plot(mc,s2760,mode)

  string algos[] = {"akVs4PF","akPu4PF","akPu3PF"};

  for(int i = 0; i < 3; ++i){
    algo = algos[i];

    //    plot(0,0,1);

    //    plot(1,0,1);

    plot(0,0,1);
    plot(0,1,1);
    plot(1,1,1);
    plot(1,0,1);


  if(1){
    compare(0,0);
    compare(0,1);
    
    compare(1,0,0);
    compare(1,0,1);
  }
  }

}


