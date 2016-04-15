//
//  ExecutionSet.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef ExecutionSet_hpp
#define ExecutionSet_hpp

#include "Execution.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>


typedef boost::shared_ptr<Execution> ExecutionPtr;

struct date_key { };
struct side_key { };

typedef boost::multi_index::multi_index_container<

ExecutionPtr,

boost::multi_index::indexed_by<
boost::multi_index::ordered_unique<boost::multi_index::identity<Execution> >,
boost::multi_index::ordered_non_unique<boost::multi_index::tag<date_key>, boost::multi_index::const_mem_fun<Execution, boost::gregorian::date, &Execution::dt> >,
boost::multi_index::ordered_non_unique<boost::multi_index::tag<side_key>, boost::multi_index::const_mem_fun<Execution, Execution::Side, &Execution::side> >
>
> __ExecutionSet;


/*!
 ExecutionSet is a multi index collection containing a set of Executions, indexed by Execution ID,
 execution date and execution side.
 */
class ExecutionSet: private __ExecutionSet
{
public:
    typedef __ExecutionSet::iterator iterator;
    typedef __ExecutionSet::const_iterator const_iterator;
    
    using __ExecutionSet::size;
    using __ExecutionSet::empty;
    using __ExecutionSet::begin;
    using __ExecutionSet::end;
    using __ExecutionSet::insert;
    using __ExecutionSet::find;
    
public:
    ExecutionSet(void);
    ~ExecutionSet(void) { }
    
    //! Add transactions.
    Execution::ID buy(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size);
    Execution::ID sell(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size);
    Execution::ID sell_short(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size);
    Execution::ID cover(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size);
    
    //! Return the first Execution by date.
    //! \note Execution is a pure virtual class, return type must be a reference or a pointer
    const ExecutionPtr first_by_date(void) const { return *(get<date_key>().begin()); }
    //! Return the last Execution by date.
    //! \note Execution is a pure virtual class, return type must be a reference or a pointer
    const ExecutionPtr last_by_date(void) const { return *(get<date_key>().rbegin()); }
    
    //! Print all Execution data in this collection.
    void print(void) const;
    
private:
    //! Unique execution identifier
    static Execution::ID _eid;
    
private:
    typedef __ExecutionSet::index<side_key>::type by_side;
    typedef __ExecutionSet::index<date_key>::type by_date;
};

#endif /* ExecutionSet_hpp */
