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

    // Vector 2 implementation.
    template<typename T>
    Vector2<T>::Vector2()
    { }

    template<typename T>
    Vector2<T>::Vector2(const T scalar) :
        x(scalar),
        y(scalar)
    { }

    template<typename T>
    Vector2<T>::Vector2(const T p_x, const T p_y) :
        x(p_x),
        y(p_y)
    { }

    template<typename T>
    Vector2<T>::Vector2(const Vector2<T> & vector) :
        x(vector.x),
        y(vector.y)
    { }

    template<typename T>
    template<typename U>
    Vector2<T>::Vector2(const Vector2<U> & vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y))
    { }

    template<typename T>
    Vector2<T> Vector2<T>::operator + (const Vector2<T> & vector) const
    {
        return Vector2<T>(x + vector.x, y + vector.y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator - () const
    {
        return Vector2<T>(-x, -y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator - (const Vector2<T> & vector) const
    {
        return Vector2<T>(x - vector.x, y - vector.y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator * (const Vector2<T> & vector) const
    {
        return Vector2<T>(x * vector.x, y * vector.y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator * (const T scalar) const
    {
        return Vector2<T>(x * scalar, y * scalar);
    }
    
    template<typename T>
    Vector2<T> Vector2<T>::operator / (const Vector2<T> & vector) const
    {
        return Vector2<T>(x / vector.x, y / vector.y);
    }

    template<typename T>
    Vector2<T> Vector2<T>::operator / (const T scalar) const
    {
        return Vector2<T>(x / scalar, y / scalar);
    }

    template<typename T>
    Vector2<T> & Vector2<T>::operator += (const Vector2<T> & vector)
    {
        x += vector.x;
        y += vector.y;
        return *this;
    }

    template<typename T>
    Vector2<T> & Vector2<T>::operator -= (const Vector2<T> & vector)
    {
        x -= vector.x;
        y -= vector.y;
        return *this;
    }

    template<typename T>
    Vector2<T> & Vector2<T>::operator *= (const Vector2<T> & vector)
    {
        x *= vector.x;
        y *= vector.y;
        return *this;
    }

    template<typename T>
    Vector2<T> & Vector2<T>::operator *= (const T scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    template<typename T>
    Vector2<T> & Vector2<T>::operator /= (const Vector2<T> & vector)
    {
        x /= vector.x;
        y /= vector.y;
        return *this;
    }

    template<typename T>
    Vector2<T> & Vector2<T>::operator /= (const T scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    U Vector2<T>::length() const
    {
        return std::sqrt(static_cast<U>((x*x) + (y*y)));
    }

    template<typename T>
    Vector2<T> Vector2<T>::normal() const
    {
        T len = std::sqrt(static_cast<T>((x*x) + (y*y)));

        if (len == 0.0f)
        {
            return Vector2<T>(static_cast<T>(0), static_cast<T>(0));
        }

        len = static_cast<T>(1) / len;
        return Vector2<T>(x * len, y * len);
    }

    template<typename T>
    Vector2<T> & Vector2<T>::normalize()
    {
        T len = std::sqrt(static_cast<T>((x*x) + (y*y)));

        if (len == 0.0f)
        {
            *this;
        }

        len = static_cast<T>(1) / len;
        x *= len;
        y *= len;
        return *this;
    }

    template<typename T>
    Vector2<T> Vector2<T>::absolute() const
    {
        return Vector2<T>(std::abs(x), std::abs(y));
    }

    template<typename T>
    template<typename U>
    U Vector2<T>::dot(const Vector2<T> & vector) const
    {
        return static_cast<U>((x * vector.x) +
                              (y * vector.y));
    }


    template<typename T>
    template<typename U>
    U Vector2<T>::dot(const Vector2<T> & vector1, const Vector2<T> & vector2)
    {
        return static_cast<U>((vector1.x * vector2.x) +
                              (vector1.y * vector2.y));
    }


    // Vector 3 implementation.
    template<typename T>
    Vector3<T>::Vector3()
    { }

    template<typename T>
    Vector3<T>::Vector3(const T scalar) :
        x(scalar),
        y(scalar),
        z(scalar)
    { }

    template<typename T>
    Vector3<T>::Vector3(const T p_x, const T p_y, const T p_z) :
        x(p_x),
        y(p_y),
        z(p_z)
    { }

    template<typename T>
    Vector3<T>::Vector3(const Vector3<T> & vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z)
    { }

    template<typename T>
    template<typename U>
    Vector3<T>::Vector3(const Vector3<U> & vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y)),
        z(static_cast<T>(vector.z))
    { }

    template<typename T>
    Vector3<T> Vector3<T>::operator + (const Vector3<T> & vector) const
    {
        return Vector3<T>(x + vector.x, y + vector.y, z + vector.z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator - () const
    {
        return Vector3<T>(-x, -y, -z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator - (const Vector3<T> & vector) const
    {
        return Vector3<T>(x - vector.x, y - vector.y, z - vector.z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator * (const Vector3<T> & vector) const
    {
        return Vector3<T>(x * vector.x, y * vector.y, z * vector.z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator * (const T scalar) const
    {
        return Vector3<T>(x * scalar, y * scalar, z * scalar);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator / (const Vector3<T> & vector) const
    {
        return Vector3<T>(x / vector.x, y / vector.y, z / vector.z);
    }

    template<typename T>
    Vector3<T> Vector3<T>::operator / (const T scalar) const
    {
        return Vector3<T>(x / scalar, y / scalar, z / scalar);
    }

    template<typename T>
    Vector3<T> & Vector3<T>::operator += (const Vector3<T> & vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return *this;
    }

    template<typename T>
    Vector3<T> & Vector3<T>::operator -= (const Vector3<T> & vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return *this;
    }

    template<typename T>
    Vector3<T> & Vector3<T>::operator *= (const Vector3<T> & vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        return *this;
    }

    template<typename T>
    Vector3<T> & Vector3<T>::operator *= (const T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    template<typename T>
    Vector3<T> & Vector3<T>::operator /= (const Vector3<T> & vector)
    {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        return *this;
    }

    template<typename T>
    Vector3<T> & Vector3<T>::operator /= (const T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    U Vector3<T>::length() const
    {
        return std::sqrt(static_cast<U>((x*x) + (y*y) + (z*z)));
    }

    template<typename T>
    Vector3<T> Vector3<T>::normal() const
    {
        T len = std::sqrt(static_cast<T>((x*x) + (y*y) + (z*z)));

        if (len == 0.0f)
        {
            return Vector3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
        }

        len = static_cast<T>(1) / len;
        return Vector3<T>(x * len, y * len, z * len);
    }

    template<typename T>
    Vector3<T> & Vector3<T>::normalize()
    {
        T len = std::sqrt(static_cast<T>((x*x) + (y*y) + (z*z)));

        if (len == 0.0f)
        {
            *this;
        }

        len = static_cast<T>(1) / len;
        x *= len;
        y *= len;
        z *= len;
        return *this;
    }

    template<typename T>
    Vector3<T> Vector3<T>::absolute() const
    {
        return Vector3<T>(std::abs(x), std::abs(y), std::abs(z));
    }

    template<typename T>
    template<typename U>
    U Vector3<T>::dot(const Vector3<T> & vector) const
    {
        return static_cast<U>((x * vector.x) +
                              (y * vector.y) +
                              (z * vector.z));
    }

    template<typename T>
    Vector3<T> Vector3<T>::cross(const Vector3<T> & vector) const
    {
        return Vector3<T>((y * vector.z) - (z * vector.y),
                          (z * vector.x) - (x * vector.z),
                          (x * vector.y) - (y * vector.x));
    }


    template<typename T>
    template<typename U>
    U Vector3<T>::dot(const Vector3<T> & vector1, const Vector3<T> & vector2)
    {
        return static_cast<U>((vector1.x * vector2.x) +
                              (vector1.y * vector2.y) +
                              (vector1.z * vector2.z));
    }


    // Vector 4 implementation.
    template<typename T>
    Vector4<T>::Vector4()
    { }

    template<typename T>
    Vector4<T>::Vector4(const T scalar) :
        x(scalar),
        y(scalar),
        z(scalar),
        w(scalar)
    { }

    template<typename T>
    Vector4<T>::Vector4(const T p_x, const T p_y, const T p_z, const T p_w) :
        x(p_x),
        y(p_y),
        z(p_z),
        w(p_w)
    { }

    template<typename T>
    Vector4<T>::Vector4(const Vector4<T> & vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z),
        w(vector.w)
    { }

    template<typename T>
    template<typename U>
    Vector4<T>::Vector4(const Vector4<U> & vector) :
        x(static_cast<T>(vector.x)),
        y(static_cast<T>(vector.y)),
        z(static_cast<T>(vector.z)),
        w(static_cast<T>(vector.w))
    { }

    template<typename T>
    Vector4<T> Vector4<T>::operator + (const Vector4<T> & vector) const
    {
        return Vector4<T>(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
    }

    template<typename T>
    Vector4<T> Vector4<T>::operator - () const
    {
        return Vector4<T>(-x, -y, -z, -w);
    }

    template<typename T>
    Vector4<T> Vector4<T>::operator - (const Vector4<T> & vector) const
    {
        return Vector4<T>(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
    }

    template<typename T>
    Vector4<T> Vector4<T>::operator * (const Vector4<T> & vector) const
    {
        return Vector4<T>(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
    }

    template<typename T>
    Vector4<T> Vector4<T>::operator * (const T scalar) const
    {
        return Vector4<T>(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    template<typename T>
    Vector4<T> Vector4<T>::operator / (const Vector4<T> & vector) const
    {
        return Vector4<T>(x / vector.x, y / vector.y, z / vector.z, w / vector.w);
    }

    template<typename T>
    Vector4<T> Vector4<T>::operator / (const T scalar) const
    {
        return Vector4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    template<typename T>
    Vector4<T> & Vector4<T>::operator += (const Vector4<T> & vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        w += vector.w;
        return *this;
    }

    template<typename T>
    Vector4<T> & Vector4<T>::operator -= (const Vector4<T> & vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        w -= vector.w;
        return *this;
    }

    template<typename T>
    Vector4<T> & Vector4<T>::operator *= (const Vector4<T> & vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        w *= vector.w;
        return *this;
    }

    template<typename T>
    Vector4<T> & Vector4<T>::operator *= (const T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    template<typename T>
    Vector4<T> & Vector4<T>::operator /= (const Vector4<T> & vector)
    {
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
        w /= vector.w;
        return *this;
    }

    template<typename T>
    Vector4<T> & Vector4<T>::operator /= (const T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    template<typename T>
    template<typename U>
    U Vector4<T>::length() const
    {
        return std::sqrt(static_cast<U>((x*x) + (y*y) + (z*z) + (w*w)));
    }

    template<typename T>
    Vector4<T> Vector4<T>::normal() const
    {
        T len = std::sqrt(static_cast<T>((x*x) + (y*y) + (z*z) + (w*w)));

        if (len == 0.0f)
        {
            return Vector4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
        }

        len = static_cast<T>(1) / len;
        return Vector4<T>(x * len, y * len, z * len, w * len);
    }

    template<typename T>
    Vector4<T> & Vector4<T>::normalize()
    {
        T len = std::sqrt(static_cast<T>((x*x) + (y*y) + (z*z) + (w*w)));

        if (len == 0.0f)
        {
            *this;
        }

        len = static_cast<T>(1) / len;
        x *= len;
        y *= len;
        z *= len;
        w *= len;
        return *this;
    }

    template<typename T>
    Vector4<T> Vector4<T>::absolute() const
    {
        return Vector3<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
    }

    template<typename T>
    template<typename U>
    U Vector4<T>::dot(const Vector4<T> & vector) const
    {
        return static_cast<U>((x * vector.x) +
                              (y * vector.y) +
                              (z * vector.z) +
                              (w * vector.w));
    }

    template<typename T>
    template<typename U>
    U Vector4<T>::dot(const Vector4<T> & vector1, const Vector4<T> & vector2)
    {
        return static_cast<U>((vector1.x * vector2.x) +
                              (vector1.y * vector2.y) +
                              (vector1.z * vector2.z) +
                              (vector1.w * vector2.w));
    }

}
