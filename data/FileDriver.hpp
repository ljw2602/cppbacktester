//
//  FileDriver.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef FileDriver_hpp
#define FileDriver_hpp

#include <string>
#include <stdexcept>
#include <fstream>

#include "DataEntry.hpp"

class DriverException: public std::exception
{
public:
    DriverException(const std::string& msg):
    _Str("DriverException: ")
    {
        _Str += msg;
    }
    
    virtual ~DriverException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }
    
private:
    std::string _Str;
};


class FileDriver
{
public:
    FileDriver(void);
    virtual ~FileDriver(void);
    
    virtual bool open(const std::string& filename);
    virtual void close(void);
    virtual bool next(std::shared_ptr<DataEntry> pEntry) throw(DriverException);
    virtual bool next(DataEntry& lp) throw(DriverException);
    virtual bool eof(void);
    
private:
    std::ifstream _infile;
    std::string _line;
    unsigned _linenum;
};

#endif /* FileDriver_hpp */
