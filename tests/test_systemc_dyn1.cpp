/* Test synapses.  */

#include "systemc.h"

#include <random>
#include <chrono>
#include <random>
#include <memory>
#include <map>
#include <cmath>

#include "../include/tantrika.h"
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
    // sc_signal<bool> spike;
    sc_signal<bool>* pSpike;

    sc_signal<double> post;

    // A voltage comes out of synapse.
    sc_signal<double> alphaInh;
    sc_signal<double> expExc;
    sc_signal<double> expInh;

    void gen_stim()
    {
        // spike.write(false);
        pSpike->write(false);
        // Post is always at Erest.
        post.write(-65e-3);

        while(true)
        {
            wait(10, SC_MS);
            // spike.write(true);
            pSpike->write(true);
            wait(0.1, SC_MS);
            // spike.write(false);
            pSpike->write(false);
        }
    }

    void tb_process()
    {
        // Store to plot later.
        data["time"].push_back(sc_time_stamp().to_seconds());
        // data["spike"].push_back(spike);
        data["spike"].push_back(pSpike->read());
        data["post"].push_back(post);
        data["expexc"].push_back(expExc);
        data["expinh"].push_back(expInh);
    }

    TestExpSyn(sc_module_name tb)
    {
        pSpike = new sc_signal<bool>();

        SC_THREAD(gen_stim);

        SC_METHOD(tb_process);
        sensitive << clock.neg();

        // Excitatory and inhibitory synapses.
        dutExc_ = make_unique<SynapseExp>("excExp", 1e-9, 1e-3, 0.0);
        dutExc_->clock.bind(clock);
        dutExc_->spike.bind(*pSpike);
        dutExc_->post(post);
        dutExc_->psc(expExc);

        dutInh_ = make_unique<SynapseExp>("inhExp", 1e-9, 10e-3, -90e-3);
        dutInh_->clock(clock);
        dutInh_->spike.bind(*pSpike);
        dutInh_->post(post);
        dutInh_->psc(expInh);
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
    std::map<string, vector<double> > data;

};

int sc_main(int argc, char *argv[])
{
    // global clock is 0.1 ms.
    sc_clock clock("clock", 0.1, SC_MS);

    auto tb = make_unique<TestExpSyn>("test_tb");
    tb->clock(clock);

    sc_start(100, SC_MS);

    tb->save_data();
    auto resExc     = min_max_mean_std(tb->data["expexc"]);
    auto resInh     = min_max_mean_std(tb->data["expinh"]);
    valarray<double> excExpected = {-5.88168e-11, 0, -5.55675e-12, 1.18621e-11};
    valarray<double> inhExpected = {0, 3.89261e-11, 2.07922e-11, 9.82262e-12};
    std::cout << "Testing for equality ... " << std::endl;
    std::cout << "Exp   Exc: Got " << resExc << " Expected:" << excExpected << std::endl;
    std::cout << "Exp   Inh: Got " << resInh << " Expected:" << inhExpected << std::endl;
    for (size_t i = 0; i < excExpected.size(); i++)
    {
        ASSERT_EQ(resExc[i], excExpected[i], "Exp Exc");
        ASSERT_EQ(resInh[i], inhExpected[i], "Exp Inh");
    }

    std::cout << "\t\t.... PASSED." << std::endl;
    return 0;
}
