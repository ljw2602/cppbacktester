//
//  main.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/7/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>

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
#include "ETFTrader.hpp"
#include "BalanceSet.hpp"

using namespace boost::gregorian;
using namespace std;

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

int main() {
    
    date today(from_simple_string(string("2005-2-17")));
    vector<string> names;
    names.push_back("ABC"); names.push_back("XYZ"); names.push_back("OPQ");
    
    BalanceSet::instance().initialize(today, 1000, names);
    BalanceSet::instance().print();
    
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
    
    return 0;
    
}

void etftrader_run() {
    FileDriver fd;
    date load_begin(from_simple_string(string("2005-2-17")));
    date load_end(from_simple_string(string("2006-5-20")));
    
    const string xlu_symbol("XLU");
    const string xlu_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU.csv");
    const string xlu_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU_actions.csv");
    
    const string xlv_symbol("XLV");
    const string xlv_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLV.csv");
    const string xlv_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLV_actions.csv");
    
    // load function advances the data to EOM
    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
    
    
    ETFTrader one(1);
    one.run();  // this function advances the data
}

void etftrader_offset() {
    FileDriver fd;
    date load_begin(from_simple_string(string("2005-2-17")));
    date load_end(from_simple_string(string("2006-5-20")));
    
    const string xlu_symbol("XLU");
    const string xlu_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU.csv");
    const string xlu_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU_actions.csv");
    
    const string xlv_symbol("XLV");
    const string xlv_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLV.csv");
    const string xlv_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLV_actions.csv");
    
    // load function advances the data to EOM
    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
        cout << *it << endl;
        
        PriceSeries ds = DB::instance().get(*it).daily();
        PriceSeries::const_iterator pDaily = DB::instance().get(*it).pDaily();
        cout << "Daily Price" << endl;
        for (; pDaily != DB::instance().get(*it).daily().end(); pDaily++) {
            cout << pDaily->first << "\t" << pDaily->second->close() << endl;
        }
        
        cout << "Monthly Price" << endl;
        PriceSeries::const_iterator pMonthly = DB::instance().get(*it).pMonthly();
        for (; pMonthly != DB::instance().get(*it).monthly().end(); pMonthly++) {
            cout << pMonthly->first << "\t" << pMonthly->second->close() << endl;
        }
        
        cout << "Action Series" << endl;
        ActionSeries::const_iterator pAction = DB::instance().get(*it).pAction();
        for (; pAction != DB::instance().get(*it).action().end(); pAction++) {
            cout << pAction->first << "\t" << pAction->second->ratio() << endl;
        }
    }
}

void etftrader() {
    date begin(from_simple_string(string("2005-2-17")));
    date end(from_simple_string(string("2005-3-11")));
    
    ETFTrader one(1);
    unsigned long id = one.buy("XYZ", begin, 34.12, 3);
    one.buy(id, begin, 41.32, 1);
    one.sell(id, begin, 63.18);
    one.close(id, end, 53.12);
    one.print(id);
    
    ETFTrader two(1);
    unsigned long id2 = two.sell_short("ABC", begin, 34.12, 3);
    two.sell_short(id2, begin, 41.32, 1);
    two.cover(id2, begin, 63.18);
    two.close(id2, end, 53.12);
    two.print(id2);

}

void positionset() {
    date begin(from_simple_string(string("2005-2-17")));
    
    PositionSet pset;
    PositionPtr longpos(new LongPosition(1, "XYZ", begin, 34.52, 2));
    PositionPtr shortpos(new ShortPosition(2, "XLV", begin, 34.52, 2));
    
    pset.insert(longpos);
    pset.insert(shortpos);
    
    PositionSet::const_iterator it = pset.find(1, pos_comp_id());
    if (it != pset.end()) {
        (*it)->buy(date(from_simple_string(string("2005-3-11"))), 42.77, 1);
        (*it)->sell(date(from_simple_string(string("2005-3-14"))), 49.77, 1);
        (*it)->close(date(from_simple_string(string("2005-3-17"))), 63.88);
    }
    
    it = pset.find(2, pos_comp_id());
    if (it != pset.end()) {
        (*it)->sell_short(date(from_simple_string(string("2005-3-12"))), 142.77, 1);
        (*it)->sell_short(date(from_simple_string(string("2005-3-19"))), 134.77, 1);
        (*it)->cover(date(from_simple_string(string("2005-3-18"))), 134.77, 1);
        (*it)->close(date(from_simple_string(string("2005-3-22"))), 120.77);
    }
    
    pset.print();
}

void position() {
    date begin(from_simple_string(string("2005-2-17")));
    
    LongPosition longpos(1, "XYZ", begin, 34.52, 2);
    longpos.buy(date(from_simple_string(string("2005-3-11"))), 42.77, 1);
    longpos.sell(date(from_simple_string(string("2005-3-14"))), 49.77, 1);
    longpos.close(date(from_simple_string(string("2005-3-17"))), 63.88);
    
    ShortPosition shortpos(2, "XLV", begin, 34.52, 1);
    shortpos.sell_short(date(from_simple_string(string("2005-3-12"))), 142.77, 2);
    shortpos.sell_short(date(from_simple_string(string("2005-3-19"))), 134.77, 1);
    shortpos.cover(date(from_simple_string(string("2005-3-18"))), 134.77, 1);
    shortpos.close(date(from_simple_string(string("2005-3-22"))), 120.77);
    
    longpos.print();
    shortpos.print();
}

void execution() {
    date load_begin(from_simple_string(string("2005-2-17")));
    boost::shared_ptr<Execution> buy(new BuyExecution("XYZ", Execution::NullID, load_begin, 34.34, 3));
    boost::shared_ptr<Execution> sell( new SellExecution("XYZ", Execution::NullID, load_begin, 23.39, 2));
    boost::shared_ptr<Execution> sshort(new SellShortExecution("XYZ", Execution::NullID, load_begin, 31.64, 5));
    boost::shared_ptr<Execution> cover(new CoverExecution("XYZ", Execution::NullID, load_begin, 38.34, 1));
}

void executionset() {
    date load_begin(from_simple_string(string("2005-2-17")));

    ExecutionSet exset;
    exset.buy("XYZ", load_begin, 34.34, 3);
    exset.sell("XYZ", load_begin, 23.39, 5);
    exset.sell_short("XYZ", load_begin, 31.64, 1);
    exset.cover("XYZ", load_begin, 37.41, 1);
    
    exset.print();
}

void database_offset() {
    FileDriver fd;
    date load_begin(from_simple_string(string("2005-2-17")));
    date load_end(from_simple_string(string("2006-5-20")));
    
    const string xlu_symbol("XLU");
    const string xlu_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU.csv");
    const string xlu_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU_actions.csv");

    const string xlv_symbol("XLV");
    const string xlv_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLV.csv");
    const string xlv_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLV_actions.csv");

    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
    DB::instance().load(xlv_symbol, xlv_price, xlv_action, fd, load_begin, load_end);
    
    DB::instance().advance(1,2);  // advance 1 year and 2 months
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
        cout << *it << endl;
        
        PriceSeries ds = DB::instance().get(*it).daily();
        PriceSeries::const_iterator pDaily = DB::instance().get(*it).pDaily();
        cout << "Daily Price" << endl;
        for (; pDaily != DB::instance().get(*it).daily().end(); pDaily++) {
            cout << pDaily->first << "\t" << pDaily->second->close() << endl;
        }
        
        cout << "Monthly Price" << endl;
        PriceSeries::const_iterator pMonthly = DB::instance().get(*it).pMonthly();
        for (; pMonthly != DB::instance().get(*it).monthly().end(); pMonthly++) {
            cout << pMonthly->first << "\t" << pMonthly->second->close() << endl;
        }
        
        cout << "Action Series" << endl;
        ActionSeries::const_iterator pAction = DB::instance().get(*it).pAction();
        for (; pAction != DB::instance().get(*it).action().end(); pAction++) {
            cout << pAction->first << "\t" << pAction->second->ratio() << endl;
        }
    }
    
}

void database() {
    FileDriver fd;
    date load_begin(from_simple_string(string("2005-3-17")));
    date load_end(from_simple_string(string("2005-4-5")));
    
    const string xlu_symbol("SPX");
    const string xlu_price("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU.csv");
    const string xlu_action("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU_actions.csv");
    
    DB::instance().load(xlu_symbol, xlu_price, xlu_action, fd, load_begin, load_end);
    
    PriceSeries ds = DB::instance().get(xlu_symbol).daily();
    cout << "Daily Price" << endl;
    for (PriceSeries::TS::const_iterator it = ds.begin(); it != ds.end(); it++) {
        cout << it->first << "\t" << it->second->close() << endl;
    }
    
    PriceSeries ms = DB::instance().get(xlu_symbol).monthly();
    cout << "Monthly Price" << endl;
    for (PriceSeries::TS::const_iterator it = ms.begin(); it != ms.end(); it++) {
        cout << it->first << "\t" << it->second->close() << endl;
    }
    
    ActionSeries as = DB::instance().get(xlu_symbol).action();
    cout << "Action Series" << endl;
    for (ActionSeries::TS::const_iterator it = as.begin(); it != as.end(); it++) {
        cout << it->first << "\t" << it->second->ratio() << endl;
    }
}

void daily() {
    date load_begin(from_simple_string(string("2005-3-15")));
    date load_end(from_simple_string(string("2005-3-30")));
    
    const string spx_symbol("SPX");
    const string spx_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/^GSPC.csv");
    
    FileDriver fd;
    PriceSeries ps(fd, spx_file, load_begin, load_end);
    cout << "Daily Price" << endl;
    for (PriceSeries::TS::const_iterator it = ps.begin(); it != ps.end(); it++) {
        cout << it->first << "\t" << it->second->close() << endl;
    }
    
    const string act_symbol("ACT");
    const string act_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/XLU_actions.csv");
    
    ActionSeries as(fd, act_file, load_begin, load_end);
    cout << "Action" << endl;
    for (ActionSeries::TS::const_iterator it = as.begin(); it != as.end(); it++) {
        cout << it->first << "\t" << it->second->ratio() << endl;
    }
    
    const string equity_symbol("EQUITY");
    const string equity_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/equity_benchmark.csv");
    
    EquitySeries es(fd, equity_file, load_begin, load_end);
    cout << "Daily equity return" << endl;
    for (EquitySeries::TS::const_iterator it = es.begin(); it != es.end(); it++) {
        cout << it->first << "\t" << it->second->ret() << endl;
    }
    
    const string treas_symbol("TREASURY");
    const string treas_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/treasury_benchmark.csv");
    
    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
    cout << "Daily treasury return" << endl;
    for (TreasurySeries::TS::const_iterator it = ts.begin(); it != ts.end(); it++) {
        cout << it->first << "\t" << it->second->ret() << endl;
    }
}

void weekly() {
    date load_begin(from_simple_string(string("2005-3-15")));
    date load_end(from_simple_string(string("2005-4-10")));
    
    const string spx_symbol("SPX");
    const string spx_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/^GSPC.csv");
    
    FileDriver fd;
    PriceSeries ps(fd, spx_file, load_begin, load_end);
    std::shared_ptr<PriceSeries> ps_mon = ps.weekly();
    cout << "Weekly Price" << endl;
    for (PriceSeries::TS::const_iterator it = ps_mon->begin(); it != ps_mon->end(); it++) {
        cout << it->first << "\t" << it->second->close() << endl;
    }
    
    const string equity_symbol("EQUITY");
    const string equity_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/equity_benchmark.csv");
    
    EquitySeries es(fd, equity_file, load_begin, load_end);
    std::shared_ptr<EquitySeries> es_mon = es.weekly();
    cout << "Weekly equity return" << endl;
    for (EquitySeries::TS::const_iterator it = es_mon->begin(); it != es_mon->end(); it++) {
        cout << it->first << "\t" << it->second->ret() << endl;
    }
    
    const string treas_symbol("TREASURY");
    const string treas_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/treasury_benchmark.csv");
    
    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
    std::shared_ptr<TreasurySeries> ts_mon = ts.weekly();
    cout << "Weekly treasury return" << endl;
    for (TreasurySeries::TS::const_iterator it = ts_mon->begin(); it != ts_mon->end(); it++) {
        cout << it->first << "\t" << it->second->ret() << endl;
    }
}

void monthly() {
    date load_begin(from_simple_string(string("2005-3-15")));
    date load_end(from_simple_string(string("2005-6-10")));
    
    const string spx_symbol("SPX");
    const string spx_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/ETF/^GSPC.csv");
    
    FileDriver fd;
    PriceSeries ps(fd, spx_file, load_begin, load_end);
    std::shared_ptr<PriceSeries> ps_mon = ps.monthly();
    cout << "Monthly Price" << endl;
    for (PriceSeries::TS::const_iterator it = ps_mon->begin(); it != ps_mon->end(); it++) {
        cout << it->first << "\t" << it->second->close() << endl;
    }
    
    const string equity_symbol("EQUITY");
    const string equity_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/equity_benchmark.csv");
    
    EquitySeries es(fd, equity_file, load_begin, load_end);
    std::shared_ptr<EquitySeries> es_mon = es.monthly();
    cout << "Monthly equity return" << endl;
    for (EquitySeries::TS::const_iterator it = es_mon->begin(); it != es_mon->end(); it++) {
        cout << it->first << "\t" << it->second->ret() << endl;
    }
    
    const string treas_symbol("TREASURY");
    const string treas_file("/Users/jeongwon/Documents/GitHub/hudsonV2/db/Benchmark/treasury_benchmark.csv");
    
    TreasurySeries ts("1YR", fd, treas_file, load_begin, load_end);
    std::shared_ptr<TreasurySeries> ts_mon = ts.monthly();
    cout << "Monthly treasury return" << endl;
    for (TreasurySeries::TS::const_iterator it = ts_mon->begin(); it != ts_mon->end(); it++) {
        cout << it->first << "\t" << it->second->ret() << endl;
    }
}
