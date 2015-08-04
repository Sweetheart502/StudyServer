/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * lock.cpp --mutex的基本用法 
 *
 * Version: 1.0  2015年07月27日 18时21分51秒
 *
 * Authors:
 *     Sweetheart502 (liyingxiao), liyingxiao502@gmail.com
 *
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex g_lock ;

/*
 * 使用lock_guard可以简化lock/unlock的写法，同时也更安全，
 * lock_guard在构造时会自动锁定互斥量，而在退出作用域进行
 * 析构时会自动解锁， * 从而保证了互斥量的正确操作，避免忘
 * 记unlock操作，故尽量使用lock_guard(它用到了RAII技术)。
 * */

void func ()
{
    // 锁
    // g_lock.lock () ;

    //  出作用域之后自动解锁
    std::lock_guard<std::mutex> locker ( g_lock ) ;

    // 显示线程id号
    std::cout << "entered thread " << std::this_thread::get_id () << std::endl ;

    // 线程休眠
    std::this_thread::sleep_for ( std::chrono::seconds ( 1 ) ) ;

    std::cout << "leaving thread " << std::this_thread::get_id () << std::endl ;

    // 解锁
    // g_lock.unlock () ;
}

int main ( int argc , char * argv[] )
{
    std::thread t1 ( func ) ;
    std::thread t2 ( func ) ;
    std::thread t3 ( func ) ;

    t1.join () ;
    t2.join () ;
    t3.join () ;

    return 0 ;
}
