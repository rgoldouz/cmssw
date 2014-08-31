#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TPad.h"
/*
Double_t GammaProfile(double alpha,double beta, double showerDepth, int intlength ) {
 double gamma = 0.0;
//radiation length =1.49
if (intlength==1){
 double x = (showerDepth)*(beta/1.49);
 gamma = (beta/1.49)*pow(x,alpha-1.0)*exp(-x)/tgamma(alpha);
}
//interaction length
if (intlength==2){
 double x = (showerDepth)*(beta/16.42);
 gamma = (beta/16.42)*pow(x,alpha-1.0)*exp(-x)/tgamma(alpha);
}

return gamma;
}

Double_t twoGammaProfile(Double_t *x,Double_t *par) {
double twoGamma=0;
twoGamma = par[0]*GammaProfile(par[1],par[2],x[0],1)+(1-par[0])*GammaProfile(par[3],par[4],x[0],2);
return twoGamma;
}
*/

Double_t twoGammaProfile(Double_t *x,Double_t *par) {
double twoGamma=0;
twoGamma = par[0]*(par[2]/1.49)*pow((x[0])*(par[2]/1.49),par[1]-1.0)*exp(-(x[0])*(par[2]/1.49))/tgamma(par[1]) +
(1-par[0])* (par[4]/16.42)*pow((x[0])*(par[4]/16.42),par[3]-1.0)*exp(-(x[0])*(par[4]/16.42))/tgamma(par[3]);
return twoGamma;
}

Double_t GammaProfile1(Double_t *x,Double_t *par) {
double Gamma1=0;
Gamma1 = (par[1]/1.49)*pow((x[0])*(par[1]/1.49),par[0]-1.0)*exp(-(x[0])*(par[1]/1.49))/tgamma(par[0]);
return Gamma1;
}
Double_t GammaProfile2(Double_t *x,Double_t *par) {
double Gamma2=0;
Gamma2 =par[2]*((par[1]/16.42)*pow((x[0])*(par[1]/16.42),par[0]-1.0)*exp(-(x[0])*(par[1]/16.42))/tgamma(par[0]));
return Gamma2;
}

void Hadronicpart_singlepion() {
TCanvas* can;

TFile *f = new TFile("Event.root","recreate");	  	  	
TTree *tree = new TTree("GFLASH","GFLASHparamter");


Float_t C, ALPHA1, BETA1, ALPHA2, BETA2;
tree->Branch("C",&C,"C/F");
tree->Branch("ALPHA1",&ALPHA1,"ALPHA1/F");
tree->Branch("BETA1",&BETA1,"BETA1/F");
tree->Branch("ALPHA2",&ALPHA2,"ALPHA2/F");
tree->Branch("BETA2",&BETA2,"BETA2/F");

std::vector<TH1F*> hists_EL;
TH1F *h1;
TH1F *h10;
TF1 *func;
TF1 *func1;
TF1 *func2;
double nbinE=150;
	TFile *input(0);
//	TString fname ="reza.root";
	TString fname ="rezanew.root";
	input = TFile::Open( fname ); 

	

	std::vector<double> *mytime=0;
	std::vector<double> *myx=0;
	std::vector<double> *myy=0;
	std::vector<double> *myz=0;
	std::vector<double> *mydepth=0;
	std::vector<double> *myenergy=0;
        double energyGen=0;
        double phiGen=0;
        double etaGen=0;
        double c1=0;
        double a1=0;
        double b1=0;
        double a2=0;
        double b2=0;
        double startingPoint=0;

	TTree* theTree = (TTree*)input->Get("FitParametersTree");
        theTree->SetBranchAddress("mytime",&mytime);
        theTree->SetBranchAddress("myx",&myx);
        theTree->SetBranchAddress("myy",&myy);
        theTree->SetBranchAddress("myz",&myz);
        theTree->SetBranchAddress("mydepth",&mydepth);
        theTree->SetBranchAddress("myenergy",&myenergy);
        theTree->SetBranchAddress("energyGen",&energyGen);
        theTree->SetBranchAddress("phiGen",&phiGen);
        theTree->SetBranchAddress("etaGen",&etaGen);
        theTree->SetBranchAddress("startingPoint",&startingPoint);
        theTree->SetBranchAddress("c",&c1);
        theTree->SetBranchAddress("alpha1",&a1);
        theTree->SetBranchAddress("beta1",&b1);
        theTree->SetBranchAddress("alpha2",&a2);
        theTree->SetBranchAddress("beta2",&b2);

	for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
        double sumECAL=0;
        double sumHCAL=0;  
	std::stringstream oname;
        h1 = new TH1F("h1", "Longitudinal Energy profile", nbinE, 0.5, 150.5);
        h10 = new TH1F("h10", "Longitudinal Energy profile 10", 2*nbinE, -150, 150);
	theTree->GetEntry(ievt);
	double finalweight;
	double sp=0;
	if (startingPoint>0) sp=startingPoint;
	finalweight=(*mytime)[0];
		for(unsigned int idx=0; idx<mydepth->size(); ++idx){
		if ((*mydepth)[idx]>0) sumHCAL += (*myenergy)[idx];
		else sumECAL += (*myenergy)[idx];
		}
	cout<<mydepth->size()<<endl;

       		for(unsigned int idx=0; idx<mydepth->size(); ++idx){
//if (ievt==4) cout<<(*mydepth)[idx]*16.42<<endl;
//if (ievt==4) cout<<(*myenergy)[idx]*16.42<<endl;
		if ((*mydepth)[idx]>0) h1->Fill((*mydepth)[idx]*16.42-sp,(*myenergy)[idx]);
		if (ievt==9) h10->Fill((*mydepth)[idx]*16.42,(*myenergy)[idx]);
		}


 		can = new TCanvas("fit","fit",700,500);
                can->cd();
              	oname << "fit_energy_twogammafunction_event_" << ievt+1;

cout<<ievt+1<<"       "<<sp<<"       "<<startingPoint<<endl;
//	h1->Scale(1/h1->Integral());
	h1->SetLineColor(1);
	h1->SetLineWidth(3);
	func = new TF1("GammaP2", GammaProfile2, 1, 150.,3);
	func->SetParameters(1,0.1,15);
	func->SetParNames ("alfa1","beta1");
	func->FixParameter(2,sumHCAL);

cout<<"&&&&&&&&&&&&&&&&&&&&&&&&"<<h1->Integral()<<"       "<<sumHCAL<<endl;
//	func->SetParLimits(0,  0,  1.0);
	h1->Fit("GammaP2","WW");

	ALPHA1=func->GetParameter(1);
	BETA1=func->GetParameter(2);

	tree->Fill();
	hists_EL.push_back(h1);
	func->GetYaxis()->SetRangeUser(0,2.5*h1->GetMaximum());
	func->GetXaxis()->SetRangeUser(0,160);
	func->GetXaxis()->SetTitle("depth of shower from the starting point (cm)");
	func->GetYaxis()->SetTitle("1/E dE/dZ");





	func1 = new TF1("GP1", GammaProfile2, 1, 150.,3);
	func1->SetLineColor(4);
	func1->SetParameters(exp(a2),exp(b2),sumHCAL);
//        if (ievt==19) func1->SetParameter(2,14.0942); 
//        if (ievt==3) func1->SetParameter(2,13.5158); 
//        if (ievt==15) func1->SetParameter(2,15.4858); 
	func1->GetYaxis()->SetRangeUser(0,1.7*h1->GetMaximum());

	TH1F *ho = new TH1F("ho","background + signal",150,0.5,150.5);
   	ho->SetFillColor(18);
   	ho->SetLineColor(18);
	for(unsigned int idx=1; idx<151; ++idx){
	ho->SetBinContent(idx,func1->Eval(idx));
		}


	func1->Draw();
	ho->Draw("same");
	h1->Draw("same");
	func1->Draw("same");
	func->Draw("same");

	std::stringstream alpha1_,beta1_,alpha2_,beta2_,startP;
	startP.precision(3);
	alpha1_.precision(3);
	beta1_.precision(3);
	alpha2_.precision(3);
	beta2_.precision(3);
        alpha1_<< fixed << "alpha2 (fit value)="<< func->GetParameter(0) << " #pm " << func->GetParError(1);
        beta1_<< fixed << "beta2 (fit value)= " << func->GetParameter(1) << " #pm " << func->GetParError(2);
        alpha2_<< fixed << "alpha2 (True value)="<<  exp(a2) ;
        beta2_<< fixed << "beta2 (True value)= " << exp(b2) ;
	startP<< fixed << "start point="<<startingPoint<< " Energy in ECAL= "<< sumECAL<< " Energy in HCAL= "<< func1->GetParameter(2);

	TPaveText* pave_par;
	pave_par = new TPaveText(40,h1->GetMaximum(),80,1.5*h1->GetMaximum(),"TR");
	pave_par->AddText((oname.str()).c_str());
        pave_par->AddText((startP.str()).c_str());
        pave_par->AddText((alpha1_.str()).c_str());
        pave_par->AddText((beta1_.str()).c_str());
        pave_par->AddText((alpha2_.str()).c_str());
        pave_par->AddText((beta2_.str()).c_str());

                pave_par->SetFillColor(0);
                pave_par->SetBorderSize(0);
                pave_par->SetTextFont(12);
                pave_par->SetTextSize(0.035);
                pave_par->Draw("same");

	TLegend* leg = new TLegend(0.68,0.40,0.85,0.60);
	leg->AddEntry( h1, "energy profile "               , "L");
	leg->AddEntry( func, "hadronic part from the fit "               , "L");
	leg->AddEntry( func1, "hadronic part True function"               , "L");
	leg->Draw("same");



	can->Print((oname.str()+".png").c_str(),"png");
        }
//hists_EL[0]->Draw("c");

for(unsigned int idx=0; idx<theTree->GetEntries(); ++idx){
//hists_EL[idx]->Draw("csame");
//hists_EL[idx]->SetLineColor(2*idx-1);
hists_EL[idx]->SetLineColor(1);
hists_EL[idx]->SetLineWidth(3);
cout<<hists_EL[idx]->Integral()<<endl;
}

h10->Draw();
tree->Write();
f->Write();
f->Close();
//leg->Draw("same");
cout<<" 00000000000000000000000000000000"<<endl;
/*
	func = new TF1("twoGammaProfile", twoGammaProfile, 0, 150.,5);
//	func->SetParameters(0.468229,exp(0.0871855), exp(-2.64866), exp(0.669427),exp(-0.0411287));
//	func->SetParameters(0.751057,exp(0.60782), exp(-1.56529), exp(1.30441),exp(0.725733));
//	func->SetParameters(0.303598,exp(1.29797), exp(-1.45035), exp(1.08175),exp(0.489677));
	func->SetParameters(0,exp(0.433838), exp(-1.96671), exp(0.256295),exp(0.203868));
//	func->SetParameters(0.82849,exp(1.38254), exp(-0.98907), exp(0.800386),exp(0.855279));
 
	func->SetParNames ("c","alfa1","beta1","alfa2","beta2");
//	func->SetParLimits(0,  0,  1.0);
//	hists_EL[0]->Fit("twoGammaProfile");
	func1 = new TF1("GammaProfile1", GammaProfile1, 0, 150.,3);
	func1->SetLineColor(4);
	func1->SetLineWidth(3);

//	func1->SetParameters(1,1.0911, 0.0707457);
//	func1->SetParameters(1,2.92242, 0.126674);
//	func1->SetParameters(1,2.74767, 0.139014);
//	func1->SetParameters(1,3.71045, 0.239618);
	func1->SetParameters(1,1.97888, 0.119737);
	func2 = new TF1("GammaProfile2", GammaProfile2, 0, 150.,3);
	func2->SetLineColor(3);
	func2->SetLineWidth(3);
	func2->SetParameters(1, exp(0.669427),exp(-0.0411287));
//	func2->SetParameters(1, exp(1.30441),exp(0.725733));
//	func2->SetParameters(1, exp(1.08175),exp(0.489677));
//	func2->SetParameters(1, exp(0.256295),exp(0.203868));
//	func2->SetParameters(1, exp(0.800386),exp(0.855279));
	func->Draw();
	func1->Draw("same");
	func2->Draw("same");
	hists_EL[4]->Draw("same");
	func->GetXaxis()->SetTitle("Shower depth in HCAL (cm)");
	func->GetYaxis()->SetTitle("1/E dE/dZ");
    TLegend* leg2 = new TLegend(0.68,0.40,0.85,0.60);
    leg2->AddEntry( hists_EL[3], "energy profile "               , "L");
    leg2->AddEntry( func, "two gamma function "               , "L");
    leg2->AddEntry( func1, "Pi0 part "               , "L");
    leg2->AddEntry( func2, "hadronic part"               , "L");
    leg2->Draw("same");

*/



}
