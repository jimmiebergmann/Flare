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

#ifndef FLARE_MATH_VECTOR_HPP
#define FLARE_MATH_VECTOR_HPP

#include "flare/build.hpp"
#include <cmath>

namespace Flare
{

    template<typename T>
    class Vector2
    {

    public:

        Vector2();
        Vector2(const T x, const T y);
        Vector2(const Vector2<T> & vector);
        template<typename U>
        Vector2(const Vector2<U> & vector);

        Vector2<T> operator + (const Vector2<T> & vector) const;
        Vector2<T> operator - () const;
        Vector2<T> operator - (const Vector2<T> & vector) const;
        Vector2<T> operator * (const Vector2<T> & vector) const;
        Vector2<T> operator * (const T scalar) const;
        Vector2<T> operator / (const Vector2<T> & vector) const;
        Vector2<T> operator / (const T scalar) const;

        Vector2<T> & operator += (const Vector2<T> & vector);
        Vector2<T> & operator -= (const Vector2<T> & vector);
        Vector2<T> & operator *= (const Vector2<T> & vector);
        Vector2<T> & operator *= (const T scalar);
        Vector2<T> & operator /= (const Vector2<T> & vector);
        Vector2<T> & operator /= (const T scalar);

        template<typename U = T>
        U length() const;
        Vector2<T> normal() const;
        Vector2<T> & normalize();
        Vector2<T> absolute() const;
        template<typename U = T>
        U dot(const Vector2<T> & vector) const;

        template<typename U = T>
        static U dot(const Vector2<T> & vector1, const Vector2<T> & vector2);

        union
        {
            struct
            {
                T x;
                T y;
            };

            T v[2];
        };

    };

    template<typename T>
    class Vector3
    {

    public:

        Vector3();
        Vector3(const T x, const T y, const T z);
        Vector3(const Vector3<T> & vector);
        template<typename U>
        Vector3(const Vector3<U> & vector);

        Vector3<T> operator + (const Vector3<T> & vector) const;
        Vector3<T> operator - () const;
        Vector3<T> operator - (const Vector3<T> & vector) const;
        Vector3<T> operator * (const Vector3<T> & vector) const;
        Vector3<T> operator * (const T scalar) const;
        Vector3<T> operator / (const Vector3<T> & vector) const;
        Vector3<T> operator / (const T scalar) const;

        Vector3<T> & operator += (const Vector3<T> & vector);
        Vector3<T> & operator -= (const Vector3<T> & vector);
        Vector3<T> & operator *= (const Vector3<T> & vector);
        Vector3<T> & operator *= (const T scalar);
        Vector3<T> & operator /= (const Vector3<T> & vector);
        Vector3<T> & operator /= (const T scalar);

        template<typename U = T>
        U length() const;
        Vector3<T> normal() const;
        Vector3<T> & normalize();
        Vector3<T> absolute() const;
        template<typename U = T>
        U dot(const Vector3<T> & vector) const;
        Vector3<T> cross(const Vector3<T> & vector) const;

        template<typename U = T>
        static U dot(const Vector3<T> & vector1, const Vector3<T> & vector2);

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };

            T v[3];
        };

    };

    template<typename T>
    class Vector4
    {

    public:

        Vector4();
        Vector4(const T x, const T y, const T z, const T w);
        Vector4(const Vector4<T> & vector);
        template<typename U>
        Vector4(const Vector4<U> & vector);

        Vector4<T> operator + (const Vector4<T> & vector) const;
        Vector4<T> operator - () const;
        Vector4<T> operator - (const Vector4<T> & vector) const;
        Vector4<T> operator * (const Vector4<T> & vector) const;
        Vector4<T> operator * (const T scalar) const;
        Vector4<T> operator / (const Vector4<T> & vector) const;
        Vector4<T> operator / (const T scalar) const;

        Vector4<T> & operator += (const Vector4<T> & vector);
        Vector4<T> & operator -= (const Vector4<T> & vector);
        Vector4<T> & operator *= (const Vector4<T> & vector);
        Vector4<T> & operator *= (const T scalar);
        Vector4<T> & operator /= (const Vector4<T> & vector);
        Vector4<T> & operator /= (const T scalar);

        template<typename U = T>
        U length() const;
        Vector4<T> normal() const;
        Vector4<T> & normalize();
        Vector4<T> absolute() const;
        template<typename U = T>
        U dot(const Vector4<T> & vector) const;

        template<typename U = T>
        static U dot(const Vector4<T> & vector1, const Vector4<T> & vector2);

        union
        {
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };

            T v[4];
        };

    };

    typedef Vector2<float>      Vector2f;
    typedef Vector2<double>     Vector2d;
    typedef Vector2<int32_t>    Vector2i32;
    typedef Vector2<uint32_t>   Vector2ui32;
    typedef Vector2<int64_t>    Vector2i64;
    typedef Vector2<uint64_t>   Vector2ui64;

    typedef Vector3<float>      Vector3f;
    typedef Vector3<double>     Vector3d;
    typedef Vector3<int32_t>    Vector3i32;
    typedef Vector3<uint32_t>   Vector3ui32;
    typedef Vector3<int64_t>    Vector3i64;
    typedef Vector3<uint64_t>   Vector3ui64;
    
    typedef Vector4<float>      Vector4f;
    typedef Vector4<double>     Vector4d;
    typedef Vector4<int32_t>    Vector4i32;
    typedef Vector4<uint32_t>   Vector4ui32;
    typedef Vector4<int64_t>    Vector4i64;
    typedef Vector4<uint64_t>   Vector4ui64;

}

#include "flare/math/vector.inl"

#endif