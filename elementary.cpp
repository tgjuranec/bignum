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

	bignum a(0x9DAF3B4C94CFE30C,0x649A74D6BD89F250,0x8C2C8092A0D234A7,0x7072008745545ADF);
    bignum b(0xCaDFDA,0x19885FF5FFFE,0XFFaaFFFFFFF,0xFFF9876543F4FF7);



    bignum three(0,0,0,10);
    bignum two(0,0,0,2558);
    two.div(three);
    two.print();
    std::cout << two.rem[3] << "\n";
    a.div(b);
    a.print();
    return 0;
}
