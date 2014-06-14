#include    "range_extend.hpp"
#include  <vector>
#include  <iostream>
#include  <iterator>
#include  <boost/format.hpp>
using namespace std;
int main(int argc, char *argv[])
{
    std::vector<int> v = {1,2,3,4};
    boost::foreach_adjacent<boost::with_prepare>(v,[](int i){ cout << i << endl;  },
            [](int a, int b){ cout << "("<<a<<","<<b<<")"<<endl; });

    boost::foreach_adjacent<boost::with_cleanup>(vector<int>{1,2,3,4,5},[](int a, int b){ cout << "("<<a<<","<<b<<")"<<endl;},
            [](int last){ cout << last << endl; });


    auto string_maker = [](int a, int b){ return (boost::format("(%d,%d)")%a%b).str(); };
    auto last_maker = [](int last){ return (boost::format("(%d,nil)") % last).str(); };
    auto first_maker = [](int first){ return (boost::format("(nil,%d)")%first).str(); };

    //copy_adjacent(v, ostream_iterator<string>(cout,","), string_maker);
    //cout << endl;
    //copy_adjacent<with_cleanup>(v, ostream_iterator<string>(cout,","), string_maker,last_maker);
    //cout << endl;
    //copy_adjacent<with_prepare>(v, ostream_iterator<string>(cout,","), first_maker, string_maker);
    //cout << endl << endl;



    std::vector<int> v2 = { 5,6,7,8 };


    //for(auto i : zip_range(v,v2)){
    //    cout << i.get<0>() << i.get<1>() << endl;
    //}

    for(auto tup : v2 | boost::adjacented){
        tup.get<0>() = 0;
        tup.get<1>() = 1;
    }

    for(auto tup: v2 | boost::adjacented){
        cout << string_maker(tup.get<0>(), tup.get<1>()) << endl;
    }


    cout <<"test enumerate"<<endl;
    for(auto tup:  v2 | boost::adjacented | boost::enumerated(1)){
        auto & adjtup = tup.get<1>();
        cout << tup.get<0>() << ":" << string_maker(adjtup.get<0>(), adjtup.get<1>()) << endl;
    }

    return 0;
}
