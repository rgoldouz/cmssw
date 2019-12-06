#define GFlash_cxx
#include "GFlash.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TLine.h"
#include "TStopwatch.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TGraphAsymmErrors.h"
#include <cmath>
#include "TLorentzVector.h"
#include "TPaveText.h"
#include "TPie.h"
#include "TF1.h"
#include "TVector3.h"
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include "TRandom3.h"
#define nGammaPar 3 //#pars for Gamma fn

Double_t GammaProfile(Double_t *x,Double_t *par) {
double Gamma1=0;
Gamma1 = par[2]*((par[1]*pow((x[0])*(par[1]),par[0]-1.0)*exp(-(x[0])*(par[1])))/tgamma(par[0]));
return Gamma1;
}

void displayProgress(long current, long max){
  using std::cerr;
  if (max<1000) return;
  if (current%(max/1000)!=0 && current<max-1) return;

  int width = 52; // Hope the terminal is at least that wide.
  int barWidth = width - 2;
  cerr << "\x1B[2K"; // Clear line
  cerr << "\x1B[2000D"; // Cursor left
  cerr << '[';
  for(int i=0 ; i<barWidth ; ++i){ if(i<barWidth*current/max){ cerr << '=' ; }else{ cerr << ' ' ; } }
  cerr << ']';
  cerr << " " << Form("%8d/%8d (%5.2f%%)", (int)current, (int)max, 500.0*current/max) ;
  cerr.flush();
}

TH1F* RandomGammaProfile(double alpha, double beta, double range, int nbin, const char* name, double weight){
  int n=1000;
  double rand;
  TH1F *Eprofile;
  TF1 *funcH1;
  Eprofile = new TH1F(name,name,nbin,0,range);
  funcH1 = new TF1("GammaH", GammaProfile, 0, range, nGammaPar);
  funcH1->SetParameters(alpha ,beta,1);
  for (int j = 0; j < n; ++j){
    rand = funcH1->GetRandom();
    Eprofile->Fill(rand, weight);
  }
  delete funcH1;
  return Eprofile;
}


void GFlash::Loop(TString fname)
{
gStyle->SetOptStat(0);
cout<<fname<<endl;
//   In a ROOT session, you can do:
//      root> .L GFlash.C
//      root> GFlash t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

   TFile *f = new TFile("Hist"+fname+".root","RECREATE");
   f->cd();

   double hcalradlen =143;
   double hcalintlen =150.5;
//   double ecalintlen =185;
   double ecalintlen =300;   
   double rand;

   TH1F *fDp, *fPi0, *fPi0L, *fHadECAL;
   TH1F *HadEPHcal, *HadEPEcal, *HadEPFullsimH, *HadEPFullsimHpE, *HadEPFullsimHFit, *HadEPFullsimHpEFit, *HadEPFullsimHCAL, *HadEPFullsimECAL, *HHcal, *HEcal;
   TH1F *Class, *fSP;
   TH1F *SP, *Eprofile, *EPHCAL, *EPECAL;
   TF1 *funcH;
   fDp = new TH1F( "fDp", "fDp" ,   50, 0  , 1    );
   fPi0 = new TH1F( "fPi0", "fPi0" ,   50, 0  , 1    );
   fPi0L = new TH1F( "fPi0L", "fPi0L" ,   50, 0  , 1    );
   fHadECAL = new TH1F( "fHadECAL", "fHadECAL" ,   50, 0  , 1    );
   Class = new TH1F( "Class", "Class" ,   4, 0  , 4    );

   fSP = new TH1F( "fSP", "fSP" ,   4, 0  , 4    );
   SP = new TH1F( "SP", "SP" ,   400, 0  , 4000    );
   HadEPFullsimH = new TH1F( "HadEPFullsimH","HadEPFullsimH" ,   30, 0  , 10    );
   HadEPFullsimHpE = new TH1F( "HadEPFullsimHpE","HadEPFullsimHpE" ,   30, 0  , 10    );

   HadEPFullsimHFit = new TH1F( "HadEPFullsimHFit","HadEPFullsimHFit" ,   30, 0  , 10    );
   HadEPFullsimHpEFit = new TH1F( "HadEPFullsimHpEFit","HadEPFullsimHpEFit" ,   30, 0  , 10    );

   HadEPFullsimHCAL = new TH1F( "HadEPFullsimHCAL","HadEPFullsimHCAL" ,   30, 0  , 10    );
   HadEPFullsimECAL = new TH1F( "HadEPFullsimECAL","HadEPFullsimECAL" ,   30, 0  , 10    );

//   HadEPFullsimHCALFit = new TH1F( "HadEPFullsimHCALFit","HadEPFullsimHCALFit" ,   30, 0  , 10    );
//   HadEPFullsimECALFit = new TH1F( "HadEPFullsimECALFit","HadEPFullsimECALFit" ,   30, 0  , 10    );

   Double_t mean, var;

   if (fChain == 0) return;

   Int_t nentries = (Int_t) fChain->GetEntries();
   float N = float(nentries);

   Long64_t nbytes = 0, nb = 0;
   TCanvas *c1;
   stringstream ss;
   stringstream Intgral;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
//   for (Long64_t jentry=0; jentry<200;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      displayProgress(jentry, nentries) ;

//energy parameters
      fDp->Fill(sim_eTotal/p_E);
      if(sim_ePi0First+sim_ePi0Late !=0) fPi0->Fill((sim_ePi0First+sim_ePi0Late)/sim_eTotal);
      if(sim_ePi0First!=0 && sim_ePi0Late !=0) fPi0L->Fill(sim_ePi0Late/(sim_ePi0First+sim_ePi0Late));
      if(sim_ePi0First+sim_ePi0Late ==0) Class->Fill(0.5,1/N);
      if(sim_ePi0First!=0 && sim_ePi0Late ==0) Class->Fill(1.5,1/N);   
      if(sim_ePi0First==0 && sim_ePi0Late !=0) Class->Fill(2.5,1/N);
      if(sim_ePi0First!=0 && sim_ePi0Late !=0) Class->Fill(3.5,1/N);

//starting points
      SP->Fill(sim_pvIneInt_z);
      if(sim_pvIneInt_z<1290) fSP->Fill(0.5,1/N);
      if(sim_pvIneInt_z>1290 && sim_pvIneInt_z<1790) fSP->Fill(1.5,1/N);
      if(sim_pvIneInt_z>1790 && sim_pvIneInt_z<2950) fSP->Fill(2.5,1/N);
      if(sim_pvIneInt_z>2950) fSP->Fill(3.5,1/N);

//Hadronic eneergy profile
      if(h_eHadronic->Integral() < 0.1) continue;
      HadEPHcal = new TH1F( "hadhcal","hadhcal" ,   30, 0  , 10    );
      HadEPEcal = new TH1F( "hadecal","hadecal" ,   30, 0  , 10    );

      HHcal = new TH1F( "hhcal","hhcal" ,   30, 0  , 10    );
      HEcal = new TH1F( "hecal","hecal" ,   30, 0  , 10    );
      for (int b = 1; b < h_eHadronic->GetNbinsX(); ++b){
          HHcal->Fill((h_eHadronic_HCAL->GetXaxis()->GetBinCenter(b) - sim_pvIneInt_z)/hcalintlen, h_eHadronic_HCAL->GetBinContent(b));
          HEcal->Fill((h_eHadronic_ECAL->GetXaxis()->GetBinCenter(b) - sim_pvIneInt_z)/hcalintlen, h_eHadronic_ECAL->GetBinContent(b));
          // if shower starts in HCAL
          if (sim_pvIneInt_z > 1400) {HadEPHcal->Fill((h_eHadronic->GetXaxis()->GetBinCenter(b) - sim_pvIneInt_z)/hcalintlen, h_eHadronic->GetBinContent(b));}
          else {
//              if (h_eHadronic->GetXaxis()->GetBinCenter(b)<1790) HadEPEcal->Fill((h_eHadronic->GetXaxis()->GetBinCenter(b) - sim_pvIneInt_z)/ecalintlen, h_eHadronic->GetBinContent(b));
//              if (h_eHadronic->GetXaxis()->GetBinCenter(b)>1790) HadEPEcal->Fill(((1790.0-sim_pvIneInt_z)/ecalintlen)  + (h_eHadronic->GetXaxis()->GetBinCenter(b) - 1790)/hcalintlen, h_eHadronic->GetBinContent(b));
          HadEPEcal->Fill((h_eHadronic->GetXaxis()->GetBinCenter(b)- sim_pvIneInt_z)/hcalintlen, h_eHadronic->GetBinContent(b));
          }
      }
      if(HadEPHcal->Integral() < 0.1 && HadEPEcal->Integral()< 0.1) {
          delete HadEPHcal;
          delete HadEPEcal;
          continue;}
      else{
//          if (jentry<10 &&sim_pvIneInt_z < 1790){
//c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//c1->cd();
//h_eHadronic->Draw("h");
//Intgral<<sim_pvIneInt_z;
//h_eHadronic->SetTitle((Intgral.str()).c_str());
//ss <<"eHadronic"<<fname<<jentry<<".png";
//c1->Print((ss.str()).c_str(),"png");
//ss.str("");
//Intgral.str("");
//delete c1;
//}
//
if (jentry<30&&sim_pvIneInt_z < 1400){
c1 = new TCanvas("c1","A Simple Graph Example",800,100,800,500);
c1->cd();
HadEPEcal->SetLineColor(0);
HadEPEcal->SetLineWidth(2);
HadEPEcal->SetLineStyle(2);
HadEPEcal->SetTitle("pion 50GeV");
HadEPEcal->GetXaxis()->SetTitle("Interaction length");
HadEPEcal->GetYaxis()->SetTitle("Energy GeV");
HadEPEcal->Draw("hist");
HHcal->SetLineColor(1);
HHcal->Draw("histsame");
HEcal->SetLineColor(2);
HEcal->Draw("histsame");
 double m1 = HHcal->GetMean();
 double v1 = pow(HHcal->GetRMS(),2);
 double m2 = HEcal->GetMean();
 double v2 = pow(HEcal->GetRMS(),2);
              EPECAL = new TH1F( "Ep","Ep" ,   30, 0  , 10    );
              EPHCAL = new TH1F( "Ep","Ep" ,   30, 0  , 10    );
              funcH = new TF1("GammaH",GammaProfile,0  , 10,nGammaPar);
funcH->SetParameters(pow(m1,2)/v1,m1/v1, 10*HHcal->Integral()/30);
              for (int j = 0; j <1000 ; ++j){
                  rand = funcH->GetRandom();
                  EPHCAL->Fill(rand);
              }
funcH->SetParameters(pow(m2,2)/v2,m2/v2, 10*HHcal->Integral()/30);
              for (int j = 0; j <1000 ; ++j){
                  rand = funcH->GetRandom();
                  EPECAL->Fill(rand);
              }
EPECAL->Scale(HEcal->Integral()/EPECAL->Integral());
EPHCAL->Scale(HHcal->Integral()/EPHCAL->Integral());
EPECAL->SetLineStyle(2);
EPHCAL->SetLineStyle(2);
EPECAL->SetLineColor(2);
EPHCAL->SetLineColor(1);
EPECAL->Draw("histsame");
EPHCAL->Draw("histsame");
delete funcH;
ss <<"eHadronic"<<fname<<jentry<<".png";
c1->Print((ss.str()).c_str(),"png");
ss.str("");
delete c1;

delete EPECAL;
delete EPHCAL;
}

          if (sim_pvIneInt_z > 1400){
              mean =  HadEPHcal->GetMean();
              var = pow(HadEPHcal->GetRMS(),2);
              if(HadEPHcal->Integral()== 0 || mean == 0 || var == 0) {
                  delete HadEPHcal;
                  delete HadEPEcal;
                  continue;
              }
              Eprofile = new TH1F( "Ep","Ep" ,   30, 0  , 10    );
              funcH = new TF1("GammaH",GammaProfile,0  , 10,nGammaPar);
              funcH->SetParameters(pow(mean,2)/var,mean/var, 10*HadEPHcal->Integral()/30);
              for (int j = 0; j <1000 ; ++j){
                  rand = funcH->GetRandom();
                  Eprofile->Fill(rand);
              }
              if (Eprofile->Integral()>0) Eprofile->Scale(1/Eprofile->Integral());
              if (Eprofile->Integral()>0) HadEPFullsimHFit->Add(Eprofile);
              HadEPHcal->Scale(1/HadEPHcal->Integral());
              HadEPFullsimH->Add(HadEPHcal);
              delete HadEPHcal;
              delete HadEPEcal;
              delete Eprofile;
              delete funcH;
              delete HHcal;
              delete HEcal;
          }
          else {
              fHadECAL->Fill(HEcal->Integral()/(HHcal->Integral()+HEcal->Integral()));
              double m1 = HHcal->GetMean();
              double v1 = pow(HHcal->GetRMS(),2);
              double m2 = HEcal->GetMean();
              double v2 = pow(HEcal->GetRMS(),2);
              if(m1==0 || m2 == 0 || v1 == 0 || v2 ==0 || HEcal->Integral() < 1 || HHcal->Integral() <1) {
                  delete HadEPHcal;
                  delete HadEPEcal;
                  delete HHcal;
                  delete HEcal;
                  continue;
              }
              EPECAL = new TH1F( "EpE","EpE" ,   30, 0  , 10    );
              EPHCAL = new TH1F( "EpH","EpH" ,   30, 0  , 10    );
              funcH = new TF1("GammaH",GammaProfile,0  , 10,nGammaPar);
              funcH->SetParameters(pow(m1,2)/v1,m1/v1, 10*HHcal->Integral()/30);
              for (int j = 0; j <1000 ; ++j){
                  rand = funcH->GetRandom();
                  EPHCAL->Fill(rand);
              }
              funcH->SetParameters(pow(m2,2)/v2,m2/v2, 10*HHcal->Integral()/30);
              for (int j = 0; j <1000 ; ++j){
                  rand = funcH->GetRandom();
                  EPECAL->Fill(rand);
              }
delete funcH;
              EPECAL->Scale(HEcal->Integral()/EPECAL->Integral());
              EPHCAL->Scale(HHcal->Integral()/EPHCAL->Integral());             
              EPECAL->Add(EPHCAL);
              if (HHcal->Integral()>0) HadEPFullsimHCAL->Scale(1/HHcal->Integral());
              if (HEcal->Integral()>0) HadEPFullsimECAL->Scale(1/HEcal->Integral());
              HadEPFullsimHCAL->Add(HHcal);
              HadEPFullsimECAL->Add(HEcal);
              delete HHcal;
              delete HEcal;
              mean =  HadEPEcal->GetMean();
              var = pow(HadEPEcal->GetRMS(),2);
              if(HadEPEcal->Integral()==0 || mean == 0 || var == 0) {
                  delete HadEPHcal;
                  delete HadEPEcal;
                  delete EPECAL;
                  delete EPHCAL;
                  continue;
              }
              Eprofile = new TH1F( "Ep","Ep" ,   30, 0  , 10    );
              funcH = new TF1("GammaH",GammaProfile,0  , 10,nGammaPar);
              funcH->SetParameters(pow(mean,2)/var,mean/var, 10*HadEPEcal->Integral()/30);
              for (int j = 0; j <1000 ; ++j){
                  rand = funcH->GetRandom();
                  Eprofile->Fill(rand);
              }              
//              if (Eprofile->Integral()>0) Eprofile->Scale(1/Eprofile->Integral());
//              if (Eprofile->Integral()>0) HadEPFullsimHpEFit->Add(Eprofile);
              HadEPEcal->Scale(1/HadEPEcal->Integral());
              HadEPFullsimHpE->Add(HadEPEcal);
              if (EPECAL->Integral()>0) EPECAL->Scale(1/EPECAL->Integral());
              if (EPECAL->Integral()>0) HadEPFullsimHpEFit->Add(EPECAL);
              delete HadEPHcal;
              delete HadEPEcal;
              delete Eprofile;
              delete funcH;
delete EPECAL;
delete EPHCAL;
         }
      }
  }

  HadEPFullsimH->Scale(1/HadEPFullsimH->Integral());
  HadEPFullsimHpE->Scale(1/HadEPFullsimHpE->Integral());
  HadEPFullsimHFit->Scale(1/HadEPFullsimHFit->Integral());
  HadEPFullsimHpEFit->Scale(1/HadEPFullsimHpEFit->Integral());
  HadEPFullsimHCAL->Scale(1/HadEPFullsimHCAL->Integral());
  HadEPFullsimECAL->Scale(1/HadEPFullsimECAL->Integral());

  const Int_t nx = 4;
  const char *type[nx] = {"H","H+#pi_{0}^{f}","H+#pi_{0}^{l}","H+#pi_{0}^{f}+#pi_{0}^{l}"};
  Int_t i;
  for (i=1;i<=nx;i++) {
     Class->GetXaxis()->SetBinLabel(i,type[i-1]);
     Class->GetXaxis()->SetLabelSize(0.07);
  }


  const char *SPtype[nx] = {"Before ECAL","In ECAL","IN HCAL","After HCAL"};
  for (i=1;i<=nx;i++) {
     fSP->GetXaxis()->SetBinLabel(i,SPtype[i-1]);
     fSP->GetXaxis()->SetLabelSize(0.07);
  }

  fPi0->Write("",TObject::kOverwrite);
  fDp->Write("",TObject::kOverwrite);
  fPi0L->Write("",TObject::kOverwrite);
  fHadECAL->Write("",TObject::kOverwrite);
  Class->Write("",TObject::kOverwrite);
  HadEPFullsimH->Write("",TObject::kOverwrite);
  HadEPFullsimHpE->Write("",TObject::kOverwrite);
  HadEPFullsimHCAL->Write("",TObject::kOverwrite);
  HadEPFullsimECAL->Write("",TObject::kOverwrite);
  HadEPFullsimHFit->Write("",TObject::kOverwrite);
  HadEPFullsimHpEFit->Write("",TObject::kOverwrite);
  SP->Write("",TObject::kOverwrite);
  fSP->Write("",TObject::kOverwrite);
  delete fPi0;
  delete fDp;
  delete fPi0L;
  delete fHadECAL;
  delete Class;
  delete HadEPFullsimH;
  delete HadEPFullsimHCAL;
  delete HadEPFullsimECAL;
  delete HadEPFullsimHpE;
  delete HadEPFullsimHFit;
  delete HadEPFullsimHpEFit;
  delete fSP;
  delete SP;
  f->Close();
}
