#include <iostream>

using namespace std;

int main()
{
    float temp;

    *(unsigned int *)&temp = 0x41F00000;

    cout << temp << endl;

    return 0;
}
