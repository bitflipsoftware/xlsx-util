#pragma once
#include "Val.h"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace xlsx
{
    class Sheet
    {
    public:
        Sheet();
        const std::vector<Val>& getRow( int index ) const;
        std::vector<Val>* getMutableRow( int index );
        const std::vector<std::vector<Val>>& getData() const;
        int getNumRows() const;
        void addRow( std::vector<Val>&& row );
        void setHeaders( std::vector<std::string>&& headers );
        const std::vector<std::string>& getHeaders() const;

    private:
        std::vector<std::string> myHeaders;
        std::set<std::string> myDeleteHeaders;
        std::vector<std::vector<Val>> myData;
    };
}