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
    can = 'can'
    for i in range(len(H)):
        a,b,c =HN[i].split("_")
        can = c
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
    canvas.Print("ND_" + can + ".png")
    del canvas
    gc.collect()

def compare2Hist(A, B, textA="A", textB="B", label_name="sample", can_name="can"):

    a,b = label_name.split("_")
    canvas = ROOT.TCanvas(can_name,can_name,10,10,1100,628)
    canvas.cd()

    pad_name = "pad"
    pad1=ROOT.TPad(pad_name, pad_name, 0.05, 0.3, 1, 0.99 , 0)
    pad1.Draw()
    pad2=ROOT.TPad(pad_name, pad_name, 0.05, 0.05, 1, 0.3 , 0)
    pad2.SetGridy();
    pad2.Draw()
    pad1.cd()

    A.SetLineColor( 2 )
    B.SetLineColor( 4 )

    A.SetTitle('(E(#pi^{#pm}) =' + b +')')
    A.GetXaxis().SetTitle('#lambda')
    A.GetYaxis().SetTitle('1/E dE/dx')
    A.GetXaxis().SetTitleSize(0.05)
    A.GetYaxis().SetTitleSize(0.05)
    A.SetMaximum(1.2*max(A.GetMaximum(),B.GetMaximum()));
    A.SetMinimum(0.8*min(A.GetMinimum(),B.GetMinimum()));
    A.Draw()
    B.Draw('esame')

    legend = ROOT.TLegend(0.7,0.8,1,1)
    legend.AddEntry(A ,textA,'l')
    legend.AddEntry(B ,textB,'l')
    legend.SetBorderSize(0)
    legend.SetTextFont(42)
    legend.SetTextSize(0.04)
    legend.Draw("same")

    pad2.cd()
    ratio = A.Clone()
    ratio.Divide(B)
    ratio.SetTitle("")
    ratio.SetMaximum(1.25)
    ratio.SetMinimum(0.75)
    ratio.GetXaxis().SetTitle('')
    ratio.GetYaxis().SetTitle('')
    ratio.GetXaxis().SetLabelSize(0.1)
    ratio.GetYaxis().SetLabelSize(0.1)
    ratio.GetYaxis().SetNdivisions(505);
    ratio.SetTitleSize(1)
    ratio.SetTitle(textA +"/"+textB)
    ratio.Draw("e")

    canvas.Print("2H_" + b + "_" + can_name + ".png")
    del canvas
    gc.collect()

samples = [
#'RezaAnalysis_200GeV',
#'RezaAnalysis_100GeV',
#'RezaAnalysis_50GeV',
#'RezaAnalysis_20GeV',
#'RezaAnalysis_10GeV',
#'RezaAnalysis_5GeV',
#'RezaAnalysis_2GeV',
#'RezaAnalysis_1GeV'
'RezaAnalysis_50gev'
]

variable = ['fDp','fPi0','fPi0L', 'Class','HadEPFullsimH','HadEPFullsimHpE','fSP','SP','HadEPFullsimHFit','HadEPFullsimHpEFit', 'fHadECAL','HadEPFullsimHCAL','HadEPFullsimECAL']

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


Fitvariable = [['HadEPFullsimH', 'HadEPFullsimHFit'],['HadEPFullsimHpE','HadEPFullsimHpEFit']]

for num, sample in enumerate(samples):
    for v in Fitvariable:
        file1 = ROOT.TFile.Open('Hist'+sample+'.root')
        if v[0] not in  [file1.GetListOfKeys()[ih].GetName() for ih in range(file1.GetListOfKeys().GetSize())]:
            print('do not find the hist')
            continue
        histA = file1.Get(v[0])
        histB = file1.Get(v[1])
        compare2Hist(histA, histB,'FullSim', 'Fit', sample, v[0] )
        del histA
        del histB
