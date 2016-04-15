//
//  DataEntry.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "DataEntry.hpp"


void DataEntry::update(const TokensRdr &tok, const unsigned linenum) {
    int i = 0;
    for( TokensRdr::iterator iter = tok.begin(); iter != tok.end(); ++iter, ++i ) {
        const std::string field = *iter;
        _update(tok, linenum, field, i); /* Design pattern = Template pattern */
    }
    
    return;
}


YahooPrice::YahooPrice(boost::gregorian::date dt, double open_, double high_, double low_, double close_, double adjclose_, unsigned long volume_): DataEntry(dt), _open(open_), _high(high_), _low(low_), _close(close_), _adjclose(adjclose_), _volume(volume_) {}


void YahooPrice::_update(const TokensRdr &tok, const unsigned linenum, std::string field, int i) {
    
    switch( i ) {
            
        case DATE: {
            _dt = boost::gregorian::from_string(field);
            if( _dt.is_not_a_date() ) {
                std::stringstream ss;
                ss << "Invalid dt at line " << linenum;
                throw DataEntryException(ss.str());
            }
        }
            break;
            
        case OPEN:
            _open = atof(field.c_str());
            break;
            
        case HIGH:
            _high = atof(field.c_str());
            break;
            
        case LOW:
            _low = atof(field.c_str());
            break;
            
        case CLOSE:
            _close = atof(field.c_str());
            break;
            
        case VOLUME:
            _volume = atoi(field.c_str());
            break;
            
        case ADJCLOSE:
            _adjclose = atof(field.c_str());
            break;
            
        default: {
            std::stringstream ss;
            ss << "Unknown field at line " << linenum;
            throw DataEntryException(ss.str());
        }
    } // token type switch
    
    return;
}


void USTreasuryYield::_update(const TokensRdr &tok, const unsigned linenum, std::string field, int i) {
    
    if (i == DATE) {
        _dt = boost::gregorian::from_string(field);
        if( _dt.is_not_a_date() ) {
            std::stringstream ss;
            ss << "Invalid dt at line " << linenum;
            throw DataEntryException(ss.str());
        }
    }
    else if (i <= DGS30) {
        if (i == maturity)
            _ret = atof(field.c_str());
    }
    else {
        std::stringstream ss;
        ss << "Unknown field at line " << linenum;
        throw DataEntryException(ss.str());
    }
    
    return;
}


void YahooAction::_update(const TokensRdr &tok, const unsigned linenum, std::string field, int i) {
    
    switch( i ) {
            
        case DATE: {
            _dt = boost::gregorian::from_string(field);
            if( _dt.is_not_a_date() ) {
                std::stringstream ss;
                ss << "Invalid dt at line " << linenum;
                throw DataEntryException(ss.str());
            }
        }
            break;
            
        case ACTION:
            transform(field.begin(), field.end(), field.begin(), ::tolower);  // make lowercase
            _action = field;
            break;
            
        case RATIO:
            _ratio = atof(field.c_str());
            break;
            
        default: {
            std::stringstream ss;
            ss << "Unknown field at line " << linenum;
            throw DataEntryException(ss.str());
        }
    } // token type switch
    
    return;
}


void USEquityReturn::_update(const TokensRdr &tok, const unsigned linenum, std::string field, int i) {
    
    switch( i ) {
            
        case DATE: {
            _dt = boost::gregorian::from_string(field);
            if( _dt.is_not_a_date() ) {
                std::stringstream ss;
                ss << "Invalid dt at line " << linenum;
                throw DataEntryException(ss.str());
            }
        }
            break;
            
        case RET:
            _ret = atof(field.c_str());
            break;
            
        default: {
            std::stringstream ss;
            ss << "Unknown field at line " << linenum;
            throw DataEntryException(ss.str());
        }
    } // token type switch
    
    return;
}