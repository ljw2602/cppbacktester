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
    PriceSeries(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
        load(driver, filename, begin, end);
    }
    std::size_t load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end);
    std::shared_ptr<PriceSeries> monthly() const;
    std::shared_ptr<PriceSeries> weekly() const;
};

#endif /* PriceSeries_hpp */
