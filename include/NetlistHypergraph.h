#ifndef NETLISTHYPERGRAPH_H
#define NETLISTHYPERGRAPH_H

#include "Partition.h"
#include <vector>
#include <memory>
#include "RandomDistribution.h"
#include "Iteration.h"
#include "Bucket.h"
#include "Solution.h"
#include <fstream>

namespace KLFM {

class NetlistHypergraph
{
    protected:
        friend class Iteration;
        std::vector<Cell*> m_AllCells;

    public:
        NetlistHypergraph();
        virtual ~NetlistHypergraph();
        std::vector<Net> nets;
        Partition p0,p1;
        Solution bestSolution;

        void FillBuckets();
        Net::Weight UpdateGains(Cell&);

        struct CutStat {
            long m_NetCut;
            Net::Weight m_totWeigth;
            CutStat() { m_NetCut=0; m_totWeigth=0; }
            };

        CutStat GetStats(std::ofstream&,bool fWrite=true);

    private:
};

};

#endif // NETLISTHYPERGRAPH_H
