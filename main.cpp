#include "serializer.h"
#include <iostream>
#include <cstring>

int main()
{
    Serializer s;
    s.initializeBuffer<int,double, uint32_t>();
    uint8_t *datacpy = new uint8_t[s.size()];
    {
        int i = 1; double f = 1.123; uint32_t time = 5;
        std::cout << "i:" << i << ", f:" << f << ", time:" << time << std::endl;
        uint8_t *data = s.serialize(i,f, time);
        memcpy(datacpy, data, s.size());
    }

    {
        int j; double k; uint32_t t;
        Serializer::deserialize(datacpy, j, k, t);
        std::cout << "j:" << j << ", k:" << k << ", time:" << t<< std::endl;
    }
    return 0;
}
