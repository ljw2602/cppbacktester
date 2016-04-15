//
//  ActionSeries.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/10/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ActionSeries.hpp"

std::size_t ActionSeries::load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
    return _load(driver, filename, begin, end);
}