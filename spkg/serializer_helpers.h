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


//    https://stackoverflow.com/questions/11184223/c-function-variadic-templates-with-no-arguments
template <class T>inline std::string type_to_string();
template <> inline std::string type_to_string<bool>()      {return "?";}
template <> inline std::string type_to_string<uint8_t>()   {return "u1";}
template <> inline std::string type_to_string<uint16_t>()  {return "u2";}
template <> inline std::string type_to_string<uint32_t>()  {return "u4";}
template <> inline std::string type_to_string<uint64_t>()  {return "u8";}
template <> inline std::string type_to_string<int8_t>()    {return "i1";}
template <> inline std::string type_to_string<int16_t>()   {return "i2";}
template <> inline std::string type_to_string<int>()       {return "i4";}
//template <> std::string type_to_string<int32_t>()   {return "i4";}
template <> inline std::string type_to_string<int64_t>()   {return "i8";}
template <> inline std::string type_to_string<float>()     {return "f4";}
template <> inline std::string type_to_string<double>()    {return "f8";}

static_assert(std::is_same<int,int32_t>::value, "Int is not 32 bits!");

template<typename...> struct typelist{};
template<typename T, typename ... Rest>
inline std::string call(typelist<T,Rest...>){
  return type_to_string<T>() + "," + call(typelist<Rest...>());
}
inline std::string call(typelist<>) {return ""; }

template<typename...classes>
constexpr std::string toNumpyStr(){
      return call(typelist<classes...>());
}

template<std::size_t ... X> struct add_all : std::integral_constant< std::size_t,0 > {};
template<std::size_t X, std::size_t ... Xs> struct add_all<X,Xs...> : std::integral_constant< std::size_t, X + add_all<Xs...>::value > {};
template<typename... T> static constexpr size_t types_size(){return add_all<sizeof(T)...>::value;}
// https://stackoverflow.com/questions/12024304/c11-number-of-variadic-template-function-parameters

}//namespace SpkG
#endif // SERIALIZER_HELPERS_H
