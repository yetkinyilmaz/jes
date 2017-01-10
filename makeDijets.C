
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using namespace std;

void process(bool pp = 0,
		bool mb = 1,
		bool mc = 1,
		bool s2760 = 0,
		string algo = "akVs4PF",
	     int pthat = 30,
             string sample = "pythia8",
	     int n = 0
		){

   TFile* inf;

   double jetEtaMax = 2.;



   if(mc){

     if(!mb){

       if(sample == "pyquenColl"){
	 inf = TFile::Open(Form("/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/%s/qcd%d/merged_HiForestAOD.root",sample.data(),pthat));
       }else{
	 if(pthat == 80){
	   if(s2760){
	     inf = TFile::Open(Form("./HiForest_PYTHIA_HYDJET_pthat%d_Track9_Jet30_matchEqR_merged_forest_0.root",pthat));
	   }else{	 
	     inf = TFile::Open(Form("/data_CMS/cms/mnguyen/bJet2015/mc/PbPb/pythia6/qcd80/cleanedGen/merged_HiForestAOD.root"));
	   }	 
	 }else{
	   
	   if(s2760){
	     
	     inf = TFile::Open(Form("root://xrootd.unl.edu//store/user/dgulhan/PYTHIA_HYDJET_Track9_Jet30_Pyquen_DiJet_TuneZ2_Unquenched_Hydjet1p8_2760GeV_merged/HiForest_PYTHIA_HYDJET_pthat%d_Track9_Jet30_matchEqR_merged_forest_0.root",pthat));
	   }else{	   
	     if(pthat == 30) inf = TFile::Open(Form("root://xrootd.unl.edu//store/user/velicanu/Merged/Pythia8_Dijet30_pp_TuneCUETP8M1_Hydjet_MinBias_5020GeV-FOREST-v28/0.root"));
	   }
	   
	 }
       }
       
     }
     
     if(mb){
       if(s2760){

	 inf = TFile::Open(Form("/data_CMS/cms/yilmaz/prod/hydjet2760_20160302/output/forest_hydjet2760_20160302.root"));

       }else{
	 inf = TFile::Open(Form("root://xrootd.unl.edu//store/user/velicanu/Merged/Hydjet_Quenched_MinBias_5020GeV_758p2_FOREST_v20/0.root"));
       }
     }

   }else{ // DATA

     if(mb){
       if(s2760){

	 int nn = n % 1000;
	 int lum = (n - nn)/1000;

	 const char* filename = Form("root://xrootd.unl.edu//store/user/belt/HiForest_MinBias_JetRAA_v1_final/HiForest_HIMinBiasUPC_14Mar2014_v2_JetRAA_lumi%d_Part%d.root",lum,nn);
	 cout<<"Processing file : "<<filename<<endl;
	 inf = TFile::Open(filename);

       }else{
	 inf = TFile::Open(Form("root://xrootd.unl.edu//store/group/phys_heavyions/jisun/ForestRun2015/PbPbMB/PbPb_HIMinimumBias2_FullForestAOD_goldenjson_Run263233-263293_02162016/HIMinimumBias2/crab_PbPb_HIMinimumBias2_FullForestAOD_goldenjson_Run263233-263293_02162016/160215_222950/0000/HiForestAOD_%d.root",n));
       }
     }
   }


   TTree* tref, *tjet, *tskim, *ttrig;

   if(!pp) tref= (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
   tskim = (TTree*)inf->Get("skimanalysis/HltTree");
   tjet = (TTree*)inf->Get(Form("%sJetAnalyzer/t",algo.data()));
   ttrig = (TTree*)inf->Get("hltanalysis/HltTree");

   TFile* outf;
   if(!mb){
     outf = new TFile(Form("fullsim_%s_%dGeV_dijets_pythia6_pthat%d_%s_%s.root",sample.data(),s2760?2760:5020,pthat,pp?"pp":"PbPb",algo.data()),"recreate");
   }

   if(mb){
     outf = new TFile(Form("minbias_%s_%dGeV_dijets_%s_file%d.root",mc?"mc":"data",s2760?2760:5020,algo.data(),n),"recreate");
   }

   float hiHF;
   float jtpt[200], jteta[200], jtphi[200];
   float refpt[200], refeta[200], refphi[200];

   int pcollisionEventSelection;

   int subid[200];
   int nref;
   int hiBin;

   int trigger = 1;
   ULong64_t evt;

   if(!pp){
     tref->SetBranchAddress("hiHF",&hiHF);
     tref->SetBranchAddress("hiBin",&hiBin);
   }

   tref->SetBranchAddress("evt",&evt);
   tskim->SetBranchAddress("pcollisionEventSelection",&pcollisionEventSelection);

   if(!mc && mb && s2760) ttrig->SetBranchAddress("HLT_HIMinBiasHfOrBSC_v1",&trigger);
   //   if(!mc && mb && !s2760) ttrig->SetBranchAddress("HLT_HIMinBiasHfOrBSC_v1",&trigger);

   if(0){
     tjet->SetBranchAddress("ngen",&nref);
     tjet->SetBranchAddress("genpt",jtpt);
     tjet->SetBranchAddress("geneta",jteta);
     tjet->SetBranchAddress("genphi",jtphi);
     tjet->SetBranchAddress("gensubid",subid);

   }else{
     tjet->SetBranchAddress("nref",&nref);
     tjet->SetBranchAddress("jtpt",jtpt);
     tjet->SetBranchAddress("jteta",jteta);
     tjet->SetBranchAddress("jtphi",jtphi);
     tjet->SetBranchAddress("refpt",refpt);
     tjet->SetBranchAddress("refeta",refeta);
     tjet->SetBranchAddress("refphi",refphi);

     tjet->SetBranchAddress("subid",subid); 
  }


   float pt1, pt2, eta1, eta2, phi1, phi2, dphi,
    refpt1, refpt2, refeta1, refeta2, refphi1, refphi2;
   
   int i1, i2, subid1, subid2;

   TTree* t = new TTree("t","t");

   t->Branch("evt",&evt,"evt/l");

   t->Branch("bin",&hiBin,"bin/I");
   t->Branch("hf",&hiHF,"hf/F");

   t->Branch("pt1",&pt1,"pt1/F");
   t->Branch("pt2",&pt2,"pt2/F");
   t->Branch("eta1",&eta1,"eta1/F");
   t->Branch("eta2",&eta2,"eta2/F");
   t->Branch("phi1",&phi1,"phi1/F");
   t->Branch("phi2",&phi2,"phi2/F");

   t->Branch("refpt1",&refpt1,"refpt1/F");
   t->Branch("refpt2",&refpt2,"refpt2/F");

   t->Branch("subid1",&subid1,"subid1/I");
   t->Branch("subid2",&subid2,"subid2/I");

   t->Branch("i1",&i1,"i1/I");
   t->Branch("i2",&i2,"i2/I");
   t->Branch("dphi",&dphi,"dphi/F");

   int N = tjet->GetEntries();

   //   N = 50000;

   for(int iev = 0; iev < N; ++iev){
      if(iev % 100 == 0) cout<<"processing event : "<<iev<<endl;

      ttrig->GetEntry(iev);
      if(trigger == 0) continue;

      tskim->GetEntry(iev);
      if(pcollisionEventSelection == 0) continue;

      tref->GetEntry(iev);
      tjet->GetEntry(iev);


      i1 = -1;
      i2 = -1;
      pt1 = -1;
      pt2 = -1;

      // dijets
      for(int i = 0; i < nref; ++i){

	 if(fabs(jteta[i]) > jetEtaMax) continue;

	 if(jtpt[i] > pt1 && (!mc || mb || subid[i] == 0)){
	    pt1 = jtpt[i];
            eta1 = jteta[i];
            phi1 = jtphi[i];
	    refpt1 = refpt[i];
            refeta1 = refeta[i];
            refphi1 = refphi[i];

	    subid1 = subid[i];
	    i1 = i;
	 }
	 
	 if(jtpt[i] > pt2 && i1 != i){
            pt2 = jtpt[i];
	    eta2 = jteta[i];
            phi2 = jtphi[i];
	    refpt2 = refpt[i];
            refeta2 = refeta[i];
            refphi2 = refphi[i];

            subid2 = subid[i];

            i2 = i;
         }

      } 

      if(i1 >= 0 && i2 >= 0){
	 dphi = acos(cos(jtphi[i1]-jtphi[i2]));
      }

      t->Fill();
   }


   cout<<"Finished."<<endl;
   t->Write();
   outf->Write();

}


void makeDijets(){

  //  process(pp,mb,mc,s2760,algo,pthat,n);

  //  process(0,1,1,1,"akVs4PF","pthat80");

  string sample = "pythia8";

  if(0){ // min bias data 2.76 TeV

    process(0,1,0,1,"akPu3PF",80,sample,1002);

    for(int i = 1003; i < 1008; ++i){
      process(0,1,0,1,"akVs4PF",80,sample,i);
      process(0,1,0,1,"akPu4PF",80,sample,i);
      process(0,1,0,1,"akPu3PF",80,sample,i);
    }

    for(int i = 2000; i < 2064; ++i){
      process(0,1,0,1,"akVs4PF",80,sample,i);
      process(0,1,0,1,"akPu4PF",80,sample,i);
      process(0,1,0,1,"akPu3PF",80,sample,i);
    }

  }

  if(0){ // min bias data 5.02 TeV
    for(int i = 10; i < 21; ++i){
      process(0,1,0,0,"akVs4PF",80,sample,i);
      process(0,1,0,0,"akPu4PF",80,sample,i);
      process(0,1,0,0,"akPu3PF",80,sample,i);
    }
    for(int i = 100; i < 110; ++i){
      process(0,1,0,0,"akVs4PF",80,sample,i);
      process(0,1,0,0,"akPu4PF",80,sample,i);
      process(0,1,0,0,"akPu3PF",80,sample,i);
    }
    for(int i = 111; i < 151; ++i){
      process(0,1,0,0,"akVs4PF",80,sample,i);
      process(0,1,0,0,"akPu4PF",80,sample,i);
      process(0,1,0,0,"akPu3PF",80,sample,i);
    }
  }


  if(0){ // min bias mc
    process(0,1,1,0,"akVs4PF",80);
    process(0,1,1,1,"akVs4PF",80);

    process(0,1,1,0,"akPu4PF",80);
    process(0,1,1,1,"akPu4PF",80);

    process(0,1,1,0,"akPu3PF",80);
    process(0,1,1,1,"akPu3PF",80);
  }

  if(0){ // embedded dijets pthat50
    process(0,0,1,1,"akVs4PF",50);
    process(0,0,1,0,"akVs4PF",50);
    process(0,0,1,1,"akVs3PF",50);
    process(0,0,1,0,"akVs3PF",50);

    process(0,0,1,1,"akPu3PF",50);
    process(0,0,1,0,"akPu3PF",50);
  }

  if(0){ //pthat30 from official sample
    process(0,0,1,0,"akVs4PF",30);
    process(0,0,1,0,"akPu4PF",30);
    process(0,0,1,0,"akPu3PF",30);
  }

  if(0){ // embedded dijets
    process(0,0,1,1,"akVs4PF",30);
    process(0,0,1,0,"akVs4PF",80);
    process(0,0,1,1,"akVs4PF",80);

    process(0,0,1,1,"akPu4PF",30);
    process(0,0,1,0,"akPu4PF",80);
    process(0,0,1,1,"akPu4PF",80);

    process(0,0,1,1,"akPu3PF",30);
    process(0,0,1,0,"akPu3PF",80);
    process(0,0,1,1,"akPu3PF",80);
  }

  if(1){ // quenched dijets
    sample = "pyquenColl";
    process(0,0,1,0,"akPu4PF",100,sample);
  }

  //  process(pp,mb,mc,s2760,algo,pthat,n);

}
