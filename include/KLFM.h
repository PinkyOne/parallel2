#ifndef KLFM_H
#define KLFM_H

#include "CellMove.h"

namespace KLFM {

class KLFMDistribution : public CellMove
{
    public:
        KLFMDistribution(Partition&,Partition&);
        virtual ~KLFMDistribution();
    protected:
        void run();
    private:
};

};
#endif // KLFM_H
