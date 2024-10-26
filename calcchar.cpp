#include <iostream>
#include <cstdint>
#include <algorithm>




bool isbase10(const std::string &bdec){
    auto bd = bdec;
    if(!std::all_of(bd.begin(),bd.end(), ::isdigit)){
        return false;
    }
    return true;
}