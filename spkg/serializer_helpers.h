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

#ifndef SERIALIZER_HELPERS_H
#define SERIALIZER_HELPERS_H
#include <type_traits>
#include <string>
namespace SpkG{
namespace helpers{
#define SPKGINT2STR(X) #X

static_assert(std::is_same<int,int32_t>::value, "int is not 32 bits on this compiler!");

// template <class T> inline std::string type_to_string<T, std::enable_if<std::is_enum<T>::value>::type>(){
//   return "a";
// }

template<class T, class EnumEnable = void>
struct NpStr{
  static const std::string value();
};

template<> struct NpStr<uint8_t>{static const std::string value(){return "u1";}};
template<> struct NpStr<uint16_t>{static const std::string value(){return "u2";}};
template<> struct NpStr<uint32_t>{static const std::string value(){return "u4";}};
template<> struct NpStr<uint64_t>{static const std::string value(){return "u8";}};
template<> struct NpStr<int8_t>{static const std::string value(){return "i1";}};
template<> struct NpStr<int16_t>{static const std::string value(){return "i2";}};
template<> struct NpStr<int32_t>{static const std::string value(){return "i4";}};
template<> struct NpStr<int64_t>{static const std::string value(){return "i8";}};
template<> struct NpStr<float>{static const std::string value(){return "f4";}};
template<> struct NpStr<double>{static const std::string value(){return "f8";}};

template<class T> struct NpStr<T, typename std::enable_if< std::is_enum<T>::value>::type>{
  static const std::string value(){return NpStr<typename std::underlying_type<T>::type>::value();};
};


template<typename...> struct typelist{};
template<typename T, typename ... Rest>
inline std::string call(typelist<T,Rest...>){
  return NpStr<T>::value() + "," + call(typelist<Rest...>());
}
inline std::string call(typelist<>) {return ""; }

template<std::size_t ... X> struct add_all : std::integral_constant< std::size_t,0 > {};
template<std::size_t X, std::size_t ... Xs> struct add_all<X,Xs...> : std::integral_constant< std::size_t, X + add_all<Xs...>::value > {};

}//namespace helpers

//https://stackoverflow.com/questions/12024304/c11-number-of-variadic-template-function-parameters
}//namespace SpkG
#endif // SERIALIZER_HELPERS_H
