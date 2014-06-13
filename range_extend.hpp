#ifndef  RANGE_EXTEND_HPP
#define  RANGE_EXTEND_HPP




#include  <boost/range.hpp>
#include  <boost/range/adaptors.hpp>
#include  <boost/iterator.hpp>
#include  <boost/iterator/zip_iterator.hpp>
#include  <type_traits>
#include  <tuple>

struct with_prepare{};
struct with_cleanup{};

template<typename InputIterator, typename BinaryFunction>
BinaryFunction foreach_adjacent(InputIterator first, InputIterator last, BinaryFunction fun){
    InputIterator second = first + 1;
    while(first != last and second != last){
        fun(*first++, *second++);
    }
    return fun;
}

template<typename SinglePassRange, typename BinaryFunction>
BinaryFunction foreach_adjacent(SinglePassRange && rng, BinaryFunction fun){
    return foreach_adjacent(boost::begin(std::forward<SinglePassRange>(rng)), boost::end(std::forward<SinglePassRange>(rng)), fun);
}

template<typename Tag, typename InputIterator, typename UnaryFunction,typename BinaryFunction>
typename std::enable_if<
    std::is_same<with_prepare,Tag>::value,
    BinaryFunction>::type 

    foreach_adjacent(InputIterator first, InputIterator last, UnaryFunction prepare, BinaryFunction fun){
    if (first != last)
        prepare(*first);
    InputIterator second = first + 1;
    while(first != last and second != last){
        fun(*first++, *second++);
    }
    return fun;
}


template<typename Tag, typename SinglePassRange, typename UnaryFunction,typename BinaryFunction>
typename std::enable_if<
    std::is_same<with_prepare,Tag>::value,
    BinaryFunction>::type 

foreach_adjacent(SinglePassRange&& rng, UnaryFunction prepare, BinaryFunction fun){
    return foreach_adjacent<Tag>(boost::begin(std::forward<SinglePassRange>(rng)), boost::end(std::forward<SinglePassRange>(rng)),
                prepare, fun
                );
}


template<typename Tag, typename InputIterator, typename UnaryFunction, typename BinaryFunction>
typename std::enable_if<
    std::is_same<with_cleanup, Tag>::value,
    UnaryFunction
    >::type

foreach_adjacent(InputIterator first, InputIterator last, BinaryFunction fun, UnaryFunction cleanup){
    InputIterator second = first + 1;
    if(second == last)
        cleanup(*first);
    while(first != last and second != last){
        fun(*first++, *second++);
        if(second == last)
            cleanup(*first);
    }
    return cleanup;
}

template<typename Tag, typename SinglePassRange, typename UnaryFunction, typename BinaryFunction>
typename std::enable_if<
    std::is_same<with_cleanup, Tag>::value,
    UnaryFunction
    >::type

foreach_adjacent(SinglePassRange&& rng, BinaryFunction fun, UnaryFunction cleanup){
    return foreach_adjacent<Tag>(boost::begin(std::forward<SinglePassRange>(rng)), boost::end(std::forward<SinglePassRange>(rng)),
            fun, cleanup);
}

template<typename InputIterator, typename OutputIterator, typename BinaryFunction>
OutputIterator copy_adjacent(InputIterator first, InputIterator last, OutputIterator d_first, BinaryFunction fun){
    InputIterator second = first + 1;
    while(first != last and second != last){
        *d_first++ = fun(*first++, *second++);
    }
    return d_first;
}

template<typename SinglePassRange, typename OutputIterator, typename BinaryFunction>
OutputIterator copy_adjacent(SinglePassRange&& rng, OutputIterator d_first, BinaryFunction fun){
    return copy_adjacent(boost::begin(std::forward<SinglePassRange>(rng)), boost::end(std::forward<SinglePassRange>(rng)),
            d_first, fun);
}

template<typename Tag, typename InputIterator, typename OutputIterator, typename BinaryFunction, typename UnaryFunction>
typename std::enable_if<
    std::is_same<Tag, with_prepare>::value,
    OutputIterator>::type
copy_adjacent(InputIterator first, InputIterator last, OutputIterator d_first, UnaryFunction prepare, BinaryFunction fun){
    InputIterator second = first + 1;
    if(first != last)
        *d_first++ = prepare(*first);
    while(first != last and second != last){
        *d_first++ = fun(*first++, *second++);
    }
    return d_first;
}


template<typename Tag, typename SinglePassRange, typename OutputIterator, typename BinaryFunction, typename UnaryFunction>
typename std::enable_if<
    std::is_same<Tag, with_prepare>::value,
    OutputIterator>::type
copy_adjacent(SinglePassRange&& rng, OutputIterator d_first, UnaryFunction prepare, BinaryFunction fun){
    return copy_adjacent<Tag>(boost::begin(std::forward<SinglePassRange>(rng)), boost::end(std::forward<SinglePassRange>(rng)),
            d_first, prepare, fun);
}


template<typename Tag, typename InputIterator, typename OutputIterator, typename BinaryFunction, typename UnaryFunction>
typename std::enable_if<
    std::is_same<Tag, with_cleanup>::value,
    OutputIterator>::type
copy_adjacent(InputIterator first, InputIterator last, OutputIterator d_first, BinaryFunction fun, UnaryFunction cleanup){
    InputIterator second = first + 1;
    if(second == last)
        *d_first++ = cleanup(*first);
    while(first != last and second != last){
        *d_first++ = fun(*first++, *second++);
        if(second == last){
            *d_first++ = cleanup(*first);
        }
    }
    return d_first;
}

template<typename Tag, typename SinglePassRange, typename OutputIterator, typename BinaryFunction, typename UnaryFunction>
typename std::enable_if<
    std::is_same<Tag, with_cleanup>::value,
    OutputIterator>::type
copy_adjacent(SinglePassRange&& rng, OutputIterator d_first, BinaryFunction fun, UnaryFunction cleanup){
    return copy_adjacent<Tag>(boost::begin(std::forward<SinglePassRange>(rng)), boost::end(std::forward<SinglePassRange>(rng)),
            d_first, fun, cleanup);
}



#define MAKE_ZIP_RANGE_BODY \
boost::make_iterator_range(\
            boost::make_zip_iterator(\
                std::make_tuple(boost::begin(ranges)...)\
                ),\
            boost::make_zip_iterator( \
                std::make_tuple(boost::end(ranges)...)\
                )\
            )

//template<typename ...Ranges>
//auto zip_range(Ranges&&... ranges)-> decltype(MAKE_ZIP_RANGE_BODY) {
//    return MAKE_ZIP_RANGE_BODY;
//}

template<typename ...Ranges>
boost::iterator_range<
    boost::zip_iterator<
        boost::tuple<
            typename boost::range_iterator<typename std::remove_reference<Ranges>::type>::type...
        >
    >
>
zip_range(Ranges&&... ranges){
    return  boost::make_iterator_range(
                boost::make_zip_iterator(
                    boost::make_tuple(boost::begin(ranges)...)
                ),
                boost::make_zip_iterator(
                    boost::make_tuple( boost::end(ranges)...)
                )
            );
}
#endif  /*RANGE_EXTEND_HPP*/
