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
  cerr << " " << Form("%8d/%8d (%5.2f%%)", (int)current, (int)max, 100.0*current/max) ;
  cerr.flush();
}

void GFlash::Loop(TString fname)
{
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

   double hcalradlen =14.3;
   double hcalintlen =185;
   TH1F *fDp, *fPi0, *fPi0L;
   TH1F *HADshapeIL;
   TH1F *Class;
   fDp = new TH1F( "fDp", "fDp" ,   50, 0  , 1    );
   fPi0 = new TH1F( "fPi0", "fPi0" ,   50, 0  , 1    );
   fPi0L = new TH1F( "fPi0L", "fPi0L" ,   50, 0  , 1    );
   Class = new TH1F( "Class", "Class" ,   4, 0  , 4    );

   if (fChain == 0) return;

   Int_t nentries = (Int_t) fChain->GetEntries();
   float N = float(nentries);

   Long64_t nbytes = 0, nb = 0;
   TCanvas *c1;
   stringstream ss;
   stringstream Intgral;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
//   for (Long64_t jentry=0; jentry<10;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
    displayProgress(jentry, nentries) ;

      HADshapeIL = new TH1F( "had","had" ,   60, 0  , 20    );
      for (int b = 1; b < h_eHadronic->GetNbinsX(); ++b){
          HADshapeIL->Fill((h_eHadronic->GetXaxis()->GetBinCenter(b) - sim_pvIneInt_z)/hcalintlen, h_eHadronic->GetBinContent(b));
      }

      delete HADshapeIL;
      fDp->Fill(sim_eTotal/p_E);
      if(sim_ePi0First+sim_ePi0Late !=0) fPi0->Fill((sim_ePi0First+sim_ePi0Late)/sim_eTotal);
      if(sim_ePi0First!=0 && sim_ePi0Late !=0) fPi0L->Fill(sim_ePi0Late/(sim_ePi0First+sim_ePi0Late));

      if(sim_ePi0First+sim_ePi0Late ==0) Class->Fill(0.5,1/N);
      if(sim_ePi0First!=0 && sim_ePi0Late ==0) Class->Fill(1.5,1/N);   
      if(sim_ePi0First==0 && sim_ePi0Late !=0) Class->Fill(2.5,1/N);
      if(sim_ePi0First!=0 && sim_ePi0Late !=0) Class->Fill(3.5,1/N);

  }
  const Int_t nx = 4;
  const char *type[nx] = {"H","H+#pi_{0}^{f}","H+#pi_{0}^{l}","H+#pi_{0}^{f}+#pi_{0}^{l}"};

  Int_t i;
  for (i=1;i<=nx;i++) {
     Class->GetXaxis()->SetBinLabel(i,type[i-1]);
     Class->GetXaxis()->SetLabelSize(0.07);
  }
//      c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//      c1->cd();
//      h_eHadronic->SetLineColor(2);
//      h_eHadronic->SetLineWidth(2);
//      Intgral<<h_eHadronic->Integral();
//      h_eHadronic->SetTitle((Intgral.str()).c_str());
//      h_eHadronic->GetXaxis()->SetTitle("mm");
//      h_eHadronic->GetYaxis()->SetTitle("energy");
//      h_eHadronic->GetXaxis()->SetRangeUser(0,5000);
//      ss <<"eHadronic"<<jentry<<".png";
//      h_eHadronic->Draw("h");
//      c1->Print((ss.str()).c_str(),"png");
//      ss.str("");
//      Intgral.str("");
//      delete c1;
//      c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//      c1->cd();
//      HADshapeIL->SetLineColor(2);
//      HADshapeIL->SetLineWidth(2);
//      Intgral<<HADshapeIL->Integral();
//      HADshapeIL->SetTitle((Intgral.str()).c_str());
//      HADshapeIL->GetXaxis()->SetTitle("mm");
//      HADshapeIL->GetYaxis()->SetTitle("energy");
////      HADshapeIL->GetXaxis()->SetRangeUser(0,0);
//      ss <<"eHadronic"<<jentry<<"_Lambda.png";
//      HADshapeIL->Draw("HIST");
//      c1->Print((ss.str()).c_str(),"png");
//      ss.str("");
//      Intgral.str("");
//      delete c1;    
//      delete HADshapeIL;
//
//      c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//      c1->cd();
//      h_ePi0First->SetLineColor(2);
//      h_ePi0First->SetLineWidth(2);
//      h_ePi0First->GetXaxis()->SetTitle("mm");
//      h_ePi0First->GetYaxis()->SetTitle("energy");
//      h_ePi0First->GetXaxis()->SetRangeUser(0,5000);
//      ss <<"ePi0First"<<jentry<<".png";
//      h_ePi0First->Draw("h");
////      c1->Print((ss.str()).c_str(),"png");
//      ss.str("");
//      delete c1;
//
//      c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//      c1->cd();
//      h_ePi0Late->SetLineColor(2);
//      h_ePi0Late->SetLineWidth(2);
//      h_ePi0Late->GetXaxis()->SetTitle("mm");
//      h_ePi0Late->GetYaxis()->SetTitle("energy");
//      h_ePi0Late->GetXaxis()->SetRangeUser(0,5000);
//      ss <<"ePi0Late"<<jentry<<".png";
//      h_ePi0Late->Draw("h");
////      c1->Print((ss.str()).c_str(),"png");
//      ss.str("");
//      delete c1;
      // if (Cut(ientry) < 0) continue;

//   c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//   c1->cd();
//   fDp->Draw("h");
//   c1->Print("fDp.png","png");
//   delete c1;
//
//   c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//   c1->cd();
//   fPi0->Draw("h");
//   c1->Print("fPi0.png","png");
//   delete c1;
//
//   c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,300);
//   c1->cd();
//   fPi0L->Draw("h");
//   c1->Print("fPi0L.png","png");
//   delete c1;

  fPi0->Write("",TObject::kOverwrite);
  fDp->Write("",TObject::kOverwrite);
  fPi0L->Write("",TObject::kOverwrite);
  Class->Write("",TObject::kOverwrite);
  delete fPi0;
  delete fDp;
  delete fPi0L;
  delete Class;
  f->Close();
}
