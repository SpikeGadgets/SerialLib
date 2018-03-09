#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "binarydata.h"
//#include <array>
#include <vector>
#include <iostream>

// https://stackoverflow.com/questions/12024304/c11-number-of-variadic-template-function-parameters
// Ignore these three functions
template<std::size_t ... X>
struct add_all : std::integral_constant< std::size_t,0 > {};

template<std::size_t X, std::size_t ... Xs>
struct add_all<X,Xs...> : std::integral_constant< std::size_t, X + add_all<Xs...>::value > {};

template<typename... T>
constexpr size_t types_size(){
    return add_all<sizeof(T)...>::value;
}



//! Serializer is a class that uses template magic to generate at compile-time the
//! necessary pointer math for serializing a buffer with a set of values of varying types.
//! It also contains a deserialize function to load values from a buffer into values
//!
//! Numeric values only
//!
//! The user is responsible for the ordering of types! If you serialize in one order and
//! deserialize in different order, the data will be wrong
//!
/*!
    Serializer s;
    uint8_t *data;
    s.initializeBuffer<int,double>();
    {
        int i = 1; double f = 1.123;
        data = s.serialize(i,f);
    }
    {
        int j; double k;
        s.deserialize(data, j, k);
    }
 */
class Serializer
{
public:
    Serializer();
    ~Serializer(){
        data.clear();
    }

    /*!
     * \brief Initialize a buffer to store
     */
    template <typename H, typename... Tail>
    void initializeBuffer(){
        constexpr size_t N=types_size<H, Tail...>();
        data.resize(N);
    }

    template <typename H, typename... Tail, size_t N=types_size<H, Tail...>()>          //Variadic Template declaration
    uint8_t* serialize(const H &head, const Tail & ... t){
        serialize_impl(data.data(), data.size(), head, t...);
        return &data[0];
    }


    template <typename H, typename... Tail>
    static void serializeInto(uint8_t* data, const H &head, const Tail & ... t){
        serialize_impl(data, types_size<H,Tail...>(), head, t...);
    }


    template <typename H, typename... Tail, size_t N = types_size<H,Tail...>()>
    static void deserialize(const uint8_t *data, H &head, Tail & ... t){
        deserialize_impl(data, N, head, t...);
        return;
    }

private:
    std::vector<uint8_t> data;

    template <typename H, typename... Tail>
    static void serialize_impl(uint8_t *data, const size_t n, const H &head, const Tail & ... t){
        int pos = n - types_size<H,Tail...>();
        save<H>(data, pos, head);
        serialize_impl(data, n, t...);
    }
    static void serialize_impl(uint8_t *data, const size_t n){}

    template<typename T>
    static void save(uint8_t *data, int pos, const T &val){
        *(T*)(data+pos) = val;
    }


    template <typename H, typename... Tail>
    static void deserialize_impl(const uint8_t *data, const size_t n, H &head, Tail & ... t){
        int pos = n - types_size<H,Tail...>();
        load<H>(data, pos, head);
        deserialize_impl(data, n, t...);
    }
    static void deserialize_impl(const uint8_t *data, const size_t n){}

    template<typename T>
    static void load(const uint8_t *data, int pos, T &val){
        val = *(T*)(data+pos);
    }
};

#endif // SERIALIZER_H
