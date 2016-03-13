#include <iostream>
#include "NetlistHypergraph.h"
#include <sstream>
#include <fstream>
#ifdef CHECK_LOGIC
#include <stdexcept>
#endif//CHECK_LOGIC
#include <cstdlib>

using namespace std;

namespace KLFM
{

class Test6 : public NetlistHypergraph
{
    class Files {
        std::string m_input, m_left, m_right, m_stat;
        std::ofstream m_l,m_r,m_s;
        public:
        Files(std::string i){
            m_input=i;
            m_left=i+"_left";
            m_right=i+"_right";
            m_stat=i+="_stat";
            }

        std::ofstream& GetLeft()
        {
            if(!m_l.is_open()) m_l.open(m_left.c_str());
            return m_l;
        }

        std::ofstream& GetRight()
        {
            if(!m_r.is_open()) m_r.open(m_right.c_str());
            return m_r;
        }

        std::ofstream& GetStat()
        {
            if(!m_s.is_open()) m_s.open(m_stat.c_str());
            return m_s;
        }
        } m_Files;

    std::map<std::string,Cell*> m_name2cell;
    long current_ln;

    Pin& getNextPin(Cell& c1, const std::string& name=""){
        Pin newPin;
        newPin.SetId(c1.m_Pins.size());
        newPin.SetName(name);
        newPin.SetCell(&c1);
        c1.m_Pins.push_back(newPin);
        return c1.m_Pins.back();
        }

 #ifdef  ALGORITHM_VERBOSE
   void printNetlist(){
        for(std::list<Cell>::iterator i=p0.m_Locker.begin();i!=p0.m_Locker.end();i++) {
            Cell& c=*i;
            std::cout << "Cell '"<< c.GetName() << "' #" << c.GetId() << " connects to " << std::flush;
            for(std::list<Pin>::iterator j=c.m_Pins.begin();j!=c.m_Pins.end();j++){
                if(!j->GetNet()) throw std::logic_error("No net is initialized");
                Net& net = *j->GetNet();
                std::cout << " pin " << j->GetName() << "-> net " << net.GetName() << " ";
                for(std::vector<Pin*>::iterator ll=net.m_Pins.begin();ll!=net.m_Pins.end();ll++) {
                    if(!(*ll)->GetCell()) std::logic_error("No cell is initialized");
                    if(c.GetId()!=(*ll)->GetCell()->GetId())
                        std::cout << (*ll)->GetCell()->GetName() << ":" << (*ll)->GetName() << " ";
                    }
                }
            std::cout << std::endl;
            }
        }
#endif
    void ReadGraphFromFile(std::string fn)
    {
        std::cout << "Reading from '" << fn << "' .. " << std::flush;
        std::ifstream f(fn.c_str());
        current_ln=0;
        unsigned int cell_idx=0;
        std::vector< std::vector<std::string> > tmpNets;
        while(!f.eof())
        {
            current_ln++;
            std::string l;
            std::vector<std::string> words;
            std::getline(f,l);
            if(f.eof() && l=="") break;
            std::stringstream ss(l);
            while (ss>>l) words.push_back(l);
            #ifdef CHECK_LOGIC
            if(words.size()<2) {
                std::cout << "Error at " << fn << ":" << current_ln << " '" << l << "'" << std::endl;
                throw std::logic_error("Parsing error");
                }
            #endif
            if(words.size()==2) MakeCell(words[0],words[1],cell_idx);
            else tmpNets.push_back(words);
        }
        f.close();

        unsigned int netIdx=0;
        nets.resize(tmpNets.size());
        for(std::vector< std::vector<std::string> >::iterator k=tmpNets.begin();k!=tmpNets.end();k++,netIdx++)
            MakeNet(nets[netIdx],*k,netIdx);

        std::cout << " done" << std::endl;
    }

    void MakeCell(const std::string & cellName, const std::string& ssq, unsigned int& idx )
    {
        #ifdef CHECK_LOGIC
        if(m_name2cell.find(cellName)!=m_name2cell.end()) {
            std::stringstream msg;
            msg << "Duplicated cell '" << cellName << "' at line " << current_ln;
            throw std::logic_error(msg.str());
            }
        #endif
        Cell::Square sq=0;
        std::stringstream s(ssq);
        s >> sq;
        p0.m_Locker.emplace_back();
        Cell& tmpCell=p0.m_Locker.back();
        tmpCell.SetId(idx++);
        tmpCell.SetName(cellName);
        tmpCell.SetSquare(sq);
        #ifdef CHECK_LOGIC
        if(!sq) {
            std::stringstream msg;
            msg << "Cell '" << cellName << "' has illegal square '" << ssq << "'";
            throw std::logic_error(msg.str());
            }
        #endif
        m_name2cell[cellName]=&tmpCell;
        m_AllCells.push_back(&tmpCell);
    }

    void MakeNet(Net& net,const std::vector<std::string>& words, int idx)
    {
        net.SetId(idx);
        int cnt=0;
        Net::Weight w;
        std::stringstream s;
        for(std::vector<std::string>::const_iterator i=words.begin();i!=words.end();i++)
        {
            switch(cnt++)
            {
                case 0:
                        net.SetName(*i);
#ifdef  ALGORITHM_VERBOSE
                        std::cout << "Creating net '" << *i << "' weigth="  << std::flush;
#endif
                        break;
                case 1: s.str(*i);
                        s >> w;
 #ifdef  ALGORITHM_VERBOSE
                       std::cout << w << " " << std::flush;
#endif
                        net.SetWeight(w);
                      break;
                default:
                        MakePin(*i++,*i,net);
                        break;
            }
        }
#ifdef  ALGORITHM_VERBOSE
        std::cout << std::endl;
#endif
    }

    void MakePin(const std::string& cellName,const std::string& pinName, Net& net)
    {
        std::map<std::string,Cell*>::iterator i=m_name2cell.find(cellName);
        #ifdef CHECK_LOGIC
        if(i==m_name2cell.end()) {
            throw std::logic_error("Incorrect cell name");
            }
        #endif
        Cell& cell=*(i->second);
        Pin& pin=getNextPin(cell,pinName);
        pin.SetNet(&net);
        net.AddPin(&pin);
#ifdef  ALGORITHM_VERBOSE
        std::cout << cell.GetName() << ":" << pin.GetName() << " ";
#endif
    }

public:
    Test6(std::string fn):m_Files(fn){
        std::srand(2015);

        ReadGraphFromFile(fn);

#ifdef  ALGORITHM_VERBOSE
        printNetlist();
#endif
        RandomDistribution(p0,p1);

#ifdef  ALGORITHM_VERBOSE
        std::cout << "LOCKER0 SQ=" << p0.m_Locker.GetSquare() << " " << p0.m_Locker.dbg() << std::endl;
        std::cout << "LOCKER1 SQ=" << p1.m_Locker.GetSquare() << " " << p1.m_Locker.dbg() << std::endl;
#endif
        FillBuckets();

        for(int iter_cnt=0;;iter_cnt++){

            Iteration step(this);

            step.run();

            #ifdef PRINT_PROGRESS
            std::cout << std::endl;
            #endif // PRINT_PROGRESS

            if(step.GetImprovement()<=0) break;

            bestSolution.WriteLockers(p0.m_Locker,p1.m_Locker);

            std::cout << "ITERATION " << iter_cnt << ", IMPROVEMENT " << step.GetImprovement() << std::endl;
            }

        std::ofstream& l= m_Files.GetLeft(), & r = m_Files.GetRight();
        for(auto&& cell: p0.m_Locker) l << cell.GetName() << std::endl;
        for(auto&& cell: p1.m_Locker) r << cell.GetName() << std::endl;

        // write best solution
        GetStats(m_Files.GetStat(),true);
        }
};
};

int main(int argn, char** argv)
{
    if(argn<=1) {
       std::cout << "Usage: " << argv[0] << " <netlist file>\n"
        << "Netlist file format:\n"
        << "<cell> <square> - cell name and square, separate line\n"
        << "<net name> <weigth> [<cell> <pin> .. <cell> <pin>] - nets, separate line\n"
        << "single space symbol used as delimiter"
        << std::endl;
        return (1);
        }
    KLFM::Test6 t_6(argv[1]);
    return 0;
}
