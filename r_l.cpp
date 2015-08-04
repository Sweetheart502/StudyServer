/*
 * (C) 2007-2015 Alibaba Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * r_l.cpp -- 
 *
 * Version: 1.0  2015年07月28日 14时36分25秒
 *
 * Authors:
 *     Sweetheart502 (liyingxiao), liyingxiao502@gmail.com
 *
 */

#include <iostream>

void PrintValue ( int & i )
{
    std::cout << "lvalue : " << i << std::endl ;
}

void PrintValue ( int && i )
{
    std::cout << "rvalue : " << i << std::endl ;
}

void Forward ( int && i )
{
    PrintValue ( i ) ;
}

int main ( void )
{
    int i =0 ;

    PrintValue ( i ) ;
    PrintValue ( 1 ) ;
    Forward ( 2 ) ;

/*
 * p68 the last line.
 */

    return 0 ;
}
