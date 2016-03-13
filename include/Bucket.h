#ifndef BUCKET_H
#define BUCKET_H

#include <map>
#include "CellList.h"
#include "Net.h"

namespace KLFM {

class Bucket : public std::map<Net::Weight,CellList>
{
    public:
        virtual ~Bucket();
        Bucket(const Bucket& other);
        Bucket& operator=(const Bucket& other);
        void SetPartition(Partition* p) { m_Partition=p; }
        void FillByGain(CellList&);
        void dbg(long);
        Cell::Square GetSquare() const { return m_Square; }
        void SubtractSquare(Cell::Square s) { m_Square-=s; }
        Net::Weight GetGain() const { return m_SumGain;}
        void IncrementGain(Net::Weight g);
    protected:
    private:
        Cell::Square m_Square;
        Net::Weight m_SumGain;
        Partition* m_Partition;
        Bucket();
        friend class Partition;
};

};

#endif // BUCKET_H
