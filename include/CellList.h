#ifndef CELLLIST_H
#define CELLLIST_H

#include <list>
#include "Cell.h"
#include "Net.h"

namespace KLFM {

class CellList : public std::list<Cell>
{
    public:
        CellList();
        virtual ~CellList();
        void TransferTo(const std::list<Cell>::iterator& ,CellList&,bool UpdateGain=true);
        void TransferAllFrom(CellList&);
        Cell::Square GetSquare();
        std::string dbg();
        void SetCellGain(Net::Weight gain);
        Net::Weight GetSumGain();
        Net::Weight IncrementSumGain(Net::Weight g);
        void InvalidateGain() { m_GainComputed=false; m_SumGain=0; }
    protected:
    private:
        bool m_GainComputed;
        Net::Weight m_SumGain;
        bool m_SquareComputed;
        Cell::Square m_Square;
};

};

#endif // CELLLIST_H
