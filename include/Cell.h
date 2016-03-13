#ifndef CELL_H
#define CELL_H

#include <list>
#include <Id.h>
#include "Pin.h"
#include <set>
#include <vector>
#include "Net.h"

namespace KLFM {

class Partition;

class Cell : public Id
{
    public:
        typedef long Square;
        Cell();
        Cell(const Cell& other);
        Cell& operator=(const Cell&);
        virtual ~Cell();
        Square GetSquare() const { return m_Square; }
        void SetSquare(Square val) { m_Square = val; }
        std::set<Cell*> GetNeighbors();
        Partition* GetPartition() const;
        void SetPartition(Partition*);
        Net::Weight GetGain() const { return m_Gain; }
        void SetGain(Net::Weight w) { m_Gain=w; }
        void IncrementGain(Net::Weight w);
        bool operator==(const Cell& c) const { return c.GetId()==GetId(); }
        void MoveToLocker(bool f=true) { m_InLocker=f; }
        bool IsInLocker() const { return m_InLocker; }

        std::list<Pin> m_Pins;

   protected:
    private:
        bool m_InLocker;
        Net::Weight m_Gain;
        Partition* m_PartitionPtr;
        Square m_Square;
};
};

#endif // CELL_H
