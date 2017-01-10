

void analyzeMCb(){

  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("/data_CMS/cms/lisniak/bjet2015/mcPbbjtakPu4PF_djt.root");
  TNtuple* nt = (TNtuple*)inf->Get("nt");

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  nt->SetAlias("pt1","jtpt1");
  nt->SetAlias("pt2","jtptSL");

  nt->SetAlias("adphi","acos(cos(jtphi1-jtphiSL))");

  nt->SetAlias("b1","(abs(refparton_flavorForB1) == 5)");
  nt->SetAlias("b2","(abs(refparton_flavorForBSL) == 5)");
  nt->SetAlias("tag1","(discr_csvV1_1>0.9)");
  nt->SetAlias("tag2","(discr_csvV1_SL>0.9)");

  //  nt->SetAlias("signaltagged","(b1 && b2 && tag1 && tag2)");
  nt->SetAlias("signaltagged","(tag1)");


  TCut b1("abs(refparton_flavorForB1) == 5");
  TCut b2("abs(refparton_flavorForBSL) == 5");
  TCut tag1("discr_csvV1_1>0.9");
  TCut tag2("discr_csvV1_SL>0.9");

  TCut signaltagged = b1 && b2 && tag1 && tag2;
  TCut dijet("pt1 > 100 && pt2 > 40");
  TCut awayside("adphi > 2.0944");
  TCut nearside("adphi < 1.0472");

  TCut weight("pthatweight");
  
  TH2D* hpthat  = new TH2D("hpthat","",100,0,200,100,100,300);

  TH1D* hp = new TH1D("hp",";pthat;events",100,0,200);
  TH1D* hpw = new TH1D("hpw",";pthat;pthat weighted events",100,0,200);
  TH1D* hpfw = new TH1D("hpfw",";pthat;fully weighted events",100,0,200);

  nt->Draw("pt1:pthat>>hpthat",weight*(signaltagged && dijet && "refpt1 > 70"),"colz");


  c1->Print("figure_b_mc_pthat_contributions_2D.png");



  nt->Draw("pthat>>hp",signaltagged && dijet && "refpt1 > 70","");
  c1->Print("figure_b_mc_pthat_counts.png");

  nt->Draw("pthat>>hpw",(signaltagged && dijet && "refpt1 > 70")*"pthatweight","");
  c1->Print("figure_b_mc_pthat_pthatweighted.png");
  
  nt->Draw("pthat>>hpfw",(signaltagged && dijet && "refpt1 > 70")*"weight","");
  c1->Print("figure_b_mc_pthat_fullyweighted.png");





  //  nt->Draw("pthat>>h(200,0,200)","pthatweight*(jtpt1 > 100 && jtpt1 < 150 && jtpt2 > 40)","colz");
  //  nt->Draw("jtpt1:refpt1>>h(100,0,200,100,0,200)","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat < 50)","colz");
  //  nt->Draw("jtpt1:genpt1>>h(100,0,200,100,0,200)","pthatweight*(jtpt1 > 100 && jtpt2 > 40 && pthat < 50)","colz");


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

  nt->Draw("pt2/pt1>>h0",weight*(dijet && "refpt1 > 0"),"p");
  nt->Draw("pt2/pt1>>h1",weight*(dijet && "refpt1 > 20"),"p same");
  nt->Draw("pt2/pt1>>h2",weight*(dijet && "refpt1 > 50"),"p same");
  nt->Draw("pt2/pt1>>h3",weight*(dijet && "refpt1 > 70"),"p same");


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


  c2->Print("figure_b_mc_xj_refpttest_pthat0.png");

  nt->Draw("pt2/pt1>>h0",weight*(dijet && "pthat > 50" && "refpt1 > 0"),"p");
  nt->Draw("pt2/pt1>>h1",weight*(dijet && "pthat > 50" && "refpt1 > 20"),"p same");
  nt->Draw("pt2/pt1>>h2",weight*(dijet && "pthat > 50" && "refpt1 > 50"),"p same");
  nt->Draw("pt2/pt1>>h3",weight*(dijet && "pthat > 50" && "refpt1 > 70"),"p same");



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



  c2->Print("figure_b_mc_xj_refpttest_pthat50.png");


  
  nt->Draw("pt2/pt1>>h0",weight*(dijet && signaltagged && "pthat > 30 && refpt1 > 0"),"p same");
  nt->Draw("pt2/pt1>>h1",weight*(dijet && signaltagged && "pthat > 50 && refpt1 > 0"),"p same");                                                                           
  nt->Draw("pt2/pt1>>h2",weight*(dijet && signaltagged && "pthat > 60 && refpt1 > 0"),"p same");
  nt->Draw("pt2/pt1>>h3",weight*(dijet && signaltagged && "pthat > 70 && refpt1 > 0"),"p same");



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



  c2->Print("figure_b_mc_xj_pthattest_refpt0.png");



  nt->Draw("pt2/pt1>>h0",weight*(dijet && signaltagged && "pthat > 30 && refpt1 > 50"),"p");
  nt->Draw("pt2/pt1>>h1",weight*(dijet && signaltagged && "pthat > 50 && refpt1 > 50"),"p same");
  nt->Draw("pt2/pt1>>h2",weight*(dijet && signaltagged && "pthat > 60 && refpt1 > 50"),"p same");
  nt->Draw("pt2/pt1>>h3",weight*(dijet && signaltagged && "pthat > 70 && refpt1 > 50"),"p same");



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

  c2->Print("figure_b_mc_xj_pthattest_refpt50.png");





}

