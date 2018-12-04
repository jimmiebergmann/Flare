/*
* MIT License
*
* Copyright(c) 2018 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#ifndef FLARE_MATH_MATRIX_HPP
#define FLARE_MATH_MATRIX_HPP

#include "flare/build.hpp"
#include "flare/math/vector.hpp"
#include <cmath>

namespace Flare
{

    template<typename T>
    class Matrix4x4
    {

    public:

        Matrix4x4();
        Matrix4x4(const T s00, const T s10, const T s20, const T s30,
                  const T s01, const T s11, const T s21, const T s31,
                  const T s02, const T s12, const T s22, const T s32,
                  const T s03, const T s13, const T s23, const T s33);
        Matrix4x4(const Matrix4x4<T> & matrix);
        template<typename U>
        Matrix4x4(const Matrix4x4<U> & matrix);

        Vector4<T> row(const size_t index) const;
        Vector4<T> column(const size_t index) const;

        Matrix4x4<T> & identity();
        Matrix4x4<T> & translate(const Vector3<T> & vector);
        Matrix4x4<T> & translate(const T x, const T y, const T z);
        Matrix4x4<T> & scale(const T x, const T y, const T z);
        Matrix4x4<T> & lookAt(const Vector3<T> & position, const Vector3<T> & up, const Vector3<T> & center);

        Matrix4x4<T> operator -() const;
        Matrix4x4<T> operator *(const Matrix4x4<T> & matrix) const;
        Matrix4x4<T> operator *(const Vector4<T> & vector) const;

        Matrix4x4<T> & operator *=(const Matrix4x4<T> & matrix);


        union
        {
            T v[4][4];
            T m[16];
        };

    };

    typedef Matrix4x4<float>    Matrix4x4f;
    typedef Matrix4x4<double>   Matrix4x4d;
    typedef Matrix4x4<int32_t>  Matrix4x4i32;
    typedef Matrix4x4<uint32_t> Matrix4x4ui32;
    typedef Matrix4x4<int64_t>  Matrix4x4i64;
    typedef Matrix4x4<uint64_t> Matrix4x4ui64;

}

#include "flare/math/matrix.inl"

#endif