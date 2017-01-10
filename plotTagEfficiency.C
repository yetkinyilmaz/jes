

void plotTagEfficiency(){


  TH1::SetDefaultSumw2();

  TFile* inf = new TFile("/data_CMS/cms/lisniak/bjet2015/mcPbbfcakPu4PF_djt.root");
  TNtuple* nt = (TNtuple*)inf->Get("nt");

  TCanvas* c1 = new TCanvas("c1","c1",600,600);


  nt->SetAlias("pt1","jtpt1");
  nt->SetAlias("pt2","jtptSB");

  nt->SetAlias("ref1","refpt1");
  nt->SetAlias("ref2","refptSB");


  nt->SetAlias("adphi","acos(cos(jtphi1-jtphiSB))");

  nt->SetAlias("b1","(abs(refparton_flavorForB1) == 5)");
  nt->SetAlias("b2","(abs(refparton_flavorForBSB) == 5)");
  nt->SetAlias("tag1","(discr_csvV1_1>0.9)");
  nt->SetAlias("tag2","(discr_csvV1_SB>0.9)");

  nt->SetAlias("signaltagged","(tag1)");

  TCut b1("abs(refparton_flavorForB1) == 5");
  TCut b2("abs(refparton_flavorForBSB) == 5");
  TCut tag1("discr_csvV1_1>0.9");
  TCut tag2("discr_csvV1_SB>0.9");

  TCut signaltagged = b1 && b2 && tag1 && tag2;
  TCut dijet("pt1 > 100 && pt2 > 40");
  TCut awayside("adphi > 2.0944");
  TCut nearside("adphi < 1.0472");

  TCut central("bin < 20");
  TCut peripheral("bin >= 60");

  TCut weight("pthatweight");

  TProfile* p1[5], *p2[5];

  int N = 5;

  double ptMin[] = {40,60,80,100,120,150};



  for(int i = 0; i < N; ++i){

    p1[i] = new TProfile(Form("p1_%d",i),"",20,0,2);
    p2[i] = new TProfile(Form("p2_%d",i),"",20,0,2);

    TCut ptBin1(Form("ref1 > %f && ref1 < %f",ptMin[i],ptMin[i+1]));
    TCut ptBin2(Form("ref2 > %f && ref2 < %f",ptMin[i],ptMin[i+1]));

    nt->Draw(Form("tag1:pt1/ref1>>%s",p1[i]->GetName()),weight*(ptBin1&&b1&&peripheral));
    nt->Draw(Form("tag2:pt2/ref2>>%s",p2[i]->GetName()),weight*(ptBin2&&b2&&peripheral&&"pt1 > 100 && adphi > 2.0944"));
    p1[i]->SetMarkerStyle(25);
    p2[i]->SetMarkerStyle(25);
    p1[i]->SetMarkerColor(N-i);
    p2[i]->SetMarkerColor(N-i);
    p1[i]->SetLineColor(N-i);
    p2[i]->SetLineColor(N-i);

  }



  c1->cd();
  p1[0]->Draw();
  for(int i = 0; i < N; ++i){
    p1[i]->Draw("C same");
  }



  TCanvas* c2 = new TCanvas("c2","c2",600,600);

  p2[0]->Draw();
  for(int i = 0; i < N; ++i){
    p2[i]->Draw("C same");
  }
  

}
