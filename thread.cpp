/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * thread.cpp -- 线程的基本操作
 *
 * Version: 1.0  2015年07月27日 15时18分09秒
 *
 * Authors:
 *     Sweetheart502 (liyingxiao), liyingxiao502@gmail.com
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

void func ()
{
    std::this_thread::sleep_for ( std::chrono::seconds ( 3 ) ) ;

    std::cout << "Time out." << std::endl ;
}

int main ()
{
    // 创建线程
    std::thread t ( func ) ;

    // 打印线程ID和线程CPU核数
    std::cout << "ID : " << t.get_id () << std::endl ;
    std::cout << "CPU: " << std::thread::hardware_concurrency () << std::endl ;

    // 线程阻塞，等待其执行结束
    t.join () ;

    // 将该线程变成后台线程
    // t.detach () ;

    return 0 ;
}
