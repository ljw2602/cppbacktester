//
//  BalanceSet.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/19/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef BalanceSet_hpp
#define BalanceSet_hpp

#include <memory>
#include <map>
#include <utility>
#include <vector>
#include <set>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "Balance.hpp"
#include "DB.hpp"


typedef boost::shared_ptr<Balance> BalancePtr;

struct date_key1 { };

typedef boost::multi_index::multi_index_container <

BalancePtr,

boost::multi_index::indexed_by <
boost::multi_index::ordered_non_unique<boost::multi_index::const_mem_fun<Balance, std::string, &Balance::name> >,
boost::multi_index::ordered_non_unique<boost::multi_index::tag<date_key1>, boost::multi_index::const_mem_fun<Balance, boost::gregorian::date, &Balance::dt> >
>
> __BalanceSet;


class BalanceSet: private __BalanceSet {
public:
    using __BalanceSet::insert;
    using __BalanceSet::iterator;
    using __BalanceSet::const_iterator;
    using __BalanceSet::equal_range;
    using __BalanceSet::modify;
    using __BalanceSet::size;
    
    static BalanceSet& instance(void);
    
    void initialize(const boost::gregorian::date& dt,
                    const double init,
                    const std::vector<std::string>& names_);
    
    double split_and_dividend(std::map<std::string, int>& share_book,
                              const boost::gregorian::date& today);
    
    void update_capital(const std::map<std::string, int>& share_book,
                        const double netcash,
                        const boost::gregorian::date& today);
    
    double latest_total() const;
    
    void print(void) const;
    
//    std::map<boost::gregorian::date, double> monthly(const std::set<boost::gregorian::date>&) const;
//    std::vector<double> monthly_ret(const std::set<boost::gregorian::date>&) const;
    
    void export_to_csv(const std::string& tag) const;
    
private:
    static std::auto_ptr<BalanceSet> _pInstance;
    
    static std::string CASH, TOTAL;
    std::vector<std::string> names;
    
    typedef __BalanceSet::index<date_key1>::type by_date;
    typedef std::pair<__BalanceSet::const_iterator, __BalanceSet::const_iterator> name_pair;
    typedef std::pair<by_date::const_iterator, by_date::const_iterator> date_pair;
};

#endif /* BalanceSet_hpp */
