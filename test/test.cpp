

// test_math.cpp
#define BOOST_TEST_MODULE MathTests
// #define BOOST_TEST_DYN_LINK - only for dynamic linking!!!!
//#define BOOST_TEST_MAIN // This tells Boost to provide a main() function
#include <boost/test/unit_test.hpp>
#include <random>
#include <cmath>
#include "../bignum.h"

BOOST_AUTO_TEST_SUITE(MathTestSuite)

constexpr uint64_t countmax = std::pow(10,6);

BOOST_AUTO_TEST_CASE(Test_Multiplication)
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dividend(0,2000000000);



    for(uint64_t i = 0; i < countmax; i++){
        auto nom1 = dividend(gen);
        auto nom2 = dividend(gen);
        auto nom3 = dividend(gen);
        auto denom = dividend(gen);
        auto denom1 = dividend(gen);
        auto denom2 = dividend(gen);
        bignum<uint64_t> a{std::to_string(nom1)+std::to_string(nom2) + std::to_string(nom3),10};
        bignum<uint64_t> b{std::to_string(denom)+std::to_string(denom1)+std::to_string(denom2),10};
        auto x = a*b;
        auto z = x / b ;
        BOOST_TEST(a.eq(z));
    }
}




BOOST_AUTO_TEST_CASE(TestSubtraction)
{
   
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dividend(0,2000000000);



    for(uint64_t i = 0; i < countmax; i++){
        auto nom1 = dividend(gen);
        auto nom2 = dividend(gen);
        auto nom3 = dividend(gen);
        auto denom = dividend(gen);
        auto denom1 = dividend(gen);
        auto denom2 = dividend(gen);
        bignum<uint64_t> a{std::to_string(nom1)+std::to_string(nom2) + std::to_string(nom3),10};
        bignum<uint64_t> b{std::to_string(denom)+std::to_string(denom1),10};
        auto x = a-b;
        x.addfix(b);
        BOOST_TEST(a.eq(x));
    }
}


BOOST_AUTO_TEST_CASE(Test_Addition1)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dividend(0,2000000000);



    for(uint64_t i = 0; i < countmax; i++){
        auto nom1 = dividend(gen);
        auto nom2 = dividend(gen);
        auto nom3 = dividend(gen);
        auto denom = dividend(gen);
        auto denom1 = dividend(gen);
        auto denom2 = dividend(gen);
        bignum<uint64_t> a{std::to_string(nom1)+std::to_string(nom2) + std::to_string(nom3),10};
        bignum<uint64_t> b{std::to_string(denom)+std::to_string(denom1)+std::to_string(denom2),10};
        auto x = a+b;
        auto z = x - b ;
        BOOST_TEST(a.eq(z));
    }
}


BOOST_AUTO_TEST_CASE(Test_Busy1)
{
    uint64_t ca[] = {0,1,1,0};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    auto len = a.getBusySize();
    BOOST_TEST(len == 3ul);
}

BOOST_AUTO_TEST_CASE(Test_Busy2)
{
    uint64_t ca[] = {43,0,1,1,0};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    auto len = a.getBusySize();
    BOOST_TEST(len == 5ul);
}

BOOST_AUTO_TEST_CASE(Test_Busy3)
{
    uint64_t ca[] = {1,0,0,0,0};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    auto len = a.getBusySize();
    BOOST_TEST(len == 5ul);
}

BOOST_AUTO_TEST_CASE(Test_Busy4)
{
    uint64_t ca[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    auto len = a.getBusySize();
    BOOST_TEST(len == 1ul);
}

BOOST_AUTO_TEST_CASE(Test_Busy5)
{
    uint64_t ca[] = {0,0,0,0,0,0,0,0,0,0,5,5,1,7,0,0,0,0,0,0};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    auto len = a.getBusySize();
    BOOST_TEST(len == 10ul);
}


BOOST_AUTO_TEST_CASE(SimpleTestDiv)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dividend(0,2000000000);



    for(uint64_t i = 0; i < countmax; i++){
        auto nom1 = dividend(gen);
        auto nom2 = dividend(gen);
        auto nom3 = dividend(gen);
        auto denom = dividend(gen);
        bignum<uint64_t> a{std::to_string(nom1)+std::to_string(nom2) + std::to_string(nom3),10};
        bignum<uint64_t> b{std::to_string(denom),10};
        auto x = a/b;
        auto m = a%b;
        auto z = x * b ;
        auto r = z + m;
        BOOST_TEST(a.eq(r));
    }
}




BOOST_AUTO_TEST_SUITE_END()


