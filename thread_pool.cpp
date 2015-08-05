/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * thread_pool.cpp -- 线程池
 *
 * Version: 1.0  2015年07月28日 19时48分32秒
 *
 * Authors:
 *     Sweetheart502 (liyingxiao), liyingxiao502@gmail.com
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <functional>
#include <thread>
#include <memory>
#include <atomic>
#include "syn_2.h"

const int MaxTaskCount = 100 ;

class ThreadPool {
    public :
        // 类型别名，相当于typedef
        using Task = std::function <void ()> ;

        // 构造函数
        // hardware_concurrency ()  获得的是当前可用的process数量
        ThreadPool ( int numThreads = std::thread::hardware_concurrency () ) : m_queue ( MaxTaskCount ) {
            // 初始化线程
            // 通过shared_ptr来管理
            Start ( numThreads ) ;
        }


        // 析构函数
        ~ThreadPool ()
        {
            Stop () ;
        }

        // 终止所有线程
        void Stop ()
        {
            // call_once()为了解决多线程中出现的资源竞争导致的数据不一致问题
            std::call_once ( m_flag , [this] { StopThreadGroup () ; } ) ;
        }

        // 添加任务
        void AddTask ( const Task & task )
        {
            // 常量左值引用
            m_queue.Put ( task ) ;
        }

        // 添加任务 
        void AddTask ( Task && task ) 
        {
            // 右值引用
            m_queue.Put ( std::forward <Task> ( task ) ) ;
        }

    private :
        // 停止线程池
        void StopThreadGroup () {
            m_queue.Stop () ;
            // 运行标志设置为false表示终止！！！
            m_running = false ;

            for ( auto thread : m_threadgroup ) {
                if ( thread ) {
                    thread->join () ;
                }
            }

            m_threadgroup.clear () ;
        }

        // 启动线程池
        void Start ( int numThreads ) {
            // 运行标志设置为true表示启动！！！
            m_running = true ;

            for ( int i = 0 ; i< numThreads ; ++i ) {
                // 架循环初始化线程
                std::cout << "[Start函数]Init create threadpool." << std::endl ;
                m_threadgroup.push_back ( std::make_shared<std::thread> ( &ThreadPool::RunInThread , this ) ) ;
            }
        }
        
        // 一次取出队列中全部事件
        void RunInThread_list ()
        {
            while ( m_running ) {
                std::list<Task> list ;
                std::cout << "take " << std::endl ;

                m_queue.Take ( list ) ;

                for ( auto & task : list ) {
                    if ( !m_running ) {
                        return ;
                    }
                    task () ;
                }
            }
        }

        // 一次取出一个事件
        void RunInThread ()
        {
            std::cout << m_queue.Size () << std::endl ;

            while ( m_running ) {
                std::cout << m_queue.Size () << std::endl ; 

                Task task ;

                if ( !m_running ) {
                    return ; 
                }

                m_queue.Take ( task ) ;
            }
        }

    private :
        // 线程池
        std::list <std::shared_ptr <std::thread>> m_threadgroup ;
        // 任务队列
        SynQueue <Task> m_queue ;

        // 原子变量
        std::atomic_bool m_running ;
        // 辅助变量 call_once
        std::once_flag m_flag ;
} ;

int main ()
{
    ThreadPool pool ( 2 ) ;

    std::thread thd1 ( [&pool] {
        for ( int i = 0 ; i< 10 ;i++ ) {
            auto thdId = std::this_thread::get_id () ;

            pool.AddTask ( [thdId]() {
                std::cout << thdId << "thread execute task " << std::endl ;
            } ) ; 
        }
     } ) ;

    std::this_thread::sleep_for ( std::chrono::seconds ( 2 ) ) ;

    pool.Stop () ;

    thd1.join () ;
  
    return 0 ;
}
