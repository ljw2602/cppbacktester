//
//  PositionSet.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef PositionSet_hpp
#define PositionSet_hpp

#include "Position.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>


typedef boost::shared_ptr<Position> PositionPtr;

struct symbol_key { };
struct first_exec_key { };
struct last_exec_key { };

typedef boost::multi_index::multi_index_container<

PositionPtr,

boost::multi_index::indexed_by<
boost::multi_index::ordered_unique<boost::multi_index::identity<Position> >,
boost::multi_index::ordered_non_unique<boost::multi_index::tag<symbol_key>, boost::multi_index::const_mem_fun<Position, std::string, &Position::symbol> >,
boost::multi_index::ordered_non_unique<boost::multi_index::tag<first_exec_key>, boost::multi_index::const_mem_fun<Position, const ExecutionPtr, &Position::first_exec> >,
boost::multi_index::ordered_non_unique<boost::multi_index::tag<last_exec_key>, boost::multi_index::const_mem_fun<Position, const ExecutionPtr, &Position::last_exec> >
>
> __PositionSet;

/*!
 PositionSet is a multi index PositionPtr collection indexed by Position ID,
 symbol, first Position execution (opening transaction for first Position in time)
 or last Position execution (closing transaction for last Position in time).
 A Position can be retrieved by ID or by symbol. Example:
 
 Look for position by ID:
 PositionSet::iterator iter = positions.find(id, pos_comp_id());
 
 Look for position by symbol:
 typedef PositionSet::index<symbol_key>::type position_set_by_symbol;
 position_set_by_symbol::iterator it = _miPositions.get<symbol_key>().find("SP500");
 
 \see Position
 */
class PositionSet: private __PositionSet
{
public:
    typedef __PositionSet::iterator                     iterator;
    typedef __PositionSet::const_iterator               const_iterator;
    typedef __PositionSet::index<symbol_key>::type      by_symbol;
    typedef __PositionSet::index<first_exec_key>::type  by_first_exec;
    typedef __PositionSet::index<last_exec_key>::type   by_last_exec;
    
    using __PositionSet::get;
    using __PositionSet::key_extractor;
    using __PositionSet::size;
    using __PositionSet::empty;
    using __PositionSet::begin;
    using __PositionSet::end;
    using __PositionSet::insert;
    using __PositionSet::find;
    using __PositionSet::replace;
    
public:
    //! Return closed positions.
    PositionSet closed(void) const;
    PositionSet closed(const std::string& symbol);
    
    //! Return open positions.
    PositionSet open(void) const;
    PositionSet open(const std::string& symbol);
    
    //! Return all long/short positions.
    PositionSet longPos(void) const;
    PositionSet shortPos(void) const;
    
    //! Print all positions data.
    void print(void) const;
};


struct pos_comp_id
{
    bool operator()(Position::ID id, Position& p) const { return id < p.id(); }
    bool operator()(Position& p, Position::ID id) const { return p.id() < id; }
};

#endif /* PositionSet_hpp */
