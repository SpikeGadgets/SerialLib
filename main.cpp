#include <iostream>
#include "serializer.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    Serializer s;
    int i = 0; float f = 1.1;
    cout << "size of int + float " << s.serialize(i,f).size() << std::endl;
    return 0;
}
