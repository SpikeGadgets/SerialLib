# Serialize library

- [Serialize library](#serialize-library)
  - [C style "serialization" in C++11](#c-style-serialization-in-c11)
  - [Numpy dtype string generator](#numpy-dtype-string-generator)
  - [About](#about)
  - [TODO/Possible features](#todopossible-features)

A serialization library for auto generating C-style pointer serialization. The main
goal is so that users don't have to deal with pointer math but sacrifice
performance when serializing simple arithmetic types.

Also, another goal is to statically generate numpy dtype strings for any
instance of the Serializer class using the passed in types.

## C style "serialization" in C++11

Using the library, the following code

```cpp
SpkG::Serializer s;
s.initializeBuffer<int, double, uint32_t>();
s.serialize(1, 2.3456, 700000);
foo_SendBuffer(s.data(), s.size());
```

is the same as

```cpp
unsigned char *buffer = new unsigned char[16];
int size = 0;
*(int*)(buffer + size) = 1;
size += sizeof(int);

*(double*)(buffer+size) = 2.3456;
size += sizeof(double);

*(uint32_t*)(buffer + size) = 700000;
size += sizeof(uint32_t);

foo_SendBuffer(buffer, size);
```

The code generated by C++11 templates is the same as if you'd used raw pointers to serialize a buffer of bytes. This way, there can be compile-time type checking, and verification that you didn't mess up anything. It also preserves the speed of serializing these basic types.

## Numpy dtype string generator

```cpp
//Continuing from code snippet above
std::string numpystring = s.numpyType();
//numpystring = "i4,f8,u4,"

bar_sendNumpyString(numpystring);
```

The above code will generate, at compile time, a string containing the equivalent numpy.dtype needed to interpret the buffer you created in C++ in Python numpy. Obviously, this library doesn't take care of how you send the data buffer over, but in our own new messaging API, we'd like to have our users access the same data in C++ as well as Python.

```python
import numpy
npstr = bar_recvNumpyString() #Some function that gets you the string (messaging, hardcoded in lib, etc)
buf = bar_recvCppBytes()      #Some function that gets you the buffer (messaging, file, etc)
dt = numpy.dtype(npstr)
dat = numpy.frombuffer(buf, dtype = dt)
#dat[0] = (1, 2.3456, 700000)
```

## About

This library was written by Kevin Wang  for [SpikeGadgets]("http://www.spikegadgets.com") open source software [Trodes]("https://bitbucket.org/mkarlsso/trodes").

## TODO/Possible features

- Static asserts and compile error messages for things like compatible types
- std::array's; their size is known at compile time
- dynamic vectors? make a compile-time MAXSIZE and each vector starts with a size_t for the length, rest of data is zero'd out
- ability for python code to serialize