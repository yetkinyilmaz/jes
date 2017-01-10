#include "quickResponse.C"

double analyzePtBin(int ptMin = 40, int ptMax = 50, int etaBin = 0, int centBin = 0, string sample = "qcd"){

  int cone = 4;

  TCanvas* c1 = new TCanvas("c1","c1",600,600);

  string dir0 = "puTowerExclLimitV3";
  string dir1 = "radiusPUtoAlgoRV2";

  TH1D* h0, *h1;

  if(1){
    h0 = quickResponse(dir0,sample,ptMin,ptMax,0,etaBin,cone);
    h1 = quickResponse(dir0,sample,ptMin,ptMax,1,etaBin,cone);
  }

  if(0){
    h0 = quickResponse(dir0,sample,ptMin,ptMax,1,etaBin,cone);
    h1 = quickResponse(dir1,sample,ptMin,ptMax,1,etaBin,cone);
  }


  h0->Draw("hist");
  h1->Draw("same");

  TLegend *t3=new TLegend(0.44,0.62,0.92,0.94);
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(18);

  double m0 = h0->GetMean();
  double m1 = h1->GetMean();

  t3->AddEntry(h0,"pp, pp-PF non-PU","l");
  //  t3->AddEntry(h0,"exclude R = 0.5","l");

  t3->AddEntry(h0,Form("#mu        : %0.3f #pm %0.3f",h0->GetMean(),h0->GetMeanError()),"");
  t3->AddEntry(h0,Form("#sigma/#mu : %0.3f #pm %0.3f",h0->GetRMS()/m0,h0->GetRMSError()/m0),"");

  if(1){
  if(centBin == 0) t3->AddEntry(h1,"PbPb 0-10%, HI-PF PU","p");
  if(centBin == 1) t3->AddEntry(h1,"PbPb 10-30%, HI-PF PU","p");
  if(centBin == 2) t3->AddEntry(h1,"PbPb 30-100%, HI-PF PU","p");
  }
  //  t3->AddEntry(h0,"exclude R = 0.4","p");


  t3->AddEntry(h0,Form("#mu        : %0.3f #pm %0.3f",h1->GetMean(),h1->GetMeanError()),"");
  t3->AddEntry(h1,Form("#sigma/#mu : %0.3f #pm %0.3f",h1->GetRMS()/m1,h1->GetRMSError()/m1),"");
  t3->AddEntry(h0,Form("R = 0.%d",cone),"");
  t3->AddEntry(h0,Form("%0.1f<|#eta|<%0.1f",etaMin[etaBin],etaMax[etaBin]),"");
  t3->AddEntry(h0,Form("%d < p_{T} <%d",ptMin,ptMax),"");
  t3->Draw();

  c1->Print(Form("figure_%s_%s_centBin%d_etaBin%d.png",sample.data(),h0->GetName(),centBin,etaBin));

  return m1;

}

void analyzeJetResponse(){

  analyzePtBin(40,50,1,0,"qcd");

  if(0){
  string sample = "bfcr";
  for(int icent = 0; icent < 3; ++icent){
    for(int ieta = 0; ieta < 4; ++ieta){
      analyzePtBin(40,50,ieta,icent,sample);
      analyzePtBin(50,60,ieta,icent,sample);
      analyzePtBin(60,80,ieta,icent,sample);
      analyzePtBin(80,100,ieta,icent,sample);
      analyzePtBin(100,120,ieta,icent,sample);
    }
  }
  }


}
