#include "GFlash.h"
int main(){
TChain* TC;
TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("RezaAnalysis.root");
GFlash GFRezaAnalysis(TC);
GFRezaAnalysis.Loop("RezaAnalysis_50gev");
delete TC;

}
