/*
 * bignum.h
 *
 *  Created on: Sep 22, 2024
 *      Author: tgjuranec
 */

#ifndef BIGNUM_H_
#define BIGNUM_H_
#include <cstdint>
#include "util.h"


template <typename UINT>
class bignum{
public:
    const UINT FULLSIZE = sizeof(UINT)*8; //half word size in bits
    const UINT UINTMAX = (((1 << (FULLSIZE-1))-1) | ((1 << (FULLSIZE-1))));
	UINT SIZE;
	UINT *s;
	UINT *rem;
	bignum(UINT a[], UINT size);
	~bignum();
	void print();
	void add(bignum &a);
	void mult(bignum &a);
	void sub(bignum &a);
	void pow(bignum &a);
	void powmod(bignum &a);
	bool lt(bignum &a);
	bool eq(bignum &a);
	bool gt(bignum &a);
	void incr();
	void mod(bignum &a);
	void div(bignum &a);
};

template <typename UINT> bignum<UINT>::bignum(UINT a[], UINT size){
	s = new UINT[size];
	rem = new UINT[size];

	SIZE = size;
	for(UINT i = 0; i < size; i++){
		s[i] = a[i];
		rem[i] = 0;
	}
}

template <typename UINT> bignum<UINT>::~bignum(){
	if(s != nullptr) delete [] s;
	s = nullptr;
	if(rem != nullptr) delete[] rem;
	rem = nullptr;
}


#include <iostream>

/*
 * ADDITION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::add(bignum &a){
	uint64_t resh{0}, resl{0}, carry{0};
	for(uint64_t i=0; i < SIZE; i++){
		util_add<uint64_t>(s[SIZE-1-i], a.s[SIZE-1-i], resh, resl);
		s[SIZE-1-i] = resl + carry;
		if(resl == UINTMAX && carry == 1){
			carry = 1;
		}
		else{
			carry = resh;
		}
	}
	for(uint64_t i=0; i < SIZE; i++){
		rem[i] =0;
	}
	if(carry == 1){
		rem[SIZE-1] = 1;
	}
}

/*
 * MODULO FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::mod(bignum &a){
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

template <typename UINT> void bignum<UINT>::sub(bignum &a){
	bignum<UINT> tmparg(a.s,SIZE);
	// SECOND COMPLEMENT METHOD
	for(uint64_t i = 0; i < SIZE; ++i){
		tmparg.s[i] = ~tmparg.s[i];
	}
	add(tmparg);
	incr();
	if(rem[SIZE-1] == 0){
		rem[SIZE-1] = 1;
	}
	else{
		rem[SIZE-1] = 0;
	}

	return;
}

/*
 * MULTIPLICATION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::mult(bignum &a){
	uint64_t resh{0}, resl{0};
	uint64_t bnTmp[SIZE*2] = {0};
	for(uint64_t i = 0; i < SIZE; ++i){
		for(uint64_t j = 0; j < SIZE; ++j){
			util_mult<uint64_t>(s[SIZE-1-i], a.s[SIZE-1-j], resh, resl);
			uint64_t resh_add;
			int64_t shiftcarry = 0;

			do{
				util_add<uint64_t>(bnTmp[2*SIZE - 1-i-j-shiftcarry], resl, resh_add, bnTmp[2*SIZE - 1 -i-j-shiftcarry]);
				shiftcarry++;
				resl = resh_add;
			}
			while(resh_add > 0 && (2*SIZE - 1-i-j-shiftcarry) > 0);
			shiftcarry = 0;
			do{
				util_add<uint64_t>(bnTmp[2*SIZE - 2-i-j-shiftcarry], resh, resh_add, bnTmp[2*SIZE - 2-i-j -shiftcarry]);
				shiftcarry++;
				resh = resh_add;
			}
			while(resh_add > 0 && (2*SIZE - 2-i-j - shiftcarry) > 0);
		}
	}
	for(uint64_t i = 0;i < SIZE; i++){
		s[i] = bnTmp[i+SIZE];
	}
	for(uint64_t i = 0;i < SIZE; i++){
		rem[i] = bnTmp[i];
	}
}

/*
 * POWER FUNCTION
 * uses characteristic of powers a^(256*255*253) = ((a^256)^255)^253
 */


template <typename UINT> void bignum<UINT>::pow(bignum &a){
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
 * DIVISION FUNCTION
 *
 */

template <typename UINT> void bignum<UINT>::div(bignum &a){
	/*
	 *  dividend (*this) will be shifted left 1 bit, MSB is written to tmpdividend variable
	 *  tmp dividend will be compared wih a, if greater we write '1' to tmpresult
	 *  result will be stored into tmpres variable on the left side
	 *  remainder will be calculated (tmpdividend - a) and stored into tmpdividend for further calculations
	 */
	UINT init[SIZE] = {0};
	bignum<UINT> tmpres(init,SIZE);		// temporary result
	bignum<UINT> tmpinit(s,SIZE);
	bignum<UINT> tmpdividend(init,SIZE);
	init[SIZE-1] = 2;
	bignum<UINT> bitshiftleft(init,SIZE);
	for(uint64_t i = 0; i < SIZE*sizeof(uint64_t)*8; i++){
		tmpres.mult(bitshiftleft);
		tmpdividend.mult(bitshiftleft);	// left shift tmpdividend
		tmpinit.mult(bitshiftleft);		// left shift tmp init
		if(tmpinit.rem[SIZE-1]){		// move carry to right most tmpdividend bit
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
	// COPY REMAINDE IN tmpdividend TO rem MEMBER ARRAY
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
template <typename UINT> bool bignum<UINT>::eq(bignum &a){
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
template <typename UINT> bool bignum<UINT>::lt(bignum &a){
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

template <typename UINT> bool bignum<UINT>::gt(bignum &a){
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

#endif /* BIGNUM_H_ */
