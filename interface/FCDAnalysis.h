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
	ChannelHistogramMap2D _hist_adc_subped_vs_ts;
	ChannelHistogramMap2D _hist_linearadc_vs_ts;
	ChannelHistogramMap1D _hist_tdc; 
	ChannelHistogramMap1D _hist_tdctime; 
	ChannelHistogramMap2D _hist_adc_vs_tdc; 
	ChannelHistogramMap2D _hist_adc_vs_tdctime; 
	//new Histograms
	ChannelHistogramMap1D _cMean_Signal_Time_Estimation_TS;
	ChannelHistogramMap1D _cMean_Signal_Time_Estimation_TDC;
	ChannelHistogramMap1D _cSignal_minus_Ped;
	ChannelHistogramMap1D _cLinearADC;
	//new Histograms---

	int _nevents;

	// Infrastructure
	std::vector<HcalElectronicsId> _fcd_eids;
	hcaldqm::electronicsmap::ElectronicsMap _ehashmap;
        
        //QIE adc count to electrical charge map and other counters for new Histograms
        Double_t buf[20];
        Double_t bufTDC[20];
        Double_t signalADC;
        Double_t pedADC;
        Double_t meanTime;

        double adc2fC_QIE10[256]={
                // - - - - - - - range 0 - - - - - - - -
                //subrange0 
                1.58, 4.73, 7.88, 11.0, 14.2, 17.3, 20.5, 23.6, 
                26.8, 29.9, 33.1, 36.2, 39.4, 42.5, 45.7, 48.8,
                //subrange1
                53.6, 60.1, 66.6, 73.0, 79.5, 86.0, 92.5, 98.9,
                105, 112, 118, 125, 131, 138, 144, 151,
                //subrange2
                157, 164, 170, 177, 186, 199, 212, 225,
                238, 251, 264, 277, 289, 302, 315, 328,
                //subrange3
                341, 354, 367, 380, 393, 406, 418, 431,
                444, 464, 490, 516, 542, 568, 594, 620,
                // - - - - - - - range 1 - - - - - - - -
                //subrange0
                569, 594, 619, 645, 670, 695, 720, 745,
                771, 796, 821, 846, 871, 897, 922, 947,
                //subrange1
                960, 1010, 1060, 1120, 1170, 1220, 1270, 1320,
                1370, 1430, 1480, 1530, 1580, 1630, 1690, 1740,
                //subrange2
                1790, 1840, 1890, 1940,  2020, 2120, 2230, 2330,
                2430, 2540, 2640, 2740, 2850, 2950, 3050, 3150,
                //subrange3
                3260, 3360, 3460, 3570, 3670, 3770, 3880, 3980,
                4080, 4240, 4450, 4650, 4860, 5070, 5280, 5490,
                // - - - - - - - range 2 - - - - - - - - 
                //subrange0
                5080, 5280, 5480, 5680, 5880, 6080, 6280, 6480,
                6680, 6890, 7090, 7290, 7490, 7690, 7890, 8090,
                //subrange1
                8400, 8810, 9220, 9630, 10000, 10400, 10900, 11300,
                11700, 12100, 12500, 12900, 13300, 13700, 14100, 14500,
                //subrange2
                15000, 15400, 15800, 16200, 16800, 17600, 18400, 19300,
                20100, 20900, 21700, 22500, 23400, 24200, 25000, 25800,
                //subrange3
                26600, 27500, 28300, 29100, 29900, 30700, 31600, 32400,
                33200, 34400, 36100, 37700, 39400, 41000, 42700, 44300,
                // - - - - - - - range 3 - - - - - - - - -
                //subrange0
                41100, 42700, 44300, 45900, 47600, 49200, 50800, 52500,
                54100, 55700, 57400, 59000, 60600, 62200, 63900, 65500,
                //subrange1
                68000, 71300, 74700, 78000, 81400, 84700, 88000, 91400,
                94700, 98100, 101000, 105000, 108000, 111000, 115000, 118000,
                //subrange2
                121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
                168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
                //subrange3
                230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
                291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000};
        //QIE adc count to electrical charge map and other counters for new Histograms---

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
		for (auto& it : _hist_adc_subped_vs_ts) {
		  if (data->_hist_adc_subped_vs_ts.find(it.first) != data->_hist_adc_subped_vs_ts.end()) {
		    it.second->Add(data->_hist_adc_subped_vs_ts.at(it.first));
		  }
		}
		for (auto& it : _hist_linearadc_vs_ts) {
		  if (data->_hist_linearadc_vs_ts.find(it.first) != data->_hist_linearadc_vs_ts.end()) {
		    it.second->Add(data->_hist_linearadc_vs_ts.at(it.first));
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
		//Accumulation method for new Histograms
		for (auto& it : _cMean_Signal_Time_Estimation_TS) {
                        if (data->_cMean_Signal_Time_Estimation_TS.find(it.first) != data->_cMean_Signal_Time_Estimation_TS.end()) {
                                it.second->Add(data->_cMean_Signal_Time_Estimation_TS.at(it.first));
                        }
                }
                for (auto& it : _cMean_Signal_Time_Estimation_TDC) {
                        if (data->_cMean_Signal_Time_Estimation_TDC.find(it.first) != data->_cMean_Signal_Time_Estimation_TDC.end()) {
                                it.second->Add(data->_cMean_Signal_Time_Estimation_TDC.at(it.first));
                        }
                }
                for (auto& it : _cSignal_minus_Ped) {
                        if (data->_cSignal_minus_Ped.find(it.first) != data->_cSignal_minus_Ped.end()) {
                                it.second->Add(data->_cSignal_minus_Ped.at(it.first));
                        }
                }
                for (auto& it : _cLinearADC) {
                        if (data->_cLinearADC.find(it.first) != data->_cLinearADC.end()) {
                                it.second->Add(data->_cLinearADC.at(it.first));
                        }
                }
		//Accumulation method for new Histograms---
		
		_nevents += data->_nevents;
	}

	inline void reset() {
		this->_hist_adc.clear();
		this->_hist_adc_vs_ts.clear();
		this->_hist_tdc.clear();
		this->_hist_adc_subped_vs_ts.clear();
		this->_hist_linearadc_vs_ts.clear();
		this->_hist_tdctime.clear();
		this->_hist_adc_vs_tdc.clear();
		this->_hist_adc_vs_tdctime.clear();
		//clear new Histograms
		this->_cMean_Signal_Time_Estimation_TS.clear();
		this->_cMean_Signal_Time_Estimation_TDC.clear();
		this->_cSignal_minus_Ped.clear();
		this->_cLinearADC.clear();
                //clear new Histograms---
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

