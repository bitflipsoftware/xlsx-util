#include "Sheet.h"


namespace xlsx
{
    Sheet::Sheet()
    : myHeaders{}
    , myDeleteHeaders{}
    , myData{}
    {

    }


    const std::vector<Val>&
    Sheet::getRow( int index ) const
    {
        return myData.at( static_cast<size_t>( index ) );
    }


    std::vector<Val>*
    Sheet::getMutableRow( int index )
    {
        return &( *( myData.begin() + static_cast<size_t>( index ) ) );
    }
    

    const std::vector<std::vector<Val>>&
    Sheet::getData() const
    {
        return myData;
    }


    int
    Sheet::getNumRows() const
    {
        return static_cast<int>( myData.size() );
    }


    void
    Sheet::addRow( std::vector<Val>&& row )
    {
        myData.emplace_back( std::move( row ) );
    }
}