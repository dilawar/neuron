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

    // And record vm 
    sc_signal<double> vm;

    void gen_stim() 
    {
        inject.write(0.0);
        while(true)
        {
            wait(10, SC_MS);
            inject.write(10e-6);
            wait(10, SC_MS);
            inject.write(0.0);
        }
    }

    void record()
    {
        double t = sc_time_stamp().to_seconds();
        // cout << boost::format("t=%1% inject=%2% vm=%3%\n") % t % inject % vm;

        // Store to plot later.
        data["time"].push_back(t);
        data["inject"].push_back(inject);
        data["vm"].push_back(vm);
    }

    SC_CTOR(TestIAF) 
    {
        SC_THREAD(gen_stim);

        SC_METHOD(record);
        sensitive << clock.neg();

        // Excitatory and inhibitory synapses.
        dut = make_unique<IAF>("iaf1");
        dut->clock(clock);
        dut->vm(vm);
        dut->inject(inject);

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

    std::map<string, vector<double> > data;

};

int sc_main(int argc, char *argv[])
{
    // global clock is 0.1 ms.
    sc_clock clock("clock", 0.1, SC_MS);

    TestIAF tb("TestBench");
    tb.clock(clock);
    sc_start(100, SC_MS);
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

    return 0;
}
