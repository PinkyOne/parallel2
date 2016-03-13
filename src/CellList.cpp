#include "CellList.h"
#include "sstream"
#include "Partition.h"
#ifdef  ALGORITHM_VERBOSE
#include <iostream>
#endif
#include <algorithm>

using namespace KLFM;

CellList::CellList()
{
    //ctor
    m_SquareComputed=false;
    m_Square=0;
    m_GainComputed=false;
    m_SumGain=0;
}

CellList::~CellList()
{
    //dtor
}

Net::Weight CellList::GetSumGain()
{
    if(m_GainComputed) return m_SumGain;
    m_GainComputed=true;
    m_SumGain=0;
    for(std::list<Cell>::iterator i=begin();i!=end();i++) m_SumGain+=i->GetGain();
    return m_SumGain;
}

void CellList::TransferTo(const std::list<Cell>::iterator& it, CellList& cl,bool UpdateGain)
{
    Cell& cell=*it;

#ifdef  ALGORITHM_VERBOSE
    std::cout << "CellList::TransferTo() cell #" << cell.GetId() << std::endl;
#endif
    if(UpdateGain){
        IncrementSumGain(-cell.GetGain());
        cl.IncrementSumGain(cell.GetGain());
        }

    m_Square=GetSquare()-cell.GetSquare();
    cl.m_Square=cl.GetSquare()+cell.GetSquare();

    cl.splice(cl.end(),*this,it);
}

Cell::Square CellList::GetSquare()
{
    if(m_SquareComputed) return m_Square;
    m_Square=0;
    m_SquareComputed=true;
    for(std::list<Cell>::iterator i=begin();i!=end();i++) m_Square+=i->GetSquare();
    return m_Square;
}

void CellList::TransferAllFrom(CellList& cl)
{
    m_Square=GetSquare()+cl.GetSquare();
    cl.m_Square=0;
    splice(end(),cl);
}

std::string CellList::dbg()
{
    std::stringstream ss;
    ss << " SQ=" << GetSquare() << " GAIN=" << GetSumGain();
    for(std::list<Cell>::iterator i=begin();i!=end();i++)
       ss << " " << i->GetId() << " G" << i->GetGain() << "_p" << i->GetPartition()->GetId();
    return ss.str();
}

void CellList::SetCellGain(Net::Weight g)
{
 std::for_each(begin(),end(),std::bind2nd(std::mem_fun_ref(&Cell::SetGain), g));
}

Net::Weight CellList::IncrementSumGain(Net::Weight g)
{
#ifdef  ALGORITHM_VERBOSE
    std::cout << "CellList::IncrementSumGain(" << g << ") " << GetSumGain() << " => " << (GetSumGain()+g) << std::endl;
#endif
    return m_SumGain=GetSumGain()+g;
}
