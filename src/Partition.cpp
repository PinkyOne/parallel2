#include "Partition.h"

using namespace KLFM;

Partition::Partition()
{
    //ctor
    m_Bucket.SetPartition(this);
}

Partition::~Partition()
{
    //dtor
}

Cell::Square Partition::GetSquare()
{
    return m_Bucket.GetSquare()+m_Locker.GetSquare();
}

Net::Weight Partition::GetGain()
{
    return m_Bucket.GetGain()+m_Locker.GetSumGain();
}
