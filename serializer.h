#ifndef SERIALIZER_H
#define SERIALIZER_H

//#include "binarydata.h"
//#include <array>
#include <vector>
#include <iostream>

struct BinaryData{
    uint8_t *dataptr;
    size_t size;
};

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
   Example:
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
private:
    // Ignore these three functions
    template<std::size_t ... X> struct add_all : std::integral_constant< std::size_t,0 > {};
    template<std::size_t X, std::size_t ... Xs> struct add_all<X,Xs...> : std::integral_constant< std::size_t, X + add_all<Xs...>::value > {};
    template<typename... T> static constexpr size_t types_size(){return add_all<sizeof(T)...>::value;}
    // https://stackoverflow.com/questions/12024304/c11-number-of-variadic-template-function-parameters

public:
    Serializer(){}
    ~Serializer(){
        _data.clear();
    }

    /*!
     * \brief size_t initializeBuffer<Types...>()
     * Initialize an internal buffer to store these types.
     *
     */
    template <typename H, typename... Tail>
    size_t initializeBuffer(){
        constexpr size_t N = types_size<H, Tail...>();
        _data.resize(N);
        return N;
    }

    /*!
     * \brief uint8_t* dataPtr()
     * Get a pointer to the internal buffer. DO NOT use this pointer and
     * modify the data yourself.
     * \return uint8_t* pointer to internal data buffer
     */
    const uint8_t* dataPtr() const{
        return _data.data();
    }

    /*!
     * \brief Get size of internal buffer
     * \return size_t internal buffer size
     */
    size_t size() const{
        return _data.size();
    }

    /*!
     * \brief Serialize these values into the internal buffer.
     * \return Returns a pointer to the internal buffer. DO NOT use the pointer for
     * anything but to copy the data.
     *
     * Example usage:
     * int i=1,j=2,k=3; float f=3.14; uint32_t t=12345;
     * uint8_t *data = serialize(i, j, k, f, t); //data now points to the buffer
     */
    template <typename H, typename... Tail>          //Variadic Template declaration
    uint8_t* serialize(const H &head, const Tail & ... t){
        serialize_impl(_data.data(), _data.size(), head, t...);
        return &_data[0];
    }

    /*!
     * \brief Serialize these values into your own buffer.
     * There is no size parameter, so your buffer *must* be large enough.
     *
     * Example usage:
     * int i=1,j=2,k=3; float f=3.14; uint32_t t=12345; //int=4bytes,float=4,uint32=4
     * uint8_t data[20];
     * Serializer::serializeInto(data, i, j, k, f, t); //Values are copied into data[]
     */
    template <typename H, typename... Tail>
    static void serializeInto(uint8_t* data, const H &head, const Tail & ... t){
        constexpr size_t N = types_size<H,Tail...>();
        serialize_impl(data, N, head, t...);
    }

    /*!
     * \brief Get the size of these types in bytes, known at compile time
     * \return Returns the size
     *
     * Example usage:
     * size_t size = Serializer::sizeofTypes<int8_t, int16_t, int32_t, int64_t>();
     * //size = 1 + 2 + 4 + 8 = 15
     */
    template <typename H, typename... Tail>
    constexpr static size_t sizeofTypes(){
        constexpr size_t N = types_size<H,Tail...>();
        return N;
    }

    /*!
     * \brief Deserialize values from a buffer into the variables passed by reference
     * There is no size parameter, so the buffer to read from *must* be large enough.
     *
     * Example usage:
     * uint8_t *data = dataBufferFromSomeOtherProgram();
     * int i,j,k; float f; uint32_t t;
     * Serialize::deserialize(buffer, i, j, k, f, t);
     */
    template <typename H, typename... Tail, size_t N = types_size<H,Tail...>()>
    static void deserialize(uint8_t *data, H &head, Tail & ... t){
        deserialize_impl(data, N, head, t...);
    }

private:
    std::vector<uint8_t> _data;

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
