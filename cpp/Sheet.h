#pragma once
#include "Val.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace xlsx
{
    using Row = std::vector<Val>;
    using Data = std::vector<Row>;

    class Sheet
    {
    public:
        Sheet();
        const Row& getRow( int index ) const;
        Row* getMutableRow( int index );
        const Data& getData() const;
        int getNumRows() const;
        void addRow( Row&& row );
        void setHeaders( std::vector<std::string>&& headers );
        const std::vector<std::string>& getHeaders() const;

    private:
        std::vector<std::string> myHeaders;
        std::set<std::string> myDeleteHeaders;
        Data myData;
    };
}