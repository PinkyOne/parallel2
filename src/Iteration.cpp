#include "Iteration.h"
#include "NetlistHypergraph.h"
#ifdef  ALGORITHM_VERBOSE
#include <iostream>
#endif
#ifdef PRINT_PROGRESS
#include <iostream>
#endif // PRINT_PROGRESS

using namespace KLFM;

Iteration::Iteration(NetlistHypergraph* _graph):CellMove(_graph->p0,_graph->p1)
{
    m_Improvement=-1;

    //ctor
    graph=_graph;
}

Iteration::~Iteration()
{
    //dtor
}

void Iteration::run()
{
    // Gains are incrementally updated
    p0.m_Bucket.FillByGain(p0.m_Locker);
    p1.m_Bucket.FillByGain(p1.m_Locker);

#ifdef  ALGORITHM_VERBOSE
    std::cout << "*** STARTING ITERATIONS ****" << std::endl;
    p0.m_Bucket.dbg(0);
         std::cout << "LOCKER0 " << p0.m_Locker.dbg() << std::endl;
    p1.m_Bucket.dbg(1);
        std::cout << "LOCKER1 " << p1.m_Locker.dbg() << std::endl;
    std::cout << "P0sq=" << p0.GetSquare() << " P0gn=" << p0.GetGain() <<  " P1sq=" << p1.GetSquare() << " P1gn=" << p1.GetGain() << "\n" << std::endl;
#endif

    // Only save solution after setting an initial gain
    graph->bestSolution=Solution(p0,p1,graph->m_AllCells);

    m_Improvement=0;
#ifdef  ALGORITHM_VERBOSE
    int cnt=0;
#endif
#ifdef  PRINT_PROGRESS
    long Ll=p0.m_Locker.size(),Lr=p1.m_Locker.size();
#endif //PRINT_PROGRESS

    while(!p0.m_Bucket.empty() || !p1.m_Bucket.empty()){

#ifdef  ALGORITHM_VERBOSE
        std::cout << "STEP #" << ++cnt << std::endl;
#endif
        if(!p0.m_Bucket.empty()) {

            if(!p1.m_Bucket.empty()){
                #ifdef PRINT_PROGRESS
                if(p0.GetSquare()>p1.GetSquare()) Lr++,moveRight(); else Ll++,moveLeft();
                #else
                if(p0.GetSquare()>p1.GetSquare()) moveRight(); else moveLeft();
                #endif
                } else moveRight();

            } else moveLeft();

 #ifdef  ALGORITHM_VERBOSE
        p0.m_Bucket.dbg(0);
        std::cout << "LOCKER0 " << p0.m_Locker.dbg() << std::endl;

        p1.m_Bucket.dbg(1);
        std::cout << "LOCKER1 " << p1.m_Locker.dbg() << std::endl;

        std::cout << "P0sq=" << p0.GetSquare() << " P0gn=" << p0.GetGain() <<  " P1sq=" << p1.GetSquare() << " P1gn=" << p1.GetGain() << std::endl;
#endif
#ifdef  PRINT_PROGRESS
        std::cout << "\rLl=" << Ll << " Lr=" << Lr << " T=" << (Ll+Lr) << std::flush;
#endif//PRINT_PROGRESS
        if(Solution::SolutionImproved(graph->bestSolution,p0.GetSquare(),p1.GetSquare(),p0.GetGain()+p1.GetGain())) {
            m_Improvement+=graph->bestSolution.Cut();

            // Store best solution as current
            graph->bestSolution=Solution(p0,p1,graph->m_AllCells);

            m_Improvement-=graph->bestSolution.Cut();
            }

#ifdef  ALGORITHM_VERBOSE
        std::cout << std::endl;
#endif
        }
}

void Iteration::moveCell(Partition* from,Partition* to)
{
    CellList& tgCL = from->m_Bucket.rbegin()->second;
    Net::Weight topGain=from->m_Bucket.rbegin()->first;

    Cell& cell=*tgCL.begin();

    from->m_Bucket.SubtractSquare(cell.GetSquare());
    from->m_Bucket.IncrementGain(-cell.GetGain());

    cell.SetPartition(to);
    cell.MoveToLocker();

#ifdef  ALGORITHM_VERBOSE
    std::cout << "MOVE "<< (from==&p0?"RIGHT":"LEFT");
    std::cout << " Cell #" << cell.GetId() << " CGain=" << cell.GetGain() << " Bgain=" << topGain << std::endl;
#endif
    m_Improvement+=graph->UpdateGains(cell);

    // cell reference is not valid after moving the cell in this line
    tgCL.TransferTo(tgCL.begin(),to->m_Locker,false);

    if(tgCL.empty()) {
        // Remove empty gain lists
        from->m_Bucket.erase(topGain);
        }
}

