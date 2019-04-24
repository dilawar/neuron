/* Test synapses.  */

#include "systemc.h"

#include <random>
#include <chrono>
#include <random>
#include <memory>
#include <map>
#include <cmath>

#include "../synapse/Synapse.h"
#include "../utility/plot_util.h"

using namespace std;

SC_MODULE(TestExpSyn) 
{
    sc_in<bool> clock;

    // Spike goes into synapse.
    sc_signal<bool> spike;
    sc_signal<double> post;

    // A voltage comes out of synapse.
    sc_signal<double> injectExc;
    sc_signal<double> odeExc;
    sc_signal<double> injectInh;
    sc_signal<double> odeInh;

    void gen_stim() 
    {
        spike.write(false);
        //post.write(-65e-3);
        // Generate a 100Hz stimulation i.e. 10ms is the gap.
        while(true)
        {
            wait(10, SC_MS);
            spike = true;
            wait(0.1, SC_MS);
            spike = false;
            // Post is always at Erest.
            post = -65e-3;
        }
    }

    void process()
    {
        // Store to plot later.
        data["time"].push_back(sc_time_stamp().to_seconds());
        data["spike"].push_back(spike);
        data["post"].push_back(post);
        data["exc"].push_back(injectExc);
        data["inh"].push_back(injectInh);
        data["odeinh"].push_back(odeInh);
        data["odeexc"].push_back(odeExc);
    }

    SC_CTOR(TestExpSyn) 
    {
        SC_THREAD(gen_stim);

        SC_METHOD(process);
        sensitive << clock.neg();

        // Excitatory and inhibitory synapses.
        dutExc_ = make_unique<Synapse>("exc", 1e-9, 1e-3, 0.0);
        dutExc_->clock(clock);
        dutExc_->pre(spike);
        dutExc_->post(post);
        dutExc_->inject(injectExc);

        odeExc_ = make_unique<Synapse>("odeexc", 1e-9, 1e-3, 1e-3, 0.0);
        odeExc_->clock(clock);
        odeExc_->pre(spike);
        odeExc_->post(post);
        odeExc_->inject(odeExc);

        dutInh_ = make_unique<Synapse>("inh", 1e-9, 5e-3, -90e-3);
        dutInh_->clock(clock);
        dutInh_->pre(spike);
        dutInh_->post(post);
        dutInh_->inject(injectInh);

        odeInh_ = make_unique<Synapse>("odeinh", 1e-9, 5e-3, 5e-3, -90e-3);
        odeInh_->clock(clock);
        odeInh_->pre(spike);
        odeInh_->post(post);
        odeInh_->inject(odeInh);

        gen_.seed(rd_());
        dist_.param(std::poisson_distribution<int>::param_type {50});
    }

    // Methods
    void plot_data( )
    {
        map2csv(data, "exp_syn1.csv");
    }

    // Data members.
    std::random_device rd_;
    std::mt19937 gen_;
    std::poisson_distribution<> dist_;

    unique_ptr<Synapse> dutExc_;
    unique_ptr<Synapse> odeExc_;
    unique_ptr<Synapse> dutInh_;
    unique_ptr<Synapse> odeInh_;

    std::map<string, vector<double> > data;

};

int sc_main(int argc, char *argv[])
{
    // global clock is 0.1 ms.
    sc_clock clock("clock", 0.1, SC_MS);

    TestExpSyn tb("TestBench");
    tb.clock(clock);

    sc_start(40, SC_MS);

    tb.plot_data();

    return 0;
}
