#ifndef __HEPTOPTAGGER_HH__
#define __HEPTOPTAGGER_HH__

#include <vector>
#include <algorithm>
#include <math.h>
#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/tools/Pruner.hh>
#include <fastjet/tools/Filter.hh>
#include <fastjet/contrib/Nsubjettiness.hh>

FASTJET_BEGIN_NAMESPACE

namespace HEPTopTagger{

    using namespace std;
    using namespace fastjet;

    enum Mode {EARLY_MASSRATIO_SORT_MASS,
        LATE_MASSRATIO_SORT_MASS,
        EARLY_MASSRATIO_SORT_MODDJADE,
        LATE_MASSRATIO_SORT_MODDJADE,
        TWO_STEP_FILTER};

    class HEPTopTagger_fixed_R {
    public:
        HEPTopTagger_fixed_R();

        HEPTopTagger_fixed_R(fastjet::PseudoJet jet);

        HEPTopTagger_fixed_R(fastjet::PseudoJet jet,
                             double mtmass, double mwmass);

        //run tagger
        void run();

        //settings

        void set_mass_drop_threshold(double x) {_mass_drop_threshold = x;}
        void set_max_subjet_mass(double x) {_max_subjet_mass = x;}

        void set_filtering_n(unsigned nfilt) {_nfilt = nfilt;}
        void set_filtering_R(double Rfilt) {_Rfilt = Rfilt;}
        void set_filtering_minpt_subjet(double x) {_minpt_subjet = x;}
        void set_filtering_jetalgorithm(JetAlgorithm jet_algorithm) {_jet_algorithm_filter = jet_algorithm;}

        void set_reclustering_jetalgorithm(JetAlgorithm jet_algorithm) {_jet_algorithm_recluster = jet_algorithm;}

        void set_mode(enum Mode mode) {_mode = mode;}
        void set_mt(double x) {_mtmass = x;}
        void set_mw(double x) {_mwmass = x;}
        void set_top_mass_range(double xmin, double xmax) {_mtmin = xmin; _mtmax = xmax;}
        void set_fw(double fw) {_rmin = (1.-fw)*_mwmass/_mtmass; _rmax=(1.+fw)*_mwmass/_mtmass;}
        void set_mass_ratio_range(double rmin, double rmax) {_rmin = rmin; _rmax = rmax;}
        void set_mass_ratio_cut(double m23cut, double m13cutmin,double m13cutmax) {_m23cut = m23cut; _m13cutmin = m13cutmin; _m13cutmax = m13cutmax;}
        void set_top_minpt(double x) {_minpt_tag = x;}

        void set_pruning_zcut(double zcut) {_zcut = zcut;}
        void set_pruning_rcut_factor(double rcut_factor) {_rcut_factor = rcut_factor;}

        void set_debug(bool debug) {_debug = debug;}


        //get information
        bool is_maybe_top() const {return _is_maybe_top;}
        bool is_masscut_passed() const {return _is_masscut_passed;}
        bool is_minptcut_passed() const {return _is_ptmincut_passed;}
        bool is_tagged() const {return (_is_masscut_passed && _is_ptmincut_passed);}

        double delta_top() const {return _delta_top;}
        double djsum() const {return _djsum;}
        double pruned_mass() const {return _pruned_mass;}
        double unfiltered_mass() const {return _unfiltered_mass;}

        double f_rec();
        const PseudoJet & t() const {return _top_candidate;}
        const PseudoJet & b() const {return _top_subjets[0];}
        const PseudoJet & W() const {return _W;}
        const PseudoJet & W1() const {return _top_subjets[1];}
        const PseudoJet & W2() const {return _top_subjets[2];}
        const std::vector<PseudoJet> & top_subjets() const {return _top_subjets;}
        const PseudoJet & j1() const {return _top_subs[0];}
        const PseudoJet & j2() const {return _top_subs[1];}
        const PseudoJet & j3() const {return _top_subs[2];}
        const std::vector<PseudoJet> & top_hadrons() const {return _top_hadrons;}
        const std::vector<PseudoJet> & hardparts() const {return _top_parts;}
        const PseudoJet & fat_initial() {return _fat;}

        void get_setting() const;
        void get_info() const;

        double nsub(fastjet::PseudoJet jet, int order, fastjet::contrib::Njettiness::AxesMode axes = fastjet::contrib::Njettiness::kt_axes,
                    double beta = 1., double R0 = 1.);

    private:

        PseudoJet _jet;
        PseudoJet _initial_jet;

        double _mass_drop_threshold;
        double _max_subjet_mass;

        Mode _mode;
        double _mtmass, _mwmass;
        double _mtmin, _mtmax;
        double _rmin, _rmax;
        double _m23cut, _m13cutmin, _m13cutmax;
        double _minpt_tag;

        unsigned _nfilt;
        double _Rfilt;
        JetAlgorithm _jet_algorithm_filter;
        double _minpt_subjet;

        JetAlgorithm _jet_algorithm_recluster;

        double _zcut;
        double _rcut_factor;

        PseudoJet _fat;
        bool _debug;

        bool _is_masscut_passed;
        bool _is_ptmincut_passed;
        bool _is_maybe_top;

        double _delta_top;
        double _djsum;

        double _pruned_mass;
        double _unfiltered_mass;

        double _fw;
        unsigned _parts_size;

        PseudoJet _top_candidate;
        PseudoJet _W;
        std::vector<PseudoJet> _top_subs;
        std::vector<PseudoJet> _top_subjets;
        std::vector<PseudoJet> _top_hadrons;
        std::vector<PseudoJet> _top_parts;

        static bool _first_time;

        //internal functions
        void FindHardSubst(const PseudoJet& jet, std::vector<fastjet::PseudoJet>& t_parts);
        std::vector<PseudoJet> Filtering(const std::vector <PseudoJet> & top_constits, const JetDefinition & filtering_def);
        void store_topsubjets(const std::vector<PseudoJet>& top_subs);
        bool check_mass_criteria(const std::vector<fastjet::PseudoJet> & top_subs) const;
        double perp(const PseudoJet & vec, const fastjet::PseudoJet & ref);
        double djademod (const fastjet::PseudoJet & subjet_i, const fastjet::PseudoJet & subjet_j, const fastjet::PseudoJet & ref);

        void print_banner();

    };

    class HEPTopTagger {

    public:
        HEPTopTagger();

        HEPTopTagger(double mtmass, double mwmass);

        ~HEPTopTagger();

        //run tagger
        void run(const fastjet::PseudoJet & jet);

        //get information
        bool is_maybe_top() const {return _HEPTopTagger_opt.is_maybe_top();}
        bool is_masscut_passed() const {return _HEPTopTagger_opt.is_masscut_passed();}
        bool is_minptcut_passed() const {return _HEPTopTagger_opt.is_minptcut_passed();}
        bool is_tagged() const {return _HEPTopTagger_opt.is_tagged();}

        double delta_top() const {return _HEPTopTagger_opt.delta_top();}
        double djsum() const {return _HEPTopTagger_opt.djsum();}
        double pruned_mass() const {return _HEPTopTagger_opt.pruned_mass();}
        double unfiltered_mass() const {return _HEPTopTagger_opt.unfiltered_mass();}

        double f_rec() {return _HEPTopTagger_opt.f_rec();}
        const PseudoJet & t() const {return _HEPTopTagger_opt.t();}
        const PseudoJet & b() const {return _HEPTopTagger_opt.b();}
        const PseudoJet & W() const {return _HEPTopTagger_opt.W();}
        const PseudoJet & W1() const {return _HEPTopTagger_opt.W1();}
        const PseudoJet & W2() const {return _HEPTopTagger_opt.W2();}
        const std::vector<PseudoJet> & top_subjets() const {return _HEPTopTagger_opt.top_subjets();}
        const PseudoJet & j1() const {return _HEPTopTagger_opt.j1();}
        const PseudoJet & j2() const {return _HEPTopTagger_opt.j2();}
        const PseudoJet & j3() const {return _HEPTopTagger_opt.j3();}
        const std::vector<PseudoJet> & top_hadrons() const {return _HEPTopTagger_opt.top_hadrons();}
        const std::vector<PseudoJet> & hardparts() const {return _HEPTopTagger_opt.hardparts();}
        const PseudoJet & fat_initial() {return _fat;}
        const PseudoJet & fat_Ropt() {return _HEPTopTagger_opt.fat_initial();}
        HEPTopTagger_fixed_R HEPTopTaggeragger(int i)  {return _HEPTopTagger[i];}

        double Ropt() const {return _Ropt/10.;}
        double Ropt_calc() const {return _R_opt_calc;}

        int optimalR_type();
        double nsub_unfiltered(int order, fastjet::contrib::Njettiness::AxesMode axes = fastjet::contrib::Njettiness::kt_axes,
                               double beta = 1., double R0 = 1.);
        double nsub_filtered(int order, fastjet::contrib::Njettiness::AxesMode axes = fastjet::contrib::Njettiness::kt_axes,
                             double beta = 1., double R0 = 1.);

        void get_setting() const; //{return _HEPTopTagger_opt.get_setting();};
        void get_info() const {return _HEPTopTagger_opt.get_info();};


        //settings
        void do_optimalR(bool optimalR) {_do_optimalR = optimalR;}

        void set_mass_drop_threshold(double x) {_mass_drop_threshold = x;}
        void set_max_subjet_mass(double x) {_max_subjet_mass = x;}

        void set_filtering_n(unsigned nfilt) {_nfilt = nfilt;}
        void set_filtering_R(double Rfilt) {_Rfilt = Rfilt;}
        void set_filtering_minpt_subjet(double x) {_minpt_subjet = x;}
        void set_filtering_jetalgorithm(JetAlgorithm jet_algorithm) {_jet_algorithm_filter = jet_algorithm;}

        void set_reclustering_jetalgorithm(JetAlgorithm jet_algorithm) {_jet_algorithm_recluster = jet_algorithm;}

        void set_mode(enum Mode mode) {_mode = mode;}
        void set_mt(double x) {_mtmass = x;}
        void set_mw(double x) {_mwmass = x;}
        void set_top_mass_range(double xmin, double xmax) {_mtmin = xmin; _mtmax = xmax;}
        void set_fw(double fw) {_rmin = (1.-fw)*_mwmass/_mtmass; _rmax=(1.+fw)*_mwmass/_mtmass;}
        void set_mass_ratio_range(double rmin, double rmax) {_rmin = rmin; _rmax = rmax;}
        void set_mass_ratio_cut(double m23cut, double m13cutmin,double m13cutmax) {_m23cut = m23cut;
            _m13cutmin = m13cutmin;
            _m13cutmax = m13cutmax;}
        void set_top_minpt(double x) {_minpt_tag = x;}

        void set_optimalR_max(double x) {_max_fatjet_R = x;}
        void set_optimalR_min(double x) {_min_fatjet_R = x;}
        void set_optimalR_step(double x) {_step_R = x;}
        void set_optimalR_threshold(double x) {_optimalR_threshold = x;}

        void set_filtering_optimalR_calc_R(double x) {_R_filt_optimalR_calc = x;}
        void set_filtering_optimalR_calc_n(unsigned x) {_N_filt_optimalR_calc = x;}
        void set_optimalR_calc_fun(double (*f)(double)) {_r_min_exp_function = f;}

        void set_optimalR_type_top_mass_range(double x, double y) {_optimalR_mmin = x; _optimalR_mmax = y;}
        void set_optimalR_type_fw(double x) {_optimalR_fw = x;}
        void set_optimalR_type_max_diff(double x) {_R_opt_diff = x;}

        void set_filtering_optimalR_pass_R(double x) {_R_filt_optimalR_pass = x;}
        void set_filtering_optimalR_pass_n(unsigned x) {_N_filt_optimalR_pass = x;}
        void set_filtering_optimalR_fail_R(double x) {_R_filt_optimalR_fail = x;}
        void set_filtering_optimalR_fail_n(unsigned x) {_N_filt_optimalR_fail = x;}

        void set_pruning_zcut(double zcut) {_zcut = zcut;}
        void set_pruning_rcut_factor(double rcut_factor) {_rcut_factor = rcut_factor;}

        void set_debug(bool debug) {_debug = debug;}



    private:
        bool _do_optimalR;

        PseudoJet _jet;
        PseudoJet _initial_jet;

        double _mass_drop_threshold;
        double _max_subjet_mass;

        Mode _mode;
        double _mtmass, _mwmass;
        double _mtmin, _mtmax;
        double _rmin, _rmax;
        double _m23cut, _m13cutmin, _m13cutmax;
        double _minpt_tag;

        unsigned _nfilt;
        double _Rfilt;
        fastjet::JetAlgorithm _jet_algorithm_filter;
        double _minpt_subjet;

        fastjet::JetAlgorithm _jet_algorithm_recluster;

        double _zcut;
        double _rcut_factor;

        double _max_fatjet_R, _min_fatjet_R, _step_R, _optimalR_threshold;

        double  _R_filt_optimalR_calc, _N_filt_optimalR_calc;
        double (*_r_min_exp_function)(double);

        double _optimalR_mmin, _optimalR_mmax, _optimalR_fw, _R_opt_calc, _R_opt_diff;
        double _R_filt_optimalR_pass, _N_filt_optimalR_pass, _R_filt_optimalR_fail, _N_filt_optimalR_fail;


        PseudoJet _fat, _filt_fat;
        map<int,int> _n_small_fatjets;
        map<int,HEPTopTagger_fixed_R> _HEPTopTagger;
        HEPTopTagger_fixed_R _HEPTopTagger_opt;

        int _Ropt;

        bool _debug;

        void UnclusterFatjets(const vector<fastjet::PseudoJet> & big_fatjets,
                              vector<fastjet::PseudoJet> & small_fatjets,
                              const ClusterSequence & cs, const double small_radius);

    };
}
//--------------------------------------------------------------------

FASTJET_END_NAMESPACE
#include "HEPTopTagger.cc"

#endif // __HEPTOPTAGGER_HH__

