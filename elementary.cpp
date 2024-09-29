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
    bignum b(0xFFABFFAAFCaDFDA,0x195FF5FFFE,0XFFaaFFFFFFF,0xFFF98765438F4FF7);



    bignum three(0,0,0,0xFFFabFFFF1ABC1);
    bignum two(0,0,0,0xFFFFFFFF123456789);
    uint64_t xpowmod = util_pow_mod<uint64_t>(two.s[3], three.s[3], (uint64_t)0xFFFFFFFFFFFFFFFF);
    two.powmod(three);
    two.print();
    std::cout << xpowmod << "\n";
    a.pow(b);
    a.print();
    return 0;
}
