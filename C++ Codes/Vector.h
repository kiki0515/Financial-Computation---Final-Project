//
//  Vector.h
//  Final6883
//

#pragma once
#include<vector>
#include<iostream>
#include<iomanip>
#include<cmath>


using namespace std;

class Vector
{
private:
    vector<double> v;
    //vector<string> w;
    
public:

    Vector():v(vector<double>(0)) {}
    Vector(const vector<double>& v_):v(v_){}
    ~Vector(){}

    // update the vector
    void pushback(const double& value);
    
    // get some info of the vector
    int size()const;
    
    void resize(int u) { v.resize(u); }
    void clear() { v.clear(); }
    vector<double> Getv() const { return v; }
    
    // calculate pct_change, sum, and cumsum
    Vector pct_change()const;
    double sum()const;
    Vector cumsum()const;
    
    Vector sqrtV();

    // overload operators
    double operator[](int const idx) const
    {
        return v[idx];
    };

    //两个vector相加
    Vector operator+(const Vector& u)
    {
        int d = u.size();
        vector<double> sum(d);
        for(int i = 0;i<d;i++)
        {
            sum[i] = v[i] + u[i];
        }
        return Vector(sum);

    }

    //两个vector相减去
    Vector operator-(const Vector& u)
    {
        int d = u.size();
        vector<double> minus(d);
        for (int i = 0; i < d; i++)
        {
            minus[i] = v[i] - u[i];
        }
        return Vector(minus);
    }

    //两个vector相除
    Vector operator/(const double n)
    {
        int d = (int)v.size();
        vector<double>result(d);
        for (int i = 0; i < d; i++)
        {
            result[i] = v[i] / n;
        }
        return Vector(result);
    }
    //两个vector相乘
    Vector operator*(const double n)
    {
        int d = (int)v.size();
        vector<double>result(d);
        for (int i = 0; i < d; i++)
        {
            result[i] = v[i] * n;
        }
        return Vector(result);
    }

    Vector operator*(const Vector U)
    {
        int d = (int)v.size();
        vector<double>result(d);
        for (int i = 0; i < d; i++)
        {
            result[i] = v[i] * U.v[i];
        }
        return Vector(result);
    }

    

    friend ostream& operator<<(ostream& out, const Vector& V)
    {
        for (auto itr = V.v.begin(); itr != V.v.end(); itr++)
            out << setw(15) << *itr;
        out << endl;
        return out;
    };

    // cout overload in C
    friend ostream& operator<<(ostream& out, const vector<Vector>& M)
    {
        vector<vector<double>> m;
        for (auto iter = M.begin(); iter != M.end(); iter++) {
            m.emplace_back(iter->Getv());
        }
        int row = (int)m.begin()->size();
        int col = (int)m.size();
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                out << setw(15) << m[j][i];
            }
            out << endl;
        }
        out << endl << endl;
        return out;
    }
    
};
