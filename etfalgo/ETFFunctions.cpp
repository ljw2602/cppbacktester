//
//  ETFFunctions.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/26/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ETFFunctions.hpp"

//#define PRINT 1


void preprocess_data(unsigned yr, unsigned mon)
{
    // Advance all data pointers by yr and mon
    // DB::advance() checks data is longer than requested duration
    
    DB::instance().advance(yr, mon);
    assert(mon < 12);
    std::cout << "Trading starts on " << *DB::instance().daily_dt().begin() << std::endl;
}


double rate_of_change(const std::string& name, unsigned yr, unsigned mon)
{
    // Calculates return over x year and y month using adjusted close price
    
    unsigned lookback_mon = yr*12 + mon;
    
    TA ta;
    PriceSeries series = DB::instance().get(name).monthly();
    PriceSeries::const_iterator pSeries = DB::instance().get(name).pMonthly();
    TA::ROCRes resROC = ta.ROC(series.adjclose(pSeries,lookback_mon+1), lookback_mon);
    
    return resROC.roc[0];
}

std::map<double, std::string, std::greater<double> > rate_of_change_map(const unsigned lookback_yr, const unsigned lookback_mo)
{
    // Calculates one year return using adjusted close price for all symbols in DB
    
    std::map<double, std::string, std::greater<double> > ret;  // map with descending order
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ )
        ret.insert( std::pair<double, std::string>(rate_of_change(*it, lookback_yr, lookback_mo), *it) );
    
    return ret;
}

std::map<double, std::string, std::greater<double> > monthly_estimation(const boost::gregorian::date& today, const unsigned lookback_yr, const unsigned lookback_mo)
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


std::map<std::string, int> monthly_allocation(const std::map<double, std::string, std::greater<double> >& est, double minimum, const boost::gregorian::date& today)
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
    
    if (it1->first < minimum) {
        // If highest return is negative, close all positions
#ifdef PRINT
        std::cout << it1->first << " (highest ETF ret) < " << minimum << " (treasury ret): close all positions on " << today << std::endl;
#endif
        return target;
    }
    
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

void daily_settlement(std::map<std::string, int>& share_book,
                      const double netcash,
                      const boost::gregorian::date& today)
{
    // EOD split & dividend recognition
    // EOD balance check
    
    double netdividend = BalanceSet::instance().split_and_dividend(share_book, today);
    BalanceSet::instance().update_capital(share_book, netcash + netdividend, today);
}


void summary()
{
}