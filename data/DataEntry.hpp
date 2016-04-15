//
//  DataEntry.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef DataEntry_hpp
#define DataEntry_hpp

#include <string>
#include <boost/tokenizer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

typedef boost::tokenizer<boost::char_separator<char> > TokensRdr;


class DataEntryException: public std::exception
{
public:
    DataEntryException(const std::string& msg):
    _Str("DataEntryException: ")
    {
        _Str += msg;
    }
    
    virtual ~DataEntryException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }
    
private:
    std::string _Str;
};


class DataEntry{
private:
    virtual void _update(const TokensRdr &tok, const unsigned linenum, std::string field, int i) = 0;
protected:
    boost::gregorian::date _dt;
public:
    DataEntry(){}
    DataEntry(boost::gregorian::date dt_): _dt(dt_){}
    virtual void update(const TokensRdr &tok, const unsigned linenum);
    boost::gregorian::date date() const { return _dt; }
};


class YahooPrice: public DataEntry {
private:
    enum field{
        DATE = 0,
        OPEN, HIGH, LOW, CLOSE, VOLUME, ADJCLOSE
    };
    double _open, _high, _low, _close, _adjclose;
    unsigned long _volume;
    
    void _update(const TokensRdr &tok, const unsigned linenum, std::string field, int i);
protected:
public:
    YahooPrice(){}
    YahooPrice(boost::gregorian::date dt, double open_, double high_, double low_, double close_, double adjclose_, unsigned long volume_);
    double open() const { return _open; }
    double high() const { return _high; }
    double low() const {return _low; }
    double close() const { return _close; }
    double adjclose() const { return _adjclose; }
    unsigned long volume() const { return _volume; }
};


class YahooAction: public DataEntry {
private:
    enum field{
        DATE = 0,
        ACTION, RATIO
    };
    std::string _action; // DIVIDEND or SPLIT
    double _ratio;
    
    void _update(const TokensRdr &tok, const unsigned linenum, std::string field, int i);
protected:
public:
    double ratio() const { return _ratio; }
    std::string action() const { return _action; }
};


class USTreasuryYield: public DataEntry {
private:
    enum field{
        DATE = 0,
        DGS1MO, DGS3MO, DGS6MO,
        DGS1, DGS2, DGS3, DGS5, DGS7, DGS10, DGS20, DGS30
    };
    double _ret;
    field maturity;
    
    void _update(const TokensRdr &tok, const unsigned linenum, std::string field, int i);
    
    void maturity_map(std::string maturity_) {
        if (maturity_ == "1MO") maturity = DGS1MO;
        else if (maturity_ == "3MO") maturity = DGS3MO;
        else if (maturity_ == "6MO") maturity = DGS6MO;
        else if (maturity_ == "1YR") maturity = DGS1;
        else if (maturity_ == "2YR") maturity = DGS2;
        else if (maturity_ == "3YR") maturity = DGS3;
        else if (maturity_ == "5YR") maturity = DGS5;
        else if (maturity_ == "7YR") maturity = DGS7;
        else if (maturity_ == "10YR") maturity = DGS10;
        else if (maturity_ == "20YR") maturity = DGS20;
        else if (maturity_ == "30YR") maturity = DGS30;
        else {
            throw DataEntryException("Unknown maturity");
        }
    }
protected:
public:
    USTreasuryYield(std::string maturity_) {
        maturity_map(maturity_);
    }
    USTreasuryYield(field maturity_): maturity(maturity_) {}
    USTreasuryYield(boost::gregorian::date dt, double ret_, std::string maturity_): DataEntry(dt), _ret(ret_) {
        maturity_map(maturity_);
    }
    
    double ret() const { return _ret; }
};


class USEquityReturn: public DataEntry {
private:
    enum field{
        DATE = 0,
        RET
    };
    double _ret;

    void _update(const TokensRdr &tok, const unsigned linenum, std::string field, int i);
protected:
public:
    USEquityReturn(){}
    USEquityReturn(boost::gregorian::date dt, double ret_): DataEntry(dt), _ret(ret_){}
    double ret() const { return _ret; }
};


#endif /* DataEntry_hpp */
