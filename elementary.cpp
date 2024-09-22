/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include <iostream>
#include <string>
#include "bignum.h"




int main(){
    std::string str_in;
    bignum a(1,2,3,4);
    bignum b(0,0,0,4);
    bignum c = a;
    c.mult(b);
    c.print();
    a.pow(b);
    a.print();

    return 0;
}
