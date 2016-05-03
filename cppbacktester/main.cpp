//
//  main.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/7/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <numeric>
/*
#include "FileDriver.hpp"
#include "Series.hpp"
#include "PriceSeries.hpp"
#include "EquitySeries.hpp"
#include "TreasurySeries.hpp"
#include "DB.hpp"
#include "Asset.hpp"
#include "Execution.hpp"
#include "ExecutionSet.hpp"
#include "LongPosition.hpp"
#include "ShortPosition.hpp"
#include "PositionSet.hpp"
#include "BalanceSet.hpp"
#include "BlackLitterman.hpp"*/
#include "ETFTrader.hpp"
#include "GEMTrader.hpp"

using namespace boost::gregorian;
using namespace std;
/*
void daily();
void weekly();
void monthly();
void database();
void database_offset();
void execution();
void executionset();
void position();
void positionset();
void etftrader();
void etftrader_offset();
void etftrader_run();
void balance();
void action();
void blacklitterman();
*/
void etftrader_temp_final();
void gemtrader_test();

//#define PRINT 1

int main() {
    
//    gemtrader_test();
    etftrader_temp_final();
    
//    daily();
//    weekly();
//    monthly();
//    database();
//    database_offset();
//    execution();
//    executionset();
//    position();
//    positionset();
//    etftrader();
//    etftrader_offset();
//    etftrader_run();
//    blacklitterman();
    
    return 0;
    
}


void gemtrader_test() {
    FileDriver fd;
    
    date load_begin(from_simple_string(string("1972-12-31")));
    date load_end(from_simple_string(string("2015-12-31")));
    
    const string filepath("/Users/jeongwon/Documents/GitHub/cppbacktester/db/GEM/");
    const string benchpath("/Users/jeongwon/Documents/GitHub/cppbacktester/db/Benchmark/");
    
    const string acwi_symbol("ACWIexUS");
    const string acwi_price(filepath+"ACWIexUS_daily.csv");
    const string acwi_action(filepath+"ACWIexUS_actions.csv");
    
    const string sp500_symbol("SP500TR");
    const string sp500_price(filepath+"SP500TR_daily.csv");
    const string sp500_action(filepath+"SP500TR_actions.csv");
    
    const string bond_symbol("USBond");
    const string bond_price(filepath+"USBond_daily.csv");
    const string bond_action(filepath+"USBond_actions.csv");
    
    const string treas_symbol("TREASURY");
    const string treas_file(benchpath+"treasury_benchmark.csv");
    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
    
    // load function advances the data to EOM
    DB::instance().load(acwi_symbol, acwi_price, acwi_action, fd, load_begin, load_end);
    DB::instance().load(sp500_symbol, sp500_price, sp500_action, fd, load_begin, load_end);
    DB::instance().load(bond_symbol, bond_price, bond_action, fd, load_begin, load_end);
    
    GEMTrader one(100000, ts);
    one.set_bond(bond_symbol);
    one.run();  // this function advances the data*/
    
#ifdef PRINT
    BalanceSet::instance().print();
#endif
    BalanceSet::instance().export_to_csv("GEM");
}

void etftrader_temp_final(){
    FileDriver fd;
    
    date load_begin(from_simple_string(string("1999-12-20")));
    date load_end(from_simple_string(string("2016-4-20")));
    
    const string filepath("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/");
    const string benchpath("/Users/jeongwon/Documents/GitHub/cppbacktester/db/Benchmark/");
    
    const string xlu_symbol("XLU");
    const string xlu_price(filepath+"XLU.csv");
    const string xlu_action(filepath+"XLU_actions.csv");
    
    const string xlb_symbol("XLB");
    const string xlb_price(filepath+"XLB.csv");
    const string xlb_action(filepath+"XLB_actions.csv");
    
    const string xli_symbol("XLI");
    const string xli_price(filepath+"XLI.csv");
    const string xli_action(filepath+"XLI_actions.csv");
    
    const string xle_symbol("XLE");
    const string xle_price(filepath+"XLE.csv");
    const string xle_action(filepath+"XLE_actions.csv");
    
    const string xly_symbol("XLY");
    const string xly_price(filepath+"XLY.csv");
    const string xly_action(filepath+"XLY_actions.csv");
    
    const string xlp_symbol("XLP");
    const string xlp_price(filepath+"XLP.csv");
    const string xlp_action(filepath+"XLP_actions.csv");
    
    const string xlv_symbol("XLV");
    const string xlv_price(filepath+"XLV.csv");
    const string xlv_action(filepath+"XLV_actions.csv");
    
    const string xlk_symbol("XLK");
    const string xlk_price(filepath+"XLK.csv");
    const string xlk_action(filepath+"XLK_actions.csv");
    
    const string xlf_symbol("XLF");
    const string xlf_price(filepath+"XLF.csv");
    const string xlf_action(filepath+"XLF_actions.csv");
    
    const string bond_symbol("USBond");
    const string bond_price(filepath+"USBond_daily.csv");
    const string bond_action(filepath+"USBond_actions.csv");

    const string treas_symbol("TREASURY");
    const string treas_file(benchpath+"treasury_benchmark.csv");
    TreasurySeries ts("3MO", fd, treas_file, load_begin, load_end);
    
    // load function advances the data to EOM
    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
    DB::instance().load(xlb_symbol, xlb_price, xlb_action, fd, load_begin, load_end);
    DB::instance().load(xli_symbol, xli_price, xli_action, fd, load_begin, load_end);
    DB::instance().load(xle_symbol, xle_price, xle_action, fd, load_begin, load_end);
    DB::instance().load(xly_symbol, xly_price, xly_action, fd, load_begin, load_end);
    DB::instance().load(xlp_symbol, xlp_price, xlp_action, fd, load_begin, load_end);
    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
    DB::instance().load(xlk_symbol, xlk_price, xlk_action, fd, load_begin, load_end);
    DB::instance().load(xlf_symbol, xlf_price, xlf_action, fd, load_begin, load_end);
//    DB::instance().load(bond_symbol, bond_price, bond_action, fd, load_begin, load_end);
    
    ETFTrader one(100000, ts);
    one.set_bond(bond_symbol);
    one.run();  // this function advances the data*/
    
#ifdef PRINT
    BalanceSet::instance().print();
#endif
    BalanceSet::instance().export_to_csv("ETF");
}


//void blacklitterman() {
//    double _hist_return[] = {0.0832,0.1514,0.0731,0.1225,0.0656,0.0524,0.118,0.1492,0.1301,0.1047,0.1345,0.1743,0.1463,0.2283,0.0949,0.2595,0.1899,0.1177};
//    std::vector<double> hist_return_(_hist_return, _hist_return + sizeof(_hist_return)/sizeof(double));
//    
//    double _hist_stdev[] = {0.2509,0.2121,0.2356,0.2081,0.1892,0.1994,0.1666,0.2072,0.2091,0.1572,0.1935,0.1968,0.2517,0.162,0.1616,0.286,0.2569,0.1725};
//    std::vector<double> hist_stdev_(_hist_stdev, _hist_stdev + sizeof(_hist_stdev)/sizeof(double));
//    
//    double _market_cap[] = {0.0165,0.1504,0.0122,0.018,0.0126,0.0285,0.029,0.103,0.0412,0.0459,0.0519,0.0689,0.0327,0.1024,0.0227,0.1103,0.1056,0.0483};
//    std::vector<double> market_cap_(_market_cap, _market_cap + sizeof(_market_cap)/sizeof(double));
//    
//    double _corr[] = {1,0.74,0.73,0.83,0.78,0.75,0.73,0.55,0.73,0.71,0.79,0.72,0.46,0.43,0.68,0.69,0.65,0.64,
//        0.74,1,0.63,0.73,0.74,0.75,0.71,0.59,0.92,0.75,0.75,0.87,0.39,0.63,0.62,0.67,0.59,0.64,
//        0.73,0.63,1,0.83,0.81,0.78,0.6,0.66,0.69,0.56,0.78,0.56,0.44,0.31,0.59,0.62,0.52,0.41,
//        0.83,0.73,0.83,1,0.85,0.82,0.72,0.67,0.72,0.74,0.82,0.7,0.51,0.45,0.69,0.65,0.57,0.57,
//        0.78,0.74,0.81,0.85,1,0.9,0.72,0.66,0.75,0.76,0.89,0.64,0.54,0.39,0.67,0.66,0.63,0.64,
//        0.75,0.75,0.78,0.82,0.9,1,0.67,0.64,0.79,0.7,0.87,0.63,0.58,0.43,0.67,0.7,0.63,0.56,
//        0.73,0.71,0.6,0.72,0.72,0.67,1,0.55,0.69,0.75,0.69,0.74,0.41,0.58,0.73,0.53,0.59,0.71,
//        0.55,0.59,0.66,0.67,0.66,0.64,0.55,1,0.59,0.59,0.59,0.54,0.28,0.43,0.55,0.43,0.32,0.46,
//        0.73,0.92,0.69,0.72,0.75,0.79,0.69,0.59,1,0.75,0.73,0.85,0.39,0.61,0.58,0.64,0.57,0.58,
//        0.71,0.75,0.56,0.74,0.76,0.7,0.75,0.59,0.75,1,0.62,0.74,0.27,0.63,0.61,0.4,0.41,0.66,
//        0.79,0.75,0.78,0.82,0.89,0.87,0.69,0.59,0.73,0.62,1,0.65,0.72,0.38,0.68,0.82,0.77,0.67,
//        0.72,0.87,0.56,0.7,0.64,0.63,0.74,0.54,0.85,0.74,0.65,1,0.36,0.67,0.61,0.6,0.56,0.68,
//        0.46,0.39,0.44,0.51,0.54,0.58,0.41,0.28,0.39,0.27,0.72,0.36,1,0.21,0.42,0.75,0.77,0.57,
//        0.43,0.63,0.31,0.45,0.39,0.43,0.58,0.43,0.61,0.63,0.38,0.67,0.21,1,0.43,0.35,0.37,0.58,
//        0.68,0.62,0.59,0.69,0.67,0.67,0.73,0.55,0.58,0.61,0.68,0.61,0.42,0.43,1,0.52,0.53,0.57,
//        0.69,0.67,0.62,0.65,0.66,0.7,0.53,0.43,0.64,0.4,0.82,0.6,0.75,0.35,0.52,1,0.81,0.55,
//        0.65,0.59,0.52,0.57,0.63,0.63,0.59,0.32,0.57,0.41,0.77,0.56,0.77,0.37,0.53,0.81,1,0.7,
//        0.64,0.64,0.41,0.57,0.64,0.56,0.71,0.46,0.58,0.66,0.67,0.68,0.57,0.58,0.57,0.55,0.7,1};
//    std::vector<double> corr_(_corr, _corr + sizeof(_corr)/sizeof(double));
//    
//    double _p[] = {0,0,0,0,0,0,0,0,0,0,0.0519/(0.0519+0.1024),0,0,0.1024/(0.0519+0.1024),0,-0.1103/(0.1103+0.1056),-0.1056/(0.1103+0.1056),0,
//        0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0};
//    std::vector<double> p_(_p, _p + sizeof(_p)/sizeof(double));
//    
//    double _v[] = {0.03, 0.075};
//    std::vector<double> v_(_v, _v + sizeof(_v)/sizeof(double));
//    
//    double _v_range[] = {0.01, 0.015};
//    std::vector<double> v_range_(_v_range, _v_range + sizeof(_v_range)/sizeof(double));
//    
//    double _v_conf[] = {0.9, 0.9};
//    std::vector<double> v_conf_(_v_conf, _v_conf + sizeof(_v_conf)/sizeof(double));
//    
//    double gamma = 3;
//    
//    double tau = 0.3;
//
//    std::vector<double> norm_weight = bl_norm_weight(hist_return_, hist_stdev_, market_cap_, corr_, p_, v_, v_range_, v_conf_, gamma, tau);
//    for (int i = 0; i < norm_weight.size(); i++)
//        printf("%5.2f\n", norm_weight[i]*100);
//    std::cout << std::accumulate(norm_weight.begin(), norm_weight.end(), 0.0) << std::endl;
//}
//
//void action() {
//    FileDriver fd;
//    date load_begin(from_simple_string(string("2012-12-17")));
//    date load_end(from_simple_string(string("2014-4-20")));
//    
//    const string xlu_symbol("XLU");
//    const string xlu_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU.csv");
//    const string xlu_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//    
//    const string xlv_symbol("XLV");
//    const string xlv_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV.csv");
//    const string xlv_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV_actions.csv");
//    
//    const string xlf_symbol("XLF");
//    const string xlf_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLF.csv");
//    const string xlf_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLF_actions.csv");
//    
//    const string nflx_symbol("GOOG");
//    const string nflx_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/FANG/GOOG.csv");
//    const string nflx_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/FANG/GOOG_actions.csv");
//    
//    // load function advances the data to EOM
//    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
//    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
//    DB::instance().load(xlf_symbol, xlf_price, xlf_action, fd, load_begin, load_end);
//    DB::instance().load(nflx_symbol, nflx_price, nflx_action, fd, load_begin, load_end);
//    
//    ETFTrader one(500000);
//    one.run();  // this function advances the data*/
//    
//    BalanceSet::instance().print();
//    BalanceSet::instance().export_to_csv("ETF");
//}
//
//void balance() {
//    FileDriver fd;
//    date load_begin(from_simple_string(string("2005-2-17")));
//    date load_end(from_simple_string(string("2006-5-20")));
//    
//    const string xlu_symbol("XLU");
//    const string xlu_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU.csv");
//    const string xlu_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//    
//    const string xlv_symbol("XLV");
//    const string xlv_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV.csv");
//    const string xlv_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV_actions.csv");
//    
//    // load function advances the data to EOM
//    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
//    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
//    
//    date today(from_simple_string(string("2006-4-7")));
//    date tmr(from_simple_string(string("2006-4-11")));
//    
//    std::vector<string> names;
//    names.push_back("XLU"); names.push_back("XLV");
//    
//    BalanceSet::instance().initialize(today, 1000, names);
//    
//    std::map<std::string, int> share_book;
//    share_book.insert( std::pair<std::string, int>("XLU", 3) );
//    share_book.insert( std::pair<std::string, int>("XLV", -1) );
//    
//    BalanceSet::instance().update_capital(share_book, 10, tmr);
//    
//    BalanceSet::instance().print();
//    BalanceSet::instance().export_to_csv("ETF");
//}
//
//void etftrader_run() {
//    FileDriver fd;
//    date load_begin(from_simple_string(string("2005-2-17")));
//    date load_end(from_simple_string(string("2006-5-20")));
//    
//    const string xlu_symbol("XLU");
//    const string xlu_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU.csv");
//    const string xlu_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//    
//    const string xlv_symbol("XLV");
//    const string xlv_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV.csv");
//    const string xlv_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV_actions.csv");
//    
//    // load function advances the data to EOM
//    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
//    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
//    
//    
//    ETFTrader one(1);
//    one.run();  // this function advances the data
//}
//
//void etftrader_offset() {
//    FileDriver fd;
//    date load_begin(from_simple_string(string("2005-2-17")));
//    date load_end(from_simple_string(string("2006-5-20")));
//    
//    const string xlu_symbol("XLU");
//    const string xlu_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU.csv");
//    const string xlu_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//    
//    const string xlv_symbol("XLV");
//    const string xlv_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV.csv");
//    const string xlv_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV_actions.csv");
//    
//    // load function advances the data to EOM
//    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
//    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
//    
//    std::vector<std::string> symbols = DB::instance().dblist();
//    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
//        cout << *it << endl;
//        
//        //PriceSeries ds = DB::instance().get(*it).daily();
//        PriceSeries::const_iterator pDaily = DB::instance().get(*it).pDaily();
//        cout << "Daily Price" << endl;
//        for (; pDaily != DB::instance().get(*it).daily().end(); pDaily++) {
//            cout << pDaily->first << "\t" << pDaily->second->close() << endl;
//        }
//        
//        cout << "Monthly Price" << endl;
//        PriceSeries::const_iterator pMonthly = DB::instance().get(*it).pMonthly();
//        for (; pMonthly != DB::instance().get(*it).monthly().end(); pMonthly++) {
//            cout << pMonthly->first << "\t" << pMonthly->second->close() << endl;
//        }
//        
//        cout << "Action Series" << endl;
//        ActionSeries::const_iterator pAction = DB::instance().get(*it).pAction();
//        for (; pAction != DB::instance().get(*it).action().end(); pAction++) {
//            cout << pAction->first << "\t" << pAction->second->ratio() << endl;
//        }
//    }
//}
//
//void etftrader() {
//    date begin(from_simple_string(string("2005-2-17")));
//    date end(from_simple_string(string("2005-3-11")));
//    
//    ETFTrader one(1);
//    unsigned long id = one.buy("XYZ", begin, 34.12, 3);
//    one.buy(id, begin, 41.32, 1);
//    one.sell(id, begin, 63.18);
//    one.close(id, end, 53.12);
//    one.print(id);
//    
//    ETFTrader two(1);
//    unsigned long id2 = two.sell_short("ABC", begin, 34.12, 3);
//    two.sell_short(id2, begin, 41.32, 1);
//    two.cover(id2, begin, 63.18);
//    two.close(id2, end, 53.12);
//    two.print(id2);
//
//}
//
//void positionset() {
//    date begin(from_simple_string(string("2005-2-17")));
//    
//    PositionSet pset;
//    PositionPtr longpos(new LongPosition(1, "XYZ", begin, 34.52, 2));
//    PositionPtr shortpos(new ShortPosition(2, "XLV", begin, 34.52, 2));
//    
//    pset.insert(longpos);
//    pset.insert(shortpos);
//    
//    PositionSet::const_iterator it = pset.find(1, pos_comp_id());
//    if (it != pset.end()) {
//        (*it)->buy(date(from_simple_string(string("2005-3-11"))), 42.77, 1);
//        (*it)->sell(date(from_simple_string(string("2005-3-14"))), 49.77, 1);
//        (*it)->close(date(from_simple_string(string("2005-3-17"))), 63.88);
//    }
//    
//    it = pset.find(2, pos_comp_id());
//    if (it != pset.end()) {
//        (*it)->sell_short(date(from_simple_string(string("2005-3-12"))), 142.77, 1);
//        (*it)->sell_short(date(from_simple_string(string("2005-3-19"))), 134.77, 1);
//        (*it)->cover(date(from_simple_string(string("2005-3-18"))), 134.77, 1);
//        (*it)->close(date(from_simple_string(string("2005-3-22"))), 120.77);
//    }
//    
//    pset.print();
//}
//
//void position() {
//    date begin(from_simple_string(string("2005-2-17")));
//    
//    LongPosition longpos(1, "XYZ", begin, 34.52, 2);
//    longpos.buy(date(from_simple_string(string("2005-3-11"))), 42.77, 1);
//    longpos.sell(date(from_simple_string(string("2005-3-14"))), 49.77, 1);
//    longpos.close(date(from_simple_string(string("2005-3-17"))), 63.88);
//    
//    ShortPosition shortpos(2, "XLV", begin, 34.52, 1);
//    shortpos.sell_short(date(from_simple_string(string("2005-3-12"))), 142.77, 2);
//    shortpos.sell_short(date(from_simple_string(string("2005-3-19"))), 134.77, 1);
//    shortpos.cover(date(from_simple_string(string("2005-3-18"))), 134.77, 1);
//    shortpos.close(date(from_simple_string(string("2005-3-22"))), 120.77);
//    
//    longpos.print();
//    shortpos.print();
//}
//
//void execution() {
//    date load_begin(from_simple_string(string("2005-2-17")));
//    boost::shared_ptr<Execution> buy(new BuyExecution("XYZ", Execution::NullID, load_begin, 34.34, 3));
//    boost::shared_ptr<Execution> sell( new SellExecution("XYZ", Execution::NullID, load_begin, 23.39, 2));
//    boost::shared_ptr<Execution> sshort(new SellShortExecution("XYZ", Execution::NullID, load_begin, 31.64, 5));
//    boost::shared_ptr<Execution> cover(new CoverExecution("XYZ", Execution::NullID, load_begin, 38.34, 1));
//}
//
//void executionset() {
//    date load_begin(from_simple_string(string("2005-2-17")));
//
//    ExecutionSet exset;
//    exset.buy("XYZ", load_begin, 34.34, 3);
//    exset.sell("XYZ", load_begin, 23.39, 5);
//    exset.sell_short("XYZ", load_begin, 31.64, 1);
//    exset.cover("XYZ", load_begin, 37.41, 1);
//    
//    exset.print();
//}
//
//void database_offset() {
//    FileDriver fd;
//    date load_begin(from_simple_string(string("2005-2-17")));
//    date load_end(from_simple_string(string("2006-5-20")));
//    
//    const string xlu_symbol("XLU");
//    const string xlu_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU.csv");
//    const string xlu_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//
//    const string xlv_symbol("XLV");
//    const string xlv_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV.csv");
//    const string xlv_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLV_actions.csv");
//
//    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
//    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
//    
//    DB::instance().advance(1,2);  // advance 1 year and 2 months
//    
//    std::vector<std::string> symbols = DB::instance().dblist();
//    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
//        cout << *it << endl;
//        
//        PriceSeries ds = DB::instance().get(*it).daily();
//        PriceSeries::const_iterator pDaily = DB::instance().get(*it).pDaily();
//        cout << "Daily Price" << endl;
//        for (; pDaily != DB::instance().get(*it).daily().end(); pDaily++) {
//            cout << pDaily->first << "\t" << pDaily->second->close() << endl;
//        }
//        
//        cout << "Monthly Price" << endl;
//        PriceSeries::const_iterator pMonthly = DB::instance().get(*it).pMonthly();
//        for (; pMonthly != DB::instance().get(*it).monthly().end(); pMonthly++) {
//            cout << pMonthly->first << "\t" << pMonthly->second->close() << endl;
//        }
//        
//        cout << "Action Series" << endl;
//        ActionSeries::const_iterator pAction = DB::instance().get(*it).pAction();
//        for (; pAction != DB::instance().get(*it).action().end(); pAction++) {
//            cout << pAction->first << "\t" << pAction->second->ratio() << endl;
//        }
//    }
//    
//}
//
//void database() {
//    FileDriver fd;
//    date load_begin(from_simple_string(string("2005-3-17")));
//    date load_end(from_simple_string(string("2005-4-5")));
//    
//    const string xlu_symbol("SPX");
//    const string xlu_price("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU.csv");
//    const string xlu_action("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//    
//    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
//    
//    PriceSeries ds = DB::instance().get(xlu_symbol).daily();
//    cout << "Daily Price" << endl;
//    for (PriceSeries::TS::const_iterator it = ds.begin(); it != ds.end(); it++) {
//        cout << it->first << "\t" << it->second->close() << endl;
//    }
//    
//    PriceSeries ms = DB::instance().get(xlu_symbol).monthly();
//    cout << "Monthly Price" << endl;
//    for (PriceSeries::TS::const_iterator it = ms.begin(); it != ms.end(); it++) {
//        cout << it->first << "\t" << it->second->close() << endl;
//    }
//    
//    ActionSeries as = DB::instance().get(xlu_symbol).action();
//    cout << "Action Series" << endl;
//    for (ActionSeries::TS::const_iterator it = as.begin(); it != as.end(); it++) {
//        cout << it->first << "\t" << it->second->ratio() << endl;
//    }
//}
//
//void daily() {
//    date load_begin(from_simple_string(string("2005-3-15")));
//    date load_end(from_simple_string(string("2005-3-30")));
//    
//    const string spx_symbol("SPX");
//    const string spx_file("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/^GSPC.csv");
//    
//    FileDriver fd;
//    PriceSeries ps(fd, spx_file, load_begin, load_end);
//    cout << "Daily Price" << endl;
//    for (PriceSeries::TS::const_iterator it = ps.begin(); it != ps.end(); it++) {
//        cout << it->first << "\t" << it->second->close() << endl;
//    }
//    
//    const string act_symbol("ACT");
//    const string act_file("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/XLU_actions.csv");
//    
//    ActionSeries as(fd, act_file, load_begin, load_end);
//    cout << "Action" << endl;
//    for (ActionSeries::TS::const_iterator it = as.begin(); it != as.end(); it++) {
//        cout << it->first << "\t" << it->second->ratio() << endl;
//    }
//    
//    const string equity_symbol("EQUITY");
//    const string equity_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/equity_benchmark.csv");
//    
//    EquitySeries es(fd, equity_file, load_begin, load_end);
//    cout << "Daily equity return" << endl;
//    for (EquitySeries::TS::const_iterator it = es.begin(); it != es.end(); it++) {
//        cout << it->first << "\t" << it->second->ret() << endl;
//    }
//    
//    const string treas_symbol("TREASURY");
//    const string treas_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/treasury_benchmark.csv");
//    
//    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
//    cout << "Daily treasury return" << endl;
//    for (TreasurySeries::TS::const_iterator it = ts.begin(); it != ts.end(); it++) {
//        cout << it->first << "\t" << it->second->ret() << endl;
//    }
//}
//
//void weekly() {
//    date load_begin(from_simple_string(string("2005-3-15")));
//    date load_end(from_simple_string(string("2005-4-10")));
//    
//    const string spx_symbol("SPX");
//    const string spx_file("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/^GSPC.csv");
//    
//    FileDriver fd;
//    PriceSeries ps(fd, spx_file, load_begin, load_end);
//    std::shared_ptr<PriceSeries> ps_mon = ps.weekly();
//    cout << "Weekly Price" << endl;
//    for (PriceSeries::TS::const_iterator it = ps_mon->begin(); it != ps_mon->end(); it++) {
//        cout << it->first << "\t" << it->second->close() << endl;
//    }
//    
//    const string equity_symbol("EQUITY");
//    const string equity_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/equity_benchmark.csv");
//    
//    EquitySeries es(fd, equity_file, load_begin, load_end);
//    std::shared_ptr<EquitySeries> es_mon = es.weekly();
//    cout << "Weekly equity return" << endl;
//    for (EquitySeries::TS::const_iterator it = es_mon->begin(); it != es_mon->end(); it++) {
//        cout << it->first << "\t" << it->second->ret() << endl;
//    }
//    
//    const string treas_symbol("TREASURY");
//    const string treas_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/treasury_benchmark.csv");
//    
//    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
//    std::shared_ptr<TreasurySeries> ts_mon = ts.weekly();
//    cout << "Weekly treasury return" << endl;
//    for (TreasurySeries::TS::const_iterator it = ts_mon->begin(); it != ts_mon->end(); it++) {
//        cout << it->first << "\t" << it->second->ret() << endl;
//    }
//}
//
//void monthly() {
//    date load_begin(from_simple_string(string("2005-3-15")));
//    date load_end(from_simple_string(string("2005-6-10")));
//    
//    const string spx_symbol("SPX");
//    const string spx_file("/Users/jeongwon/Documents/GitHub/cppbacktester/db/SPDR_ETF/^GSPC.csv");
//    
//    FileDriver fd;
//    PriceSeries ps(fd, spx_file, load_begin, load_end);
//    std::shared_ptr<PriceSeries> ps_mon = ps.monthly();
//    cout << "Monthly Price" << endl;
//    for (PriceSeries::TS::const_iterator it = ps_mon->begin(); it != ps_mon->end(); it++) {
//        cout << it->first << "\t" << it->second->close() << endl;
//    }
//    
//    const string equity_symbol("EQUITY");
//    const string equity_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/equity_benchmark.csv");
//    
//    EquitySeries es(fd, equity_file, load_begin, load_end);
//    std::shared_ptr<EquitySeries> es_mon = es.monthly();
//    cout << "Monthly equity return" << endl;
//    for (EquitySeries::TS::const_iterator it = es_mon->begin(); it != es_mon->end(); it++) {
//        cout << it->first << "\t" << it->second->ret() << endl;
//    }
//    
//    const string treas_symbol("TREASURY");
//    const string treas_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/treasury_benchmark.csv");
//    
//    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
//    std::shared_ptr<TreasurySeries> ts_mon = ts.monthly();
//    cout << "Monthly treasury return" << endl;
//    for (TreasurySeries::TS::const_iterator it = ts_mon->begin(); it != ts_mon->end(); it++) {
//        cout << it->first << "\t" << it->second->ret() << endl;
//    }
//}
