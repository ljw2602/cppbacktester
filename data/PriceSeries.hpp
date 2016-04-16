//
//  PriceSeries.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef PriceSeries_hpp
#define PriceSeries_hpp

#include "Series.hpp"


class PriceSeries: public Series<YahooPrice>{
public:
    PriceSeries(){}
    PriceSeries(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end)
    {
        load(driver, filename, begin, end);
    }
    std::size_t load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end);
    std::shared_ptr<PriceSeries> monthly() const;
    std::shared_ptr<PriceSeries> weekly() const;
    
    std::vector<double> adjclose(TS::const_iterator iter, unsigned long num ) const
    {
        std::vector<double> v;
        
        if( iter == end() )
            return v;
        
        // reverse iterator init skips the first element in collection. We must manually insert the current element.
        v.insert(v.begin(), iter->second->adjclose());
        unsigned i = 1;
        for(TS::const_reverse_iterator rev_iter(iter); i < num && rev_iter != rend(); ++rev_iter, ++i )
            v.insert(v.begin(), rev_iter->second->adjclose());
        
        return v;
    }
};

#endif /* PriceSeries_hpp */
