/***
 *    Description:  Model of syanpse.
 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 */

#ifndef SIMPLE_SYNAPSE_H
#define SIMPLE_SYNAPSE_H

#include "systemc.h"

#include <cmath>
#include <algorithm>

#define MIN_SYNAPTIC_WEIGHT 0.1
#define MAX_SYNAPTIC_WEIGHT 1.0

#define MIN_SYNAPTIC_EFFICACY 1.0
#define MAX_SYNAPTIC_EFFICACY 100.0

#include <algorithm>

using namespace std;

SC_MODULE (SimpleSynapse) 
{
    /* Declare ports and variable */
    sc_in_clk clock;                            /* Everyone is listening */

    sc_in<bool> reset;                          /* Reset all */
    sc_in<bool> enable;                         /* I am in network */

    /* pre- and post-synaptic firing activities */
    sc_in<bool> pre_synaptic_event;
    sc_in<bool> post_synaptic_event;

    
    double a_, b_;

    double delta_ltd_, delta_ltp_;

    /**
     * @brief synaptic weight_. TODO: Upper and lower limit on weight_.
     */
    double weight_; //  
    double min_weight_, max_weight_;

    vector<double> timeseries_weight_;

    /**
     * @brief Synaptic efficacy of this synapse. Minimum is 0.0 and maximum is
     * 100.0
     */
    double synaptic_efficacy_;  // Synaptic efficacy between 0 and 100.0. 


    double tau_ltp_, tau_ltd_, decay_tau_;

    double inFiringRate, outFiringRate;
    double timeWindow;  /* To compute rates, use this time windows. */ 

    vector<double> preSynEvents, postSynEvents;

    void monitor()
    {
        while(true)
        {
            double currTime = sc_time_stamp().to_seconds();
            cerr << "@" << currTime << ": weight=" << weight_ << endl;
            wait(100);
        }
    }

    /** Compute the rate in a given time window 
     */
    double firingRate(const vector<double>& vec, const double windowTime)
    {
        assert(vec.size() > 0);
        // Last element is the current time.
        double currentTime = vec.back();
        double totalSpikes = 0.0;
        for(auto it = vec.end(); it != vec.begin(); --it)
        {
            double t = *it;
            if(currentTime - t <= windowTime)
                totalSpikes += 1.0;
            else
                break;
        }

        cerr << " total spikes " << totalSpikes << " in time " << currentTime << endl;

        if(currentTime > windowTime)
            return totalSpikes/windowTime;
        else
            return totalSpikes/currentTime;
    }

    /**
     * Return the spikes from the vector in a given time window.
     */
    void eventsInWindows(const vector<double>& eventVec
            , const double windowTime
            , vector<double>& result
            )
    {
        // Test 
        if(eventVec.size() <= 1)
            return;

        // Function.
        double currentTime = eventVec.back();
        
        // Else get all elements of a vector within this range.
        for(auto it = eventVec.rbegin(); it != eventVec.rend(); ++it)
        {
            if((*it) >= max(0.0, currentTime - windowTime))
                result.push_back(*it);
            else
                return;
        }
    }

    /* This function models the synapse when pre- and post-synaptic firings
     * profile are known.
     */
    void hebbian()
    {

        // sc_time t =  sc_get_time_resolution();
        // double dt = t.to_seconds();

        sc_time st = sc_time_stamp();
        double currTime = st.to_seconds();

        //timeseries_weight_.push_back(weight_);

#ifdef VERBOSE
        // Tests on synaptic weight.
        if(weight_ > max_weight_ || weight_ < min_weight_)
            BOOST_LOG_TRIVIAL(warn) << "Holy synapse. Weight is outside bound: weight: " << weight_;
#endif

        /* The syaptic efficacy must decay at each step */
        //weight_  = weight_ * exp( - dt / decay_tau_ ); 

        //bool pairFound = false, preFound = false, postFound = true;

        if(pre_synaptic_event.event())
        {
            preSynEvents.push_back(currTime);

            //cout << "++ Pre synaptic firing: " << preSynEvents.size() << endl;
            //cout << " ++ Firing rate " << firingRate(preSynEvents, 0.1) << endl;

            /* Here we implement the logic of pairing */
            /* Pair them with all previous post-synaptic events */
            vector<double>  postActs;
            eventsInWindows(postSynEvents, 20e-3, postActs);

            //cerr << "Current Time: " << currTime << endl;
            for(auto t : postActs)
            {
                assert(currTime - t >= 0.0);
                // ltd 
                weight_ -= delta_ltd_ * exp(-(currTime - t)/ tau_ltd_);
            }
        }

        if(post_synaptic_event.event())
        {
            postSynEvents.push_back(currTime);
            //cout << "++ Post synaptic firing: " << postSynEvents.size() << endl;

            vector<double> preActs;
            eventsInWindows(preSynEvents, 20e-3, preActs);

            //cerr << "Current Time: " << currTime << endl;
            for(auto t : preActs)
            {
                // ltp
                assert(currTime - t >= 0.0);
                weight_ += delta_ltp_ * exp(-(currTime - t)/ tau_ltp_);
            }
        }

        return;

    }

    /* This is default constructor */
    SC_CTOR (SimpleSynapse) 
    {
        a_ = 0.0; delta_ltp_ = 0.0096;
        b_ = 0.0; delta_ltd_ = 0.0053;

        weight_ = 50.0;
        min_weight_ = 1.0;
        max_weight_ = 200.0;

        decay_tau_ = 20e-3;

        tau_ltp_ = 16.8e-3; tau_ltd_ = 33.7e-3;

        inFiringRate = 0.0, outFiringRate = 0.0;

        timeWindow = 10e-3;

        synaptic_efficacy_ = 50.0;


        SC_METHOD(hebbian);
        sensitive << pre_synaptic_event << post_synaptic_event;
        sensitive << clock.pos();
    
#ifdef MONITOR
        SC_THREAD(monitor);
        sensitive << clock.pos();
#endif


    }
};

#endif /* end of include guard: SIMPLE_SYNAPSE_H */
