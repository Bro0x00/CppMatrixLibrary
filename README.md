# C++MatrixLibrary

Matrix<T>
A matrix with no arithmetic operators overloaded. parent of ArMatrix, which has arithmetic operators overloaded. 
private members (in order):
  unsigned width;
  unsigned height;
  T* data;

constructors:
  Matrix<T>::Matrix<T>(unsigned width, unsigned height); makes a width x height matrix of just zeros
  Matrix<T>::Matrix<T>(unsigned width, unsigned height, const T* data); makes a width x height with this->data = data, if size of data < width*height, you'll get a segfault. if it's more, the matrix will just copy until data[width * height] (not including)
  Matrix<T>::Matrix<T>(const Matrix<T>&); copy constructor
  Matrix<T>::Matrix<T>(Matrix<T>&&); move constructor
                                                                                                                                                       
getters (indexing starts at 0 for all of these):
  unsigned Matrix<T>::getWidth() const; gets width
  unsigned Matrix<T>::getHeight() const; gets height
  T* Matrix<T>::getData(); NOT CONST! use at your own risk. returns the pointer to data. will be a concatenation of each row. its length must equal width * height otherwise you'll probably get a segfault. also, class uses malloc/calloc to allocate memory, and destructor uses free, so don't make data into something allocated with new or something you don't want to be deallocated
  T Matrix<T>::getAt(unsigned collumn, unsigned row) const; returns the value at that collumn and row. throws "Out of Range Error"
  T Matrix<T>::getAt(unsigned index) const; returns the value at data[index]; throws "Out of Range Error!" if out of range
  T& Matrix<T>::operator[](unsigned index); NOT CONST! returns a reference to data[index]. Changing whatever it returns will change the value in the actual matrix
  std::vector<T> Matrix<T>::getCollumn(unsigned collumn) const; returns that collumn.
  std::vector<T> Matrix<T>::getRow(unsigned row) const; returns that row.

setters (again, indexing starts at 0):
  void Matrix<T>::setRow(unsigned index, const T* newRow); sets that row to newRow. if newRow is shorter than width, you'll get a segmentation fault. if it's longer, then it will copy until (not including) newRow[width].
  void Matrix<T>::setCollumn(unsigned index, const T* newCollumn); just like setRow, but it sets a collumn.
  Matrix<T>::setAt(unsigned collumn, unsigned row, T newVal); sets matrix at that collumn and row to newVal
  T& Matrix<T>::operator[](unsigned index) const; covered in getters
  
operators:
  void Matrix<T>::operator[](unsigned index); covered in getters
  void Matrix<T>::operator=(const Matrix<T>&); copy operator. matrix on the left will equal matrix on the right
  void Matrix<T>::operator==(const Matrix<T>&&); move operator. matrix on the rleft will equal matrix on the right, then matrix on right.data will equal NULL
  bool Matrix<T>::operator==(const Matrix<T>&/&&); comparison. if equal 

other:
  void Matrix<T>::transpose(); transposes matrix
  Matrix<T> getTransform(const Matrix<T>&); retuns transpose of matrix
  Matrix<T> getTransform(Matrix<T>&&); transposes matrix and returns it. just use member function

ArMatrix : public Matrix<T>
If you want a matrix that can do arithmetic, use an ArMatrix<T>. operators * and + are overloaded on these. If they aren't overloaded on whatever T is, then you'll get an error.
operator+: adds two matricies. returns left + right. height and width of matrix must be equal. throws "Shape Error!" if not equal.
operator*(ArMatrix<T>, ArMatrix<T>): just like normal matrix multiplication. returns left * right. width of left matrix must equal height of right matrix (I don't make the rules). throws "Shape Error!" if not equal.
operator*(ArMatrix<T>, T scalar)/(T scalar, ArMatrix<T>): returns an ArMatrix<T> with all values multiplied by scalar.
If you handle "Shape Error!", function will return an ArMatrix<T>{ 1, 1 }. (just a zero)
