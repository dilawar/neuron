/*
 * ==============================================================================
 *
 *       Filename:  integrate_and_fire.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Friday 18 April 2014 10:42:34  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh, dilawars@ncbs.res.in
 *   Organization:  NCBS Bangalore
 *
 * ==============================================================================
 */

#include "integrate_and_fire.h"

/* Count the firing */
void integrate_and_fire::firingCount()
{
    if(reset.read() == 1)
    {
        reset_ = true;
        firingCount_ = 0;
    }
    else if(enable.read() == 1)
    {
        firingCount_ += 1;
    }
    else
        firingCount_ += 1;
}

/**
 * @brief firePulse. Neuron fires.
 */
void integrate_and_fire::firePulse() 
{
    cout << "I am firing. Oh yeah!  .\n";
    firingCount();
}

void integrate_and_fire::stimGen()
{
}


/**
 * @brief fire. This process fires the neuron. Once launched, it always checking
 * the predicate whether it should fire or not.
 */
void integrate_and_fire::fire()
{
    while(true)
    {
        if(fire_ == true)
        {
            firePulse();
            fire_ = false;
        }
    }
}

void integrate_and_fire::test()
{
    /* Test this neuron */
    while(true)
    {
        reset_ = true;
        fire_ = true;
        wait(10);
        reset_ = false;
        fire_ = true;
        wait(10);
    }
}
