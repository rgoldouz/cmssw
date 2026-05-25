from PhysicsTools.NanoAODTools.postprocessing.framework.datamodel import Collection
from PhysicsTools.NanoAODTools.postprocessing.framework.eventloop import Module
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True


class lepTopskimModule(Module):
    def __init__(self):
        self.ellFilt = lambda x: (x.pt>14 and abs(x.eta)<2.5)
        self.muFilt = lambda x: (x.pt>14 and abs(x.eta)<2.5)

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

        electrons = [x for x in filter(self.ellFilt, Collection(event,"Electron"))]
        muons     = [x for x in filter(self.muFilt, Collection(event,"Muon" ))]

        if len(electrons)+len(muons)>= 2:
            acceptEvent = True

        return acceptEvent

# define modules using the syntax 'name = lambda : constructor' to avoid having them loaded when not needed
lepTopskimModuleConstr = lambda: lepTopskimModule()
