/*
arithmetic matrix
assumes that + operator and * operator are implemented for T
if they're not, then use a normal matrix, or don't, I'm not a cop
Unless you implement it, an arithmetic matrix of type T can only
be arithmeticed by a scalar of type T or a matrix of type T
*/
#ifndef ARITHMETIC_MATRIX_HPP
#define ARITHMETIC_MATRIX_HPP
#include "matrix.hpp"


template<class T>
class ArMatrix : public Matrix<T> {
    public:
    ArMatrix(unsigned width, unsigned height) : 
    Matrix<T>(width, height) {}

    ArMatrix(unsigned width, unsigned height, const T* data) : 
    Matrix<T>(width, height, data) {}

    ArMatrix(const ArMatrix& other) : 
    Matrix<T>(other) {}

    ArMatrix(ArMatrix&& other) : 
    Matrix<T>(other) {}

    static T dot(const T* lh, const T* rh, unsigned size);
};

template<class T>
T ArMatrix<T>::dot(const T* lh, const T* rh, unsigned size){
    T dprod{};
    for(unsigned i{ 0 }; i < size; ++i) {
        dprod += lh[i] * rh[i];
    }
    return dprod;
}

// * scalars
template<class T>
ArMatrix<T> operator*(const ArMatrix<T>& mat, T scalar) {
    ArMatrix<T> product{ mat.getWidth(), mat.getHeight() };
    for(unsigned i{ 0 }; i < mat.getWidth() * mat.getHeight(); ++i) {
        product[i] = mat.getAt(i) * scalar;
    }
    return product;
}
template<class T>
ArMatrix<T> operator*(ArMatrix<T>&& mat, T scalar) {
    return mat * scalar;
}
template<class T>
ArMatrix<T> operator*(T scalar, const ArMatrix<T>& mat) {
    return mat * scalar;
}
template<class T>
ArMatrix<T> operator*(T scalar, ArMatrix<T>&& mat) {
    return mat * scalar;
}

// * matrix
template<class T>
ArMatrix<T> operator*(const ArMatrix<T>& lh, const ArMatrix<T>& rh) {
    
    if(lh.getWidth() == rh.getHeight())
    {
        ArMatrix<T> product{ rh.getWidth(), lh.getHeight() };
        for(unsigned row{ 0 }; row < lh.getHeight(); ++row) {
            for(unsigned collumn{ 0 }; collumn < rh.getWidth(); ++collumn) {
                product[row * lh.getWidth() + collumn] = ArMatrix<T>::dot(
                    lh.getRow(row).data(), rh.getCollumn(collumn).data(), rh.getHeight());
            }
        }
        return product;
    }
    else {
        throw "Shape Error!";
        return ArMatrix<T> { 1, 1 };
    }
}
template<class T>
ArMatrix<T> operator*(const ArMatrix<T>&& lh, const ArMatrix<T>& rh) { return lh * rh; }
template<class T>
ArMatrix<T> operator*(const ArMatrix<T>&& lh, const ArMatrix<T>&& rh) { return lh * rh; }
template<class T>
ArMatrix<T> operator*(const ArMatrix<T>& lh, const ArMatrix<T>&& rh) { return lh * rh; }


template<class T>
ArMatrix<T> operator+(const ArMatrix<T>& lh, const ArMatrix<T>& rh) {
    if(!memcmp(&lh, &rh, 2 * sizeof(unsigned))) //if widths and heights are equal
    {
        ArMatrix<T> sum{ lh.getWidth(), rh.getHeight() };
        for(unsigned i{ 0 }; i < lh.getWidth() * lh.getHeight(); ++i) {
            sum[i] = lh.getAt(i) + rh.getAt(i);
        }
        return sum;
    }
    throw "Shape Error!";
    return ArMatrix<T>{ 1, 1 };
}
template<class T>
ArMatrix<T> operator+(const ArMatrix<T>&& lh, const ArMatrix<T>& rh) { return lh + rh; }
template<class T>
ArMatrix<T> operator+(const ArMatrix<T>&& lh, const ArMatrix<T>&& rh) { return lh + rh; }
template<class T>
ArMatrix<T> operator+(const ArMatrix<T>& lh, const ArMatrix<T>&& rh) { return lh + rh; }

#endif