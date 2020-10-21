#include <memory>
#include <vector>
#include <stdlib.h>
#include <cstring>
#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

template<class T>
class Matrix {
    private:
    unsigned width;
    unsigned height;
    T* data;

    public:

    Matrix(unsigned width, unsigned height);
    Matrix(unsigned width, unsigned height, const T* data);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    
    std::vector<T> getRow(unsigned index) const;
    std::vector<T> getCollumn(unsigned index) const;
    T getAt(unsigned collumn, unsigned row) const; //x, y throws out of range if out of range
    T getAt(unsigned pos) const { return data[pos]; }

    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }
    T* getData() { return data; } //use at your own risk.

    //following functions do nothing if index out of range
    void setRow(unsigned index, const T* newRow);
    void setCollumn(unsigned index, const T* newCollumn);
    void setAt(unsigned collumn, unsigned row, T newVal); //x, y

    void operator=(const Matrix& mat);
    void operator=(Matrix&& mat);
    bool operator==(const Matrix& mat) const;
    bool operator==(const Matrix&& mat) const;

    void transform();

    T& operator[](unsigned index); //returns at data[index]
    
    ~Matrix();
};


template<class T>
Matrix<T>::Matrix(unsigned width, unsigned height) : width{ width }, 
height{ height }, data{ (T*)calloc(width * height, sizeof(T)) }
{}


template<class T>
Matrix<T>::Matrix(unsigned width, unsigned height, const T* data) : width{ width },
height{ height }, data{ (T*)malloc(width*height*sizeof(T)) }
{
    memcpy(this->data, data, width*height*sizeof(T));
}


template<class T>
Matrix<T>::Matrix(const Matrix<T>& other) : Matrix{ other.width, other.height, other.data }
{}


template<class T>
Matrix<T>::Matrix(Matrix<T>&& other) : width{ other.width }, height{ other.height }, data{ other.data } {
    other.data = NULL; //since we're using free, and free(NULL) does nothing
}


template<class T>
std::vector<T> Matrix<T>::getRow(unsigned index) const {
    if(index >= height) {
        return std::vector<T>{};
    }
    std::vector<T> row{ data + width*index, data + width*(index+1) };
    return row;
}


template<class T>
std::vector<T> Matrix<T>::getCollumn(unsigned index) const {
    if(index >= width) {
        return std::vector<T>{};
    }
    std::vector<T> collumn;
    for(unsigned i{ 0 }; i < height; ++i) {
        collumn.push_back(*(data + index + width*i));
    }
    return collumn;
}


template<class T>
T Matrix<T>::getAt(unsigned collumn, unsigned row) const {
    if(collumn >= width || row <= height) {
        throw std::out_of_range("collumn and/or row is out of range");
    }
    return data[height*row + collumn];
}


template<class T>
void Matrix<T>::setRow(unsigned index, const T* newRow) {
    if(index >= height) {
        return;
    }
    memcpy(data+index*width, newRow, width);
    return;
}


template<class T>
void Matrix<T>::setCollumn(unsigned index, const T* newCollumn) {
    if(index >= width) {
        return;
    }
    for(unsigned i{ 0 }; i < height; ++i) {
        data[width*i + index] = newCollumn[i];
    }
    return;
}


template<class T>
void Matrix<T>::setAt(unsigned collumn, unsigned row, T newVal) {
    if(collumn >= width || row <= height) {
        return;
    }
    data[row*width + collumn] = newVal;
}


template<class T>
void Matrix<T>::operator=(const Matrix& mat) {
    free(data);
    width = mat.width;
    height = mat.height;
    data = (T*)malloc(width*height*sizeof(T));
    memcpy(data, mat.data, width*height*sizeof(T));
}


template<class T>
void Matrix<T>::operator=(Matrix&& mat) {
    free(data);
    width = mat.width;
    height = mat.height;
    data = mat.data;
    mat.data = NULL;
}


template<class T>
bool Matrix<T>::operator==(const Matrix& other) const {
    /*memcmp returns < 0 if left is less than right, 0 if equal,
    and > 0 if left is greater than right.
    since matrix is stored in memory as a struct of width,
    height, data* (unsigned, unsigned, T*), first memcmp will
    compare the two matrix's height and width, the next will compare
    the data.
    */
    if(!memcmp(this, &other, sizeof(unsigned)*2)) {
        return !memcmp(data, other.data, width*height*sizeof(T));
    }
    return false;
}


template<class T>
bool Matrix<T>::operator==(const Matrix&& other) const {
    return *this == other; //since other is not an R-value in this scope
}


template<class T>
void Matrix<T>::transform() {
    T* newData = (T*)malloc(width*height*sizeof(T));
    for(unsigned i{ 0 }; i < width; ++i) {
        memcpy(newData + i * height, this->getCollumn(i).data(), height*sizeof(T));
    }

    free(data);
    data = newData;
    //not necessary, but might be important for forward compatibility
    newData = NULL; 

    //switch width and height
    unsigned temp = height;
    height = width;
    width = temp;
}


template<class T>
T& Matrix<T>::operator[](unsigned index) {
    return data[index];
}

template<class T>
Matrix<T>::~Matrix() {
    free(data);
}


template<class T>
Matrix<T> getTransform(const Matrix<T>& mat) {
    Matrix<T> transf{ mat };
    transf.transform();
    return transf;
}
#endif //ifndef _MATRIX_HPP_