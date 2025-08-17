

// test_math.cpp
#define BOOST_TEST_MODULE MathTests
#define BOOST_TEST_DYN_LINK //- only for dynamic linking!!!!
//#define BOOST_TEST_MAIN // This tells Boost to provide a main() function
#include <boost/test/included/unit_test.hpp>
#include <random>
#include <cmath>
#include "../bignum.h"

BOOST_AUTO_TEST_SUITE(MathTestSuite)

constexpr uint64_t countmax = std::pow(10,5);
constexpr uint64_t countmaxmult = std::pow(10,5);

uint64_t extgcd_test(const uint64_t a, const uint64_t m){
    std::vector<uint64_t> as{m};
    uint64_t x1, y1, x, y;
    uint64_t tmpa = a, tmpm = m;
    while( tmpa > 0){
        as.push_back(tmpa);
        auto tmp = tmpa;
        tmpa = tmpm % tmpa;
        tmpm = tmp;        
    }
    if (as.back() != 1) return 0UL;
    x1 = 0;
    y1 = 1;
    for(auto it = as.crbegin(); it != (as.crend()-1); ++it){
        x = y1 - (*(it+1) / (*it))*x1;
        y = x1;
        x1 = x;
        y1 = y;
    }
    return (x + m) % m;
}



BOOST_AUTO_TEST_CASE(Test_ExtendedGCD)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> randu64;
    for(int i = 0; i < countmax; i++){    
        uint64_t ce[] = {65537};
        bignum<uint64_t> e{ce,1};
        uint64_t cm[] = {randu64(gen)};
        bignum<uint64_t> m{cm,1};
        auto dtest = extgcd_test(ce[0], cm[0]);
        if(dtest != 0){
            auto d = extendedGCD(e,m);
            BOOST_TEST(d.s[d.SIZE-1] == dtest);
        }
    }

}

BOOST_AUTO_TEST_CASE(Test_StringConversion)
{
std::string cp = {"00:e3:f7:18:c0:51:2b:02:88:b9:3a:29:91:0b:45:\
c0:7c:c6:f8:b0:c2:13:e1:67:2a:78:1a:2d:83:8f:\
ad:1a:84:2e:11:14:08:4a:0b:21:ec:35:b9:b8:c8:\
ab:03:9b:67:28:10:4a:f4:0a:ed:f4:76:f2:a4:f8:\
83:7a:a0:6d:1b:de:2d:28:9b:a0:5f:eb:69:fb:a9:\
63:ee:9a:2d:a1:bd:eb:e8:bc:d7:d2:f3:f7:bf:42:\
c7:94:87:38:98:fb:ab:62:1d:2e:dd:f8:1f:d9:33:\
ab:b8:28:d8:d0:d5:3e:51:94:3c:5f:d0:1a:9f:24:\
d2:22:e3:d6:6a:11:cb:ed:3f"};

std::string cq = {"00:e3:0d:7e:8d:d4:cd:17:61:72:fd:d2:b3:76:0d:\
d0:7a:e1:8c:12:84:44:eb:ee:47:75:af:0f:f9:86:\
9b:0d:42:11:94:a5:fc:cf:b4:9c:7a:8d:6e:a4:4f:\
42:cf:13:56:30:37:a9:e2:18:6e:f8:87:11:3c:dd:\
4e:b9:2c:07:9a:7e:07:7e:63:1c:55:28:90:e1:3c:\
2e:f6:a3:4e:61:c3:ed:f0:33:c3:94:46:d8:d3:6c:\
20:dc:53:88:1a:39:2d:8c:ce:4b:ca:c2:94:c9:b3:\
2c:60:c4:87:4d:cf:e7:7b:99:ce:93:24:2e:d0:19:\
56:88:17:6e:f4:c1:24:41:dd"};
std::string cmodulus = {"00:ca:30:1f:30:b4:83:ce:7e:4c:98:00:d5:d9:ab:\
13:66:48:d5:6b:1c:76:ea:24:92:c5:92:7e:37:96:\
a0:55:2a:a1:4f:b1:f6:5e:a7:a9:05:99:1f:12:04:\
ad:bd:9a:3b:a5:2e:c6:2a:7e:8e:a4:dd:4f:fa:ce:\
3a:bf:fc:31:44:8e:c3:39:a4:38:7b:5b:97:72:ed:\
22:06:b6:6a:92:1c:d9:22:ae:f2:71:06:f1:14:c8:\
1d:1e:03:de:02:cb:cd:70:ca:da:cf:de:e7:78:8a:\
17:c4:d5:f1:a1:25:fc:7c:ed:a5:d5:64:be:f0:af:\
e6:27:03:ee:7c:dc:4e:65:d2:3d:69:75:22:d0:4c:\
f0:7c:0e:7f:11:5d:c8:71:19:f9:0a:85:99:8b:d6:\
e0:03:dc:cb:5b:ad:38:fe:d6:3f:e1:4a:4a:7b:cf:\
d9:10:46:e9:94:08:0f:03:4b:9b:91:ef:b6:6a:32:\
94:1d:52:b5:43:5b:1a:3e:4f:26:85:12:95:a4:20:\
9a:54:ae:5b:20:cf:63:0b:a7:7c:ba:9f:e6:df:07:\
3f:e1:1c:20:cb:9c:23:61:f2:9f:b1:db:0a:5d:27:\
12:b5:84:60:00:ea:db:59:09:3b:e9:f0:41:e7:d3:\
5f:c4:29:dd:c1:bb:b9:8b:38:75:6e:78:ac:00:24:\
ce:63"};
uint64_t ce[] = {65537};
    
    bignum<uint64_t> p{cp,16};
    bignum<uint64_t> q{cq,16};
    bignum<uint64_t> n{cmodulus,16};
    bignum<uint64_t> e{ce,1};
    
    uint64_t cone[]{1};
    bignum<uint64_t> one{cone,sizeof(cone)/sizeof(uint64_t)};
    auto p_1 = p - one;
    auto q_1 = q - one;
    auto phi = p_1 * q_1;
    auto ncalc = p * q;
    auto d = extendedGCD(e,n);
    BOOST_TEST(n.eq(ncalc));
}



BOOST_AUTO_TEST_CASE(Test_Multiplication)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> randu64;
    for(uint64_t i = 0; i < countmax; i++){
        uint64_t ac[]{1,randu64(gen),randu64(gen),(uint64_t)-1};
        uint64_t bc[]{randu64(gen),randu64(gen),0};

        bignum<uint64_t> a{ac, sizeof(ac)/sizeof(uint64_t)};
        bignum<uint64_t> b{bc, sizeof(bc)/sizeof(uint64_t)};
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


BOOST_AUTO_TEST_CASE(SimpleTestDiv)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dividend;



    for(uint64_t i = 0; i < countmax; i++){
        auto nom1 = dividend(gen);
        auto nom2 = dividend(gen);
        auto nom3 = dividend(gen);
        auto denom = dividend(gen);
        auto denom1 = dividend(gen);
        bignum<uint64_t> a{std::to_string(nom1)+std::to_string(nom2) + std::to_string(nom3),10};
        bignum<uint64_t> b{std::to_string(denom) + std::to_string(denom1),10};
        auto x = a/b;
        auto m = a%b;
        auto z = x * b ;
        auto r = z + m;
        BOOST_TEST(a.eq(r));
    }
}

BOOST_AUTO_TEST_CASE(BIGBIGREALLYBIGNUM)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dividend;


    
    for(uint64_t i = 0; i < countmax; i++){
        std::string nom, denom;
        for(uint64_t j = 0; j < 20; j++){
            auto nom1 = dividend(gen);
            auto denom1 = dividend(gen) >> 16;
            nom += std::to_string(nom1);
            denom += std::to_string(denom1);
        }
        
        bignum<uint64_t> a{nom,10};
        bignum<uint64_t> b{denom,10};
        auto x = a/b;
        auto m = a%b;
        auto z = x * b ;
        auto r = z + m;
        BOOST_TEST(a.eq(r));
    }
}


BOOST_AUTO_TEST_SUITE_END()


