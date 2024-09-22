/*
 * elementary.cpp
 *
 *  Created on: Sep 21, 2024
 *      Author: tgjuranec
 */



#include <iostream>
#include <string>
#include "bignum.h"
#include "util.h"




int main(){
    std::string str_in;
    std::cout << "Insert 1st number: ";
    std::getline(std::cin, str_in);
    auto a = static_cast<uint64_t>(std::stoull(str_in));
    std::cout << "Insert 2nd number: ";
    std::getline(std::cin, str_in);
    auto b = static_cast<uint64_t>(std::stoull(str_in));
    uint64_t res{0}, c{0};
    util_add<uint64_t>(a,b,c,res);
    std::cout << std::hex << std::uppercase <<  "carry: " << (uint64_t) c << ", Add: " <<(uint64_t) res <<"\n";
    util_mult<uint64_t>(a,b,c,res);
    std::cout << std::hex << std::uppercase << "carry: " << (uint64_t) c << ", Mult: " << (uint64_t) res << "\n";
    return 0;
}
