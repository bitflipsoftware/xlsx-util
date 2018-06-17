#include "Sheet.h"


namespace xlsx
{
    Sheet::Sheet()
    : myHeaders{}
    , myDeleteHeaders{}
    , myData{}
    {

    }


    const Row&
    Sheet::getRow( int index ) const
    {
        return myData.at( static_cast<size_t>( index ) );
    }


    Row*
    Sheet::getMutableRow( int index )
    {
        return &( *( myData.begin() + static_cast<size_t>( index ) ) );
    }
    

    const Data&
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
    Sheet::addRow( Row&& row )
    {
        myData.emplace_back( std::move( row ) );
    }


    void
    Sheet::setHeaders( std::vector<std::string>&& headers )
    {
        myHeaders = std::move( headers );
    }


    const std::vector<std::string>&
    Sheet::getHeaders() const
    {
        return myHeaders;
    }
}