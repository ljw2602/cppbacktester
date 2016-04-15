//
//  TreasurySeries.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef TreasurySeries_hpp
#define TreasurySeries_hpp

#include "Series.hpp"

class TreasurySeries: public Series<USTreasuryYield>{
public:
    TreasurySeries(const std::string& maturity_): Series<USTreasuryYield>(maturity_), maturity(maturity_) {}
    TreasurySeries(const std::string& maturity_, FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end): Series<USTreasuryYield>(maturity_), maturity(maturity_) {
        load(driver, filename, begin, end);
    }
    std::size_t load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end);
    std::shared_ptr<TreasurySeries> monthly() const;
    std::shared_ptr<TreasurySeries> weekly() const;
private:
    std::string maturity;
};

#endif /* TreasurySeries_hpp */
