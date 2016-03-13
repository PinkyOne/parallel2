#include <algorithm>
#include "NetlistHypergraph.h"
#include <iostream>
#ifdef CHECK_LOGIC
#include <stdexcept>
#endif//CHECK_LOGIC
#include <sstream>

using namespace KLFM;

NetlistHypergraph::NetlistHypergraph():m_AllCells(0),bestSolution(p0,p1,m_AllCells)
{
    //ctop
    p0.SetId(0);
    p1.SetId(1);
}

NetlistHypergraph::~NetlistHypergraph()
{
    //dtor
}


void NetlistHypergraph::FillBuckets()
{
    std::vector<Net>::iterator n;
    std::vector<Pin*>::iterator p;
    Net::Weight left=0,right=0;

    for(n=nets.begin();n!=nets.end();n++) {
        // Set gain for all cells beloging to this partition, with other net cells in other partition
        Net& net = *n;
        left=right=0;

#ifdef  ALGORITHM_VERBOSE
        std::cout << " NET #" << net.GetId() << "=======" << std::endl;
#endif
        // compute gains
        for(p=net.m_Pins.begin();p!=net.m_Pins.end();p++){
            Cell& cell = *(*p)->GetCell();

            if(cell.GetPartition()==&p0) left+=net.GetWeight();
            else if(cell.GetPartition()==&p1) right+=net.GetWeight();
            #ifdef CHECK_LOGIC
            else throw std::logic_error("cell does not belong to ANY partition");
            #endif // CHECK_LOGIC
            }

        // set gains
        for(p=net.m_Pins.begin();p!=net.m_Pins.end();p++){
            Cell& cell = *(*p)->GetCell();

            if(cell.GetPartition()==&p0){
                // cell on the left
                cell.SetGain(cell.GetGain()+right-left+net.GetWeight());

                } else if(cell.GetPartition()==&p1){
                    // cell on the right
                    cell.SetGain(cell.GetGain()+left-right+net.GetWeight());
                    }

#ifdef  ALGORITHM_VERBOSE
            std::cout << "Cell #" << cell.GetId() << " gain " << cell.GetGain() << std::endl;
#endif
            }
        }

#ifdef  ALGORITHM_VERBOSE
    std::cout << "P0 " << p0.m_Locker.dbg() << "\nP1 " << p1.m_Locker.dbg() << std::endl;
#endif
}

// This function is called after changing partition in the cell
/*
Table of gains N=24 Weight=2, moving left to right
* dMC - delta gain on the cell being moved
L	R	dLR	Gl	Gr	dMC
12	12	0	1W	1W	-
11	13	-2	3W	-1W	2W
10	14	-4	5W	-3W	2W
9	15	-6	7W	-5W	2W
8	16	-8	9W	-7W	2W
7	17	-10	11W	-9W	2W
6	18	-12	13W	-11W	2W
5	19	-14	15W	-13W	2W
4	20	-16	17W	-15W	2W
3	21	-18	19W	-17W	2W
2	22	-20	21W	-19W	2W
1	23	-22	23W	-21W	2W
0	24	-24	25W	-23W	2W
*/
Net::Weight NetlistHypergraph::UpdateGains(Cell& c)
{
    Net::Weight rv=0;

    Partition* newP=c.GetPartition(), *oldP=NULL;

    if(newP==&p0) oldP=&p1;
    else if(newP==&p1) oldP=&p0;
    #ifdef CHECK_LOGIC
    else throw std::logic_error("Wrong parition pointer");
    #endif // CHECK_LOGIC

    std::map<Cell*,Net::Weight> prevGain;

    c.SetGain(0);

    // Find all connected nets
    for(std::list<Pin>::iterator p=c.m_Pins.begin();p!=c.m_Pins.end();p++){

        Net& net = *p->GetNet();

#ifdef  ALGORITHM_VERBOSE
        std::cout << "UPDATE GAIN NET #" << net.GetId() << std::endl;
#endif
        long newPcnt=0,oldPcnt=0;

        // Adjust gain for all cells affected
        // It'll work for the cell moved if you do the math, same dG altough different formula
        for(std::vector<Pin*>::iterator i=net.m_Pins.begin();i!=net.m_Pins.end();i++) {
            Cell& cell=*(*i)->GetCell();

            if(cell==c) continue;

            Net::Weight dG=0;
            if(cell.GetPartition()==newP) { newPcnt++; dG=-2*net.GetWeight(); }
            else {
                    if(cell.GetPartition()==oldP) { oldPcnt++; dG=2*net.GetWeight(); }
                    #ifdef CHECK_LOGIC
                    else { throw std::logic_error("Wrong parition pointer in cell"); }
                    #endif // CHECK_LOGIC
            }

            // Store previous gain when we hit the cell for the first time
            if(!cell.IsInLocker() && prevGain.find(&cell)==prevGain.end()) prevGain[&cell]=cell.GetGain();

            cell.IncrementGain(dG);
            rv+=dG;
            }

        c.IncrementGain((oldPcnt-newPcnt)*net.GetWeight());
        }

    for(std::map<Cell*,Net::Weight>::iterator k=prevGain.begin();k!=prevGain.end();k++){
        Cell& cell=*(k->first);
        if(cell.IsInLocker()) continue;
        Bucket& b=cell.GetPartition()->m_Bucket;
        CellList& cl =b[k->second];
        std::list<Cell>::iterator it=std::find(cl.begin(),cl.end(),cell);
        #ifdef CHECK_LOGIC
        if(it==cl.end()) throw std::logic_error("Cannot find cell in a bucket list");
        #endif // CHECK_LOGIC
        cl.TransferTo(it,b[cell.GetGain()]);
        if(cl.empty())  b.erase(k->second);
        }

    return rv;
}

NetlistHypergraph::CutStat NetlistHypergraph::GetStats(std::ofstream& o,bool fWrite)
{
    CutStat rv;
    if(fWrite) o << "Nets cutting: " << std::endl;
    for(std::vector<Net>::iterator i=nets.begin();i!=nets.end();i++)
    {
        Net& net=*i;
        Partition* p=NULL;
        for(std::vector<Pin*>::iterator j=net.m_Pins.begin();j!=net.m_Pins.end();j++){
            if(!p) {
                    p=(*j)->GetCell()->GetPartition();
            }
            else if(p!=(*j)->GetCell()->GetPartition()){
                if(fWrite) o << net.GetName() << std::endl;
                rv.m_NetCut++;
                rv.m_totWeigth+=net.GetWeight();
                break;
                }
            }
    }
    std::stringstream msg;
    msg << "Cutting nets "<< rv.m_NetCut << ", total weight is " << rv.m_totWeigth;
    std::cout << msg.str()  << std::endl;
    if(fWrite) o << msg.str()  << std::endl;
    return rv;
}
