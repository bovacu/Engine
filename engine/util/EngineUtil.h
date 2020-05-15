#pragma once

#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include "pch.h"

/*  =================================================================================================
    =                                      ENGINE UTIL                                              =
    =================================================================================================
    = (*) This class is a collection of objects used all over the code and functions that           =
    =     that can be useful, but doesn't really belong to any class but still need to exist.       =
    =                                                                                               =
    = (*) Vec2f is the representation of a vector or point in a 2D plane, with the expected         =
    =     operations between vectors.                                                               =
    =                                                                                               =
    = (*) Size is the representation of the width and height of an Object.                          =
    =                                                                                               =
	= (*) Color represents the RGBA values of any possible color combination						=
    =                                                                                               =
    = (*) Functions will be described on themselves.                                                =
    =                                                                                               =
    ================================================================================================= 
*/

#define BIT(x) (1 << x)

namespace engine {

    struct Vec2f {
		float x, y;

		Vec2f(const Vec2f& _point)  = default;
		Vec2f() : x(0), y(0) {  }
		Vec2f(float _x, float _y) : x(_x), y(_y) {  } 

		float dotProduct(const Vec2f& _p) const {
			return this->x * _p.x + this->y * _p.y;
		}

		float magnitude() const {
			return std::sqrt(this->x * this->x + this->y * this->y);
		}

		static Vec2f normalize(Vec2f& _p) {
			float _magnitude = _p.magnitude();
			return Vec2f(_p.x / _magnitude, _p.y / _magnitude);
		}

		inline void operator=(const Vec2f& _p) {
			this->x = _p.x;
			this->y = _p.y;
		}

		inline Vec2f operator-(const Vec2f& _p) {
			return Vec2f(this->x - _p.x, this->y - _p.y);
		} 

		inline void operator-=(const Vec2f& _p) {
			this->x -= _p.x;
			this->y -= _p.y;
		} 

		inline Vec2f operator+(const Vec2f& _p) const{
			return Vec2f(this->x + _p.x, this->y + _p.y);
		} 

		inline void operator+=(const Vec2f& _p) {
			this->x += _p.x;
			this->y += _p.y;
		} 

		inline Vec2f operator/(const float _a) const{
			return Vec2f(this->x / _a, this->y / _a);
		} 

		inline Vec2f operator*(const float _a) const{
			return Vec2f(this->x * _a, this->y *_a);
		} 

		inline Vec2f operator*(const Vec2f& _p) const{
			return Vec2f(this->x * _p.x, this->y *_p.y);
		}

		inline Vec2f operator*=(const Vec2f& _p) const{
			return Vec2f(this->x * _p.x, this->y *_p.y);
		} 

		inline Vec2f operator+(const float _a) const{
			return Vec2f(this->x + _a, this->y + _a);
		} 

		inline Vec2f operator+=(const float _a) const{
			return Vec2f(this->x + _a, this->y + _a);
		} 

		inline Vec2f operator-(const float _a) const{
			return Vec2f(this->x - _a, this->y - _a);
		} 

		inline bool operator==(const Vec2f& _p) const {
			return this->x == _p.x && this->y == _p.y;
		}

		inline bool operator>(float _value) const {
		    return this->x > _value && this->y > _value;
		}

        inline bool operator>=(float _value) const {
            return this->x >= _value && this->y >= _value;
        }

        inline bool operator<(float _value) const {
            return this->x < _value && this->y < _value;
        }

        inline bool operator<=(float _value) const {
            return this->x <= _value && this->y <= _value;
        }
	};

    struct Size {
		float width, height;

		Size() : width(0), height(0) {  }
		Size(float _width, float _height) : width(_width), height(_height) {  } 

		inline Size operator-(const Size& _p) const {
			return Size(this->width - _p.width, this->height - _p.height);
		} 

		inline Size operator+(const Size& _p) const{
			return Size(this->width + _p.width, this->height + _p.height);
		} 

		inline bool operator==(const Size& _p) {
			return this->width == _p.width && this->height == _p.height;
		}
	};

	struct Color {
        unsigned char r,g,b,a;

		Color() : r(0), g(0), b(0), a(0) {  }
		Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 1) : r(_r), g(_g), b(_b), a(_a) {  }

		// Color toGrayScale(const Color& _color) {
		// 	std::cerr << "toGrayScale from color currently not supported" << std::endl;
		// 	return _color;
		// }

		// long toHex() {
		// 	std::cerr << "toHex from color currently not supported" << std::endl;
		// 	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
        //    + (a & 0xff);;
		// }

		static const Color White;
		static const Color Black;
		static const Color Blue;
		static const Color Red;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Orange;
		static const Color Green;
		static const Color Brown;
		static const Color Purple;
		static const Color Gray;
	};

    namespace functions {

        // This method return _value if _value is between _minValue and _maxValue,
        // _minValue if _value is lower and _maxValue if _value is higher
        float clamp(float _value, float _minValue, float _maxValue);

    }
}

#endif // ENGINE_UTIL_H