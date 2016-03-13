#ifndef PARTITION_H
#define PARTITION_H

#include "Id.h"
#include "Bucket.h"

namespace KLFM {

class Partition : public Id
{
    public:
        Partition();
        virtual ~Partition();

        CellList m_Locker;
        Bucket m_Bucket;

        Cell::Square GetSquare();
        Net::Weight GetGain();

    protected:
    private:
        friend class NetlistHypergraph;
};

};

#endif // PARTITION_H
