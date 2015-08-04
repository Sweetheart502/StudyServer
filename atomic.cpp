/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * atomic.cpp --  了解原子变量不需要加锁。
 *
 * Version: 1.0  2015年07月28日 09时48分36秒
 *
 * Authors:
 *     Sweetheart502 (liyingxiao), liyingxiao502@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

// 创建int类型的原子变量
std::atomic <int> atc ( 0 ) ;

void func ()
{
    std::cout << atc << std::endl ;
    // 原子变量自增
    atc++ ;
}

int main ( int argc , char * argv [] )
{
    std::vector <std::thread> tvec ;

    for ( int i = 0 ; i < 10 ; i++ ) {
        std::thread t ( func ) ;

        // 线程对象移动语义
        tvec.push_back ( std::move ( t ) ) ;
        tvec[i].join () ;
    }

    return 0 ;
}
