#include "GFlash.h"
int main(){
TChain* TC;
TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_100GeV*");
GFlash GFRezaAnalysis_100GeV(TC);
GFRezaAnalysis_100GeV.Loop("RezaAnalysis_100GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_10GeV*");
GFlash GFRezaAnalysis_10GeV(TC);
GFRezaAnalysis_10GeV.Loop("RezaAnalysis_10GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_1GeV*");
GFlash GFRezaAnalysis_1GeV(TC);
GFRezaAnalysis_1GeV.Loop("RezaAnalysis_1GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_200GeV*");
GFlash GFRezaAnalysis_200GeV(TC);
GFRezaAnalysis_200GeV.Loop("RezaAnalysis_200GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_20GeV*");
GFlash GFRezaAnalysis_20GeV(TC);
GFRezaAnalysis_20GeV.Loop("RezaAnalysis_20GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_2GeV*");
GFlash GFRezaAnalysis_2GeV(TC);
GFRezaAnalysis_2GeV.Loop("RezaAnalysis_2GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_50GeV*");
GFlash GFRezaAnalysis_50GeV(TC);
GFRezaAnalysis_50GeV.Loop("RezaAnalysis_50GeV");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis_5GeV*");
GFlash GFRezaAnalysis_5GeV(TC);
GFRezaAnalysis_5GeV.Loop("RezaAnalysis_5GeV");
delete TC;

}