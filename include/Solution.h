#ifndef SOLUTION_H
#define SOLUTION_H

#include    "Net.h"
#include    "Cell.h"
#include    "Partition.h"
#include    <map>

namespace KLFM {


class Solution
{
    public:
        struct CellRecord {
            CellRecord():cell(NULL),gain(0),p(NULL){}
            Cell* cell;
            Net::Weight gain;
            Partition* p;
            };
        Solution(Partition&,Partition&,std::vector<Cell*>&);
        virtual ~Solution();

        Solution& operator=(const Solution&);

        // The closer to 1 the better
        bool IsInitial() const { return !s1||!s2; }
        float Ratio() const { return float(std::max(s1,s2)) / float(std::min(s1,s2)); }

        // Quality of the solution
        Net::Weight Cut() const { return g1+g2; }

        void AddCell(Cell*);

        static bool SolutionImproved(
            Solution&,

            Cell::Square s2_0,
            Cell::Square s2_1,
            Net::Weight g2);

        // Interchanges cells in lockers and initializes gain
        void WriteLockers(CellList& l0, CellList& l1);

    protected:
        Partition *p1, *p2;
        Net::Weight g1,g2;
        Cell::Square s1,s2;
        std::vector<CellRecord> m_Recs;
};

};

#endif // SOLUTION_H
