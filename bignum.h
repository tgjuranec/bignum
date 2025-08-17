/*
 * bignum.h
 *
 *  Created on: Sep 22, 2024
 *      Author: tgjuranec
 */

#ifndef BIGNUM_H_
#define BIGNUM_H_
#include <cstdint>
#include <limits>
#include <utility>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include "util.h"


template <typename UINT>
class bignum{
public:
    static const UINT FULLSIZE = sizeof(UINT)*8; //half word size in bits	
    static const UINT UINTMAX = (UINT)((UINT) 0 - (UINT)1);
	UINT SIZE;
	std::vector<UINT> s;
	std::vector<UINT> rem;
	bignum(const UINT a[], UINT size);
	bignum(const bignum &a);
	bignum(const std::string &a, uint8_t base );
	bignum(UINT size);
	bignum(const std::vector<UINT> &a);
	~bignum();
	std::string to_string() const;
	std::string to_string_bin() const;
	void bignum_from_string(const std::string& a, uint8_t base);
	void print();
	void add(const bignum &a);
	void mult(const bignum &a);
	void multFixed(const bignum &a);
	void sub(const bignum &a);
	void pow(const bignum &a);
	void powmod(const bignum &a, const bignum &mod);
	bool lt(const bignum &a);
	bool eq(const bignum &a);
	bool gt(const bignum &a);
	void incr();
	void mod(const bignum &a);
	void div(const bignum &a);
	void inflateSize(UINT newsize);
	void deflateSize(UINT newSize);
	UINT getBusySize();
	void shiftleft(UINT nShifts);
	void shiftright(UINT nShifts);
	void addFixed(const bignum &a);  // same as add() but without carry
};


template <typename UINT> bignum<UINT> extendedGCD(const bignum<UINT> a, const bignum<UINT> m){
	bignum<UINT> zero(1);
	bignum<UINT> one(1);
	one.incr();
	
    std::vector<bignum<UINT>> as{m};
    bignum<UINT> x1(m.SIZE), y1(m.SIZE), x(m.SIZE), y(m.SIZE);
    auto tmpa = a, tmpm = m;
    while( tmpa.gt(zero)){
        as.push_back(tmpa);
        auto tmp = tmpa;
        tmpa = tmpm % tmpa;
        tmpm = tmp;        
    }
    if (!as.back().eq(one)) return zero;
    x1 = zero;
    y1 = one;
    for(auto it = as.crbegin(); it != (as.crend()-1); ++it){
		auto ratio = (*(it+1) / (*it));
        auto prod{ratio};
		prod.multFixed(x1);
		x = y1 - prod;
        y = x1;
		// std::cout << x.to_string() << " " << y.to_string() << "\n";
        x1 = x;
        y1 = y;
    }
	x.addFixed(m);
    return ((x % m));
}



template <typename UINT> bignum<UINT>::bignum( UINT size):s(size,0),rem(size,0){
	SIZE = size;
	for(UINT i = 0; i < size; i++){
		s[i] = 0;
		rem[i] = 0;
	}
}

template <typename UINT> bignum<UINT>::bignum(const UINT a[], UINT size):s(size,0),rem(size,0){
	SIZE = size;
	for(UINT i = 0; i < size; i++){
		s[i] = a[i];
		rem[i] = 0;
	}
}

template <typename UINT> bignum<UINT>::~bignum(){
}

template <typename UINT> bignum<UINT>::bignum(const std::vector<UINT> &a):
	s(a),rem(a.size(),0),SIZE(a.size()){}


template <typename UINT> bignum<UINT>::bignum(const bignum& a){
	SIZE = a.SIZE;
	s = a.s;
	rem = a.rem;
}

template <typename UINT> bignum<UINT>::bignum(const std::string& a, uint8_t base ):
	SIZE((UINT) std::ceil((a.length()*std::log2(base)/std::log2(16)+1)/16)),
	s(SIZE,0),rem(SIZE,0){
		// DYNAMIC SIZE CALCUCATION:
	//  10 digits 		hex digits 	uint64_t 
	//		3 				3			1 
	//  	9 				8			1 
	// 		18				15			1
	// 		19				16			1
	// 		20				17			2
	// 		38				32			2
	// 		39				33			3
	// 		58				49			4
	// 		78				65			5
	//		97				81			6
	//		116				97			7
	//		135				113			8
	//		155				129			9
	// size = ceil((input_size*log2(base)/log2(16)+1)/16)
	// default initialization and allocation 

	for (UINT i = 0; i < SIZE; ++i){
		s[i] = 0;
		rem[i] = 0;
	}
	bignum_from_string(a,base);
	deflateSize(getBusySize());
}

template <typename UINT> void bignum<UINT>::bignum_from_string(const std::string& a, uint8_t base){	
	//
	// TODO: CHECK if all characters are numeric (10) or alphanumeric
	
	// TODO: CHECK SIZE

	// tmp variables
	bignum<UINT> one(SIZE);
	one.s[SIZE-1] = 1;
	bignum<UINT> basemult(SIZE);
	basemult.s[SIZE-1] = base;

	bignum<UINT> multiplier(one);
	for(auto it = a.crbegin(); it != a.crend(); it++){
		std::string cdig {*it,0};
		uint8_t digit;
		try{
			digit = std::stoull(cdig, nullptr, base);
		}
		catch(std::invalid_argument& e){
			// std::cout << e.what() << " - not a digit: " << *it << "\n";
			continue;
		}
		catch(std::out_of_range &e){
			std::cout << e.what() << *it << "\n";
			return;
		}
		bignum<UINT> digitvalue(SIZE);
		digitvalue.s[SIZE-1] = digit;
		digitvalue.mult(multiplier);
		add(digitvalue);
		multiplier.mult(basemult);
	}
	return;
}


template <typename UINT> std::string bignum<UINT>::to_string() const{
	// TODO: force to print 0
	std::string str;
	bignum<UINT> work{*this};
	work.deflateSize(work.getBusySize());
	bool leading_zeros{true};
	for(uint64_t i = 0; i < work.SIZE; i++){
		work.rem[i] = 0;
	}
	for(uint64_t i = 0; i < sizeof(UINT)*work.SIZE*8/4;++i){	
		// shift left 4 bits			
		work.shiftleft(4);
		// REMOVE LEADING ZEROS
		if((work.rem[work.SIZE-1]) == 0 && leading_zeros == true){			
			continue;
		}
		leading_zeros = false;
		char a{};
		if((work.rem[work.SIZE-1] & 0xF) >= 0 && (work.rem[work.SIZE-1] & 0xF) < 0xA){
			a = (work.rem[work.SIZE-1] & 0xF) + 0x30;
		}
		else if ((work.rem[work.SIZE-1] & 0xF) >= 0xA && (work.rem[work.SIZE-1] & 0xF) < 0x10){
			a = (work.rem[work.SIZE-1] & 0xF) + 55;
		}
		else{
			str = "Error in to_string() Wrong input!\n";
			return str;
		}
		str.push_back(a);
	}
	// if it's '0'
	if(leading_zeros == true){
		str.push_back('0');
	}
	return str;
}


template <typename UINT> std::string bignum<UINT>::to_string_bin() const{
	std::string str;
	bignum<UINT> work{*this};
	work.deflateSize(work.getBusySize());
	bool leading_zeros{true};
	for(uint64_t i = 0; i < work.SIZE; i++){
		work.rem[i] = 0;
	}
	for(uint64_t i = 0; i < sizeof(UINT)*work.SIZE*8;++i){	
		// shift left 4 bits			
		work.shiftleft(1);
		char a{};
		a = (work.rem[work.SIZE-1] & 0X1) + 0X30;
		str.push_back(a);
	}
	return str;
}

/*
INCREASE bignum SIZE
- only increases size
- allocates new memory of new size
- copy data from old memory
- if everything is OK - deallocates old memory
- set new SIZE
- 
*/

template <typename UINT> void bignum<UINT>::inflateSize(UINT newSize){
	if(newSize <= SIZE ){
		return; 
	}
	while(newSize > s.size()){
		s.insert(s.begin(),0);
	}
	while(newSize > rem.size()){
		rem.insert(rem.begin(),0);
	}
	SIZE = newSize;
}

template <typename UINT> void bignum<UINT>::deflateSize(UINT newSize){
	if(newSize >= SIZE){
		return;
	}
	else if(newSize == 0){
		newSize = 1;
	}
	while(newSize < s.size()){
		s.erase(s.begin());
	}
	while(newSize < rem.size()){
		rem.erase(rem.begin());
	}
	SIZE = newSize;
}


/*
 * ADDITION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::add(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}

	UINT resh{0}, resl{0}, carry{0};
	uint64_t i=0;
	for(uint64_t i=0; i < SIZE; i++){
		rem[i] =0;
	}
	for(i = 0; i < SIZE && i < a.SIZE; i++){
		util_add_carry<UINT>(s[SIZE-1-i], a.s[a.SIZE-1-i],carry, resh, resl);
		s[SIZE-1-i] = resl;
		carry = resh;
	}

	if(carry > 0UL && i == SIZE){
		inflateSize(SIZE+1);
		s[0] = carry;
		rem[SIZE-1] = carry;
	}
}


/*
 * ADDITION FUNCTION WITH FIXED SIZE (NO OVERFLOW)
 *
 */

template <typename UINT> void bignum<UINT>::addFixed(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	UINT resh{0}, resl{0}, carry{0};
	uint64_t i=0;
	for(uint64_t i=0; i < SIZE; i++){
		rem[i] =0;
	}
	for(i = 0; i < SIZE && i < a.SIZE; i++){
		util_add_carry<UINT>(s[SIZE-1-i], a.s[a.SIZE-1-i],carry, resh, resl);
		s[SIZE-1-i] = resl;
		carry = resh;
	}
}

/*
 * MODULO FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::mod(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	// TEST DIV/0
	UINT zeros[a.SIZE];
	bignum<UINT> zero(zeros,a.SIZE);
	if(a.eq(zero)){
		throw std::overflow_error("Division by zero exception");
		return;
	}
	// first do division
	div(a);
	// then copy division remainder
	for(uint64_t i = 0; i < SIZE; i++){
		s[i] = rem[i];
		rem[i] = 0;
	}

}

/*
 * SUBTRACTION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::sub(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	bignum<UINT> tmparg{a};
	// SECOND COMPLEMENT METHOD
	for(uint64_t i = 0; i < a.SIZE; ++i){
		tmparg.s[i] = ~tmparg.s[i];
	}
	bignum<uint64_t> one{1};
	one.incr();
	tmparg.addFixed(one);			//prevent overflow if we do incr() -> ERROR
	addFixed(tmparg);
	return;
}

/*
 * MULTIPLICATION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::mult(const bignum &value){
	bignum<UINT> a{value};
	a.deflateSize(a.getBusySize());  // reduce 2nd factor's size to only filled locations (leading zeros ignored)
	
	const UINT finalSize = SIZE + a.SIZE;
	uint64_t resh{0}, resl{0};
	// unique pointer as array 
	std::unique_ptr<uint64_t[]> bnTmp = std::make_unique<uint64_t[]>(finalSize);
	for(uint64_t i = 0; i < SIZE; ++i){
		for(uint64_t j = 0; j < a.SIZE; ++j){
			util_mult<uint64_t>(s[SIZE-1-i], a.s[a.SIZE-1-j], resh, resl);
			uint64_t resh_add;
			int64_t shiftcarry = 0;

			do{
				util_add<uint64_t>(bnTmp[finalSize - 1-i-j-shiftcarry], resl, resh_add, bnTmp[finalSize - 1 -i-j-shiftcarry]);
				shiftcarry++;
				resl = resh_add;
			}
			while(resh_add > 0 && (finalSize - 1-i-j-shiftcarry) != (UINT)-1);
			shiftcarry = 0;
			do{
				util_add<uint64_t>(bnTmp[finalSize - 2-i-j-shiftcarry], resh, resh_add, bnTmp[finalSize - 2-i-j -shiftcarry]);
				shiftcarry++;
				resh = resh_add;
			}
			while(resh_add > 0 && (finalSize - 2-i-j - shiftcarry) != (UINT)-1);
		}
	}
	
	auto oldSize = SIZE;
	inflateSize(finalSize); //update SIZE


	for(uint64_t i = 0;i < SIZE; i++){
		s[i] = bnTmp[i];
	}
	//update rem
	for(uint64_t i = oldSize; i < finalSize; ++i){
		rem[i] = bnTmp[i-oldSize];
	}
	deflateSize(getBusySize());
}



/*
 * MULTIPLICATION FUNCTION - FIXED 
 *
 */

template <typename UINT> void bignum<UINT>::multFixed(const bignum &value){
	bignum<UINT> a{value};
	a.deflateSize(a.getBusySize());  // reduce 2nd factor's size to only filled locations (leading zeros ignored)
	
	const UINT finalSize = SIZE + a.SIZE;
	uint64_t resh{0}, resl{0};
	// unique pointer as array 
	std::unique_ptr<uint64_t[]> bnTmp = std::make_unique<uint64_t[]>(finalSize);
	for(uint64_t i = 0; i < SIZE; ++i){
		for(uint64_t j = 0; j < a.SIZE; ++j){
			util_mult<uint64_t>(s[SIZE-1-i], a.s[a.SIZE-1-j], resh, resl);
			uint64_t resh_add;
			int64_t shiftcarry = 0;

			do{
				util_add<uint64_t>(bnTmp[finalSize - 1-i-j-shiftcarry], resl, resh_add, bnTmp[finalSize - 1 -i-j-shiftcarry]);
				shiftcarry++;
				resl = resh_add;
			}
			while(resh_add > 0 && (finalSize - 1-i-j-shiftcarry) != (UINT)-1);
			shiftcarry = 0;
			do{
				util_add<uint64_t>(bnTmp[finalSize - 2-i-j-shiftcarry], resh, resh_add, bnTmp[finalSize - 2-i-j -shiftcarry]);
				shiftcarry++;
				resh = resh_add;
			}
			while(resh_add > 0 && (finalSize - 2-i-j - shiftcarry) != (UINT)-1);
		}
	}
	
	
	for(uint64_t i = finalSize-SIZE;i < finalSize; i++){
		s[i-finalSize+SIZE] = bnTmp[i];
	}
	//update rem
	for(uint64_t i = 0; i < finalSize-SIZE; ++i){
		rem[i] = bnTmp[i];
	}
	deflateSize(getBusySize());
}

/*
 * POWER FUNCTION
 * uses characteristic of powers a^(256*255*253) = ((a^256)^255)^253
 */


template <typename UINT> void bignum<UINT>::pow(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	UINT oneinit[SIZE] = {0};
	bignum<UINT> zero(oneinit,SIZE);
	oneinit[SIZE-1]= 1;
	bignum<UINT> one(oneinit, SIZE);
	if(eq(zero)){
		for(uint64_t i = 0; i < SIZE; i++){
			s[i] = one.s[i];
		}
	}
	if(eq(one)){
		return;
	}
	bignum<UINT> tmpPower(s,SIZE);
	bignum<UINT> tmp(one.s,SIZE);
	for (uint64_t i = 0; i < SIZE ; ++i){
		for(uint64_t bit = 0; bit < (sizeof(uint64_t)*8); bit++){
			uint64_t isone = (a.s[SIZE - 1 - i] & ((uint64_t)1 << (uint64_t) bit)); 	// those cast must be
																					// otherwise they are converted to 32-bit
																					// e.g. (1<<34) = 0x4
			// "this" is fixed until the end of the algorythm
			// Temporary value is stored in the tmp variable
			// with every increment (bit shift) multiply (square) tmpPower
			if(isone){
				// store temporary value by multiplying existing and current tmpPower result
				tmp.mult(tmpPower);
			}
			// square current value of tmpPower with every incrementation
			tmpPower.mult(tmpPower);
		}
	}
	for(uint64_t i = 0; i< SIZE; i++){
		s[i] = tmp.s[i];
	}
}


/*
 * POWER FUNCTION WITH MODULO
 * uses characteristic of powers a^(256*255*253) = ((a^256)^255)^253
 */


template <typename UINT> void bignum<UINT>::powmod(const bignum &value, const bignum &mod){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	UINT oneinit[SIZE] = {0};
	bignum<UINT> zero(oneinit,SIZE);
	oneinit[SIZE-1]= 1;
	bignum<UINT> one(oneinit, SIZE);
	if(eq(zero)){
		for(uint64_t i = 0; i < SIZE; i++){
			s[i] = one.s[i];
		}
	}
	if(eq(one)){
		return;
	}
	bignum<UINT> tmpPower(s,SIZE);
	bignum<UINT> tmp(one.s,SIZE);
	tmpPower.mod(mod);
	for (uint64_t i = 0; i < SIZE ; ++i){
		for(uint64_t bit = 0; bit < (sizeof(uint64_t)*8); bit++){
			uint64_t isone = (a.s[SIZE - 1 - i] & ((uint64_t)1 << (uint64_t) bit)); 	// those cast must be
																					// otherwise they are converted to 32-bit
			tmpPower.mod(mod);																		// e.g. (1<<34) = 0x4
			// "this" is fixed until the end of the algorythm
			// Temporary value is stored in the tmp variable
			// with every increment (bit shift) multiply (square) tmpPower
			if(isone){
				// store temporary value by multiplying existing and current tmpPower result
				tmp.mult(tmpPower);
				tmp.mod(mod);
			}
			// square current value of tmpPower with every incrementation
			tmpPower.mult(tmpPower);
		}
	}
	for(uint64_t i = 0; i< SIZE; i++){
		s[i] = tmp.s[i];
	}
}


/*
 * DIVISION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::div(const bignum &value){
	/*
	 *  dividend (*this) will be shifted left 1 bit, MSB is written to tmpdividend variable
	 *  tmp dividend will be compared wih a, if greater we write '1' to tmpresult
	 *  result will be stored into tmpres variable on the left side
	 *  remainder will be calculated (tmpdividend - a) and stored into tmpdividend for further calculations
	 */
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	UINT init[SIZE] = {0};
	bignum<UINT> tmpres(init,SIZE);		// temporary result
	// TEST DIV/0
	if(tmpres.eq(a)){
		throw std::overflow_error("Division by zero exception");
		return;
	}
	bignum<UINT> tmpinit(s);
	bignum<UINT> tmpdividend(init,SIZE);
	for(uint64_t i = 0; i < SIZE*sizeof(uint64_t)*8; i++){
		tmpres.shiftleft(1);
		tmpdividend.shiftleft(1);	// left shift tmpdividend
		tmpinit.shiftleft(1);		// left shift tmp init
		if(tmpinit.rem[SIZE-1] & 1){		// move carry to right most tmpdividend bit
			tmpdividend.s[SIZE-1] |= 1;
		}
		if(tmpdividend.lt(a)){	// write '0' to tmpres
			continue;
		}
		else{
			tmpres.s[SIZE-1] |= 1;  // write '1' to tmpres
			tmpdividend.sub(a);
		}
	}

	// COPY tmpres INTO MEMBER ARRAY
	for(uint64_t i = 0;i < SIZE; i++){
		s[i] = tmpres.s[i];
	}
	// COPY REMAINDER IN tmpdividend TO rem MEMBER ARRAY
	for(uint64_t i = 0;i < SIZE; i++){
		rem[i] = tmpdividend.s[i];
	}

}

template <typename UINT> void bignum<UINT>::print(){
	for(uint64_t i=0; i < SIZE; i++){
		if(rem[i] != 0)
			std::cout << std::hex << std::uppercase << rem[i] << " ";
	}
	for(uint64_t i=0; i < SIZE; i++){
		std::cout << std::hex << std::uppercase << s[i] << " ";
	}
	std::cout << "\n";
}

/*
 * TEST IF NUMBERS ARE EQUAL
 */
template <typename UINT> bool bignum<UINT>::eq(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	for(uint64_t i = 0; i < SIZE; i++){
		if(s[i] != a.s[i]){
			return false;
		}
	}
	return true;
}

/*
 * TEST IF IT IS LESS THAN ARGUMENT
 */
template <typename UINT> bool bignum<UINT>::lt(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	for(uint64_t i = 0; i < SIZE; i++){
		if(s[i] < a.s[i]){
			return true;
		}
		else if(s[i] > a.s[i]){
			return false;
		}
	}
	return false;
}

/*
 * TEST IF IT IS GREATER THAN ARGUMENT
 *
 */

template <typename UINT> bool bignum<UINT>::gt(const bignum &value){
	bignum<UINT> a{value};
	if(SIZE > a.SIZE){
		a.inflateSize(SIZE);
	}
	else if(SIZE < a.SIZE){
		inflateSize(a.SIZE);
	}
	for(uint64_t i = 0; i < SIZE; i++){
		if(s[i] > a.s[i]){
			return true;
		}
		else if(s[i] < a.s[i]){
			return false;
		}
	}
	return false;
}

template <typename UINT> void bignum<UINT>::incr(){
	UINT a[SIZE] = {0};
	a[SIZE-1] = 1;
	bignum one(a,SIZE);
	add(one);
}

/*
LEFT SHIFT 
*/

template <typename UINT> void bignum<UINT>::shiftleft(UINT nShifts){
	// REDUCE THE NUMBER OF nShifts to sizeof(UINT)*8
	UINT maxShifts = sizeof(UINT)*8 -1;
	nShifts %= maxShifts;
	UINT carry = 0;
	for(uint64_t i = 0; i < SIZE; ++i){
		UINT resh;
		util_shiftleft<UINT>(s[SIZE-1-i],nShifts,resh,s[SIZE-1-i]);
		s[SIZE-1-i] |= carry;	//insert carry from previous iteration
		carry = resh;  //UPDATE carry for the next iteration		
	}
	rem[SIZE-1] = carry;
}

/*
RIGHT SHIFT 
*/

template <typename UINT> void bignum<UINT>::shiftright(UINT nShifts){
	// REDUCE THE NUMBER OF nShifts to sizeof(UINT)*8
	UINT maxShifts = sizeof(UINT)*8 -1;
	nShifts %= maxShifts;
	UINT carry = 0;
	for(uint64_t i = 0; i < SIZE; ++i){
		UINT resh;
		util_shiftright<UINT>(s[i],nShifts,resh,s[i]);

		//   CONTINUE HERE!!!!
		s[i] |= carry;	//insert carry from previous iteration
		carry = resh;  //UPDATE carry for the next iteration		
	}
	rem[0] = carry;
}

template <typename UINT> const bignum<UINT> operator+ (const bignum<UINT> num1, const bignum<UINT> num2){
	bignum<UINT> a{num1};
	bignum<UINT> b{num2};
	a.add(b);
	return a;
}

template <typename UINT> bignum<UINT> operator- (const bignum<UINT> &num1, const bignum<UINT> &num2){
	bignum<UINT> a{num1};
	bignum<UINT> b{num2};
	a.sub(b);
	
	return a;
}


template <typename UINT> bignum<UINT> operator* (const bignum<UINT> &num1, const bignum<UINT> &num2){
	bignum<UINT> a{num1};
	bignum<UINT> b{num2};
	a.mult(b);
	return a;
}

template <typename UINT> bignum <UINT> operator/ (const bignum<UINT> &num1, const bignum<UINT> &num2){
	bignum<UINT> a{num1};
	bignum<UINT> b{num2};
	a.div(b);

	return a;
}

template <typename UINT> bignum <UINT> operator% (const bignum<UINT> &num1, const bignum<UINT> &num2){
	bignum<UINT> a{num1};
	bignum<UINT> b{num2};
	a.mod(b);
	return a;
}


/*
FUNCTION RETURNS NUMBER OF WORDS WITH NUMBERS
OMITTING LEADING ZEROS
*/
template <typename UINT> UINT bignum<UINT>::getBusySize(){
	// leading zeros are not counted!!!!
	UINT freeSize{0};
	// test from 0 to SIZE-1 if it's '0'
	// we don't test the LSB word so prevent SIZE = 0
	// size mustnot be zero!!!!
	for(uint64_t i = 0; i < SIZE-1; i++){
		if(s[i] == 0){
			// non busy
			freeSize++;
		}
		else{
			break;
		}
	}
	return SIZE-freeSize;
}

#endif /* BIGNUM_H_ */
