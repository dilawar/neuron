/*
 * ==============================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  Neuron in systemc.
 *
 *        Version:  1.0
 *        Created:  Friday 18 April 2014 06:20:18  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dilawar Singh (), dilawar@ee.iitb.ac.in
 *   Organization:  
 *
 * ==============================================================================
 */

#if ENABLE_TEST
extern void test_simple_synapse();
#endif

#include <systemc.h>

int sc_main(int argc, char* argv[]) 
{
    cerr << "Top most" << endl;

#if ENABLE_TEST
    test_simple_synapse();
#endif

    return(0);
}

