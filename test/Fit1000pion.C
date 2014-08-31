//mean variance and correlation of the GFLASH parameters for 1000 pion events 
// please run saveparametersof1000pion.C before running this file

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TSpectrum.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TPolyMarker.h"
#include "TLine.h"
#include "TStyle.h"
#include "TDirectory.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TPad.h"
#include "TPaveStats.h"


void Fit1000pion() {
TCanvas* can;
TCanvas* can2;
TCanvas* can3;

std::vector<TH1F*> hists_EL;
TH1F *alpha;
TH1F *beta;
TH1F *alphatrue;
TH1F *betatrue;
TH2F *alphabeta;
TH2F *alphabetatrue;
//THF2 *coralphabeta;
        double Emin =-1.5;
        double Emax = 2.5;
        int nbins =50;

	TFile *input(0);
	TString fname ="Event.root";
	input = TFile::Open( fname ); 
	TTree* theTree = (TTree*)input->Get("GFLASH");
        std::stringstream drawnamealpha;
        std::stringstream drawnamebeta;
        std::stringstream drawnamealphaTrue;
        std::stringstream drawnamebetaTrue;
        std::stringstream drawnamecorr;
        std::stringstream cutname;

cutname << "" ;

        drawnamealpha << "log(ALPHA2)>>halpha(" << nbins << "," << Emin << "," << Emax << ")";
        drawnamebeta << "log(BETA2)>>hbeta(" << nbins << "," << Emin << "," << Emax << ")";
        drawnamealphaTrue << "log(ALPHA2TRUE)>>halphaTrue(" << nbins << "," << Emin << "," << Emax << ")";
        drawnamebetaTrue << "log(BETA2TRUE)>>hbetaTrue(" << nbins << "," << Emin << "," << Emax << ")";

        theTree->Draw((drawnamealpha.str()).c_str(),(cutname.str()).c_str(),"hist1");
        theTree->Draw((drawnamebeta.str()).c_str(),(cutname.str()).c_str(),"hist2");
        theTree->Draw((drawnamealphaTrue.str()).c_str(),(cutname.str()).c_str(),"hist3");
        theTree->Draw((drawnamebetaTrue.str()).c_str(),(cutname.str()).c_str(),"hist4");
        theTree->Draw("log(ALPHA2):log(BETA2)>>hAlphaBeta(nbins,Emin,Emax,nbins,Emin,Emax)","","hist5");
        theTree->Draw("log(ALPHA2TRUE):log(BETA2TRUE)>>hAlphaBetaTrue(nbins,Emin,Emax,nbins,Emin,Emax)","","hist6");

//        theTree->Draw((drawnamealpha.str()).c_str(),"reza","histgoff");
        alpha = (TH1F*)gDirectory->Get("halpha");
	beta = (TH1F*)gDirectory->Get("hbeta");
	alphatrue = (TH1F*)gDirectory->Get("halphaTrue");
	betatrue = (TH1F*)gDirectory->Get("hbetaTrue");
	alphabeta = (TH2F*)gDirectory->Get("hAlphaBeta");
	alphabetatrue = (TH2F*)gDirectory->Get("hAlphaBetaTrue");

can = new TCanvas("fit","fit",700,500);
can->cd();

	TLegend* leg = new TLegend(0.8,0.20,0.95,0.35);
	leg->AddEntry( alpha, " From the fit  "               , "F");
	leg->AddEntry( alphatrue, " True value "               , "L");

alpha->SetStats(kTRUE);
alpha->SetLineColor(4);
alpha->SetFillColor(4);
alpha->SetLineWidth(2);
alpha->SetTitle("distribution of alpha parameter");
alpha->GetXaxis()->SetTitle("Log(alpha)");
alphatrue->SetLineColor(2);
alphatrue->SetLineWidth(2);
alpha->Draw();
alphatrue->Draw("sames");
gPad->Update();
TPaveStats *st = (TPaveStats*)alphatrue->FindObject("stats");
st->SetX1NDC(0.1); //new x start position
st->SetX2NDC(0.3); //new x end position
leg->Draw("same");
can->Print("alpha.png","png");

can2 = new TCanvas("fit2","fit2",700,500);
can2->cd();
beta->SetLineColor(4);
beta->SetLineWidth(2);
beta->SetFillColor(4);
beta->SetTitle("distribution of beta parameter");
beta->GetXaxis()->SetTitle("Log(beta)");
betatrue->SetLineColor(2);
betatrue->SetLineWidth(2);
beta->Draw();
betatrue->Draw("sames");
 gPad->Update();
TPaveStats *st2 = (TPaveStats*)betatrue->FindObject("stats");
st2->SetX1NDC(0.1); //new x start position
st2->SetX2NDC(0.3); //new x end position
leg->Draw("same");
can2->Print("beta.png","png");

can3 = new TCanvas("fit3","fit3",700,500);
can3->cd();
alphabeta->SetMarkerColor(4);
alphabeta->SetMarkerStyle(7);
alphabetatrue->SetMarkerColor(2);
alphabetatrue->SetMarkerStyle(7);
alphabeta->SetTitle("alpha beta correlation");
alphabeta->GetXaxis()->SetTitle("Log(alpha)");
alphabeta->GetYaxis()->SetTitle("Log(beta)");
alphabeta->Draw();
alphabetatrue->Draw("sames");
leg->Draw("same");
 gPad->Update();
TPaveStats *st3 = (TPaveStats*)alphabetatrue->FindObject("stats");
st3->SetX1NDC(0.1); //new x start position
st3->SetX2NDC(0.3); //new x end position
can3->Print("correlation.png","png");
cout<<27.227+26.921*tanh(-0.35664*(log(20)+4.8584))+0.1169*log(20)<<endl;
cout<<21.544-22.048*tanh(0.25227*(log(20)+5.2186))+0.067578*log(20)<<endl;
cout<<alphabetatrue->GetCorrelationFactor()<<endl;
cout<<alphabeta->GetCorrelationFactor()<<endl;
}
