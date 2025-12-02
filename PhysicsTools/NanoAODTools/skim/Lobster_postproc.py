#!/usr/bin/env python
import os
import sys
import re
import string
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


year=options.year
moduleList = []
outdir=[]
myargs=[]

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
    from PhysicsTools.NanoAODTools.postprocessing.utils.crabhelper import inputFiles, runsAndLumis

if options.DataProcessing == 'mc':
    moduleList = [lepTopskimModuleConstr()]
    if options.cr == 'Yes':
        p=PostProcessor(".", inputFiles(), modules=moduleList, provenance=True, fwkJobReport=True, outputbranchsel="keep_and_drop.txt")
    else:
        if options.eft== 'Yes':
            del moduleList[0]
        p=PostProcessor(".", myargs, modules=moduleList, provenance=True, fwkJobReport=True, outputbranchsel="keep_and_drop.txt")
else:
    moduleList = [lepTopskimModuleConstr()]
    if options.cr == 'Yes':
        p=PostProcessor(".", inputFiles(), jsonInput=options.json, modules=moduleList, provenance=True, fwkJobReport=True)
    else:
        p=PostProcessor(".", myargs, jsonInput=options.json, modules=moduleList, provenance=True, fwkJobReport=True)

p.run()

print("DONE")
