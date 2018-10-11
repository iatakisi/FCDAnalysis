#ifndef FCDAnalysis_h
#define FCDAnalysis_h

#include "FWCore/Framework/interface/global/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/Concurrency/interface/SerialTaskQueue.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "EventFilter/HcalRawToDigi/interface/HcalUnpacker.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"

#include "DQM/HcalCommon/interface/Utilities.h"
#include "DQM/HcalCommon/interface/ElectronicsMap.h"

#include <string>
#include <map>
#include <vector>
#include <TTree.h>
#include <TSpectrum.h>
#include <TF1.h>
#include <TMath.h>

typedef std::map<uint32_t, TH1F*> ChannelHistogramMap1D;
typedef std::map<uint32_t, TH2F*> ChannelHistogramMap2D;

// Stream data container holds objects duplicated for each stream.
// This means basically any histogram that gets filled for each event
class FCDAnalysisStreamData {
public:
	// Histograms and counters
	ChannelHistogramMap1D _hist_adc; 
	ChannelHistogramMap2D _hist_adc_vs_ts; 
	ChannelHistogramMap1D _hist_tdc; 
	ChannelHistogramMap1D _hist_tdctime; 
	ChannelHistogramMap2D _hist_adc_vs_tdc; 
	ChannelHistogramMap2D _hist_adc_vs_tdctime; 

	int _nevents;

	// Infrastructure
	std::vector<HcalElectronicsId> _fcd_eids;
	hcaldqm::electronicsmap::ElectronicsMap _ehashmap;

	// Accumulation method: for adding the streams back together at the end of the job
	inline void add(const FCDAnalysisStreamData* data) {
		for (auto& it : _hist_adc) {
			if (data->_hist_adc.find(it.first) != data->_hist_adc.end()) {
				it.second->Add(data->_hist_adc.at(it.first));
			}
		}
		for (auto& it : _hist_adc_vs_ts) {
			if (data->_hist_adc_vs_ts.find(it.first) != data->_hist_adc_vs_ts.end()) {
				it.second->Add(data->_hist_adc_vs_ts.at(it.first));
			}
		}
		for (auto& it : _hist_tdc) {
			if (data->_hist_tdc.find(it.first) != data->_hist_tdc.end()) {
				it.second->Add(data->_hist_tdc.at(it.first));
			}
		}
		for (auto& it : _hist_tdctime) {
			if (data->_hist_tdctime.find(it.first) != data->_hist_tdctime.end()) {
				it.second->Add(data->_hist_tdctime.at(it.first));
			}
		}
		for (auto& it : _hist_adc_vs_tdc) {
			if (data->_hist_adc_vs_tdc.find(it.first) != data->_hist_adc_vs_tdc.end()) {
				it.second->Add(data->_hist_adc_vs_tdc.at(it.first));
			}
		}
		for (auto& it : _hist_adc_vs_tdctime) {
			if (data->_hist_adc_vs_tdctime.find(it.first) != data->_hist_adc_vs_tdctime.end()) {
				it.second->Add(data->_hist_adc_vs_tdctime.at(it.first));
			}
		}

		_nevents += data->_nevents;
	}

	inline void reset() {
		this->_hist_adc.clear();
		this->_hist_adc_vs_ts.clear();
		this->_hist_tdc.clear();
		this->_hist_tdctime.clear();
		this->_hist_adc_vs_tdc.clear();
		this->_hist_adc_vs_tdctime.clear();
	}
};

class FCDAnalysis : public edm::global::EDAnalyzer<edm::StreamCache<FCDAnalysisStreamData>, edm::RunSummaryCache<FCDAnalysisStreamData>> {
	struct FCDChannel {
		int crate;
		int slot;
		int fiber;
		int fiberChannel;
	};

public:
	edm::InputTag	_tagQIE10;
	edm::EDGetTokenT<QIE10DigiCollection> _tokQIE10;
	std::vector<FCDChannel> _channels;
	edm::Service<TFileService> _fs;
	mutable edm::SerialTaskQueue _queue; //queue is used to serialize access to output file

public:
	explicit FCDAnalysis(const edm::ParameterSet& ps);// : pset(iConfig) {}
	~FCDAnalysis();

 private:
	
	void beginJob();
	void beginRun(edm::Run const&, edm::EventSetup const&) {}
	void initializeStreamData(FCDAnalysisStreamData*, edm::EventSetup const& es) const;
	std::shared_ptr<FCDAnalysisStreamData> globalBeginRunSummary(edm::Run const&, edm::EventSetup const&) const;
	std::unique_ptr<FCDAnalysisStreamData> beginStream(edm::StreamID) const;
	void streamBeginRun(edm::StreamID, edm::Run const&, edm::EventSetup const&) const;
	void analyze(edm::StreamID, const edm::Event&, const edm::EventSetup&) const;
	void streamEndRunSummary(edm::StreamID, edm::Run const&, edm::EventSetup const&, FCDAnalysisStreamData*) const;
	void globalEndRunSummary(edm::Run const&, edm::EventSetup const&, FCDAnalysisStreamData*) const;

	void endJob(){}
	void endRun(edm::Run const&, edm::EventSetup const&){}
	
};

#endif
