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
        const std::vector<std::vector<Val>>& getData() const;
        int getNumRows() const;
        void addRow( std::vector<Val>&& row );

    private:
        std::vector<std::string> myHeaders;
        std::set<std::string> myDeleteHeaders;
        std::vector<std::vector<Val>> myData;
    };
}