

// test_math.cpp
#define BOOST_TEST_MODULE MathTests
#define BOOST_TEST_DYN_LINK - only for dynamic linking!!!!
//#define BOOST_TEST_MAIN // This tells Boost to provide a main() function
#include <boost/test/unit_test.hpp>
#include <random>
#include <cmath>
#include "../bignum.h"

BOOST_AUTO_TEST_SUITE(MathTestSuite)

constexpr uint64_t countmax = std::pow(10,2);
constexpr uint64_t countmaxmult = std::pow(10,8);

BOOST_AUTO_TEST_CASE(ReproduceError){
    bignum<uint64_t> a{"209865F5C3CA30A9D70585F3",16};
    bignum<uint64_t> b{"98203D20A5FCC204",16};
    // bignum<uint64_t> x{"135E975E23733CFA5888984BDECB55BF05CC3DCC",16};
    uint64_t cx[] = {1,0,0};
    bignum<uint64_t> x{cx,sizeof(cx)/sizeof(uint64_t)};

    auto z = x/b;
    bignum<uint64_t> remain{z.rem,z.SIZE};

    std::cout << "z = " << z.to_string_bin() << "\n";
    std::cout << "a = " << a.to_string_bin() << "\n";
    BOOST_TEST(a.eq(z), "\na: " << a.to_string() << 
        "\nb: " << b.to_string() <<
        "\nx: " << x.to_string() <<
        "\nz: " << z.to_string() << 
        "\n remainder: " << remain.to_string() << "\n");
}

/*
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
        BOOST_TEST(a.eq(z), "\na: " << a.to_string() << 
        "\nb: " << b.to_string() <<
        "\nx: " << x.to_string() <<
        "\nz: " << z.to_string());
    }
}


BOOST_AUTO_TEST_CASE(Test_Multiplication2)
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dividend;
    for(uint64_t i = 0; i < countmaxmult; i++){
        auto nom1 = dividend(gen);
        auto nom2 = dividend(gen);
        auto nom3 = dividend(gen);
        auto denom = dividend(gen);
        auto denom2 = dividend(gen);
        uint64_t ac[] = {nom1};
        uint64_t bc[] = {denom};
        bignum<uint64_t> a{ac,1};
        bignum<uint64_t> b{bc,1};
        auto x = a*b;
        auto z = x / b ;
        BOOST_TEST(a.eq(z), "\na: " << a.to_string() << 
        "\nb: " << b.to_string() <<
        "\nx: " << x.to_string() <<
        "\nz: " << z.to_string());
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
        if(i%100 == 1) {
            // std::cout << a.to_string() << " - " << b.to_string() << " = " << x.to_string() << "\n";
        }
        x.addFixed(b);
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


*/

BOOST_AUTO_TEST_SUITE_END()


