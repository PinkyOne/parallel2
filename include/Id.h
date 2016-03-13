#ifndef ID_H
#define ID_H

#include <string>

namespace KLFM {

class Id
{
    public:
        Id();
        virtual ~Id();
        Id(const Id& other);
        Id& operator=(const Id& other);
        std::string GetName() const { return m_Name; }
        void SetName(std::string val) { m_Name = val; }
        inline long GetId() const { return m_Id; }
        void SetId(long val) { m_Id = val; }
    protected:
    private:
        std::string m_Name;
        long m_Id;
};

};
#endif // ID_H
