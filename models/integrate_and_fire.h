/*
 * ==============================================================================
 *
 *       Filename:  neuron.cpp
 *
 *    Description:  Neuron modules.
 *
 *        Version:  1.0
 *        Created:  Friday 18 April 2014 06:33:59  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh (), dilawar@ee.iitb.ac.in
 *   Organization:  
 *
 * ==============================================================================
 */

#ifndef  INTEGRATE_AND_FIRE_INC
#define  INTEGRATE_AND_FIRE_INC

#include "systemc.h"

SC_MODULE (integrate_and_fire) 
{
    /* Declare ports and variable */
    sc_in_clk clock;                            /* Everyone is listening */
    sc_in<bool> reset;                          /* Reset all */
    sc_in<bool> enable;                         /* I am in network */

    sc_in<double*> inputs_;
    sc_out<double*> outputs_;

    
    /**
     * @brief fire_. If '1', neuron fires.
     */
    bool fire_;

    /*  Local variable */
    sc_uint<32> firingCount_;
    bool reset_;

    void firingCount();
    void firePulse();


    /* Testing function. It generates stimulus */
    void  stimGen();

    void fire();

    void test();


    /* This is default constructor */
    SC_CTOR (integrate_and_fire) 
    {
        reset_ = false;
        fire_ = false;

        SC_CTHREAD(test, clock.pos());

        SC_CTHREAD(fire, clock.pos());

        SC_METHOD(firingCount);
        sensitive << reset;
        sensitive << clock.pos();

        SC_CTHREAD(stimGen, clock.pos());
    }
};


#endif   /* ----- #ifndef INTEGRATE_AND_FIRE_INC  ----- */
