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
to analays Express Physics PD stored in EOS(local):
```
cd $CMSSW_BASE/src/HCALPFG/FCDAnalysis/test
put locations of root files into fcdanalysis_cfg.py
cmsRun fcdanalysis_cfg.py 
```

to analays Zero Bias PD() stored in grid(global):
```
voms-proxy-init -voms cms
cd $CMSSW_BASE/src/HCALPFG/FCDAnalysis/test
put locations of root files into fcdanalysis_cfg.py
cmsRun fcdanalysis_cfg.py outpuFile=file:test.root processEvents=100 threads=4
```

- or you can use CRAB:
```
cd $CMSSW_BASE/src/HCALPFG/FCDAnalysis/test
voms-proxy-init -voms cms
crab submit -c crab_fcdanalysis_cfg.py
```
