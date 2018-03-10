# Serialize library

A serialization library for auto generating C-style pointer serialization. Main
goal is so that users don't have to deal with pointer math but sacrifice
performance when serializing simple arithmetic types.

Also, another goal is to statically generate numpy dtype strings for any
instance of the Serializer class using the passed in types.

```cpp
Serializer s;
s.initializeBuffer<int, float>();
s.serialize(1, 2.1);
```

is the same as

```c
unsigned char buffer = new unsigned char[8];
*(int*)(buffer) = 1;
*(float*)(buffer+sizeof(int)) = 2.1;
```

TODO:

- numpy strings generator
- Make Serializer hold a static std::array instead of a vector when using
  internal buffer, so that nearly everything is decided on compile time
