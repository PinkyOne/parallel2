#ifndef ITERATION_H
#define ITERATION_H

#include "Net.h"
#include "CellMove.h"

namespace KLFM {

class NetlistHypergraph;

class Iteration : public CellMove
{
    public:
        Iteration(NetlistHypergraph*);
        virtual ~Iteration();
        Net::Weight GetImprovement() const { return m_Improvement; }
        void moveCell(Partition*,Partition*);

        void moveLeft() { moveCell(&p1,&p0); }
        void moveRight() { moveCell(&p0,&p1); }

        void run();

   protected:

    private:
        Net::Weight m_Improvement;
        NetlistHypergraph* graph;
};

};
#endif // ITERATION_H
