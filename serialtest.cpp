#include "serialtest.h"

void serialtest(){
        uint8_t *datacpy;
    {
        SpkG::Serializer<int,double,uint32_t> s;
        int i = 1; double f = 1.123; uint32_t time = 5;
        std::cout << "i:" << i << ", f:" << f << ", time:" << time << std::endl;
        std::cout << "numpystr: " << s.numpyType() << std::endl;
        s.serialize(i,f, time);
        datacpy = new uint8_t[s.size()];
        memcpy(datacpy, s.data(), s.size());
    }
 
    {
        int j; double k; uint32_t t;
        SpkG::Serializer<>::deserialize(datacpy, j, k, t);
        std::cout << "j:" << j << ", k:" << k << ", time:" << t<< std::endl;
        delete [] datacpy;
    }

    {
        std::cout << "numpystr: " << SpkG::toNumpyStr<int, double, uint32_t>() << '\n';
    }
}