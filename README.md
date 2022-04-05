# HEPTopTagger
This repository includes modified version of HEPTopTagger v2 interface tuned to be included in MadAnalysis workflow.
For the original software please see the links below
 * [HEPTopTagger v1](https://www.ippp.dur.ac.uk/~mspannow/HEPTopTagger.html)
 * [HEPTopTagger v2](https://www.thphys.uni-heidelberg.de/~plehn/index.php?show=heptoptagger&visible=tools)

This version does not include `Qjets` plugin that has been offered in original `HTTv2`. If you use this repository 
please cite the following papers:

**MadAnalysis 5 integration:**
* TBA

**Original papers of the tool:**
* [JHEP 1506 (2015) 203 [arXiv:1503.05921 [hep-ph]]](https://arxiv.org/abs/1503.05921)
* [JHEP 1010 (2010) 078 [arXiv:1006.2833 [hep-ph]]](https://arxiv.org/abs/1006.2833)

# Usage

This repository is designed to be downloaded through MadAnalysis 5 interface via the following command
```bash
install HEPTopTagger
```
Once installed `SampleAnalyzer` backend will automatically add `HTT` libraries to the analysis workspace. 
HTT can be initialized and executed via following commands;

```c++
Substructure::HTT::InputParameters parameters;
parameters.mode = Substructure::HTT::EARLY_MASSRATIO_SORT_MASS; // execution mode

parameters.do_optimalR = true; // initialize optimal R or set to fixed R
// optimal R parameters
parameters.optimalR_min = 0.5; // min jet size
parameters.optimalR_step = 0.1; // step size
parameters.optimalR_threshold = 0.2; // step size

// massdrop - unclustering
parameters.mass_drop = 0.8;
parameters.max_subjet = 30.; // set_max_subjet_mass

// filtering
parameters.filt_N = 5; // set_nfilt
parameters.filtering_R = 0.3; // max subjet distance for filtering
parameters.filtering_minpt = 0.; // min subjet pt for filtering
// jet algorithm for filtering
parameters.filtering_algorithm = Substructure::Algorithm::cambridge;

// Reclustering
// reclustering jet algorithm
parameters.reclustering_algorithm = Substructure::Algorithm::cambridge;

//top mass range
parameters.top_mass = 172.3;
parameters.W_mass = 80.4;
parameters.Mtop_min = 150.;
parameters.Mtop_max = 200.; //set_top_range(min,max)

// set top mass ratio range
parameters.fw = 0.15;
parameters.mass_ratio_range_min = (1-parameters.fw)*80.379/172.9;
parameters.mass_ratio_range_max = (1+parameters.fw)*80.379/172.9;

//mass ratio cuts
parameters.m23cut = 0.35;
parameters.m13cutmin = 0.2;
parameters.m13cutmax = 1.3;

// pruning
parameters.prun_zcut = 0.1; // set_prun_zcut
parameters.prun_rcut = .5; // set_prun_rcut

// Initialize HTT 
// Note initialization during execution may slowdown the execution time. For the best performance 
// Initialize HTT during initialization of the analysis.
Substructure::HTT tagger;
tagger.Initialize(parameters);

// Execute HTT
RecJets Jets = filter(event.rec()->jets(), 200., 2.5);
tagger.Execute(Jets[0]);

const RecJet Top;
if (tagger.is_tagged()) Top = tagger.top();
```