/***
 *    Description:  Show info.
 *
 *        Created:  2019-05-13

 *         Author:  Dilawar Singh <dilawars@ncbs.res.in>
 *   Organization:  NCBS Bangalore
 *        License:  MIT License
 */

#ifndef NETWORKSHOWINFOVISITOR_H
#define NETWORKSHOWINFOVISITOR_H

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  Visitor for showing information.
 */
/* ----------------------------------------------------------------------------*/
class NetworkShowInfoVisitor : public boost::static_visitor<int> {
public:
    int operator()(SpikeGeneratorBase* ptr) const 
    {
        ptr->showGroupInfo( );
        return 0;
    }

    int operator()(NeuronGroup* ptr) const
    {
        ptr->showGroupInfo();
        return 0;
    }

    int operator()(SynapseGroup* ptr) const
    {
        ptr->showGroupInfo();
        return 0;
    }
};




#endif /* end of include guard: NETWORKSHOWINFOVISITOR_H */
