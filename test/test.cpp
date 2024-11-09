

// test_math.cpp
#define BOOST_TEST_MODULE MathTests
// #define BOOST_TEST_DYN_LINK - only for dynamic linking!!!!
#define BOOST_TEST_MAIN // This tells Boost to provide a main() function
#include <boost/test/unit_test.hpp>
#include "../bignum.h"

BOOST_AUTO_TEST_SUITE(MathTestSuite)

BOOST_AUTO_TEST_CASE(Test_Addition)
{
    bignum<uint64_t> a("5099406220130890465403251032250899774001031030",10);
    bignum<uint64_t> b("4545089807799898650545406056504540659880980770",10);
    bignum<uint64_t> c("9644496027930789115948657088755440433882011800",10);
    auto x = a + b;
    BOOST_ASSERT(x.eq(c));
}

BOOST_AUTO_TEST_CASE(Test_Multiplication)
{
    bignum<uint64_t> a("64065405895065403210",10);
    bignum<uint64_t> b("54065988098077051406",10);
    bignum<uint64_t> c("3463759472621081456483606472359287413260",10);
    auto x = a * b;
    x.print();
    BOOST_ASSERT(x.eq(c));
}

BOOST_AUTO_TEST_CASE(TestShift)
{
    bignum<uint64_t> a("65406126451403210654061264514032106540612645140321065406126451403210654061264",10);
    auto ac{a};
    uint64_t c[] = {0,0,0,0xF};
    bignum<uint64_t> b(c,sizeof(c)/sizeof(uint64_t));

    a.print();
    a.shiftleft(60);
    a.print();
    a.shiftright(8);
    b.print();
    b.shiftright(4);
    b.print();
    BOOST_TEST(b.rem[0] == 0xF000000000000000);
}

BOOST_AUTO_TEST_CASE(TestDivision)
{
    bignum<uint64_t> a("65406038159015901585260381590159015852603815901590158526038159015913105087968",10);
    auto ac{a};
    bignum<uint64_t> b("351403540354035403514035403540354035140354035403540351403540354035465406",10);
    auto x = a /b ;
    bignum<uint64_t> modulo = a % b;
    bignum<uint64_t> y = x *b;
    auto z = y + modulo;
    BOOST_TEST(ac.eq(z));
}



BOOST_AUTO_TEST_CASE(TestSubtraction)
{
    bignum<uint64_t> a("999998565016504999901231032109999023103099999012321035410321099991032109012310909999",10);
    bignum<uint64_t> b("45605214635400994502160999999999999960312109999999999999999352103105109999999",10);
    bignum<uint64_t> c("999998610621719635302225534270999023103099959324431035410321099990384212117420909998",10);
    auto x = c-b;
    BOOST_ASSERT(x.eq(a));
}


BOOST_AUTO_TEST_CASE(Test_Addition4)
{
    bignum<uint64_t> a("54521099405630999450536054106354032109999999999999999999999045361023103609",10);
    bignum<uint64_t> b("980546354099999405465402168909999994056314039999999935403654039999999999",10);
    bignum<uint64_t> c("55501645759730998856001456275264032104056314039999999935402699401023103608",10);
    auto x = a + b;
    BOOST_ASSERT(x.eq(c));
}

// This causes free():invalid pointer error!!!
BOOST_AUTO_TEST_CASE(Test_Addition1)
{
    bignum<uint64_t> a("54065061213210840959509450561403514035403514035403540354",10);
    bignum<uint64_t> b("3514035403540354035140354035403540351403540354035403514035403540354",10);
    bignum<uint64_t> c("3514035403594419096353564876363049801964943868070807028070807080708",10);
    auto x = a + b;
    BOOST_TEST(x.eq(c));
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
    uint64_t ca[] = {0,0,0,250};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    uint64_t cb[] = {0,0,0,10};
    bignum<uint64_t> b(cb,sizeof(ca)/sizeof(uint64_t));
    auto x = a/b;
    x.print();

    BOOST_TEST(x.s[x.SIZE-1] == 25);
}


BOOST_AUTO_TEST_CASE(SimpleTestDiv1)
{
    uint64_t ca[] = {0,0,0,18000000};
    bignum<uint64_t> a(ca,sizeof(ca)/sizeof(uint64_t));
    uint64_t cb[] = {0,0,0,10};
    bignum<uint64_t> b(cb,sizeof(ca)/sizeof(uint64_t));
    auto x = a/b;
    x.print();

    BOOST_TEST(x.s[x.SIZE-1] == 1800000);
}



BOOST_AUTO_TEST_SUITE_END()


