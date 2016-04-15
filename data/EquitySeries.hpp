//
//  EquitySeries.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef EquitySeries_hpp
#define EquitySeries_hpp

#include "Series.hpp"

class EquitySeries: public Series<USEquityReturn>{
public:
    EquitySeries() {}
    EquitySeries(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
        load(driver, filename, begin, end);
    }
    std::size_t load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end);
    std::shared_ptr<EquitySeries> monthly() const;
    std::shared_ptr<EquitySeries> weekly() const;
};

#endif /* EquitySeries_hpp */
