#include "Cell.h"
#include "Partition.h"
#ifdef  ALGORITHM_VERBOSE
#include    <iostream>
#endif

using namespace KLFM;

Cell::Cell()
{
    //ctor
    m_PartitionPtr=NULL;
    SetSquare(1);
    SetPartition(NULL);
    SetGain(0);
    m_InLocker=false;
}

Cell::~Cell()
{
    //dtor
}

Cell::Cell(const Cell& rhs):Id(rhs)
{
    m_Pins=rhs.m_Pins;
    SetGain(rhs.GetGain());
    SetPartition(rhs.GetPartition());
    SetSquare(rhs.GetSquare());
    MoveToLocker(rhs.IsInLocker());
}

Cell& Cell::operator=(const Cell& rhs)
{
    Id::operator=(rhs);
    m_Pins=rhs.m_Pins;
    SetGain(rhs.GetGain());
    SetPartition(rhs.GetPartition());
    SetSquare(rhs.GetSquare());
    MoveToLocker(rhs.IsInLocker());

    return *this;
}

std::set<Cell*> Cell::GetNeighbors()
{
    std::set<Cell*> rv;
    return rv;
}

Partition* Cell::GetPartition() const
{
    return m_PartitionPtr;
}

void Cell::SetPartition(Partition* p)
{
    m_PartitionPtr=p;
}

void Cell::IncrementGain(Net::Weight w)
{
#ifdef  ALGORITHM_VERBOSE
    std::cout << "cell " << (IsInLocker()?"L":"B") << " #" << GetId() << " Cell::IncrementGain(" << w << ") " << GetGain() << " => " << (GetGain()+w) << std::endl;
#endif
    m_Gain+=w;
    if(!IsInLocker()) GetPartition()->m_Bucket.IncrementGain(w);
        else GetPartition()->m_Locker.IncrementSumGain(w);
}
