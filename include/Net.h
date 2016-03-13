#ifndef NET_H
#define NET_H

#include <string>
#include "Id.h"
#include <vector>

namespace KLFM {
class Pin;
class Partition;

class Net : public Id
{
    public:
        typedef long Weight;
        Net();
        virtual ~Net();
        Net(const Net& other);
        Net& operator=(const Net& other);
        void SetWeight(Weight w) { m_Weight = w; }
        Weight GetWeight() const { return m_Weight; }
        void AddPin(Pin*);
        unsigned int Dim() const { return m_Pins.size(); }
        unsigned int Dim(Partition*);
        std::vector<Pin*> m_Pins;
    protected:
    private:
        Weight m_Weight;
};

};
#endif // NET_H
