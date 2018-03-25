#include "serialtest.h"

void serialtest(){
        uint8_t *datacpy;
        enum class E {e1, e2, e3};
    {
        SpkG::Serializer<int,double,uint32_t, E> s;
        int i = 1; double f = 1.123; uint32_t time = 5; E e = E::e2;
        std::cout << "numpystr: " << s.numpyType() << std::endl;
        std::cout << "i:" << i << ", f:" << f << ", time:" << time << ", enum: " << (e == E::e2) << std::endl;
        s.serialize(i,f, time, e);
        datacpy = new uint8_t[s.size()];
        memcpy(datacpy, s.data(), s.size());
    }
 
    {
        int j; double k; uint32_t t; E e;
        SpkG::Serializer<>::deserialize(datacpy, j, k, t, e);
        std::cout << "j:" << j << ", k:" << k << ", time:" << t<< ", enum: " << (e == E::e2) <<  std::endl;
        delete [] datacpy;
    }

    {
        std::cout << "numpystr: " << SpkG::toNumpyStr<int, double, uint32_t>() << '\n';
    }
}