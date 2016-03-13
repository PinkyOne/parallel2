#include "Pin.h"

using namespace KLFM;

Pin::Pin()
{
    //ctor
    SetCell(NULL);
    SetNet(NULL);
}

Pin::~Pin()
{
    //dtor
}

Pin::Pin(const Pin& other):Id(other)
{
    //copy ctor
    SetCell(other.GetCell());
    SetNet(other.GetNet());
}

Pin& Pin::operator=(const Pin& rhs)
{
    Id::operator=(rhs);
    SetCell(rhs.GetCell());
    SetNet(rhs.GetNet());
    return *this;
}
