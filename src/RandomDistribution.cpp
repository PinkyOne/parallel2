#include <algorithm>
#include "RandomDistribution.h"
#include <random>
#ifdef  ALGORITHM_VERBOSE
#include <iostream>
#endif

using namespace KLFM;

RandomDistribution::RandomDistribution(Partition& _p0,Partition& _p1):CellMove(_p0,_p1)
{
   //ctor
    run();
}

RandomDistribution::~RandomDistribution()
{
    //dtor
}

void RandomDistribution::run()

{
    const long HRM = RAND_MAX / 2;
    p0.m_Locker.TransferAllFrom(p1.m_Locker);

    std::for_each(p0.m_Locker.begin(),p0.m_Locker.end(),std::bind2nd(std::mem_fun_ref(&Cell::SetPartition), &p0));

    while(p0.m_Locker.GetSquare()>p1.m_Locker.GetSquare())
    {

        for(std::list<Cell>::iterator i=p0.m_Locker.begin();
            i!=p0.m_Locker.end() && p0.m_Locker.GetSquare()>p1.m_Locker.GetSquare();
            i++)
        {
            if(std::rand()>HRM)
            {
                i->SetPartition(&p1);
                p0.m_Locker.TransferTo(i--,p1.m_Locker);
           }
        }
    }
}
