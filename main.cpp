#include "serializer.h"
#include <iostream>
#include <cstring>
#include <type_traits>
#include <tuple>
#include <string>
#include "serialtest.h"
enum E{e1,e2,e3};

int main()
{
//    s.initializeBuffer<int,double, uint32_t>();
    serialtest();
    return 0;
}
