

void analyzeMC(){

  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("/data_CMS/cms/lisniak/bjet2015/mcPbqcdakPu4PF_djt.root");
  TNtuple* nt = (TNtuple*)inf->Get("nt");

  TCanvas* c1 = new TCanvas("c1","c1",600,600);


  TH1D* hp = new TH1D("hp",";pthat;events",200,0,200);
  TH1D* hpw = new TH1D("hpw",";pthat;pthat weighted events",200,0,200);
  TH1D* hpfw = new TH1D("hpfw",";pthat;fully weighted events",200,0,200);

  TCut dphiCut("acos(cos(jtphi1-jtphi2)) > 2.0944");

  nt->Draw("jtpt1:pthat>>h(200,0,200,200,100,300)","pthatweight*(jtpt1 > 100 && jtpt2 > 40)","colz");


  c1->Print("figure_mc_pthat_contributions_2D.png");



  nt->Draw("pthat>>hp","(jtpt1 > 100)","");
  c1->Print("figure_mc_pthat_counts.png");

  nt->Draw("pthat>>hpw","pthatweight*(jtpt1 > 100)","");
  c1->Print("figure_mc_pthat_pthatweighted.png");

  nt->Draw("pthat>>hpfw","weight*(jtpt1 > 100)","");
  c1->Print("figure_mc_pthat_fullyweighted.png");





  nt->Draw("pthat>>h(200,0,200)","pthatweight*(jtpt1 > 100 && jtpt1 < 150 && jtpt2 > 40)","colz");






  nt->Draw("jtpt1:refpt1>>h(100,0,200,100,0,200)","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat < 50)","colz");



  nt->Draw("jtpt1:genpt1>>h(100,0,200,100,0,200)","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat < 50)","colz");


  TH1D* h0 = new TH1D("h0","h0;p_{T,1}/p_{T,2};",20,0,1);
  TH1D* h1 = new TH1D("h1","h1;p_{T,1}/p_{T,2};",20,0,1);
  TH1D* h2 = new TH1D("h2","h2;p_{T,1}/p_{T,2};",20,0,1);
  TH1D* h3 = new TH1D("h3","h3;p_{T,1}/p_{T,2};",20,0,1);

  h0->SetMarkerStyle(25);
  h0->SetMarkerColor(4);
  h0->SetLineColor(4);

  h1->SetMarkerStyle(25);
  h1->SetMarkerColor(3);
  h1->SetLineColor(3);

  h2->SetMarkerStyle(25);
  h2->SetMarkerColor(2);
  h2->SetLineColor(2);

  h3->SetMarkerStyle(25);


  TLegend *leg=0;

  TCanvas* c2 = new TCanvas("c2","c2",600,600);

  nt->Draw("jtpt2/jtpt1>>h0","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat > 0 && refpt1 > 0)","p");
  nt->Draw("jtpt2/jtpt1>>h1","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat > 0 && refpt1 > 20)","same");
  nt->Draw("jtpt2/jtpt1>>h2","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat > 0 && refpt1 > 50)","same");
  nt->Draw("jtpt2/jtpt1>>h3","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat > 0 && refpt1 > 70)","same");



  leg=new TLegend(0.18,0.72,0.62,0.94);
  leg->AddEntry(h0,"pthat > 0","");
  leg->AddEntry(h0,"refpt1 > 0","p");
  leg->AddEntry(h1,"refpt1 > 20","p");
  leg->AddEntry(h2,"refpt1 > 50","p");
  leg->AddEntry(h3,"refpt1 > 70","p");

  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);

  leg->Draw();


  c2->Print("figure_mc_xj_refpttest_pthat0.png");


  nt->Draw("jtpt2/jtpt1>>h0","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 50 && refpt1 > 0)","p");
  nt->Draw("jtpt2/jtpt1>>h1","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 50 && refpt1 > 20)","same");
  nt->Draw("jtpt2/jtpt1>>h2","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 50 && refpt1 > 50)","same");
  nt->Draw("jtpt2/jtpt1>>h3","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 50 && refpt1 > 70)","same");


  leg=new TLegend(0.18,0.72,0.62,0.94);
  leg->AddEntry(h0,"pthat > 50","");
  leg->AddEntry(h0,"refpt1 > 0","p");
  leg->AddEntry(h1,"refpt1 > 20","p");
  leg->AddEntry(h2,"refpt1 > 50","p");
  leg->AddEntry(h3,"refpt1 > 70","p");

  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);

  leg->Draw();



  c2->Print("figure_mc_xj_refpttest_pthat50.png");


  nt->Draw("jtpt2/jtpt1>>h0","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 30 && refpt1 > 0)","p");
  nt->Draw("jtpt2/jtpt1>>h1","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 50 && refpt1 > 0)","same");
  nt->Draw("jtpt2/jtpt1>>h2","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 60 && refpt1 > 0)","same");
  nt->Draw("jtpt2/jtpt1>>h3","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 70 && refpt1 > 0)","same");


  leg=new TLegend(0.18,0.72,0.62,0.94);
  leg->AddEntry(h0,"refpt1 > 0","");
  leg->AddEntry(h0,"pthat > 30","p");
  leg->AddEntry(h1,"pthat > 50","p");
  leg->AddEntry(h2,"pthat > 60","p");
  leg->AddEntry(h3,"pthat > 70","p");


  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);

  leg->Draw();



  c2->Print("figure_mc_xj_pthattest_refpt0.png");

  nt->Draw("jtpt2/jtpt1>>h0","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 30 && refpt1 > 50)","p");
  nt->Draw("jtpt2/jtpt1>>h1","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 50 && refpt1 > 50)","same");
  nt->Draw("jtpt2/jtpt1>>h2","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 60 && refpt1 > 50)","same");
  nt->Draw("jtpt2/jtpt1>>h3","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && acos(cos(jtphi1-jtphi2)) > 2.0944 && pthat > 70 && refpt1 > 50)","same");


  leg=new TLegend(0.18,0.72,0.62,0.94);
  leg->AddEntry(h0,"refpt1 > 50","");
  leg->AddEntry(h0,"pthat > 30","p");
  leg->AddEntry(h1,"pthat > 50","p");
  leg->AddEntry(h2,"pthat > 60","p");
  leg->AddEntry(h3,"pthat > 70","p");


  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(18);


  leg->Draw();

  c2->Print("figure_mc_xj_pthattest_refpt50.png");





}

