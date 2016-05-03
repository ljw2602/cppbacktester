//
//  BalanceSet.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/19/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "BalanceSet.hpp"


std::auto_ptr<BalanceSet> BalanceSet::_pInstance;
std::string BalanceSet::CASH("Cash");
std::string BalanceSet::TOTAL("Total");


BalanceSet& BalanceSet::instance(void)
{
    if ( _pInstance.get() == 0)
        _pInstance.reset(new BalanceSet());
    return *_pInstance;
}


void BalanceSet::initialize(const boost::gregorian::date& dt,
                            const double init,
                            const std::vector<std::string>& names_)
{
    
    assert(names.empty());  // initialize has to be run only once
    
    names = names_;

    // initial amount has to be positive
    if(init < 0)
        throw BalanceException("Negative base capital");
    
    // insert entries
    for (std::vector<std::string>::const_iterator it = names_.begin(); it != names_.end(); it++) {
        BalancePtr pBalance( new Balance(*it, dt, 0) );
        insert(pBalance);
    }
    
    BalancePtr pCASH(new Balance(CASH, dt, init));
    insert(pCASH);
    
    BalancePtr pTOTAL(new Balance(TOTAL, dt, init));
    insert(pTOTAL);
}


void BalanceSet::print(void) const
{
    // Print total balance
    
    name_pair its = equal_range(TOTAL);
    for (BalanceSet::const_iterator it = its.first; it != its.second; it++) {
        (*it)->print();
    }
    return;
}


double BalanceSet::split_and_dividend(std::map<std::string, int>& share_book,
                                      const boost::gregorian::date& today)
{
    double netdividend = 0.0;
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ )
    {
        std::string sym = *it;
        ActionSeries::const_iterator pAS = DB::instance().get(sym).pAction();
        
        if (pAS->first != today) continue;
        
        if (pAS->second->action() == "dividend")
        {
            if (share_book.find(sym) != share_book.end())
            {
                double dividend = pAS->second->ratio();
                int current_share = share_book.at(sym);
                netdividend += current_share * dividend;
#ifdef PRINT
                std::cout << "Dividend from " << sym << " is " << dividend << " per share on " << today << std::endl;
                std::cout << "Received amount is " << current_share * dividend << std::endl;
#endif
            }
            DB::instance().get(sym).action_advance();
        }
        else if (pAS->second->action() == "split")
        {
            if (share_book.find(sym) != share_book.end())
            {
                int split = 1/pAS->second->ratio();
                int current_share = share_book.at(sym);
                share_book.at(sym) = current_share * split;
#ifdef PRINT
                std::cout << sym << " has splitted into 1:" << split << " on " << today << std::endl;
                std::cout << "Holding share changed from " << current_share << " to " << share_book.at(sym) << std::endl;
#endif
            }
            DB::instance().get(sym).action_advance();
        }
    }
    
    return netdividend;
}


void BalanceSet::update_capital(const std::map<std::string, int>& share_book,
                                const double netcash,
                                const boost::gregorian::date& today)
{
    double total = 0.0;
    
    for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); it++) {
        if ( share_book.find(*it) != share_book.end() ) {
            int eod_shares = share_book.at(*it);
            double price = DB::instance().get(*it).daily().find(today)->second->close();
            assert(DB::instance().get(*it).daily().find(today)->first == today);
            
            BalancePtr pBalance( new Balance(*it, today, eod_shares * price, eod_shares) );
            insert(pBalance);
            total += eod_shares * price;
        } else {
            BalancePtr pBalance( new Balance(*it, today, 0) );
            insert(pBalance);
        }
    }
    
    name_pair its = equal_range(CASH);  // equal_range(key) returns [a, b)
    double prevcash = (*(--its.second))->balance();  // its.second needs to be decreased by 1 to get latest one
    double cash = prevcash + netcash;
    
    BalancePtr pCASH(new Balance(CASH, today, cash));
    insert(pCASH);
    total += cash;
    
    BalancePtr pTOTAL(new Balance(TOTAL, today, total));
    insert(pTOTAL);
    
    if (cash < 0) {
        export_to_csv("DEBUG");
        std::cout << today << " balance check" << std::endl;
        for (std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); it++) {
            if ( share_book.find(*it) != share_book.end() ) {
                int eod_shares = share_book.at(*it);
                double price = DB::instance().get(*it).daily().find(today)->second->close();
                std::cout << "Attempted to buy " << eod_shares << " shares at $" << price << std::endl;
            }
        }
        throw BalanceException("Cash goes below zero; trading suspended");
    }
}

double BalanceSet::latest_total() const
{
    name_pair its = equal_range(TOTAL);
    double prevtotal = (*(--its.second))->balance();
    return prevtotal;
}

/*
std::map<boost::gregorian::date, double> BalanceSet::monthly(const std::set<boost::gregorian::date>& EOMdate) const {
    name_pair its = equal_range(TOTAL);
    std::map<boost::gregorian::date, double> EOD;
    for (BalanceSet::const_iterator it = its.first; it != its.second; it++) {
        EOD.insert( std::pair<boost::gregorian::date, double>((*it)->dt, (*it)->balance) );
    }
    
    std::map<boost::gregorian::date, double> EOM;
    for (std::set<boost::gregorian::date>::const_iterator pdate = EOMdate.begin(); pdate != EOMdate.end(); pdate++) {
        boost::gregorian::date dt = (*pdate);
        EOM.insert( *EOD.find(dt) );
    }
    
    return EOM;
}

std::vector<double> BalanceSet::monthly_ret(const std::set<boost::gregorian::date>& m_date) const {
    
    std::map<boost::gregorian::date, double> m_total = monthly(m_date);
    
    std::map<boost::gregorian::date, double>::const_iterator it = m_total.begin();
    std::vector<double> result;
    double prev = it->second; it++;
    for (; it != m_total.end(); it++) {
        double now = it->second;
        double ret = (now-prev)/prev * 100;
        result.push_back(ret);
        prev = it->second;
    }
    return result;
}
*/

void BalanceSet::export_to_csv(const std::string& tag) const {
    std::ofstream myfile;
    myfile.open (tag+"Balance.csv");
    
    // Header
    myfile << "Date";
    for (int i = 0; i < names.size(); i++) {
        myfile << "," << names[i] << "," << names[i] << " #";
    }
    myfile << "," << CASH << "," << TOTAL << "\n";
    
    // Content
    by_date::const_iterator it_date = get<1>().begin();
    by_date::const_iterator it_end = get<1>().end();
    boost::gregorian::date date;
    
    while (it_date != it_end) {
        date = (*it_date)->dt();
        date_pair its = by_date::equal_range(date);

        std::map<std::string, std::pair<double, int> > temp;  // name, balance, shares
        for (by_date::const_iterator it = its.first; it != its.second; it++) {
            temp.insert( std::pair<std::string, std::pair<double, int> >((*it)->name(), std::pair<double, int>((*it)->balance(), (*it)->shares()) ));
        }
        
        myfile << date << ",";
        for (int i = 0; i < names.size(); i++) {
            myfile << temp.at(names[i]).first << "," << temp.at(names[i]).second << ",";
        }
        myfile << temp.at(CASH).first << "," << temp.at(TOTAL).first << "\n";
        
        it_date = its.second;
    }
    
    myfile.close();
    std::cout << tag+"Balance.csv is saved" << std::endl;
    
    return;
}
