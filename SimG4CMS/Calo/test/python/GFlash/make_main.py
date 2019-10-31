import sys
import os
import subprocess
import readline
import string

samples = [
'RezaAnalysis_100GeV',
'RezaAnalysis_10GeV',
'RezaAnalysis_1GeV',
'RezaAnalysis_200GeV',
'RezaAnalysis_20GeV',
'RezaAnalysis_2GeV',
'RezaAnalysis_50GeV',
'RezaAnalysis_5GeV'
]


text = ''
for num, sample in enumerate(samples):
    text += 'TC = new TChain("g4SimHits/eventTree") ;\n' +\
    'TC ->Add("' + sample +'*");\n' +\
    'GFlash GF' + sample +'(TC);\n' +\
    'GF'+ sample + '.Loop("' + sample + '");\n' +\
    'delete TC;\n' +\
    '\n' +\
    ''


SHNAME = 'main_GFlash.C'
SHFILE='#include "GFlash.h"\n' +\
'int main(){\n' +\
'TChain* TC;\n' +\
text +\
'}'

open(SHNAME, 'wt').write(SHFILE)
os.system("chmod +x "+SHNAME)
