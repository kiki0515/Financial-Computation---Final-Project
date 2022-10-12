//
//  Vector.cpp
//  Final6883
//

#include"Vector.h"
#include<vector>

void Vector::pushback(const double& value)
{
    v.push_back(value);
}
int Vector::size() const
{
    return (int)v.size();
}

//stock.cpp里用
//Vector格式的v 是price
Vector Vector::pct_change() const
{
    int d = (int)v.size();
    vector<double> pct_c(d - 1); //d - 1是长度 第一天没有
    for (int i = 0; i < d - 1; i++)
    {
        pct_c[i] = v[i + 1] / v[i] - 1;
    }
    return Vector(pct_c);
}

//Vector格式的v 是daily return
//算sum
double Vector::sum() const
{
    double sum = 0.0;
    for (auto itr = v.begin(); itr != v.end(); itr++) //const function use auto to avoid error
    {
        sum += *itr;
    }
    return sum;
}

//stock.cpp里用
//存sum
Vector Vector::cumsum() const
{
    Vector cums;
    double sum = 0;
    for (auto itr = v.begin(); itr != v.end(); itr++)
    {
        sum += *itr;
        cums.pushback(sum);
    }

    return cums;
}

Vector Vector::sqrtV() {
    int d = (int)v.size();
    for (int i = 0; i < d; i++) {
        v[i] = sqrt(v[i]);
    }
    return Vector(v);
}




