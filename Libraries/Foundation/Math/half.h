#pragma once

#include "Foundation/API.h"
#include "Platform/Types.h"

//-------
// Limits
//-------

// Smallest positive Half
#define HALF_MIN	5.96046448e-08

// Smallest positive normalized Half
#define HALF_NRM_MIN	6.10351562e-05

// Largest positive Half
#define HALF_MAX	65504.0

// Smallest positive e for which Half (1.0 + e) != Half (1.0)
#define HALF_EPSILON	0.00097656

// Number of digits in mantissa (significand + hidden leading 1)
#define HALF_MANT_DIG	11		

// Number of base 10 digits that can be represented without change
#define HALF_DIG	2

// Base of the exponent
#define HALF_RADIX	2

// Minimum negative integer such that HALF_RADIX raised to the power of
// one less than that integer is a normalized Half
#define HALF_MIN_EXP	-13

// Maximum positive integer such that HALF_RADIX raised to the power of
// one less than that integer is a normalized Half
#define HALF_MAX_EXP	16

// Minimum positive integer such that 10 raised to that power is
// a normalized Half
#define HALF_MIN_10_EXP	-4

// Maximum positive integer such that 10 raised to that power is
// a normalized Half
#define HALF_MAX_10_EXP	4


namespace Math
{
    //---------------------------------------------------------------------------
    //
    // Implementation --
    //
    // Representation of a f32:
    //
    //	We assume that a f32, f, is an IEEE 754 single-precision
    //	floating point number, whose bits are arranged as follows:
    //
    //	    31 (msb)
    //	    | 
    //	    | 30     23
    //	    | |      | 
    //	    | |      | 22                    0 (lsb)
    //	    | |      | |                     |
    //	    X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
    //
    //	    s e        m
    //
    //	S is the sign-bit, e is the exponent and m is the significand.
    //
    //	If e is between 1 and 254, f is a normalized number:
    //
    //	            s    e-127
    //	    f = (-1)  * 2      * 1.m
    //
    //	If e is 0, and m is not zero, f is a denormalized number:
    //
    //	            s    -126
    //	    f = (-1)  * 2      * 0.m
    //
    //	If e and m are both zero, f is zero:
    //
    //	    f = 0.0
    //
    //	If e is 255, f is an "infinity" or "not a number" (NAN),
    //	depending on whether m is zero or not.
    //
    //	Examples:
    //
    //	    0 00000000 00000000000000000000000 = 0.0
    //	    0 01111110 00000000000000000000000 = 0.5
    //	    0 01111111 00000000000000000000000 = 1.0
    //	    0 10000000 00000000000000000000000 = 2.0
    //	    0 10000000 10000000000000000000000 = 3.0
    //	    1 10000101 11110000010000000000000 = -124.0625
    //	    0 11111111 00000000000000000000000 = +infinity
    //	    1 11111111 00000000000000000000000 = -infinity
    //	    0 11111111 10000000000000000000000 = NAN
    //	    1 11111111 11111111111111111111111 = NAN
    //
    // Representation of a Half:
    //
    //	Here is the bit-layout for a Half number, h:
    //
    //	    15 (msb)
    //	    | 
    //	    | 14  10
    //	    | |   |
    //	    | |   | 9        0 (lsb)
    //	    | |   | |        |
    //	    X XXXXX XXXXXXXXXX
    //
    //	    s e     m
    //
    //	S is the sign-bit, e is the exponent and m is the significand.
    //
    //	If e is between 1 and 30, h is a normalized number:
    //
    //	            s    e-15
    //	    h = (-1)  * 2     * 1.m
    //
    //	If e is 0, and m is not zero, h is a denormalized number:
    //
    //	            S    -14
    //	    h = (-1)  * 2     * 0.m
    //
    //	If e and m are both zero, h is zero:
    //
    //	    h = 0.0
    //
    //	If e is 31, h is an "infinity" or "not a number" (NAN),
    //	depending on whether m is zero or not.
    //
    //	Examples:
    //
    //	    0 00000 0000000000 = 0.0
    //	    0 01110 0000000000 = 0.5
    //	    0 01111 0000000000 = 1.0
    //	    0 10000 0000000000 = 2.0
    //	    0 10000 1000000000 = 3.0
    //	    1 10101 1111000001 = -124.0625
    //	    0 11111 0000000000 = +infinity
    //	    1 11111 0000000000 = -infinity
    //	    0 11111 1000000000 = NAN
    //	    1 11111 1111111111 = NAN
    //
    // Conversion:
    //
    //	Converting from a f32 to a Half requires some non-trivial bit
    //	manipulations.  In some cases, this makes conversion relatively
    //	slow, but the most common case is accelerated via table lookups.
    //
    //	Converting back from a Half to a f32 is easier because we don't
    //	have to do any rounding.  In addition, there are only 65536
    //	different Half numbers; we can convert each of those numbers once
    //	and store the results in a table.  Later, all conversions can be
    //	done using only simple table lookups.
    //
    //---------------------------------------------------------------------------

    class FOUNDATION_API Half
    {
    public:

        //-------------
        // Constructors
        //-------------
        Half ()
        {
        }

        Half (const Half &h)
        {
            _h = h._h;
        }

        Half (f32 f)
        {
            if (f == 0)
            {
                _h = 0;
            }
            else
            {
                //
                // We extract the combined sign and exponent, e, from our
                // floating-point number, f.  Then we convert e to the sign
                // and exponent of the Half number via a table lookup.
                //
                // For the most common case, where a normalized Half is produced,
                // the table lookup returns a non-zero value; in this case, all
                // we have to do, is round f's significand to 10 bits and combine
                // the result with e.
                //
                // For all other cases (overflow, zeroes, denormalized numbers
                // resulting from underflow, infinities and NANs), the table
                // lookup returns zero, and we call a longer, non-inline function
                // to do the f32-to-Half conversion.
                //

                uif x;

                x.f = f;

                register i32 e = (x.i >> 23) & 0x000001ff;

                e = _eLut[e];

                if (e)
                {
                    //
                    // Simple case - round the significand and
                    // combine it with the sign and exponent.
                    //

                    _h = e + (((x.i & 0x007fffff) + 0x00001000) >> 13);
                }
                else
                {
                    //
                    // Difficult case - call a function.
                    //
                    _h = convert(x.i);
                }
            }
        }

        //--------------------
        // Conversion to f32
        //--------------------
        operator f32() const
        {
            return _toFloat[_h].f;
        }


        //------------
        // Unary minus
        //------------
        Half operator - () const
        {
            Half h;
            h._h = _h ^ 0x8000;
            return h;
        }


        //-----------
        // Assignment
        //-----------
        Half operator = (Half  h)
        {
            _h = h._h;
            return *this;
        }

        Half operator = (f32 f)
        {
            *this = Half (f);
            return *this;
        }

        Half operator += (Half  h)
        {
            *this = Half (f32 (*this) + f32 (h));
            return *this;
        }

        Half operator += (f32 f)
        {
            *this = Half (f32 (*this) + f);
            return *this;
        }

        Half operator -= (Half  h)
        {
            *this = Half (f32 (*this) - f32 (h));
            return *this;
        }

        Half operator -= (f32 f)
        {
            *this = Half (f32 (*this) - f);
            return *this;
        }

        Half operator *= (Half  h)
        {
            *this = Half (f32 (*this) * f32 (h));
            return *this;
        }

        Half operator *= (f32 f)
        {
            *this = Half (f32 (*this) * f);
            return *this;
        }

        Half operator /= (Half  h)
        {
            *this = Half (f32 (*this) / f32 (h));
            return *this;
        }

        Half operator /= (f32 f)
        {
            *this = Half (f32 (*this) / f);
            return *this;
        }


        //---------------------------------------------------------
        // Round to n-bit precision (n should be between 0 and 10).
        // After rounding, the significand's 10-n least significant
        // bits will be zero.
        //---------------------------------------------------------
        Half round(u32 n) const
        {
            //
            // Parameter check.
            //

            if (n >= 10)
                return *this;

            //
            // Disassemble h into the sign, s,
            // and the combined exponent and significand, e.
            //

            u16 s = _h & 0x8000;
            u16 e = _h & 0x7fff;

            //
            // Round the exponent and significand to the nearest value
            // where ones occur only in the (10-n) most significant bits.
            // Note that the exponent adjusts automatically if rounding
            // up causes the significand to overflow.
            //

            e >>= 9 - n;
            e  += e & 1;
            e <<= 9 - n;

            //
            // Check for exponent overflow.
            //

            if (e >= 0x7c00)
            {
                //
                // Overflow occurred -- truncate instead of rounding.
                //

                e = _h;
                e >>= 10 - n;
                e <<= 10 - n;
            }

            //
            // Put the original sign bit back.
            //

            Half h;
            h._h = s | e;

            return h;
        }

        //--------------------------------------------------------------------
        // Classification:
        //
        //	h.isFinite()		returns true if h is a normalized number,
        //				a denormalized number or zero
        //
        //	h.isNormalized()	returns true if h is a normalized number
        //
        //	h.isDenormalized()	returns true if h is a denormalized number
        //
        //	h.isZero()		returns true if h is zero
        //
        //	h.isNan()		returns true if h is a NAN
        //
        //	h.isInfinity()		returns true if h is a positive
        //				or a negative infinity
        //
        //	h.isNegative()		returns true if the sign bit of h
        //				is set (negative)
        //--------------------------------------------------------------------
        bool isFinite() const
        {
            u16 e = (_h >> 10) & 0x001f;
            return e < 31;
        }

        bool isNormalized() const
        {
            u16 e = (_h >> 10) & 0x001f;
            return e > 0 && e < 31;
        }

        bool isDenormalized() const
        {
            u16 e = (_h >> 10) & 0x001f;
            u16 m =  _h & 0x3ff;
            return e == 0 && m != 0;
        }

        bool isZero() const
        {
            return (_h & 0x7fff) == 0;
        }

        bool isNan() const
        {
            u16 e = (_h >> 10) & 0x001f;
            u16 m =  _h & 0x3ff;
            return e == 31 && m != 0;
        }

        bool isInfinity() const
        {
            u16 e = (_h >> 10) & 0x001f;
            u16 m =  _h & 0x3ff;
            return e == 31 && m == 0;
        }

        bool isNegative() const
        {
            return (_h & 0x8000) != 0;
        }



        //--------------------------------------------
        // Special values
        //
        //	posInf()	returns +infinity
        //
        //	negInf()	returns +infinity
        //
        //	qNan()		returns a NAN with the bit
        //			pattern 0111111111111111
        //
        //	sNan()		returns a NAN with the bit
        //			pattern 0111110111111111
        //--------------------------------------------
        static Half posInf()
        {
            Half h;
            h._h = 0x7c00;
            return h;
        }

        static Half negInf()
        {
            Half h;
            h._h = 0xfc00;
            return h;
        }

        static Half qNan()
        {
            Half h;
            h._h = 0x7fff;
            return h;
        }

        static Half sNan()
        {
            Half h;
            h._h = 0x7dff;
            return h;
        }


        //--------------------------------------
        // Access to the internal representation
        //--------------------------------------
        u16	bits() const
        {
            return _h;
        }

        void setBits(u16 bits)
        {
            _h = bits;
        }

        union uif
        {
            u32	i;
            f32		f;
        };

        static void	createBitString(char c[19], Half h);
        static void	createBitString(char c[35], f32 f);

        static const uif _toFloat[1 << 16];
        static const u16	_eLut[1 << 9];

    private:
        static i16 convert(i32 i);
        static f32 overflow();
        u16 _h;
    };

    typedef Half f16;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  HalfToFloat()
    //
    //  written by: rob wyatt
    //
    //  Convert a standard Half to an IEEE f32.
    //  Including handling all the exception cases such as zero, nan, inf
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////

    inline f32 HalfToFloat(i16 y)
    {
        i32 s = (y >> 15) & 0x00000001;
        i32 e = (y >> 10) & 0x0000001f;
        i32 m =  y        & 0x000003ff;

        if (e == 0)
        {
            if (m == 0)
            {
                //
                // Plus or minus zero
                //

                u32 r = s << 31;
                return *(f32*)&r;
            }
            else
            {
                //
                // Denormalized number -- renormalize it
                //
                while (!(m & 0x00000400))
                {
                    m <<= 1;
                    e -=  1;
                }
                e += 1;
                m &= ~0x00000400;
            }
        }
        else if (e == 31)
        {
            if (m == 0)
            {
                //
                // Positive or negative infinity
                //

                u32 r = (s << 31) | 0x7f800000;
                return *(f32*)&r;
            }
            else
            {
                //
                // Nan -- preserve sign and significand bits
                //

                u32 r = (s << 31) | 0x7f800000 | (m << 13);
                return *(f32*)&r;
            }
        }

        //
        // Normalized number
        //

        e = e + (127 - 15);
        m = m << 13;

        //
        // Assemble s, e and m.
        //

        u32 r = (s << 31) | (e << 23) | m;
        return *(f32*)&r;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  FloatToHalf()
    //
    //  written by: rob wyatt
    //
    //  Convert an IEEE f32 to Half f32, including handling all the
    //  exception cases such as zero, nan, inf
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////

    inline i16 FloatToHalf(f32 f)
    {
        // load f as an i32 bit pattern
        i32 i = *(i32*)&f;

        i32 s =  (i >> 16) & 0x00008000;
        i32 e = ((i >> 23) & 0x000000ff) - (127 - 15);
        i32 m =   i        & 0x007fffff;

        //
        // Now reassemble s, e and m into a Half:
        //

        if (e <= 0)
        {
            if (e < -10)
            {
                return 0;
            }
            m = (m | 0x00800000) >> (1 - e);

            if (m &  0x00001000)
                m += 0x00002000;
            return s | (m >> 13);
        }
        else if (e == 0xff - (127 - 15))
        {
            if (m == 0)
            {
                return s | 0x7c00;
            }
            else
            {
                return s | 0x7c00 | (m >> 13);
            }
        }
        else
        {
            if (m &  0x00001000)
            {
                m += 0x00002000;

                if (m & 0x00800000)
                {
                    m =  0;		// overflow in significand,
                    e += 1;		// adjust exponent
                }
            }

            if (e > 30)
            {
                return s | 0x7c00;	// if this returns, the Half becomes an
            }   			// infinity with the same sign as f.

            //
            // Assemble the Half from s, e and m.
            //
            return s | (e << 10) | (m >> 13);
        }
    }
}