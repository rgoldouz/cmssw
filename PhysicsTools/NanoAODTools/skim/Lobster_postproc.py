#!/usr/bin/env python
import os
import sys
import re
import string
import PSet
from PhysicsTools.NanoAODTools.postprocessing.modules.etop.skimModule import *
from PhysicsTools.NanoAODTools.postprocessing.modules.lepTop.lepTopskimModule import *
from PhysicsTools.NanoAODTools.postprocessing.framework.postprocessor import *

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] outputDir inputFiles")
    parser.add_option("-s", "--postfix", dest="postfix", type="string", default=None,
                      help="Postfix which will be appended to the file name (default: _Friend for friends, _Skim for skims)")
    parser.add_option("-J", "--json", dest="json", type="string",
                      default="A", help="Select events using this JSON file")
    parser.add_option("-y", "--year", dest="year", type="string",
                      default=("UL2017"), help="Which year do you run on? ")
    parser.add_option("-d", "--DataProcessing", dest="DataProcessing", type="string",
                      default=("mc"), help="Which dataset do you run on? data or mc? ")
    parser.add_option("-r", "--run", dest="run", type="string",
                      default=("mc"), help="Which run period do you run on?")
    parser.add_option("-c", "--cr", dest="cr", type="string",
                      default=("Yes"), help="do you use crab??")
    parser.add_option("-e", "--eft", dest="eft", type="string",
                      default=("No"), help="is it an EFT sample??")
    (options, args) = parser.parse_args()

print("Current dir:", os.listdir("."))
year=options.year
moduleList = []
outdir=[]
myargs=[]
files=[]
print ('cr=' + options.cr)
print ('DataProcessing=' + options.DataProcessing)
print ('year=' + options.year)
print ('run=' + options.run)
print ('json=' + options.json)

if options.cr == 'No':
    print ('ajab'+str(len(args)))
    if len(args) < 2:
        parser.print_help()
        sys.exit(1)
    outdir = args[0]
    myargs = args[1:]
    print(args)
else:
    files = list(PSet.process.source.fileNames)
    print (files)
    #from PhysicsTools.NanoAODTools.postprocessing.utils.crabhelper import inputFiles, runsAndLumis

if options.DataProcessing == 'mc':
    moduleList = [lepTopskimModuleConstr()]
    if options.cr == 'Yes':
        p=PostProcessor(".", files, modules=moduleList, provenance=False, fwkJobReport=True, outputbranchsel="keep_and_drop.txt")
    else:
        if options.eft== 'Yes':
            del moduleList[0]
        p=PostProcessor(".", myargs, modules=moduleList, provenance=False, fwkJobReport=True, outputbranchsel="keep_and_drop.txt")
else:
    moduleList = [lepTopskimModuleConstr()]
    if options.cr == 'Yes':
        p=PostProcessor(".", files, jsonInput=options.json, modules=moduleList, provenance=False, fwkJobReport=True, outputbranchsel="keep_and_drop.txt")
    else:
        p=PostProcessor(".", myargs, jsonInput=options.json, modules=moduleList, provenance=False, fwkJobReport=True, outputbranchsel="keep_and_drop.txt")

p.run()

print("DONE")
print("Current dir:", os.listdir("."))

import glob
import subprocess

# Find the output file created by the PostProcessor
# It usually ends in _Skim.root or matches the input name
out_files = glob.glob("*_Skim.root")

if out_files:
    print(f"Manually merging {out_files} into tree.root...")
    # Call haddnano.py manually.
    # Make sure haddnano.py is in your inputFiles in crab_config
    cmd = ["python3", "haddnano.py", "tree.root"] + out_files
    subprocess.run(cmd, check=True)
    print("Merge Complete: tree.root created.")
else:
    print("Error: No skimmed files found to merge!")
# --- MANUAL MERGE END ---
