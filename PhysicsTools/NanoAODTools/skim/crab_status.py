import os
import sys
import re
import string
sys.path.append(os.path.abspath("."))
import Files_2024
import CRABClient
output_stream = os.popen('voms-proxy-info -path')
SubmitCommand= '--proxy=' + output_stream.read().strip()

SAMPLES = {}
UL_YEAR = 'UL24'

if UL_YEAR=='UL24':
#    SAMPLES.update(Files_2024.mc_samples)
    SAMPLES.update(Files_2024.data_samples)
    for key, value in SAMPLES.items():
        os.system("crab status -d crab_NanoAodPostProcessingUL24/crab_" + key)
        os.system("crab resubmit -d crab_NanoAodPostProcessingUL24/crab_" + key)

