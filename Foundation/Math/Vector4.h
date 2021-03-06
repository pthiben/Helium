#pragma once

#include <cmath>
#include <iostream>

#include <vector>

#include "Utils.h"
#include "Vector3.h"

namespace Helium
{
    namespace Math
    {
        class FOUNDATION_API Matrix4;

        class FOUNDATION_API Vector4
        {
        public:
            f32 x, y, z, w;

            const static Vector4 Zero;
            const static Vector4 Point;
            const static Vector4 BasisX;
            const static Vector4 BasisY;
            const static Vector4 BasisZ;
            const static Vector4 BasisW;

            Vector4           () : x(0), y(0), z(0), w(0) {}
            explicit Vector4  ( const Vector3& v ) : x(v.x), y(v.y), z(v.z), w(0) {}
            explicit Vector4  ( f32 vx, f32 vy, f32 vz, f32 vw ) : x(vx), y(vy), z(vz), w(vw) {}
            explicit Vector4  ( f32 val ) : x( val ), y( val ), z( val ), w( val ) {}

            // lame, people just called SetXYZW too much in the old lib, this is a concession to that
            inline void       Set( f32 vx, f32 vy, f32 vz, f32 vw ) { x = vx; y = vy; z = vz; w = vw; }

            Vector4&          operator= (const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
            Vector4&          operator= (const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
            Vector4&          operator+= (const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
            Vector4&          operator-= (const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
            Vector4&          operator*= (const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
            Vector4&          operator*= (const f32 v) { x *= v; y *= v; z *= v; w *= v; return *this; }
            Vector4&          operator/= (const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
            Vector4&          operator/= (const f32 v) { x /= v; y /= v; z /= v; w /= v; return *this; }

            Vector4           operator+ (const Vector4& v) const { return Vector4 (x + v.x, y + v.y, z + v.z, w + v.w); }
            Vector4           operator- (const Vector4& v) const { return Vector4 (x - v.x, y - v.y, z - v.z, w - v.w); }
            Vector4           operator* (const Vector4& v) const { return Vector4 (x * v.x, y * v.y, z * v.z, w * v.w); }
            Vector4           operator* (const f32 v) const { return Vector4 (x * v, y * v, z * v, w * v); }
            Vector4           operator/ (const Vector4& v) const { return Vector4 (x / v.x, y / v.y, z / v.z, w / v.w); }
            Vector4           operator/ (const f32 v) const { return Vector4 (x / v, y / v, z / v, w / v); }

            // unary negation
            Vector4           operator- () const { return Vector4( -x, -y, -z, -w ); }

            f32&              operator[] (const u32 i) {  HELIUM_ASSERT(i < 4); return (&x)[i]; }
            const f32&        operator[] (const u32 i) const {  HELIUM_ASSERT(i < 4); return (&x)[i]; }

            bool              operator== (const Vector4& v) const { return (x == v.x && y == v.y && z == v.z && w == v.w); }
            bool              operator!= (const Vector4& v) const { return !(x == v.x && y == v.y && z == v.z && w == v.w); }
            bool              Equal (const Vector4& v, f32 error = 0) const;
            bool              Valid() { return IsValid(x) && IsValid(y) && IsValid(z) && IsValid(w); }

            f32               LengthSquared () const { return x * x + y * y + z * z + w * w; }
            f32               Length () const;

            Vector4&          Normalize ();
            Vector4           Normalized () const;

            f32               Dot (const Vector4& other) const; 
            Vector4           Cross (const Vector4& v) const;

            void              Transform (const Matrix4& m);

            friend FOUNDATION_API tostream& operator<<(tostream& outStream, const Vector4& vector);
            friend FOUNDATION_API tistream& operator>>(tistream& inStream, Vector4& vector);
        };

        typedef std::vector< Vector4 > V_Vector4;
        typedef std::vector< V_Vector4 > VV_Vector4;

        inline bool Vector4::Equal(const Vector4& v, f32 error) const
        {
            return (fabs(x - v.x) <= error && fabs(y - v.y) <= error && fabs(z - v.z) <= error && fabs(w - v.w) <= error);
        }

        inline f32 Vector4::Length() const
        {
            f32 lenSqr = this->LengthSquared();

            if (lenSqr <= 0)
                return 0;

            return sqrt((f32)lenSqr);
        }

        inline Vector4& Vector4::Normalize() 
        { 
            f32 len = this->Length();

            if (len > DivisorNearZero)
            {
                return *this /= len;
            }
            else
            {
                return *this = Vector4 (0, 0, 0, 0);
            }
        }

        inline Vector4 Vector4::Normalized() const 
        {
            Vector4 result = *this;
            return result.Normalize();
        }

        inline f32 Vector4::Dot(const Vector4& v) const
        {
            return (x * v.x + y * v.y + z * v.z + w * v.w);
        }

        inline Vector4 Vector4::Cross(const Vector4& v) const
        {
            return Vector4 (y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x, 0);
        }

        inline tostream& operator<<(tostream& outStream, const Vector4& vector)
        {
            outStream << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w;

            return outStream;
        }

        inline tistream& operator>>(tistream& inStream, Vector4& vector)
        {
            inStream >> vector.x;
            inStream.ignore();

            inStream >> vector.y;
            inStream.ignore();

            inStream >> vector.z;
            inStream.ignore();

            inStream >> vector.w;

            return inStream;
        }
    }
}