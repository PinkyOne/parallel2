#include "Id.h"
#include <stdalign.h>

using namespace KLFM;

Id::Id()
{
    //ctor
    m_Id=-1;
}

Id::~Id()
{
    //dtor
}

Id::Id(const Id& other)
{
    //copy ctor
    *this=other;
}

Id& Id::operator=(const Id& rhs)
{
    SetId(rhs.GetId());
    SetName(rhs.GetName());
    //assignment operator
    return *this;
}
