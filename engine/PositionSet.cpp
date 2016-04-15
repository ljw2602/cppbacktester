//
//  PositionSet.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "PositionSet.hpp"


PositionSet PositionSet::closed(void) const
{
    PositionSet closedPos;
    
    for( const_iterator iter = begin(); iter != end(); ++iter )
        if( (*iter)->closed() )
            closedPos.insert(*iter);
    
    return closedPos;
}


PositionSet PositionSet::closed( const std::string& symbol )
{
    PositionSet closedPos;
    
    PositionSet::by_symbol::const_iterator symbol_key_end = get<symbol_key>().upper_bound(symbol);
    
    for( PositionSet::by_symbol::const_iterator iter = get<symbol_key>().lower_bound(symbol);
        iter != symbol_key_end; ++iter )
        if( (*iter)->closed() )
            closedPos.insert(*iter);
    
    return closedPos;
}


PositionSet PositionSet::open(void) const
{
    PositionSet openPos;
    
    for( const_iterator iter = begin(); iter != end(); ++iter )
        if( (*iter)->open() )
            openPos.insert(*iter);
    
    return openPos;
}


PositionSet PositionSet::open( const std::string& symbol )
{
    PositionSet openPos;
    
    PositionSet::by_symbol::const_iterator symbol_key_end = get<symbol_key>().upper_bound(symbol);
    
    for( PositionSet::by_symbol::const_iterator iter = get<symbol_key>().lower_bound(symbol);
        iter != symbol_key_end; ++iter )
        if( (*iter)->open() )
            openPos.insert(*iter);
    
    return openPos;
}


PositionSet PositionSet::longPos( void ) const
{
    PositionSet sPos;
    
    for( const_iterator iter = begin(); iter != end(); ++iter )
        if( (*iter)->type() == Position::LONG )
            sPos.insert(*iter);
    
    return sPos;
}


PositionSet PositionSet::shortPos( void ) const
{
    PositionSet sPos;
    
    for( const_iterator iter = begin(); iter != end(); ++iter )
        if( (*iter)->type() == Position::SHORT )
            sPos.insert(*iter);
    
    return sPos;
}


void PositionSet::print(void) const
{
    for( const_iterator iter = begin(); iter != end(); ++iter ) {
        (*iter)->print();
        std::cout << std::endl;
    }
}
