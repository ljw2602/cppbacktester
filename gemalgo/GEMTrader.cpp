//
//  GEMTrader.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "GEMTrader.hpp"

//#define PRINT 1


GEMTrader::GEMTrader(const double initial, const TreasurySeries& ts): initial_capital(initial)
{
    rf = std::shared_ptr<TreasurySeries>(new TreasurySeries(ts));
}


void GEMTrader::date_check(const boost::gregorian::date& today)
{
    // Check if all DB have data on the specified date
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
        if (DB::instance().get(*it).pDaily()->first != today)
            throw TraderException(*it + " does not have data on " + to_simple_string(today));
    }
}


double GEMTrader::daily_execution(std::map<std::string, int>& share_book,
                                  std::map<std::string, int>& order_book,
                                  const boost::gregorian::date& today)
{
    // Execute all orders in order_book, update share_book
    // Return cash settlement
    
    double netcash = 0.0;
    
    while (!order_book.empty())
    {
        std::string name = order_book.begin()->first;
        int target_share = order_book.begin()->second;
        int current_share = (share_book.find(name) == share_book.end()) ? 0 : share_book.find(name)->second;
        double open_price = DB::instance().get(name).pDaily()->second->open();
        
        if (target_share == current_share) {
            order_book.erase(name);
            continue;
        }
        
#ifdef PRINT
        std::cout << name << " current " << current_share << " target " << target_share << std::endl;
#endif
        
        if (current_share < 0) {
            
            PositionSet ps = positions().open(name);
            assert(ps.size()==1);
            Position::ID id = (*ps.begin())->id();
            
            if (target_share < current_share) {
#ifdef PRINT
                std::cout << "short sell extra " << name << " " << current_share-target_share << " shares" << std::endl;
#endif
                netcash += sell_short(id, today, open_price, current_share-target_share);
            }
            else if (current_share < target_share && target_share < 0) {
#ifdef PRINT
                std::cout << "cover " << name << " " << target_share-current_share << " shares" << std::endl;
#endif
                netcash += cover(id, today, open_price, target_share-current_share);
            }
            else if (target_share == 0) {
#ifdef PRINT
                std::cout << "close " << name << std::endl;
#endif
                netcash += close(id, today, open_price);
            }
            else if (target_share > 0) {
#ifdef PRINT
                std::cout << "close " << name << std::endl;
                std::cout << "and buy new " << name << " " << target_share << " shares" << std::endl;
#endif
                netcash += close(id, today, open_price);
                netcash += buy(name, today, open_price, target_share);
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
#ifdef PRINT
                std::cout << "buy extra " << name << " " << target_share-current_share << " shares" << std::endl;
#endif
                netcash += buy(id, today, open_price, target_share-current_share);
            }
            else if (target_share < current_share && target_share > 0) {
#ifdef PRINT
                std::cout << "sell " << name << " " << current_share-target_share << " shares" << std::endl;
#endif
                netcash += sell(id, today, open_price, current_share-target_share);
            }
            else if (target_share == 0) {
#ifdef PRINT
                std::cout << "close " << name << std::endl;
#endif
                netcash += close(id, today, open_price);
            }
            else if (target_share < 0) {
#ifdef PRINT
                std::cout << "close " << name << std::endl;
                std::cout << "and sell short new " << name << " " << -target_share << " shares" << std::endl;
#endif
                netcash += close(id, today, open_price);
                netcash += sell_short(name, today, open_price, -target_share);
            }
            else {
                throw TraderException("Order submission failed");
            }
            
            share_book.at(name) = target_share;
            
        }
        
        else if (current_share == 0) {
            
            if (target_share < 0) {
#ifdef PRINT
                std::cout << "short sell new " << name << " " << -target_share << " shares" << std::endl;
#endif
                netcash += sell_short(name, today, open_price, -target_share);
            }
            else if (target_share > 0) {
#ifdef PRINT
                std::cout << "buy new " << name << " " << target_share << " shares" << std::endl;
#endif
                netcash += buy(name, today, open_price, target_share);
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
        if (target_share == 0) share_book.erase(name);
        
    }
    
    // Re-assure order_book is empty
    assert(order_book.empty());
    
    return netcash;
}


double GEMTrader::get_rf(const boost::gregorian::date& today)
{
    boost::gregorian::date rf_day = today;
    bool rolled = false;
    double minimum = 0.0;
    
    while (rf->find(rf_day) == rf->end()){
        rf_day -= boost::gregorian::days(1);
        rolled = true;
    }
    minimum = rf->find(rf_day)->second->ret();

#ifdef PRINT
    std::cout << "Rf\t" << minimum << std::endl;
    if (rf_day < today) {
        std::cout << "Rf of " << rf_day << " is used instead of the one of " << today << std::endl;
    }
#endif
    
    return minimum;
}


void GEMTrader::run() throw(TraderException)
{
    // Preprocess data (e.g. advance all data by x year and y month)
    unsigned adv_yr = 1, adv_mo = 0;
    preprocess_data(adv_yr, adv_mo);
    
    // Retrieve date vector and their pointer
    // Pointers do not necessarily point the beginning of the vector
    const std::vector<boost::gregorian::date> dt_day = DB::instance().daily_dt();
    const std::vector<boost::gregorian::date> dt_mon = DB::instance().monthly_dt();
    std::vector<boost::gregorian::date>::const_iterator pDay = dt_day.begin();
    std::vector<boost::gregorian::date>::const_iterator pMon = dt_mon.begin();
    
    // Initialize balance
    BalanceSet::instance().initialize(*pDay, initial_capital, DB::instance().dblist());
    first_trading_day = *pDay;
    
    // Declare share book (track current shares) and order book (carry orders)
    // They only carry currently held assets (if order is executed or held share is zero, they are erased)
    std::map<std::string, int> share_book, order_book;
    
    for ( ; pDay != dt_day.end(); pDay++)
    {
        try
        {
            boost::gregorian::date today = *pDay;
#ifdef PRINT
            std::cout << std::endl << today << std::endl;
#endif
            // Check if all data have the correct date
            date_check(today);
            
            // Execute all orders from the previous day
            double netcash = daily_execution(share_book, order_book, today);
            
            if ( today == *pMon && today != *(dt_day.rbegin()) )  // every EOM except the last day
            {
                // Estimation (e.g. YoY return)
                // Lookback period for the estimation = data advancement period
                std::map<double, std::string, std::greater<double> > est = monthly_estimation(today, adv_yr, adv_mo);
                
                // Based on the estimation, determine the target shares
                double minimum = get_rf(today);
                order_book = monthly_allocation(est, minimum, today);
                
                // Advance monthly pointers
                pMon++; DB::instance().monthly_advance();
            }
            
            // Update BalanceSet with EOD price (no need to do this on the first trading day)
            if (today != first_trading_day)
                daily_settlement(share_book, netcash, today);
            
            // Advance daily pointers
            DB::instance().daily_advance();
            
        } catch( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            continue;
        }
    }
    
    PositionSet p = positions();
#ifdef PRINT
    p.print();
#endif
    
    summary();
}