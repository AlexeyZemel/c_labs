#pragma once
#include <iostream>
#include <math.h>
#include <iomanip>
#include <vector>

class EClassException
{
protected:
    char _err[256];
public:
    EClassException(const char* err);
    void Print();
    ~EClassException();
};

EClassException::EClassException(const char* err)
{
    strncpy_s(_err, err, 255);
    _err[255] = 0;
}
void EClassException::Print()
{
    std::cout << _err << std::endl;
}
EClassException::~EClassException() {}

template <typename T>
class BinaryImg
{
private:
    std::vector<std::vector<T>> array;
    int row;
    int col;
    BinaryImg<T> And(T rhs) const
    {
        BinaryImg<T> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) * rhs > std::numeric_limits<T>::max() || array.at(i).at(j) * rhs < std::numeric_limits<T>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else {
                    result.array.at(i).at(j) = array.at(i).at(j) * rhs;
                }
            }
        }
        return result;
    }
    BinaryImg<T> Or(T rhs) const
    {
        BinaryImg<T> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) + rhs > std::numeric_limits<T>::max() || array.at(i).at(j) + rhs < std::numeric_limits<T>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else {
                    result.array.at(i).at(j) = array.at(i).at(j) + rhs;
                }
            }
        }
        return result;
    }
    BinaryImg<T> And(const BinaryImg<T>& src) const
    {
        if (row != src.row || col != src.col) { throw EClassException("Invalid dimensions of images"); }
        BinaryImg<T> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) * src.array.at(i).at(j) > std::numeric_limits<T>::max() || array.at(i).at(j) * src.array.at(i).at(j) < std::numeric_limits<T>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else
                {
                    result.array.at(i).at(j) = array.at(i).at(j) * src.array.at(i).at(j);
                }
            }
        }
        return result;
    }
    BinaryImg<T> Or(const BinaryImg<T>& src) const
    {
        if (row != src.row || col != src.col) { throw EClassException("Invalid dimensions of images"); }
        BinaryImg<T> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) + src.array.at(i).at(j) > std::numeric_limits<T>::max() || array.at(i).at(j) + src.array.at(i).at(j) < std::numeric_limits<T>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else
                {
                    result.array.at(i).at(j) = array.at(i).at(j) + src.array.at(i).at(j);
                }
            }
        }
        return result;
    }


public:
    BinaryImg(int x, int y)
    {
        if (x <= 0 || y <= 0) { throw EClassException("invalid dimension"); }
        row = x;
        col = y;
        for (int i = 0; i < row; i++) {
            array.push_back(std::vector<T>());
            for (int j = 0; j < col; j++) {
                array.back().push_back(0);
            }
        }
    }

    BinaryImg(const BinaryImg<T>& src) = default;

    ~BinaryImg() = default;

    int GetRow() const { return row; }
    int GetCol() const { return col; }
    
    

    auto begin() {
        return array.begin();
    }

    auto end() {
        return array.end();
    }

 
    

    

    bool operator==(const BinaryImg<T>& src) const
    {
        if (row != src.row || col != src.col) { return false; }
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) != src.array.at(i).at(j)) { return false; }
            }
        }
        return true;
    }

    bool operator!=(const BinaryImg<T>& src) const
    {
        return !(*this == src);
    }

    T& operator()(int x, int y)
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        //T tmp = array.at(x).at(y);
        return array.at(x).at(y);
    }

    void operator()(int x, int y, T value) 
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        array.at(x).at(y) = value;
    }

    T operator()(int x, int y) const
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        return array.at(x).at(y);
    }

    BinaryImg<T> operator+(const BinaryImg<T>& src) const
    {
        return this->Or(src);
    }

    BinaryImg<T> operator*(const BinaryImg<T>& src) const
    {
        return this->And(src);
    }

    BinaryImg<T> operator+(T rhs) const
    {
        return this->Or(rhs);
    }

    BinaryImg<T> operator*(T rhs) const
    {
        return this->And(rhs);
    }

    BinaryImg<T> operator!()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) == 0)
                {
                    array.at(i).at(j) = std::numeric_limits<T>::max();
                }
                else
                {
                    array.at(i).at(j) = 0;
                }
            }
        }
        return *this;
    }

    BinaryImg<T>& operator= (const BinaryImg<T>& src) = default;

    double FillFactor() const
    {
        int k = 0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j)) { k++; }
            }
        }
        double res = (double)k / (row * col);
        return res;
    }
    friend BinaryImg<T> operator*(T rhs, const BinaryImg<T>& src)
    {
        return src.And(rhs);
    }
    friend BinaryImg<T> operator+(T rhs, const BinaryImg<T>& src)
    {
        return src.Or(rhs);
    }
};


template <>
class BinaryImg<bool>
{
private:
    std::vector<std::vector<bool>> array;
    int row;
    int col;
    BinaryImg<bool> And(bool rhs) const
    {
        BinaryImg<bool> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) * rhs > std::numeric_limits<bool>::max() || array.at(i).at(j) * rhs < std::numeric_limits<bool>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else {
                    result.array.at(i).at(j) = array.at(i).at(j) * rhs;
                }
            }
        }
        return result;
    }
    BinaryImg<bool> Or(bool rhs) const
    {
        BinaryImg<bool> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) + rhs > std::numeric_limits<bool>::max() || array.at(i).at(j) + rhs < std::numeric_limits<bool>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else {
                    result.array.at(i).at(j) = array.at(i).at(j) + rhs;
                }
            }
        }
        return result;
    }
    BinaryImg<bool> And(const BinaryImg<bool>& src) const
    {
        if (row != src.row || col != src.col) { throw EClassException("Invalid dimensions of images"); }
        BinaryImg<bool> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) * src.array.at(i).at(j) > std::numeric_limits<bool>::max() || array.at(i).at(j) * src.array.at(i).at(j) < std::numeric_limits<bool>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else
                {
                    result.array.at(i).at(j) = array.at(i).at(j) * src.array.at(i).at(j);
                }
            }
        }
        return result;
    }
    BinaryImg<bool> Or(const BinaryImg<bool>& src) const
    {
        if (row != src.row || col != src.col) { throw EClassException("Invalid dimensions of images"); }
        BinaryImg<bool> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) + src.array.at(i).at(j) > std::numeric_limits<bool>::max() || array.at(i).at(j) + src.array.at(i).at(j) < std::numeric_limits<bool>::min())
                {
                    result.array.at(i).at(j) = 0;
                }
                else
                {
                    result.array.at(i).at(j) = array.at(i).at(j) + src.array.at(i).at(j);
                }
            }
        }
        return result;
    }


public:
    BinaryImg(int x, int y)
    {
        if (x <= 0 || y <= 0) { throw EClassException("invalid dimension"); }
        row = x;
        col = y;
        for (int i = 0; i < row; i++) {
            array.push_back(std::vector<bool>());
            for (int j = 0; j < col; j++) {
                array.back().push_back(0);
            }
        }
    }

    BinaryImg(const BinaryImg<bool>& src) = default;

    ~BinaryImg() = default;

    int GetRow() const { return row; }
    int GetCol() const { return col; }

    auto begin() {
        return array.begin();
    }

    auto end() {
        return array.end();
    }

    friend std::ostream& operator<<(std::ostream& os, BinaryImg<bool>& obj)
    {
        for (auto i : obj.array) {
            for (auto j : i) {
                std::cout << j << std::setw(2);
            }
            std::cout << "\n";
        }
        return os;
    }

    bool operator==(const BinaryImg<bool>& src) const
    {
        if (row != src.row || col != src.col) { return false; }
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) != src.array.at(i).at(j)) { return false; }
            }
        }
        return true;
    }

    bool operator!=(const BinaryImg<bool>& src) const
    {
        return !(*this == src);
    }

    bool& operator()(int x, int y)
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        bool res = array.at(x).at(y);
        return res;
    }

    void operator()(int x, int y, bool value)
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        array.at(x).at(y) = value;
    }

    bool operator()(int x, int y) const
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        return array.at(x).at(y);
    }

    BinaryImg<bool> operator+(const BinaryImg<bool>& src) const
    {
        return this->Or(src);
    }

    BinaryImg<bool> operator*(const BinaryImg<bool>& src) const
    {
        return this->And(src);
    }

    BinaryImg<bool> operator+(bool rhs) const
    {
        return this->Or(rhs);
    }

    BinaryImg<bool> operator*(bool rhs) const
    {
        return this->And(rhs);
    }

    BinaryImg<bool> operator!()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) == 0)
                {
                    array.at(i).at(j) = std::numeric_limits<bool>::max();
                }
                else
                {
                    array.at(i).at(j) = 0;
                }
            }
        }
        return *this;
    }

    BinaryImg<bool>& operator= (const BinaryImg<bool>& src) = default;

    double FillFactor() const
    {
        int k = 0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j)) { k++; }
            }
        }
        double res = (double)k / (row * col);
        return res;
    }
    friend BinaryImg<bool> operator*(bool rhs, const BinaryImg<bool>& src)
    {
        return src.And(rhs);
    }
    friend BinaryImg<bool> operator+(bool rhs, const BinaryImg<bool>& src)
    {
        return src.Or(rhs);
    }
};


template <>
class BinaryImg<char>
{
private:
    std::vector<std::vector<char>> array;
    int row;
    int col;
    BinaryImg<char> And(char rhs) const
    {
        BinaryImg<char> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if ((int)array.at(i).at(j) * (int)rhs > (int)std::numeric_limits<char>::max() || (int)array.at(i).at(j) * (int)rhs < (int)std::numeric_limits<char>::min())
                {
                    result.array.at(i).at(j) = '0';
                }
                else
                {
                    result.array.at(i).at(j) = (int)array.at(i).at(j) * (int)rhs;
                }
            }
        }
        return result;
    }
    BinaryImg<char> Or(char rhs) const
    {
        BinaryImg<char> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if ((int)array.at(i).at(j) + (int)rhs > (int)std::numeric_limits<char>::max() || (int)array.at(i).at(j) + (int)rhs < (int)std::numeric_limits<char>::min())
                {
                    result.array.at(i).at(j) = '0';
                }
                else
                {
                    result.array.at(i).at(j) = (int)array.at(i).at(j) + (int)rhs;
                }
            }
        }
        return result;
    }
    BinaryImg<char> And(const BinaryImg<char>& src) const
    {
        if (row != src.row || col != src.col) { throw EClassException("Invalid dimensions of images"); }
        BinaryImg<char> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if ((int)array.at(i).at(j) * (int)src.array.at(i).at(j) > (int)std::numeric_limits<char>::max() || (int)array.at(i).at(j) * (int)src.array.at(i).at(j) < (int)std::numeric_limits<char>::min())
                {
                    result.array.at(i).at(j) = '0';
                }
                else
                {
                    result.array.at(i).at(j) = (int)array.at(i).at(j) * (int)src.array.at(i).at(j);
                }
            }
        }
        return result;
    }
    BinaryImg<char> Or(const BinaryImg<char>& src) const
    {
        if (row != src.row || col != src.col) { throw EClassException("Invalid dimensions of images"); }
        BinaryImg<char> result(row, col);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if ((int)array.at(i).at(j) + (int)src.array.at(i).at(j) > (int)std::numeric_limits<char>::max() || (int)array.at(i).at(j) + (int)src.array.at(i).at(j) < (int)std::numeric_limits<char>::min())
                {
                    result.array.at(i).at(j) = '0';
                }
                else
                {
                    result.array.at(i).at(j) = (int)array.at(i).at(j) + (int)src.array.at(i).at(j);
                }
            }
        }
        return result;
    }

public:
    BinaryImg(int x, int y)
    {
        if (x <= 0 || y <= 0) { throw EClassException("invalid dimension"); }
        row = x;
        col = y;
        for (int i = 0; i < row; i++) {
            array.push_back(std::vector<char>());
            for (int j = 0; j < col; j++) {
                array.back().push_back('0');
            }
        }
    }

    BinaryImg(const BinaryImg<char>& src) = default;
  
    ~BinaryImg() = default;
    
    int GetRow() const { return row; }
    int GetCol() const { return col; }

    //std::vector<char>::iterator
    auto begin() {
        return array.begin();
    }

    auto end() {
        return array.end();
    }

    friend std::ostream& operator<<(std::ostream& os, BinaryImg<char>& obj)
    {
        for (auto i : obj.array) {
            for (auto j : i) {
                std::cout << j << std::setw(2);
            }
            std::cout << "\n";
        }
        return os;
    }


    bool operator==(const BinaryImg<char>& src) const
    {
        if (row != src.row || col != src.col) { return false; }
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) != src.array.at(i).at(j)) { return false; }
            }
        }
        return true;
    }
    bool operator!=(const BinaryImg<char>& src) const
    {
        return !(*this == src);
    }
    char& operator()(int x, int y)
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        return array.at(x).at(y);
    }
    char operator()(int x, int y) const
    {
        if ((x >= row || y >= col) || (x < 0 || y < 0))
        {
            throw EClassException("invalid index");
        }
        return array.at(x).at(y);
    }
    BinaryImg<char> operator+(const BinaryImg<char>& src) const
    {
        return this->Or(src);
    }
    BinaryImg<char> operator*(const BinaryImg<char>& src) const
    {
        return this->And(src);
    }
    BinaryImg<char> operator+(char rhs) const
    {
        return this->Or(rhs);
    }
    BinaryImg<char> operator*(char rhs) const
    {
        return this->And(rhs);
    }
    BinaryImg<char> operator!()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) == '0')
                {
                    array.at(i).at(j) = std::numeric_limits<char>::max();
                }
                else
                {
                    array.at(i).at(j) = '0';
                }
            }
        }
        return *this;
    }

    BinaryImg<char>& operator= (const BinaryImg<char>& src) = default;
    
    double FillFactor() const
    {
        int k = 0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (array.at(i).at(j) != '0') { k++; }
            }
        }
        double res = (double)k / (row * col);
        return res;
    }
    friend BinaryImg<char> operator*(char rhs, const BinaryImg<char>& src)
    {
        return src.And(rhs);
    }
    friend BinaryImg<char> operator+(char rhs, const BinaryImg<char>& src)
    {
        return src.Or(rhs);
    }
};

