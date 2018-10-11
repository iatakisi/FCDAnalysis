import FWCore.ParameterSet.Config as cms

# FCD channel coordinates (crate, slot, fiber, fiberCh)
fcd_channels_tuple = [
	(38, 11, 0, 0),
	(38, 11, 0, 1),
	(38, 11, 0, 2),
	(38, 11, 0, 3),
	(38, 11, 1, 0),
	(38, 11, 1, 1),
	(38, 11, 1, 2),
	(38, 11, 1, 3),
	(38, 11, 2, 0),
	(38, 11, 2, 1),
	(38, 11, 2, 2),
	(38, 11, 2, 3),
	(38, 11, 3, 0),
	(38, 11, 3, 1),
	(38, 11, 3, 2),
	(38, 11, 3, 3),
	(38, 11, 4, 0),
	(38, 11, 4, 1),
	(38, 11, 4, 2),
	(38, 11, 4, 3),
	(38, 11, 5, 0),
	(38, 11, 5, 1),
	(38, 11, 5, 2),
	(38, 11, 5, 3),
]

# Convert tuple to CMSSW object
fcd_channels = cms.PSet(
	crate = cms.untracked.vint32(),
	slot = cms.untracked.vint32(),
	fiber = cms.untracked.vint32(),
	fiber_channel = cms.untracked.vint32()
)
for channel in fcd_channels_tuple:
	fcd_channels.crate.append(channel[0])
	fcd_channels.slot.append(channel[1])
	fcd_channels.fiber.append(channel[2])
	fcd_channels.fiber_channel.append(channel[3])

fcdAnalysis = cms.EDAnalyzer(
	"FCDAnalysis", 
	tagFCDDigis = cms.untracked.InputTag('hcalDigis', 'ZDC'),
	fcdChannels = fcd_channels,
)

