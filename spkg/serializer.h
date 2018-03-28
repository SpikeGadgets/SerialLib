/*
Copyright (C) 2018 Kevin Wang kevin@spikegadgets.com

This program is free software: you can redistribute it and/or modify
                               it under the terms of the GNU General Public License as published by
                               the Free Software Foundation, either version 3 of the License, or
                               (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "serializer_helpers.h"
#include <array>
#include <string>

namespace SpkG {

template<typename...classes>
constexpr std::string toNumpyStr(){
    return helpers::call(helpers::typelist<classes...>());
}

template<typename... T> static constexpr 
size_t types_size(){
    return helpers::add_all<sizeof(T)...>::value;
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
template <typename ... Types>
class Serializer
{
public:
    Serializer() : _data({0}){

    }
    ~Serializer(){
    }


    /*!
     * \brief uint8_t* dataPtr()
     * Get a pointer to the internal buffer. DO NOT use this pointer and
     * modify the data yourself.
     * \return uint8_t* pointer to internal data buffer
     */
    const uint8_t* data() const{
        return _data.data();
    }

    /*!
     * \brief Get size of internal buffer
     * \return size_t internal buffer size
     */
    constexpr size_t size() const{
        return types_size<Types...>();
    }

    /*!
     * \brief Get numpy dtype string of this array's data types
     * \return std::string, numpy dtype string
     */
    constexpr std::string numpyType() const{
        return toNumpyStr<Types...>();
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
    void serialize(const H &head, const Tail & ... t){
        static_assert(sizeof(H) + types_size<Tail...>() == types_size<Types...>(), 
                      "Invalid arguments types, total size does not match Serializer's initialized size!");
        serialize_impl(&_data[0], head, t...);
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
   static void serializeIntoBuffer(uint8_t* data, const H &head, const Tail & ... t){
    //    constexpr size_t N = types_size<H,Tail...>();
       serialize_impl(data, head, t...);
   }

private:
    std::array<uint8_t, types_size<Types...>()> _data;

    template <typename H, typename... Tail, size_t pos = types_size<Types...>()-types_size<Tail...>()-sizeof(H)>
    static void serialize_impl(uint8_t *data, const H &head, const Tail & ... t){
        save<H>(data, pos, head);
        serialize_impl(data, t...);
    }
    static void serialize_impl(uint8_t *data){}

    template<typename T>
    static void save(uint8_t *data, const int pos, const T &val){
        *(T*)(data+pos) = val;
    }


    template <typename H, typename... Tail>
    static void deserialize_impl(const uint8_t *data, const size_t n, H &head, Tail & ... t){
        const int pos = n - types_size<H,Tail...>();
        load<H>(data, pos, head);
        deserialize_impl(data, n, t...);
    }
    static void deserialize_impl(const uint8_t *data, const size_t n){}

    template<typename T>
    static void load(const uint8_t *data, const int pos, T &val){
        val = *(T*)(data+pos);
    }
};

}//namespace SpkG
#endif // SERIALIZER_H
