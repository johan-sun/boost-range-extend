#define BOOST_TEST_MODULE RangeTest

#include  <iterator>
#include  <vector>
#include  <iostream>
#include  <boost/test/unit_test.hpp>
#include  <boost/range/algorithm.hpp>
#include  <boost/range/algorithm_ext.hpp>
#include  <boost/assign.hpp>
#include    "range_extend.hpp"


BOOST_AUTO_TEST_SUITE(test_normal_alg)

BOOST_AUTO_TEST_CASE(test_foreach_normal){
    using namespace std;
    using namespace boost::assign;

    vector<int> v{1,2,3};

    vector<int> outs;
    boost::foreach_adjacent(std::begin(v), std::end(v),[&](int a, int b){
            outs += a,b;
            });

    BOOST_CHECK_EQUAL(outs[0], 1);BOOST_CHECK_EQUAL(outs[1], 2);
    BOOST_CHECK_EQUAL(outs[2], 2);BOOST_CHECK_EQUAL(outs[3], 3);


    outs.clear();
    boost::foreach_adjacent(v,[&](int a, int b){
            outs += a, b;
            });

    BOOST_CHECK_EQUAL(outs[0], 1);BOOST_CHECK_EQUAL(outs[1], 2);
    BOOST_CHECK_EQUAL(outs[2], 2);BOOST_CHECK_EQUAL(outs[3], 3);
}

BOOST_AUTO_TEST_CASE(test_foreach_with_prepare){
    using namespace std;
    using namespace boost::assign;

    vector<int> v = {1,2,3};
    vector<int> outs;

    boost::foreach_adjacent<boost::with_prepare>(std::begin(v),std::end(v),
            [&outs](int first){
            outs += first;
            }, [&outs](int a, int b){
            outs += a, b;
            });

    BOOST_CHECK_EQUAL(outs[0], 1);
    BOOST_CHECK_EQUAL(outs[1], 1);BOOST_CHECK_EQUAL(outs[2], 2);
    BOOST_CHECK_EQUAL(outs[3], 2);BOOST_CHECK_EQUAL(outs[4], 3);

    outs.clear();

    boost::foreach_adjacent<boost::with_prepare>(v,[&](int first){
            outs += first;
            },[&](int a,int b){
            outs += a,b;
            });

    BOOST_CHECK_EQUAL(outs[0], 1);
    BOOST_CHECK_EQUAL(outs[1], 1);BOOST_CHECK_EQUAL(outs[2], 2);
    BOOST_CHECK_EQUAL(outs[3], 2);BOOST_CHECK_EQUAL(outs[4], 3);
}


BOOST_AUTO_TEST_CASE(test_foreach_with_cleanup){
    using namespace std;
    using namespace boost::assign;

    vector<int> v = {1,2,3};
    vector<int> outs;
    boost::foreach_adjacent<boost::with_cleanup>(std::begin(v),std::end(v),
            [&outs](int a, int b){
            outs += a,b;
            },[&outs](int last){
            outs += last;
            }
            );

    BOOST_CHECK_EQUAL(outs[0], 1);BOOST_CHECK_EQUAL(outs[1], 2);
    BOOST_CHECK_EQUAL(outs[2], 2);BOOST_CHECK_EQUAL(outs[3], 3);
    BOOST_CHECK_EQUAL(outs[4], 3);

    outs.clear();
    boost::foreach_adjacent<boost::with_cleanup>(v,[](int& a, int& b){a = b = 0;},[](int& last){last = -1;});
    BOOST_CHECK_EQUAL(v[0],0);
    BOOST_CHECK_EQUAL(v[1],0);
    BOOST_CHECK_EQUAL(v[2],-1);
}

BOOST_AUTO_TEST_CASE(test_copy){
    std::vector<int> v{1,2,3};
    std::vector<int> outs;
    using namespace std;
    using namespace boost::assign;

    boost::copy_adjacent(v, back_inserter(outs),[](int a, int b){
            return b - a;
            });

    BOOST_CHECK_EQUAL(outs.size(), 2);

    vector<int> correct = {1,1};
    BOOST_CHECK_EQUAL_COLLECTIONS(outs.begin(),outs.end(), correct.begin(), correct.end());

    outs.clear();
    boost::copy_adjacent<boost::with_prepare>(v, back_inserter(outs),[](int first){ return 0; },[](int a, int b){ return b - a; });
    correct = {0, 1, 1};
    BOOST_CHECK_EQUAL_COLLECTIONS(outs.begin(),outs.end(), correct.begin(), correct.end());

    outs.clear();
    boost::copy_adjacent<boost::with_cleanup>(v, back_inserter(outs),[](int a, int b){return b-a;}, [](int last){ return -1; });
    correct = {1,1,-1};
    BOOST_CHECK_EQUAL_COLLECTIONS(outs.begin(),outs.end(), correct.begin(), correct.end());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(test_range_and_adaptors)

    BOOST_AUTO_TEST_CASE(adaptors){
    }

BOOST_AUTO_TEST_SUITE_END()
#include  <boost/test/included/unit_test.hpp>
