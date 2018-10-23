#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

#------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------

options = VarParsing.VarParsing()

options.register('skipEvents',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")

options.register('processEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('inputFiles',
                 #"file:/eos/cms/store/data/Run2018D/JetHT/RAW/v1/000/323/778/00000/909E5AC5-D2BA-2743-BD18-FA8920DB89B6.root",
                 #"root://eoscms.cern.ch//store/data/Run2018D/JetHT/RAW/v1/000/323/778/00000/909E5AC5-D2BA-2743-BD18-FA8920DB89B6.root",
                 #"root://eoscms.cern.ch//store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/323/277/00000/146696BA-70A6-C24A-A124-9FD551275D40.root",
		 "dummyinput",
		 #"root://cms-xrd-global.cern.ch///store/data/Run2018D/JetHT/RAW/v1/000/323/778/00000/909E5AC5-D2BA-2743-BD18-FA8920DB89B6.root",
		 #'/store/data/Run2018D/ZeroBias/RAW/v1/000/323/345/00000/952D1F9B-1874-C448-AC71-C21B83FCCA34.root',
                 #'/store/data/Run2018D/ZeroBias/RAW/v1/000/323/345/00000/9ACA3C0C-F287-044C-B937-F44F3A6E5481.root',
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

#options.register('inputFiles',
#                 #"file:inputFile.root", #default value
#                 "file:/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run314692/USC_314692.root",
#                 VarParsing.VarParsing.multiplicity.list,
#                 VarParsing.VarParsing.varType.string,
#                 "Input files")
#
options.register('outputFile',
                 "file:fcdanalysis_323336.root", #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.register('threads',
                 4, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of threads")

options.parseArguments()

print " "
print "Using options:"
print " skipEvents    =", options.skipEvents
print " processEvents =", options.processEvents
print " inputFiles    =", options.inputFiles
print " outputFile    =", options.outputFile
print " "


#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
from Configuration.StandardSequences.Eras import eras
process = cms.Process('PFG',eras.Run2_2018)

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.processEvents) )

process.source = cms.Source(
    "PoolSource",
    #fileNames  = cms.untracked.vstring(options.inputFiles),
    fileNames  = cms.untracked.vstring(
    #323336
    '/store/data/Run2018D/ZeroBias/RAW/v1/000/323/336/00000/A1C42694-F52C-D04E-ACEB-434DAE62A6DA.root',
    '/store/data/Run2018D/ZeroBias/RAW/v1/000/323/336/00000/CD73C549-4693-1943-AAC5-45B9BE271EDB.root',
    '/store/data/Run2018D/ZeroBias/RAW/v1/000/323/336/00000/CDF9494D-B0BB-1949-9BC0-0EC7C2F07138.root'
    #323345
    #'root://cms-xrd-global.cern.ch///store/data/Run2018D/ZeroBias/RAW/v1/000/323/345/00000/952D1F9B-1874-C448-AC71-C21B83FCCA34.root',
    #'root://cms-xrd-global.cern.ch///store/data/Run2018D/ZeroBias/RAW/v1/000/323/345/00000/9ACA3C0C-F287-044C-B937-F44F3A6E5481.root'
    #  "file:/home/users/jaehyeok/HCAL/ntupler/files/splashes2018/BeamSplash_run313574_beam1.root",
    #  "file:/home/users/jaehyeok/HCAL/ntupler/files/splashes2018/BeamSplash_run313574_beam2.root"
    ),
    skipEvents = cms.untracked.uint32(options.skipEvents)
    )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.outputFile)
    )

process.options = cms.untracked.PSet(
  numberOfThreads = cms.untracked.uint32(options.threads)
)


#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)
process.load('Configuration.EventContent.EventContent_cff')

process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
#process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("CondCore.CondDB.CondDB_cfi")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
process.load("HCALPFG.FCDAnalysis.FCDAnalysis_cfi")

#------------------------------------------------------------------------------------
# Specify Global Tag
# See https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions for list
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag =   '101X_dataRun2_HLT_v7'

#------------------------------------------------------------------------------------
# Analysis sequence definition
#------------------------------------------------------------------------------------
process.analysis_step = cms.Sequence(
    process.fcdAnalysis
)

#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    # Unpack digis from RAW
    process.hcalDigis*
    # Do energy reconstruction
#    process.hbheprereco*
#    process.hbhereco*
#    process.hfprereco*
#    process.hfreco*
#    process.horeco*
    # Make the ntuples
    process.analysis_step
)

#-----------------------------------------------------------------------------------
# Dump configuraiton
#-----------------------------------------------------------------------------------
#dump = file('dump.py', 'w')
#dump.write( process.dumpPython() )
#dump.close()
