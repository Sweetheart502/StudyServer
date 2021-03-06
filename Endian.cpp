/*************************************************************************
 **	    >  Name : 判断字节序.c
 **	    > Author: LiYingXiao (Sweethreart502) 
 **	    >  Mail : liyingxiao502@gmail.com
 **	    >  Blog : http://blog.csdn.net/u013166575
 **	    > Created Time: 2015年11月26日 星期四 15时04分28秒
 ************************************************************************/

#include <iostream>

void byteorder ()
{
    union {
        short value ;
        char union_bytes[sizeof ( short )] ;
    } test ;

    test.value = 0x0102 ;

    if ( ( test.union_bytes[0] == 1 ) && ( test.union_bytes[1] == 2 ) ) {
        std::cout <<  "big endian" << std::endl ;
    } else if ( ( test.union_bytes[0] == 2 ) && ( test.union_bytes[1] == 1 ) ) {
        std::cout << "little endian" << std::endl ;
    } else {
        std::cout << "unknow." << std::endl ;
    }
}

/**
 * litter endian : 高高低低 (主机字节序)
 *  big   endian : 高低低高 (网络字节序)
 */

int main ( int argc , char * argv[] )
{
    // 调用函数
    byteorder () ;

    return 0 ;
}

