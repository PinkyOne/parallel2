#ifndef RANDOMDISTRIBUTION_H
#define RANDOMDISTRIBUTION_H

#include "CellMove.h"

namespace KLFM {

class RandomDistribution : public CellMove
{
    public:
        RandomDistribution(Partition&,Partition&);
        virtual ~RandomDistribution();
    protected:
        void run();
    private:
};

};

#endif // RANDOMDISTRIBUTION_H
