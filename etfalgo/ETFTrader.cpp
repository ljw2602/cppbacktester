//
//  ETFTrader.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ETFTrader.hpp"

//#define PRINT 1

ETFTrader::ETFTrader(const double initial, const TreasurySeries& ts): initial_capital(initial)
{
    rf = std::shared_ptr<TreasurySeries>(new TreasurySeries(ts));
}


void ETFTrader::preprocess_data(unsigned yr, unsigned mon)
{
    // Advance all data pointers by yr and mon
    // DB::advance() checks data is longer than requested duration
    
    DB::instance().advance(yr, mon);
    assert(mon < 12);
    std::cout << "Trading starts on " << *DB::instance().daily_dt().begin() << std::endl;
}


void ETFTrader::date_check(const boost::gregorian::date& today)
{
    // Check if all DB have data on the specified date
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
        if (DB::instance().get(*it).pDaily()->first != today)
            throw TraderException(*it + " does not have data on " + to_simple_string(today));
    }
}


double ETFTrader::daily_execution(std::map<std::string, int>& share_book,
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


double ETFTrader::rate_of_change(const std::string& name, unsigned yr, unsigned mon)
{
    // Calculates return over x year and y month using adjusted close price
    
    unsigned lookback_mon = yr*12 + mon;
    
    TA ta;
    PriceSeries series = DB::instance().get(name).monthly();
    PriceSeries::const_iterator pSeries = DB::instance().get(name).pMonthly();
    TA::ROCRes resROC = ta.ROC(series.adjclose(pSeries,lookback_mon+1), lookback_mon);
    
    return resROC.roc[0];
}

std::map<double, std::string, std::greater<double> > ETFTrader::rate_of_change_map(const unsigned lookback_yr, const unsigned lookback_mo)
{
    // Calculates one year return using adjusted close price for all symbols in DB
    
    std::map<double, std::string, std::greater<double> > ret;  // map with descending order
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ )
        ret.insert( std::pair<double, std::string>(rate_of_change(*it, lookback_yr, lookback_mo), *it) );
    
    return ret;
}

std::map<double, std::string, std::greater<double> > ETFTrader::monthly_estimation(const boost::gregorian::date& today, const unsigned lookback_yr, const unsigned lookback_mo)
{
    // Return estimation result
    
    std::map<double, std::string, std::greater<double> > est = rate_of_change_map(lookback_yr, lookback_mo);
#ifdef PRINT
    std::cout << "Return (%) over " << lookback_yr << " year " << lookback_mon << " month on " << today << std::endl;
    for (std::map<double, std::string>::const_iterator it = est.begin(); it != est.end(); it++) {
        std::cout << it->second << " " << it->first << std::endl;
    }
#endif
    return est;
}


double ETFTrader::get_rf(const boost::gregorian::date& today)
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


std::map<std::string, int> ETFTrader::monthly_allocation(const std::map<double, std::string, std::greater<double> >& est, double minimum, const boost::gregorian::date& today)
{
    // Allocate asset weight, based on the estimation()
    // Use Black-Litterman adjusted expected return
    // View1: first > last by 5% +/- 2% with 10% confidence
    // View2: first + second > last + secondtolast by 5% +/- 2% with 50% confidence
    
    // historical data are assumed to be fixed
    double ret_arr[] = {0.093231883,0.066463536,0.102390465,0.044440414,0.082293776,0.083130299,0.091628486,0.057314101,0.07350659};
    const std::vector<double> hist_return_(ret_arr, ret_arr + sizeof(ret_arr) / sizeof(ret_arr[0]));
    
    double std_arr[] = {0.186049202,0.121432041,0.218847865,0.218700927,0.141263328,0.186681638,0.220654344,0.243276828,0.151786818};
    const std::vector<double> hist_stdev_(std_arr, std_arr + sizeof(std_arr) / sizeof(std_arr[0]));
    
    double wt_arr[] = {0.1142,0.1036,0.142,0.1764,0.1346,0.0745,0.0261,0.1432,0.0854};
    const std::vector<double> market_cap_(wt_arr, wt_arr + sizeof(wt_arr) / sizeof(wt_arr[0]));
    
    double corr_arr[] = {1.000000,0.516009,0.450629,0.779473,0.692744,0.819249,0.751527,0.688865,0.328960,
        0.516009,1.000000,0.360485,0.568740,0.522199,0.545816,0.455247,0.293421,0.530845,
        0.450629,0.360485,1.000000,0.472460,0.408279,0.619838,0.662851,0.398163,0.431011,
        0.779473,0.568740,0.472460,1.000000,0.642018,0.789147,0.692757,0.511204,0.379559,
        0.692744,0.522199,0.408279,0.642018,1.000000,0.693931,0.621979,0.615501,0.391552,
        0.819249,0.545816,0.619838,0.789147,0.693931,1.000000,0.851001,0.653443,0.439556,
        0.751527,0.455247,0.662851,0.692757,0.621979,0.851001,1.000000,0.552163,0.375685,
        0.688865,0.293421,0.398163,0.511204,0.615501,0.653443,0.552163,1.000000,0.197714,
        0.328960,0.530845,0.431011,0.379559,0.391552,0.439556,0.375685,0.197714,1.000000};
    const std::vector<double> corr_(corr_arr, corr_arr + sizeof(corr_arr) / sizeof(corr_arr[0]));
    
    double v_arr[] = {0.05, 0.05};
    const std::vector<double> v_(v_arr, v_arr + sizeof(v_arr) / sizeof(v_arr[0]));
    
    double vrange_arr[] = {0.02, 0.02};
    const std::vector<double> v_range_(vrange_arr, vrange_arr + sizeof(vrange_arr) / sizeof(vrange_arr[0]));
    
    double vconf_arr[] = {0.1, 0.1};
    const std::vector<double> v_conf_(vconf_arr, vconf_arr + sizeof(vconf_arr) / sizeof(vconf_arr[0]));
    
    const std::string names_[] = {"XLY", "XLP", "XLE", "XLF", "XLV", "XLI", "XLB", "XLK", "XLU"};
    const std::vector<std::string> names(names_, names_ + sizeof(names_) / sizeof(names_[0]));
    
    ///// ^^ END OF FIXED INPUT ^^ /////
    
    std::map<double, std::string>::const_iterator it1 = est.begin();
    std::map<double, std::string>::const_reverse_iterator it2 = est.rbegin();
    
    std::map<std::string, int> target;
    for (int i = 0; i < names.size(); i++) {
        target.insert( std::pair<std::string, int>(names[i], 0) );
    }
    
    /*if (it1->first < minimum) {
        // If highest return is negative, close all positions
#ifdef PRINT
        std::cout << it1->first << " (highest ETF ret) < " << minimum << " (treasury ret): close all positions on " << today << std::endl;
#endif
        return target;
    }*/
    
    const std::string first = it1->second; it1++;
    const std::string second = it1->second;
    const std::string last = it2->second; it2++;
    const std::string secondtolast = it2->second;
    
    std::map<std::string, double> name_wt;
    for (int i = 0; i < names.size(); i++)
        name_wt.insert(std::pair<std::string, double>(names_[i], wt_arr[i]));
    
    std::vector<double> p_;
    for (int i = 0; i < names.size(); i++) {
        if (names[i] == first) p_.push_back(1);
        else if (names[i] == last) p_.push_back(-1);
        else p_.push_back(0);
    }
    for (int i = 0; i < names.size(); i++) {
        if (names[i] == first || names[i] == second) p_.push_back(name_wt.at(names[i])/(name_wt.at(first)+name_wt.at(second)));
        else if (names[i] == last || names[i] == secondtolast) p_.push_back(-name_wt.at(names[i])/(name_wt.at(last)+name_wt.at(secondtolast)));
        else p_.push_back(0);
    }
    
#ifdef PRINT
    std::cout << "P matrix on " << today << std::endl;
    for (int i = 0; i < p_.size(); i++) {
        std::cout << p_[i] << " ";
    } std::cout << std::endl;
#endif
    
    // Run Black-Litterman MV weight (gamma = 3, tau = 0.3 by default)
    std::vector<double> target_wt = bl_norm_weight(hist_return_, hist_stdev_, market_cap_, corr_,
                                                   p_,
                                                   v_, v_range_, v_conf_);
    
    // Allocate 80% of latest total balance to equities
    double total = BalanceSet::instance().latest_total();
    
    for (int i = 0; i < names.size(); i++) {
        double price = DB::instance().get(names[i]).pMonthly()->second->close();
        int target_share = target_wt[i] * (0.8*total) / price;
        target.at(names[i]) = target_share;
    }
    
#ifdef PRINT
    std::cout << "Allocated number of shares on " << today << std::endl;
    for (std::map<std::string, int>::const_iterator it = target.begin(); it != target.end(); it++) {
        std::cout << it->first << " " << it->second << std::endl;
    }
#endif
    
    return target;
}


void ETFTrader::daily_settlement(std::map<std::string, int>& share_book,
                      const double netcash,
                      const boost::gregorian::date& today)
{
    // EOD split & dividend recognition
    // EOD balance check
    
    double netdividend = BalanceSet::instance().split_and_dividend(share_book, today);
    BalanceSet::instance().update_capital(share_book, netcash + netdividend, today);
}


void ETFTrader::run() throw(TraderException)
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
    
//    summary();
}