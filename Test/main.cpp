#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <string>
#include <iterator>
#include <vector>

//£¤§
template <typename T>
struct CVector2
{
    T x;
    T y;
};

template <typename T>
struct CVector3
{
    T x;
    T y;
    T z;
};

struct SProvinceTextVertex
{
    CVector3<float> Position;
    CVector2<float> UV;
};

struct STextVertex
{
    CVector3<float> Position;
    CVector2<float> UV;
    uint32_t FillColor;
    uint32_t BorderColor;
};

void SetVertices()

int main()
{

    return 0;
}
