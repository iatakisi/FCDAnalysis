# FCDAnalysis
Analysis skeleton for FCD

Setup directions:
```
cmsrel CMSSW_10_3_0_pre6
cd CMSSW_10_3_0_pre6/src
cmsenv
git cms-init
git clone ssh://git@github.com/iatakisi/FCDAnalysis HCALPFG/FCDAnalysis
scram b -j8
```

Run directions:
- Local:
```
cd $CMSSW_BASE/src/HCALPFG/FCDAnalysis/test
cmsRun fcdanalysis_cfg.py outpuFile=file:test.root processEvents=100 threads=4
```

- CRAB:
```
cd $CMSSW_BASE/src/HCALPFG/FCDAnalysis/test
voms-proxy-init -voms cms
crab submit -c crab_fcdanalysis_cfg.py
```
