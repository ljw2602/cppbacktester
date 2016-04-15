//
//  Position.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "Position.hpp"
#include "PriceSeries.hpp"


Position::Position(ID id, const std::string& symbol): _id(id), _symbol(symbol), _size(0) {}


void Position::print(void) const
{
    std::cout << _symbol << " [Pos ID: " << id() << "]" << ":" << std::endl;
    _sExecutions.print();
}
