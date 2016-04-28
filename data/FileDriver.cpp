//
//  FileDriver.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include <boost/tokenizer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "FileDriver.hpp"

using namespace std;
using namespace boost;
using namespace boost::gregorian;


FileDriver::FileDriver(void):
_linenum(0)
{
}


FileDriver::~FileDriver(void)
{
    if( _infile.is_open() )
        _infile.close();
}


bool FileDriver::open(const std::string& filename)
{
    // Check if another file was open previously
    if( _infile.is_open() ) {
        _infile.close();
        _linenum = 0;
    }
    
    _infile.open(filename.c_str());
    if( _infile.is_open() == false )
        return false;
    
    // First line is header line
    getline(_infile, _line);
    _linenum = 1;
    
    return _infile.good();
}


void FileDriver::close(void)
{
    if( _infile.is_open() )
        _infile.close();
    
    _infile.clear();
    
    _linenum = 0;
}


bool FileDriver::next(DataEntry& lp) throw(DriverException)
{
    if( _infile.eof() )
        return false;
    
    try {
        typedef tokenizer<boost::char_separator<char> > TokensRdr;
        getline(_infile, _line);
        ++_linenum;
        
//        TokensRdr tok(_line, boost::char_separator<char>(" ,\t\n\r", "", keep_empty_tokens));
        boost::char_separator<char> sep(",\t\n\r", "", boost::keep_empty_tokens);
        TokensRdr tok(_line, sep);
        lp.update(tok, _linenum);
    } catch (std::exception) {
        std::cout << _line << std::endl;
    }
    
    return true;
}


bool FileDriver::next(std::shared_ptr<DataEntry> pEntry) throw(DriverException)
{
    if( _infile.eof() )
        return false;
    
    typedef tokenizer<boost::char_separator<char> > TokensRdr;
    getline(_infile, _line);
    ++_linenum;
    
//    TokensRdr tok(_line, boost::char_separator<char>(" ,\t\n\r"));
    boost::char_separator<char> sep(",\t\n\r", "", boost::keep_empty_tokens);
    TokensRdr tok(_line, sep);
    pEntry->update(tok, _linenum);
    
    return true;
}


bool FileDriver::eof(void)
{
    return _infile.eof();
}