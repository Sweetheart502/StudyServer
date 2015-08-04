/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * syn.cpp --同步队列的实现 
 *
 * Version: 1.0  2015年07月27日 20时24分04秒
 *
 * Authors:
 *     Sweetheart502 (liyingxiao), liyingxiao502@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <assert.h>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <list>
#include <memory>

template <typename T>
class SynQueue {
    private :
        // 内部判空判满，故无需加锁
        bool IsFull () const
        {
            return m_queue.size () == m_maxSize ;
        }

        bool IsEmpty () const
        {
            return m_queue.empty () ;
        }

    public :
        // 构造函数
        SynQueue ( int maxSize ) : m_maxSize ( maxSize ) { 
        }

        // 析构函数
        ~SynQueue () {

        }

        // 加任务
        void  Put ( const T & x ) {
            // 加锁(说明采用哪种加锁方式以及锁的类型)
            std::lock_guard <std::mutex> locker ( m_mutex ) ;
            while ( IsFull () ) {
                std::cout << "Now It Is Full.Please Waiting~~~" << std::endl ;
                m_notFull.wait ( m_mutex ) ;
            }

            // 等待结束，进行加任务
            m_queue.push_back ( x ) ;
            // 加任务后一定非空，然后唤醒一个线程
            m_notEmpty.notify_one () ;
        }

        // 取任务
        void Take ( T & x ) {
            std::lock_guard <std::mutex> locker ( m_mutex ) ;

            while ( IsEmpty () ) {
                std::cout << "Now It Is Empty.Please Waiting~~." << std::endl ;
                m_notEmpty.wait ( m_mutex ) ;
            }
            // 等待结束，进行取任务
            x = m_queue.front () ;
            m_queue.pop_front () ;
            // 取任务后一定未满，然后唤醒一个线程
            m_notFull.notify_one () ;
        }

        // 判断队空
        bool Empty () {
            std::lock_guard<std::mutex> locker ( m_mutex ) ;
            return m_queue.empty () ;
        }

        // 判断队满
        bool Full () {
            std::lock_guard<std::mutex> locker ( m_mutex ) ;
            return m_queue.size () == m_maxSize ;
        }

        // 返回队列大小
        size_t Size () {
            std::lock_guard<std::mutex> locker ( m_mutex ) ;
            return m_queue.size () ;
        }

        int Count () {
            return m_queue.size () ;
        }

    private :
        // 任务队列
        std::list<T> m_queue ;
        // 互斥锁
        std::mutex m_mutex ;
        // 未满的条件变量
        std::condition_variable_any m_notFull ;
        // 非空的条件变量
        std::condition_variable_any m_notEmpty ;
        // 同步队列的最大size
        int m_maxSize ;
} ;

void func ( SynQueue<int> * sq )
{
    int ret ;

    sq->Take ( ret ) ;

    std::cout << ret << std::endl ;
}

int main ( void )
{
    // 创建同步队列，最大长度为20
    SynQueue<int> syn ( 20 ) ;

    // 存放数据对象
    for ( int i = 0 ; i < 10 ; i++ ) {
        syn.Put ( i ) ;
    }

    std::cout << "Size : " << syn.Size () << std::endl ;

    // 线程不能拷贝，所以采用容器和智能指针来管理线程生存
    std::vector <std::shared_ptr <std::thread>> tvec ;

    for ( int i = 0 ; i < 11 ;i++ ) {
        // 创建线程，并将管理线程的智能指针存在容器中
        tvec.push_back(std::make_shared <std::thread> ( func , &syn ) ) ;

        // 将其变为后台线程
        tvec[i]->detach () ;
        // tvec[i]->join () ;
    }

    sleep ( 3 ) ;

    syn.Put ( 11 ) ;

    sleep ( 3 ) ;

    return 0 ;
}
