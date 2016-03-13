#ifndef CELLMOVE_H
#define CELLMOVE_H

#include "Partition.h"

namespace KLFM {

class CellMove
{
    public:
        CellMove(Partition&,Partition&);
        virtual ~CellMove();
    protected:
        virtual void run() = 0;
        Partition& p0;
        Partition& p1;
    private:
};

};

#endif // CELLMOVE_H
