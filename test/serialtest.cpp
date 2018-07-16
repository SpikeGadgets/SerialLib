#include "serialtest.h"
#include <cassert>
#include <array>
void serialtest(){
    uint8_t *datacpy;
    enum class E {e1, e2, e3};
    const int i = 1; 
    const double f = 1.123; 
    const uint32_t tm = 5;
    const E e = E::e2;
    std::array<int, 5> ar = {5,4,3,2,1};
    {
        /*
            Serialize using default object array, memcpy into separate buffer
        */
        SpkG::Serializer<int,double,uint32_t, E, std::array<int,5> > s;
        s.serialize(i,f, tm, e, ar);
        datacpy = new uint8_t[s.size()];
        memcpy(datacpy, s.data(), s.size());
    }
 
    {
        /*
            Deserialize into variables
        */
        int j; double k; uint32_t t; E ee; std::array<int,5> arr;
        SpkG::Serializer<>::deserialize(datacpy, j, k, t, ee, arr);
        assert(j == i);
        assert(k == f);
        assert(t == tm);
        assert(ee == e);
        assert(arr == ar);
        delete [] datacpy;
    }



    {
        /*
            Serialize into my own buffer
        */
        datacpy = new uint8_t[SpkG::types_size<int,double,uint32_t,E>()];
        SpkG::Serializer<int,double,uint32_t,E>::serializeIntoBuffer(datacpy, i, f, tm, e);
    }

    {
        /*
            Deserialize into variables
        */
        int j; double k; uint32_t t; E ee;
        SpkG::Serializer<>::deserialize(datacpy, j, k, t, ee);
        assert(j == i);
        assert(k == f);
        assert(t == tm);
        assert(ee == e);
        delete [] datacpy;
    }
}