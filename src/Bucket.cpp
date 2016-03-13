#include "Bucket.h"
#ifdef CHECK_LOGIC
#include <stdexcept>
#endif//CHECK_LOGIC
#ifdef  ALGORITHM_VERBOSE
#include <iostream>
#endif

using namespace KLFM;

Bucket::Bucket()
{
    //ctor
    m_Square=0;
    m_SumGain=0;
}

Bucket::~Bucket()
{
    //dtor
}

Bucket::Bucket(const Bucket& other)
{
    #ifdef CHECK_LOGIC
    //copy ctor
    throw std::logic_error("Bucket must not be copied");
    #endif
}

Bucket& Bucket::operator=(const Bucket& rhs)
{
    #ifdef CHECK_LOGIC
    throw std::logic_error("Bucket must not be copied");
    #endif // CHECK_LOGIC

    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void Bucket::FillByGain(CellList& cl)
{
    #ifdef CHECK_LOGIC
    if(!empty()) throw std::logic_error("Unable to start moving cells into a non-empty bucket");
    #endif // CHECK_LOGIC

    m_Square += cl.GetSquare();
    m_SumGain=0;
    while (!cl.empty()) {
        Net::Weight g=cl.begin()->GetGain();
        m_SumGain+=(g);
        CellList& bl=(*this)[g];
        cl.TransferTo(cl.begin(),bl);
        }
    cl.InvalidateGain();
}

#ifdef  ALGORITHM_VERBOSE
void Bucket::dbg(long id)
{
    std::cout << "BUCKET #" << id << " SQ=" << m_Square  << " GAIN=" << GetGain() << std::endl;
    for(std::map<Net::Weight,CellList>::iterator i=begin();i!=end();i++){
        Net::Weight g=i->first;
        CellList& bl=i->second;
        std::cout << " Gain " << g << " - " << std::flush;
        for(std::list<Cell>::iterator j=bl.begin();j!=bl.end();j++){
            std::cout << j->GetId() << " ";
            }
        std::cout << std::endl;
        }
}
#endif

void Bucket::IncrementGain(Net::Weight g)
{
#ifdef  ALGORITHM_VERBOSE
    std::cout << "Bucket::IncrementGain g=" << g << std::endl;
#endif
    m_SumGain+=g;
}

