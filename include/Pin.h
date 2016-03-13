#ifndef PIN_H
#define PIN_H

#include "Id.h"

namespace KLFM
{

class Cell;
class Net;

class Pin : public Id
{
public:
    Pin();
    virtual ~Pin();
    Pin(const Pin& other);
    Pin& operator=(const Pin& other);
    Cell* GetCell() const
    {
        return m_Cell;
    }
    void SetCell(Cell* val)
    {
        m_Cell = val;
    }
    Net* GetNet() const
    {
        return m_Net;
    }
    void SetNet(Net* val)
    {
        m_Net = val;
    }
protected:
private:
    Cell* m_Cell;
    Net* m_Net;
};

};
#endif // PIN_H
