#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <string>
#include <iterator>
#include <vector>

//£¤§
using namespace std;

int main()
{
    int val, count = 0;

    cin >> val;

    while (val != 0)
    {
        if ((val & 1) == 1)
        {
            ++count;
        }

        *(unsigned int *)&val >>= 1;
    }

    cout << count << endl;

    return 0;
}
