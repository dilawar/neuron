/* Test synapses.  */

#include "systemc.h"

#include <random>
#include <chrono>
#include <random>
#include <memory>
#include <map>
#include <cmath>

#include "../include/Synapse.h"
#include "../utility/data_util.h"
#include "../utility/numeric.hpp"
#include "../external/prettyprint.hpp"
#include "../external/assert.hpp"

using namespace std;

struct TestExpSyn: public sc_module 
{
    SC_HAS_PROCESS(TestExpSyn);

    sc_in<bool> clock;

    // Spike goes into synapse.
    sc_signal<bool> spike;
    sc_signal<double> post;

    // A voltage comes out of synapse.
    sc_signal<double> alphaExc;
    sc_signal<double> alphaInh;
    sc_signal<double> expExc;
    sc_signal<double> expInh;

    void gen_stim() 
    {
        spike.write(false);
        // Post is always at Erest.
        post.write(-65e-3);

        while(true)
        {
            wait(10, SC_MS);
            spike.write(true);
            wait(0.1, SC_MS);
            spike.write(false);
        }
    }

    void tb_process()
    {
        // Store to plot later.
        data["time"].push_back(sc_time_stamp().to_seconds());
        data["spike"].push_back(spike);
        data["post"].push_back(post);
        data["expexc"].push_back(expExc);
        data["expinh"].push_back(expInh);
        data["alphainh"].push_back(alphaInh);
        data["alphaexc"].push_back(alphaExc);
    }

    TestExpSyn(sc_module_name tb)
    {
        SC_THREAD(gen_stim);

        SC_METHOD(tb_process);
        sensitive << clock.neg();

        // Excitatory and inhibitory synapses.
        dutExc_ = make_unique<SynapseExp>("excExp", 1e-9, 1e-3, 0.0);
        dutExc_->clock(clock);
        dutExc_->spike(spike);
        dutExc_->post(post);
        dutExc_->inject(expExc);

        dutAlphaExc_ = make_unique<SynapseAlpha>("excAlpha", 1e-9, 1e-3, 0.0);
        dutAlphaExc_->clock(clock);
        dutAlphaExc_->spike(spike);
        dutAlphaExc_->post(post);
        dutAlphaExc_->inject(alphaExc);

        dutInh_ = make_unique<SynapseExp>("inhExp", 1e-9, 10e-3, -90e-3);
        dutInh_->clock(clock);
        dutInh_->spike(spike);
        dutInh_->post(post);
        dutInh_->inject(expInh);

        dutAlphaInh_ = make_unique<SynapseAlpha>("inhAlpha", 1e-9, 10e-3, -90e-3);
        dutAlphaInh_->clock(clock);
        dutAlphaInh_->spike(spike);
        dutAlphaInh_->post(post);
        dutAlphaInh_->inject(alphaInh);

        gen_.seed(rd_());
        dist_.param(std::poisson_distribution<int>::param_type {50});
    }

    // Methods
    void save_data( )
    {
        map2csv(data, "exp_syn1.csv");
    }
    
    // Data members.
    std::random_device rd_;
    std::mt19937 gen_;
    std::poisson_distribution<> dist_;


    unique_ptr<SynapseExp> dutExc_;
    unique_ptr<SynapseExp> dutInh_;
    unique_ptr<SynapseAlpha> dutAlphaExc_;
    unique_ptr<SynapseAlpha> dutAlphaInh_;

    std::map<string, vector<double> > data;

};

int sc_main(int argc, char *argv[])
{
    // global clock is 0.1 ms.
    sc_clock clock("clock", 0.1, SC_MS);

    TestExpSyn tb("test_tb");
    tb.clock(clock);

    sc_start(100, SC_MS);

    tb.save_data();

    auto resExc     = min_max_mean_std(tb.data["expexc"]);
    auto resInh     = min_max_mean_std(tb.data["expinh"]);
    auto resAutoExc = min_max_mean_std(tb.data["alphaexc"]);
    auto resAutoInh = min_max_mean_std(tb.data["alphainh"]);

    valarray<double> excExpected = {-5.88168e-11, 0, -5.55675e-12, 1.18621e-11};
    valarray<double> inhExpected = {0, 3.89261e-11, 2.07922e-11, 9.82262e-12};

    valarray<double> excAlphaExpected =  {-2.39231e-11, 0, -5.83859e-12, 7.80734e-12};
    valarray<double> inhAlphaExpected = {0, 2.57706e-11, 1.85562e-11, 8.45435e-12};

    std::cout << "Testing for equality ... " << std::endl;
    std::cout << "Exp   Exc: Got " << resExc << " Expected:" << excExpected << std::endl;
    std::cout << "Exp   Inh: Got " << resInh << " Expected:" << inhExpected << std::endl;
    std::cout << "Alpha Inh: Got " << resAutoExc << " Expected:" << excAlphaExpected << std::endl;
    std::cout << "Alpha Inh: Got " << resAutoInh << " Expected:" << inhAlphaExpected << std::endl;

    for (size_t i = 0; i < excExpected.size(); i++) 
    {
        ASSERT_EQ(resExc[i], excExpected[i], "Exp Exc");
        ASSERT_EQ(resInh[i], inhExpected[i], "Exp Inh");
        ASSERT_EQ(resAutoExc[i], excAlphaExpected[i], "Exp Exc");
        ASSERT_EQ(resAutoInh[i], inhAlphaExpected[i], "Alpha Inh");
    }

    std::cout << "\t\t.... PASSED." << std::endl;
    return 0;
}
