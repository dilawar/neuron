/* Test IAF neuron.  */

#include "systemc.h"

#include <random>
#include <chrono>
#include <random>
#include <memory>
#include <map>
#include <cmath>

#include <boost/assert.hpp>

#include "../neuron/IAF.h"
#include "../utility/plot_util.h"
#include "../utility/numeric.hpp"
#include "../external/prettyprint.hpp"
#include "../external/assert.hpp"

using namespace std;

SC_MODULE(TestIAF) 
{
    sc_in<bool> clock;

    // Inject current if any.
    sc_signal<double> inject;
    sc_signal<bool> spike1;
    sc_signal<bool> spike2;
    sc_signal<bool> spike3;

    // And record vm 
    sc_signal<double> vm;

    void gen_inh_stim() 
    {
        wait(1, SC_MS);
        spike3.write(false);
        while(true)
        {
            wait(1, SC_MS);
            spike3.write(true);
            wait(1, SC_MS);
            spike3.write(false);
        }
    }

    void gen_exc_stim() 
    {
        spike1.write(false);
        while(true)
        {
            wait(3, SC_MS);
            spike1.write(true);
            wait(1, SC_MS);
            spike1.write(false);
        }
    }

    void record()
    {
        double t = sc_time_stamp().to_seconds();

        // Store to plot later.
        data["time"].push_back(t);
        data["inject"].push_back(inject);
        data["vm"].push_back(vm);
    }

    SC_CTOR(TestIAF) 
    {
        vm.write(-65e-3);

        SC_THREAD(gen_exc_stim);
        SC_THREAD(gen_inh_stim);

        SC_METHOD(record);
        sensitive << clock.neg();

        dut = make_unique<IAF>("iaf1");
        dut->clock(clock);
        dut->vm(vm);
        dut->inject(inject);

        // Excitatory and inhibitory synapses. Add them to this dut.
        syn1 = make_shared<Synapse>("exc1", 10e-9, 05e-3, 00.0);
        syn2 = make_shared<Synapse>("exc2", 10e-9, 05e-3, 00.0 );
        syn3 = make_shared<Synapse>("inh1", 03e-9, 15e-3, -90e-3 );

        // Add spikes.
        syn1->spike(spike1);
        syn2->spike(spike2);
        syn3->spike(spike3);
        
        // Add to DUT.
        dut->addSynapse(syn1);
        dut->addSynapse(syn2);
        dut->addSynapse(syn3);

        gen_.seed(rd_());
        dist_.param(std::poisson_distribution<int>::param_type {50});
    }

    // Methods
    void save_data( )
    {
        map2csv(data, "test_iaf1.csv");
    }

    // Data members.
    std::random_device rd_;
    std::mt19937 gen_;
    std::poisson_distribution<> dist_;

    unique_ptr<IAF> dut;
    shared_ptr<Synapse> syn1;
    shared_ptr<Synapse> syn2;
    shared_ptr<Synapse> syn3;

    std::map<string, vector<double> > data;

};

int sc_main(int argc, char *argv[])
{
    // global clock is 0.1 ms.
    sc_clock clock("clock", 0.1, SC_MS);

    TestIAF tb("TestBench");
    tb.clock(clock);
    sc_start(200, SC_MS);
    tb.save_data();

    auto vm = min_max_mean_std(tb.data["vm"]);
    cout << vm << endl;


#if 0

    ASSERT_EQ(std::get<0>(resExc), std::get<0>(excExpected), "EXC");
    ASSERT_EQ(std::get<1>(resExc), std::get<1>(excExpected), "EXC");
    ASSERT_EQ(std::get<2>(resExc), std::get<2>(excExpected), "EXC");
    ASSERT_EQ(std::get<3>(resExc), std::get<3>(excExpected), "EXC");

    ASSERT_EQ(std::get<0>(resInh), std::get<0>(inhExpected), "Inh");
    ASSERT_EQ(std::get<1>(resInh), std::get<1>(inhExpected), "Inh");
    ASSERT_EQ(std::get<2>(resInh), std::get<2>(inhExpected), "Inh");
    ASSERT_EQ(std::get<3>(resInh), std::get<3>(inhExpected), "Inh");
#endif

    // plot_data( tb.dut->data(), "Exc+Inh" );
    return 0;
}
