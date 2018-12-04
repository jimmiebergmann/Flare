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

namespace Flare
{

    template<typename T>
    Matrix4x4<T>::Matrix4x4()
    { }

    template<typename T>
    Matrix4x4<T>::Matrix4x4(const T s00, const T s10, const T s20, const T s30,
                            const T s01, const T s11, const T s21, const T s31,
                            const T s02, const T s12, const T s22, const T s32,
                            const T s03, const T s13, const T s23, const T s33) :
        m{ s00, s01, s02, s03, 
           s10, s11, s12, s13,
           s20, s21, s22, s23,
           s30, s31, s32, s33 }
    { }

    template<typename T>
    Matrix4x4<T>::Matrix4x4(const Matrix4x4<T> & matrix) :
        m{ matrix.m[0],  matrix.m[1],  matrix.m[2],  matrix.m[3],
           matrix.m[4],  matrix.m[5],  matrix.m[6],  matrix.m[7],
           matrix.m[8],  matrix.m[9],  matrix.m[10], matrix.m[11],
           matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] }
    { }
    
    template<typename T>
    template<typename U>
    Matrix4x4<T>::Matrix4x4(const Matrix4x4<U> & matrix) :
        m{ static_cast<T>(matrix.m[0]),  static_cast<T>(matrix.m[1]),  static_cast<T>(matrix.m[2]),  static_cast<T>(matrix.m[3]),
           static_cast<T>(matrix.m[4]),  static_cast<T>(matrix.m[5]),  static_cast<T>(matrix.m[6]),  static_cast<T>(matrix.m[7]),
           static_cast<T>(matrix.m[8]),  static_cast<T>(matrix.m[9]),  static_cast<T>(matrix.m[10]), static_cast<T>(matrix.m[11]),
           static_cast<T>(matrix.m[12]), static_cast<T>(matrix.m[13]), static_cast<T>(matrix.m[14]), static_cast<T>(matrix.m[15]) }
    { }

    template<typename T>
    Vector4<T> Matrix4x4<T>::row(const size_t index) const
    { 
        return { m[0 + index], m[4 + index], m[8 + index], m[12 + index] };
    }

    template<typename T>
    Vector4<T> Matrix4x4<T>::column(const size_t index) const
    {
        const size_t col = index * 4;
        return { m[0 + col], m[1 + col], m[2 + col], m[3 + col] };
    }

    template<typename T>
    Matrix4x4<T> & Matrix4x4<T>::identity()
    {
        m[0] = static_cast<T>(1); m[4] = static_cast<T>(0); m[8]  = static_cast<T>(0); m[12] = static_cast<T>(0);
        m[1] = static_cast<T>(0); m[5] = static_cast<T>(1); m[9]  = static_cast<T>(0); m[13] = static_cast<T>(0);
        m[2] = static_cast<T>(0); m[6] = static_cast<T>(0); m[10] = static_cast<T>(1); m[14] = static_cast<T>(0);
        m[3] = static_cast<T>(0); m[7] = static_cast<T>(0); m[11] = static_cast<T>(0); m[15] = static_cast<T>(1);
        return *this;
    }

    template<typename T>
    Matrix4x4<T> & Matrix4x4<T>::translate(const Vector3<T> & vector)
    {
        return (*this *= { 1, 0, 0, vector.x,
                           0, 1, 0, vector.y,
                           0, 0, 1, vector.z,
                           0, 0, 0, 1});
    }

    template<typename T>
    Matrix4x4<T> & Matrix4x4<T>::translate(const T x, const T y, const T z)
    {
        return (*this *= { 1, 0, 0, x,
                           0, 1, 0, y,
                           0, 0, 1, z,
                           0, 0, 0, 1});
    }

    template<typename T>
    Matrix4x4<T> & Matrix4x4<T>::scale(const T x, const T y, const T z)
    {
        return (*this *= { x, 0, 0, 0,
                           0, y, 0, 0,
                           0, 0, z, 0,
                           0, 0, 0, 1});
    }

    template<typename T>
    Matrix4x4<T> & Matrix4x4<T>::lookAt(const Vector3<T> & position, const Vector3<T> & up, const Vector3<T> & center)
    {
        const Vector3<T> forward = (center - position).normal();
        const Vector3<T> side = forward.cross(up).normal();
        const Vector3<T> upNormal = up.normal();

        Matrix4x4<T> lookMatrix = 
        {
            side.x,     side.y,     side.z,     0,
            upNormal.x, upNormal.y, upNormal.z, 0,
            forward.x,  forward.y,  forward.z,  0,
            0,          0,           0,         1
        };
        lookMatrix.translate(-position);

        return *this = lookMatrix;
    }

    template<typename T>
    Matrix4x4<T> Matrix4x4<T>::operator -() const
    {
        return { -m[0], -m[4], -m[8],  -m[12],
                 -m[1], -m[5], -m[9],  -m[13],
                 -m[2], -m[6], -m[10], -m[14],
                 -m[3], -m[7], -m[11], -m[15] };
    }

    template<typename T>
    Matrix4x4<T> Matrix4x4<T>::operator *(const Matrix4x4<T> & matrix) const
    {
        Matrix4x4<T> out;

        const Vector4<T> r1 = row(0);
        out.m[0]  = matrix.column(0).dot(r1);
        out.m[4]  = matrix.column(1).dot(r1);
        out.m[8]  = matrix.column(2).dot(r1);
        out.m[12] = matrix.column(3).dot(r1);
        
        const Vector4<T> r2 = row(1);
        out.m[1]  = matrix.column(0).dot(r2);
        out.m[5]  = matrix.column(1).dot(r2);
        out.m[9]  = matrix.column(2).dot(r2);
        out.m[13] = matrix.column(3).dot(r2);

        const Vector4<T> r3 = row(2);
        out.m[2]  = matrix.column(0).dot(r3);
        out.m[6]  = matrix.column(1).dot(r3);
        out.m[10] = matrix.column(2).dot(r3);
        out.m[14] = matrix.column(3).dot(r3);

        const Vector4<T> r4 = row(3);
        out.m[3]  = matrix.column(0).dot(r4);
        out.m[7]  = matrix.column(1).dot(r4);
        out.m[11] = matrix.column(2).dot(r4);
        out.m[15] = matrix.column(3).dot(r4);

        return out;
    }

    template<typename T>
    Matrix4x4<T> Matrix4x4<T>::operator *(const Vector4<T> & vector) const
    {
        return Vector4<T>((m[0] * vector.x) + (m[4] * vector.y) + (m[8]  * vector.z) + (m[12] * vector.w),
                          (m[1] * vector.x) + (m[5] * vector.y) + (m[9]  * vector.z) + (m[13] * vector.w),
                          (m[2] * vector.x) + (m[6] * vector.y) + (m[10] * vector.z) + (m[14] * vector.w),
                          (m[3] * vector.x) + (m[7] * vector.y) + (m[11] * vector.z) + (m[15] * vector.w));
    }

    template<typename T>
    Matrix4x4<T> & Matrix4x4<T>::operator *=(const Matrix4x4<T> & matrix)
    {
        const Vector4<T> r1 = row(0);
        const Vector4<T> r2 = row(1);
        const Vector4<T> r3 = row(2);
        const Vector4<T> r4 = row(3);

        m[0] = matrix.column(0).dot(r1);
        m[4] = matrix.column(1).dot(r1);
        m[8] = matrix.column(2).dot(r1);
        m[12] = matrix.column(3).dot(r1);
        
        m[1] = matrix.column(0).dot(r2);
        m[5] = matrix.column(1).dot(r2);
        m[9] = matrix.column(2).dot(r2);
        m[13] = matrix.column(3).dot(r2);
        
        m[2] = matrix.column(0).dot(r3);
        m[6] = matrix.column(1).dot(r3);
        m[10] = matrix.column(2).dot(r3);
        m[14] = matrix.column(3).dot(r3);

        m[3] = matrix.column(0).dot(r4);
        m[7] = matrix.column(1).dot(r4);
        m[11] = matrix.column(2).dot(r4);
        m[15] = matrix.column(3).dot(r4);

        return *this;
    }

}