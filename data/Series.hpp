//
//  Series.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/7/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Series_hpp
#define Series_hpp

#include <map>
#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>

#include "FileDriver.hpp"
#include "DataEntry.hpp"


class SeriesException: public std::exception
{
public:
    SeriesException(const std::string& msg):
    _Str("SeriesException: ")
    {
        _Str += msg;
    }
    
    virtual ~SeriesException(void) throw() { }
    virtual const char *what(void) const throw() { return _Str.c_str(); }
    
protected:
    std::string _Str;
};


template <class LP> /* Design pattern = Template base class */
class Series: public std::map<boost::gregorian::date, std::shared_ptr<LP> > {
public:
    typedef std::map<boost::gregorian::date, std::shared_ptr<LP> > TS;  // Timeseries
    
protected:
    Series(): _reader() {}
    Series(const std::string& maturity_): _reader(maturity_) {}
    LP _reader;
    
public:
    virtual ~Series() {}
    
    boost::gregorian::date_period period(void) const throw(SeriesException) {
        if( TS::empty() ) {
            throw SeriesException("Null series");
        }
        
        return boost::gregorian::date_period(TS::begin()->first, TS::rbegin()->first + boost::gregorian::date_duration(1));
        // ^^^ date_period(begin, end) = [begin, end)
    }
    
    boost::gregorian::date_duration duration(void) const throw(SeriesException) {
        if( TS::empty() ) {
            throw SeriesException("Null series");
        }
        
        return boost::gregorian::date_duration(TS::rbegin()->first - TS::begin()->first) + boost::gregorian::date_duration(1);
    }
    
    long days(void) const {
        if( TS::empty() ) return 0;
        return boost::gregorian::date_duration(TS::rbegin()->first - TS::begin()->first).days() + 1;
    }
    
    std::size_t _load(FileDriver& driver, const std::string& filename) {
        TS::clear();
        
        if( !driver.open(filename) )
            return 0;
        
        while( !driver.eof() ) {
            
            try {
                
                if( driver.next(_reader) == false )
                    continue;
                if( TS::insert(typename TS::value_type(_reader.date(), std::shared_ptr<LP>(new LP(_reader)))).second == false ) {
                    std::cerr << "Potential duplicated record " << _reader.date() << " in " << filename << std::endl;
                    continue;
                }
                
            } catch( SeriesException& e ) {
                std::cerr << e.what() << std::endl;
                continue;
            }
            
        }	// while not EOF
        
        driver.close();
        
        return TS::size();
    }
    
    
    std::size_t _load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
        TS::clear();
        
        if( !driver.open(filename) )
            return 0;
        
        while( !driver.eof() ) {
            
            try {
                
                if( driver.next(_reader) == false ) // EOF
                    continue;
                
                if( _reader.date() < begin || _reader.date() > end )
                    continue;					// out of range
                
                if( TS::insert(typename TS::value_type(_reader.date(), std::shared_ptr<LP>(new LP(_reader)))).second == false ) {
                    std::cerr << "Potential duplicated record " << _reader.date() << " in " << filename << std::endl;
                    continue;
                }
                
            } catch( SeriesException& e ) {
                std::cerr << e.what() << std::endl;
                continue;
            }
        }	// while not EOF
        
        driver.close();

        return TS::size();
    }
    
    //! Returns an iterator to the data item at or (if not found) before the specified date.
    typename TS::const_iterator at_or_before(const boost::gregorian::date& k) const
    {
        typename TS::const_iterator iter;
        if( (iter = TS::find(k)) != TS::end() )
            return iter;
        
        return before(k, 1);
    }
    
    
    //! Returns an iterator to recs items before a specific date.
    typename TS::const_iterator before(const boost::gregorian::date& k, unsigned recs) const
    {
        typename TS::const_iterator iter;
        if( (iter = TS::lower_bound(k)) == TS::begin() && recs > 0 )
            return TS::end();
        
        for( unsigned i = 0; i < recs; i++ )
            if( --iter == TS::begin() )
                return TS::end();
        
        return iter;
    }
    
    
    //! Returns an iterator to recs items after a specific date.
    typename TS::const_iterator after(const boost::gregorian::date& k, unsigned recs) const
    {
        typename TS::const_iterator iter;
        if( (iter = TS::find(k)) == TS::end() ) {
            if( (iter = TS::upper_bound(k)) == TS::end() ) {
                return TS::end();				// k out of range
            } else {
                // returning from upper_bound(), we are already one record past the dt
                if( recs ) --recs;
            }
        }
        
        for( unsigned i = 0; i < recs; i++ )
            if( ++iter == TS::end() )
                return TS::end();
        
        return iter;
    }
    
    
    //! Returns the first data series entry for the specified year/month.
    typename TS::const_iterator first_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const
    {
        // Call upper_bound() using the last calendar day for previous month to retrieve first record in requested month
        boost::gregorian::date request_date(year, month, 1); // first calendar day of requested month
        boost::gregorian::date dt_date = request_date - boost::gregorian::date_duration(1); // last calendar day of previous month
        typename TS::const_iterator iter = TS::upper_bound(dt_date); // first price record after last calendar day of prev month
        if( iter == TS::end() )
            return iter;
        
        if( iter->first.month() != month ) // we're over the requested month
            return TS::end();
        
        return iter;
    }
    
    
    //! Returns the last data series entry for the specified year/month.
    typename TS::const_iterator last_in_month(boost::gregorian::greg_year year, boost::gregorian::greg_month month) const
    {
        boost::gregorian::date request_date(year, month, 1);
        typename TS::const_iterator iter = TS::lower_bound(request_date);
        if( iter == TS::end() )
            return iter;
        
        // Step-in until we reach next month bar
        while( iter != TS::end() && iter->first.month() == month )
            ++iter;
        
        // Once we've reached next month first bar, go back one bar to return previous month last bar
        return --iter;
    }
    
    
    //! Returns the first data series entry of the week for the specified year/month/day.
    typename TS::const_iterator first_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const
    {
        boost::gregorian::date request_date(year, month, day);
        boost::gregorian::date begin_of_week;	// Monday in requested date week
        
        // Look for previous Monday in requested week
        begin_of_week = ( request_date.day_of_week() == boost::gregorian::Monday ) ? request_date :
        boost::gregorian::first_day_of_the_week_before(boost::gregorian::Monday).get_date(request_date); // first Monday before request_date
        
        // Look for first available day in price database
        typename TS::const_iterator iter = TS::lower_bound(begin_of_week);
        if( iter == TS::end() )
            return iter;
        
        // Make sure we're on the same week than requested date
        if( iter->first.week_number() == request_date.week_number() )
            return iter;
        
        // Different week, sorry no BOW for this request date
        return TS::end();
    }
    
    
    //! Returns the last data series entry of the week for the specified year/month/day.
    typename TS::const_iterator last_in_week(boost::gregorian::greg_year year, boost::gregorian::greg_month month, boost::gregorian::greg_day day) const
    {
        boost::gregorian::date request_date(year, month, day);
        boost::gregorian::date end_of_week;
        
        // Look for first Friday starting from requested date (included)
        end_of_week = ( request_date.day_of_week() == boost::gregorian::Friday ) ? request_date :
        boost::gregorian::first_day_of_the_week_after(boost::gregorian::Friday).get_date(request_date);	// first Friday of request_date
        
        // Look for this Friday in price database
        typename TS::const_iterator iter = TS::lower_bound(end_of_week);
        if( iter == TS::end() )
            return iter;
        
        // lower_bound() returns next week first record if Friday can't be found
        if( iter->first.day_of_week() == boost::gregorian::Friday )
            return iter;
        
        // We're on the next week. Go back one record to locate requested EOW
        --iter;
        if( iter->first.week_number() == request_date.week_number() )
            return iter;
        
        // Can't find EOW
        return TS::end();
    }
    
    LP last(void) const {
        return *(TS::rbegin()->second);
    }
    
};


#endif /* Series_hpp */
