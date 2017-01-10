

{

  TGraph* g = new TGraph();

  if(0){
  g->SetPoint(0,5,230);
  g->SetPoint(1,15,130);
  g->SetPoint(2,25,80);
  g->SetPoint(3,35,60);
  g->SetPoint(4,45,50);
  g->SetPoint(5,55,25);
  g->SetPoint(6,70,15);
  g->SetPoint(7,90,10);
  g->SetPoint(8,120,5);
  g->SetPoint(9,170,0);
  }

  if(1){
    g->SetPoint(0,5,20);
    g->SetPoint(1,15,15);
    g->SetPoint(2,25,15);
    g->SetPoint(3,35,15);
    g->SetPoint(4,45,15);
    g->SetPoint(5,55,15);
    g->SetPoint(6,70,5);
    g->SetPoint(7,90,5);
    g->SetPoint(8,120,5);
    g->SetPoint(9,170,5);
  }

  g->Draw();

  g->Fit("expo");


}
