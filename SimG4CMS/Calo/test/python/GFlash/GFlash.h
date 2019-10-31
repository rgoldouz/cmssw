//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct 16 07:37:27 2019 by ROOT version 6.12/07
// from TTree eventTree/Event tree
// found on file: RezaAnalysis.root
//////////////////////////////////////////////////////////

#ifndef GFlash_h
#define GFlash_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TH1.h"

class GFlash {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         p_E;
   Float_t         p_eta;
   Float_t         p_phi;
   Int_t           p_pdgid;
   Float_t         p_ePi0First;
   Float_t         p_ePi0Late;
   Float_t         sim_eTotal;
   Float_t         sim_ePi0First;
   Float_t         sim_ePi0Late;
   Float_t         sim_eHadronic;
   Float_t         sim_pvInt_x;
   Float_t         sim_pvInt_y;
   Float_t         sim_pvInt_z;
   Float_t         sim_pvIneInt_x;
   Float_t         sim_pvIneInt_y;
   Float_t         sim_pvIneInt_z;
   TH1F            *h_eHadronic;
   TH1F            *h_ePi0First;
   TH1F            *h_ePi0Late;

   // List of branches
   TBranch        *b_p_E;   //!
   TBranch        *b_p_eta;   //!
   TBranch        *b_p_phi;   //!
   TBranch        *b_p_pdgid;   //!
   TBranch        *b_p_ePi0First;   //!
   TBranch        *b_p_ePi0Late;   //!
   TBranch        *b_sim_eTotal;   //!
   TBranch        *b_sim_ePi0First;   //!
   TBranch        *b_sim_ePi0Late;   //!
   TBranch        *b_sim_eHadronic;   //!
   TBranch        *b_sim_pvInt_x;   //!
   TBranch        *b_sim_pvInt_y;   //!
   TBranch        *b_sim_pvInt_z;   //!
   TBranch        *b_sim_pvIneInt_x;   //!
   TBranch        *b_sim_pvIneInt_y;   //!
   TBranch        *b_sim_pvIneInt_z;   //!
   TBranch        *b_h_eHadronic;   //!
   TBranch        *b_h_ePi0First;   //!
   TBranch        *b_h_ePi0Late;   //!

   GFlash(TTree *tree=0);
   virtual ~GFlash();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   TH1F *fDp, *fPi0, *fPi0L;
};

#endif

#ifdef GFlash_cxx
GFlash::GFlash(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("RezaAnalysis.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("RezaAnalysis.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("RezaAnalysis.root:/g4SimHits");
      dir->GetObject("eventTree",tree);

   }
   Init(tree);
}

GFlash::~GFlash()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t GFlash::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t GFlash::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void GFlash::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   h_eHadronic = 0;
   h_ePi0First = 0;
   h_ePi0Late = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("p_E", &p_E, &b_p_E);
   fChain->SetBranchAddress("p_eta", &p_eta, &b_p_eta);
   fChain->SetBranchAddress("p_phi", &p_phi, &b_p_phi);
   fChain->SetBranchAddress("p_pdgid", &p_pdgid, &b_p_pdgid);
   fChain->SetBranchAddress("p_ePi0First", &p_ePi0First, &b_p_ePi0First);
   fChain->SetBranchAddress("p_ePi0Late", &p_ePi0Late, &b_p_ePi0Late);
   fChain->SetBranchAddress("sim_eTotal", &sim_eTotal, &b_sim_eTotal);
   fChain->SetBranchAddress("sim_ePi0First", &sim_ePi0First, &b_sim_ePi0First);
   fChain->SetBranchAddress("sim_ePi0Late", &sim_ePi0Late, &b_sim_ePi0Late);
   fChain->SetBranchAddress("sim_eHadronic", &sim_eHadronic, &b_sim_eHadronic);
   fChain->SetBranchAddress("sim_pvInt_x", &sim_pvInt_x, &b_sim_pvInt_x);
   fChain->SetBranchAddress("sim_pvInt_y", &sim_pvInt_y, &b_sim_pvInt_y);
   fChain->SetBranchAddress("sim_pvInt_z", &sim_pvInt_z, &b_sim_pvInt_z);
   fChain->SetBranchAddress("sim_pvIneInt_x", &sim_pvIneInt_x, &b_sim_pvIneInt_x);
   fChain->SetBranchAddress("sim_pvIneInt_y", &sim_pvIneInt_y, &b_sim_pvIneInt_y);
   fChain->SetBranchAddress("sim_pvIneInt_z", &sim_pvIneInt_z, &b_sim_pvIneInt_z);
   fChain->SetBranchAddress("h_eHadronic", &h_eHadronic, &b_h_eHadronic);
   fChain->SetBranchAddress("h_ePi0First", &h_ePi0First, &b_h_ePi0First);
   fChain->SetBranchAddress("h_ePi0Late", &h_ePi0Late, &b_h_ePi0Late);
   Notify();
}

Bool_t GFlash::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void GFlash::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t GFlash::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef GFlash_cxx
