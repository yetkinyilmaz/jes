{

  akPu4PFJetAnalyzer->cd();


  t->AddFriend("hiEvtAnalyzer/HiTree");

  t->SetAlias("pt1","Max$(jtpt*(abs(jteta)<2))");
  t->SetAlias("eta1","jteta*(jtpt == pt1)");


  t->Draw("pt1:pthat>>h(200,0,200,200,0,200)","","colz");
  t->Draw("hiHF:jteta","jtpt > 80");

  t->Draw("hiBin:eta1","","colz");

}
