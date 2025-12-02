from PhysicsTools.NanoAODTools.postprocessing.framework.datamodel import Collection
from PhysicsTools.NanoAODTools.postprocessing.framework.eventloop import Module
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True


class skimModule(Module):
    def __init__(self):
        self.elFilt = lambda x: x.pt>50
        self.muFilt = lambda x: (x.pt>50 and x.isGlobal)
        self.phFilt = lambda x: (x.pt>100 and x.hoe<0.05)

        self.ctEl = 1 # at least one electron
        self.ctMu = 1 # at least one muon
        self.ctPh = 1 # at least one photon
        pass

    def beginJob(self):
        pass

    def endJob(self):
        pass

    def beginFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
        self.out = wrappedOutputTree

    def endFile(self, inputFile, outputFile, inputTree, wrappedOutputTree):
        pass

    def analyze(self, event):
        """process event, return True (go to next module) or False (fail, go to next event)"""
        acceptEvent = False

        electrons = [x for x in filter(self.elFilt, Collection(event,"Electron"))]
        muons     = [x for x in filter(self.muFilt, Collection(event,"Muon"    ))]
        photons   = [x for x in filter(self.phFilt, Collection(event,"Photon"  ))]

        nEl = len(electrons)
        nMu = len(muons)
        nPh = len(photons)

#        if (nEl>=self.ctEl or nMu>=self.ctMu or nPh>=self.ctPh):
#        if (nMu>=self.ctMu or nPh>=self.ctPh):
        if (nPh>=self.ctPh):
            acceptEvent = True

        return acceptEvent

# define modules using the syntax 'name = lambda : constructor' to avoid having them loaded when not needed
photonSkimModuleConstr = lambda: skimModule()
