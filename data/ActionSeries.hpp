//
//  ActionSeries.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/10/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef ActionSeries_hpp
#define ActionSeries_hpp

#include "Series.hpp"


class ActionSeries: public Series<YahooAction>{
public:
    ActionSeries() {}
    ActionSeries(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
        load(driver, filename, begin, end);
    }
    std::size_t load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end);
};


#endif /* ActionSeries_hpp */
