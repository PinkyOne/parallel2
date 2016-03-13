#include "Net.h"
#include "Partition.h"

using namespace KLFM;

Net::Net()
{
    //ctor
    SetId(-1);
    SetWeight(1.0);
}

Net::~Net()
{
    //dtor
}

Net::Net(const Net& other)
{
    //copy ctor
    *this=other;
}

Net& Net::operator=(const Net& rhs)
{
    Id::operator=(rhs);

    SetWeight(rhs.GetWeight());
    m_Pins=rhs.m_Pins;
    //assignment operator
    return *this;
}

void Net::AddPin(Pin* p)
{
    m_Pins.push_back(p);
}

unsigned int Net::Dim(Partition* p)
{
    unsigned int rv=0;
    for(std::vector<Pin*>::iterator i=m_Pins.begin();i!=m_Pins.end();i++){
        if((*i)->GetCell()->GetPartition()==p) rv++;
        }
    return rv;
}
