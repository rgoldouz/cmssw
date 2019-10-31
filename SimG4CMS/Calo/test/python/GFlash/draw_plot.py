import math
import gc
import sys
import ROOT
import numpy as npi
import copy
from array import array
import os
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gROOT.ProcessLine("gErrorIgnoreLevel = 1;")
ROOT.TH1.AddDirectory(ROOT.kFALSE)
ROOT.gStyle.SetOptStat(0)
from ROOT import TColor
from ROOT import TGaxis
import gc
TGaxis.SetMaxDigits(2)
################################## MY SIGNAL AND SM BG ################################

def draw1dHist(A,textA="A", label_name="sample", can_name="can"):
    a,b = label_name.split("_")
    canvas = ROOT.TCanvas(can_name,can_name,10,10,1100,628)
    canvas.cd()

    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0.05, 0.05, 1, 0.99 , 0)
    pad1.Draw()

    A.SetLineColor( 4 )
    A.SetLineWidth( 2 )
    A.SetTitle('(E(#pi^{#pm}) =' + b +')')
    A.GetXaxis().SetTitle(textA)
    A.GetYaxis().SetTitle('N_events')
    A.GetXaxis().SetTitleSize(0.05)
    A.GetYaxis().SetTitleSize(0.05)
    A.SetMaximum(1.2*A.GetMaximum())
    A.SetMinimum(0);
    A.GetYaxis().SetTitleOffset(0.7)
    A.Draw()
    canvas.Print("1D_" + b +"_"+ textA + ".png")
    del canvas
    gc.collect()

def drawCombine(H = [], HN=[]):
    canvas = ROOT.TCanvas("can_name","can_name",10,10,1100,628)
    canvas.cd()
    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0.05, 0.05, 1, 0.99 , 0)
    pad1.Draw()
    legend = ROOT.TLegend(0.3,0.6,0.4,0.87)
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.03)
    for i in range(len(H)):
        a,b,c =HN[i].split("_")
        H[i].SetLineColor( i+1 )
        if i==0:
            H[i].SetTitle('')
            H[i].GetXaxis().SetTitle(c)
            H[i].GetYaxis().SetTitle('Fraction')
            H[i].GetXaxis().SetTitleSize(0.05)
            H[i].GetYaxis().SetTitleSize(0.05)
            H[i].SetMaximum(1.5*H[i].GetMaximum())
            H[i].SetMinimum(0);
            H[i].GetYaxis().SetTitleOffset(0.7)
            H[i].Draw()   
        H[i].Draw("Same")
        legend.AddEntry(H[i] ,b,'l')
    legend.Draw("same")
    canvas.Print("ND_" + c + ".png")
    del canvas
    gc.collect()


samples = [
'RezaAnalysis_200GeV',
'RezaAnalysis_100GeV',
'RezaAnalysis_50GeV',
'RezaAnalysis_20GeV',
'RezaAnalysis_10GeV',
'RezaAnalysis_5GeV',
'RezaAnalysis_2GeV',
'RezaAnalysis_1GeV'
]

variable = ['fDp','fPi0','fPi0L', 'Class']

for num, sample in enumerate(samples):
    for v in variable:
        file1 = ROOT.TFile.Open('Hist'+sample+'.root')
        if v not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
            print('do not find the hist')
            continue
        histA = file1.Get(v)
        print sample + v
        draw1dHist(histA, v, sample )
        del histA


Hist=[]
HistName=[]

for v in variable:
    for num, sample in enumerate(samples):
        file1 = ROOT.TFile.Open('Hist'+sample+'.root')
        if v not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
            print('do not find the hist')
            continue     
        Hist.append(file1.Get(v))
        HistName.append(sample+'_'+v)
    drawCombine(Hist,HistName)
    del Hist[:] 
    del HistName[:]

