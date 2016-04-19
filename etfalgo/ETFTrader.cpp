//
//  ETFTrader.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ETFTrader.hpp"
#include "TA.hpp"


ETFTrader::ETFTrader(const double initial): initial_capital(initial) { }


void preprocess_data(unsigned yr, unsigned mon)
{
    // Advance all data pointers by yr and mon
    // DB::advance() checks data is longer than requested duration
    
    DB::instance().advance(yr, mon);
}


double yoy_return(const std::string& name)
{
    // Calculates one year return using adjusted close price
    
    TA ta;
    PriceSeries series = DB::instance().get(name).monthly();
    PriceSeries::const_iterator pSeries = DB::instance().get(name).pMonthly();
    TA::ROCRes resROC = ta.ROC(series.adjclose(pSeries,13), 12);
    
    return resROC.roc[0];
}

std::map<double, std::string, std::greater<double> > yoy_return_map()
{
    // Calculates one year return using adjusted close price for all symbols in DB
    
    std::map<double, std::string, std::greater<double> > ret;  // map with descending order
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ )
        ret.insert( std::pair<double, std::string>(yoy_return(*it), *it) );
    
    return ret;
}

std::map<double, std::string, std::greater<double> > monthly_estimation()
{
    // Return estimation result
    
    std::map<double, std::string, std::greater<double> > est = yoy_return_map();
    std::cout << "Estimation:" << std::endl;
    for (std::map<double, std::string>::const_iterator it = est.begin(); it != est.end(); it++) {
        std::cout << it->second << " " << it->first << std::endl;
    }
    return est;
}


///////////////////////////
// THIS NEEDS MORE WORK! //
///////////////////////////
int SHARES = -1;
std::map<std::string, int> monthly_allocation(const std::map<double, std::string, std::greater<double> >& est)
{
    // Allocate asset weight, based on the estimation()
    // Use Black-Litterman
    
    std::map<std::string, int> target;
    target.insert( std::pair<std::string, int>(est.begin()->second, SHARES) );
    target.insert( std::pair<std::string, int>(est.rbegin()->second, -SHARES) );
    SHARES++;SHARES++;
    std::cout << "Target:" << std::endl;
    for (std::map<std::string, int>::const_iterator it = target.begin(); it != target.end(); it++) {
        std::cout << it->first << " " << it->second << std::endl;
    }
    return target;
}


void date_check(const boost::gregorian::date& today)
{
    // Check if all DB have data on the specified date
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
        if (DB::instance().get(*it).pDaily()->first != today)
            throw TraderException(*it + " does not have data on " + to_simple_string(today));
    }
}


void ETFTrader::daily_execution(std::map<std::string, int>& share_book,
                                std::map<std::string, int>& order_book,
                                const boost::gregorian::date& today)
{
    // Execute all orders in order_book, update share_book
    
    while (!order_book.empty())
    {
        std::string name = order_book.begin()->first;
        int target_share = order_book.begin()->second;
        int current_share = (share_book.find(name) == share_book.end()) ? 0 : share_book.find(name)->second;
        double open_price = DB::instance().get(name).pDaily()->second->open();
        
        if (target_share == current_share) {
            continue;
        }
        
        std::cout << name << " current " << current_share << " target " << target_share << std::endl;
        
        if (current_share < 0) {
            
            PositionSet ps = positions().open(name);
            assert(ps.size()==1);
            Position::ID id = (*ps.begin())->id();
            
            if (target_share < current_share) {
                std::cout << "short sell extra " << name << " " << current_share-target_share << " shares" << std::endl;
                sell_short(id, today, open_price, current_share-target_share);
            }
            else if (current_share < target_share && target_share < 0) {
                std::cout << "cover " << name << " " << target_share-current_share << " shares" << std::endl;
                cover(id, today, open_price, target_share-current_share);
            }
            else if (target_share == 0) {
                std::cout << "close " << name << std::endl;
                close(id, today, open_price);
            }
            else if (target_share > 0) {
                std::cout << "close " << name << std::endl;
                std::cout << "and buy new " << name << " " << target_share << " shares" << std::endl;
                close(id, today, open_price);
                buy(name, today, open_price, target_share);
            }
            else {
                throw TraderException("Order submission failed");
            }
            
            share_book.at(name) = target_share;
            
        }
        
        else if (current_share > 0) {
            
            PositionSet ps = positions().open(name);
            assert(ps.size()==1);
            Position::ID id = (*ps.begin())->id();
            
            if (current_share < target_share) {
                std::cout << "buy extra " << name << " " << target_share-current_share << " shares" << std::endl;
                buy(id, today, open_price, target_share-current_share);
            }
            else if (target_share < current_share && target_share > 0) {
                std::cout << "sell " << name << " " << current_share-target_share << " shares" << std::endl;
                sell(id, today, open_price, current_share-target_share);
            }
            else if (target_share == 0) {
                std::cout << "close " << name << std::endl;
                close(id, today, open_price);
            }
            else if (target_share < 0) {
                std::cout << "close " << name << std::endl;
                std::cout << "and sell short new " << name << " " << -target_share << " shares" << std::endl;
                close(id, today, open_price);
                sell_short(name, today, open_price, -target_share);
            }
            else {
                throw TraderException("Order submission failed");
            }
            
            share_book.at(name) = target_share;
            
        }
        
        else if (current_share == 0) {
            
            if (target_share < 0) {
                std::cout << "short sell new " << name << " " << -target_share << " shares" << std::endl;
                sell_short(name, today, open_price, -target_share);
            }
            else if (target_share > 0) {
                std::cout << "buy new " << name << " " << target_share << " shares" << std::endl;
                buy(name, today, open_price, target_share);
            }
            else {
                throw TraderException("Order submission failed");
            }
            
            share_book.insert(std::pair<std::string, int>(name, target_share));
            
        }
        
        else {
            throw TraderException("Order submission failed");
        }
        
        order_book.erase(name);
        
    }
    
    // Re-assure order_book is empty
    assert(order_book.empty());
}


void daily_settlement()
{
    // EOD balance check
    // EOD split & dividend recognition
    
}


void summary()
{
    
}


void ETFTrader::run() throw(TraderException)
{
    // Preprocess data (e.g. advance all data by a year)
    preprocess_data(1,0);
    
    // Retrieve date vector and their pointer
    // Pointers do not necessarily point the beginning of the vector
    const std::vector<boost::gregorian::date> dt_day = DB::instance().daily_dt();
    const std::vector<boost::gregorian::date> dt_mon = DB::instance().monthly_dt();
    std::vector<boost::gregorian::date>::const_iterator pDay = dt_day.begin();
    std::vector<boost::gregorian::date>::const_iterator pMon = dt_mon.begin();
    
    std::map<std::string, int> share_book, order_book;
    
    for ( ; pDay != dt_day.end(); pDay++) {
        boost::gregorian::date today = *pDay;
        
        // Check if all data have the correct date
        date_check(today);
        
        // Execute all orders from the previous day
        daily_execution(share_book, order_book, today);
        
        if ( today == *pMon && today != *(dt_day.rbegin()) ) {  // every EOM except the last day
            // Estimation (e.g. YoY return)
            std::map<double, std::string, std::greater<double> > est = monthly_estimation();
            
            // Based on the estimation, determine the target shares
            order_book = monthly_allocation(est);
            
            // Advance monthly pointers
            pMon++; DB::instance().monthly_advance();
        }
        
        daily_settlement();
        
        // Advance daily pointers
        DB::instance().daily_advance();
    }
    
    PositionSet p = positions();
    p.print();
    
    summary();
}