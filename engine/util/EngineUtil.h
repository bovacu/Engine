#pragma once

#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include "pch.h"
#include <random>

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


#define PI 3.14159265359

namespace engine {

    template<typename T>
    struct Vec2 {
        public:
            T x, y;

            Vec2(const Vec2& _point)  = default;
            Vec2() : x(0), y(0) {  }
            Vec2(T _x, T _y) : x(_x), y(_y) {  }

            [[nodiscard]] float dotProduct(const Vec2<T>& _p) const {
                return this->x * _p.x + this->y * _p.y;
            }

            [[nodiscard]] float distance(const Vec2<T>& _p) const {
                return std::sqrt((_p.x - this->x) * (_p.x - this->x) + (_p.y - this->y) * (_p.y - this->y));
            }

            [[nodiscard]] float magnitude() const {
                return std::sqrt(this->x * this->x + this->y * this->y);
            }

            static Vec2<float> normalize(Vec2<T>& _p) {
                float _magnitude = _p.magnitude();
                return Vec2<float>(_p.x / _magnitude, _p.y / _magnitude);
            }

            inline Vec2<T>& operator=(const Vec2<T>& _p) = default;

            inline Vec2<T> operator-(const Vec2<T>& _p) const {
                return Vec2<T>(this->x - _p.x, this->y - _p.y);
            }

            inline void operator-=(const Vec2<T>& _p) {
                this->x -= _p.x;
                this->y -= _p.y;
            }

            inline Vec2<T> operator+(const Vec2<T>& _p) const{
                return Vec2<T>(this->x + _p.x, this->y + _p.y);
            }

            inline void operator+=(const Vec2<T>& _p) {
                this->x += _p.x;
                this->y += _p.y;
            }

            inline Vec2<T> operator/(const float _a) const{
                return Vec2<T>(this->x / _a, this->y / _a);
            }

            inline Vec2 operator*(const float _a) const{
                return Vec2<T>(this->x * _a, this->y *_a);
            }

            inline Vec2<T> operator*(const Vec2<T>& _p) const{
                return Vec2<T>(this->x * _p.x, this->y *_p.y);
            }

            inline Vec2<T> operator*=(const Vec2<T>& _p) const{
                return Vec2<T>(this->x * _p.x, this->y *_p.y);
            }

            inline Vec2<T> operator+(const float _a) const{
                return Vec2<T>(this->x + _a, this->y + _a);
            }

            inline Vec2<T> operator+=(const float _a) const{
                return Vec2<T>(this->x + _a, this->y + _a);
            }

            inline Vec2<T> operator-(const float _a) const{
                return Vec2<T>(this->x - _a, this->y - _a);
            }

            inline bool operator==(const Vec2<T>& _p) const {
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

    typedef Vec2<float>         Vec2f;
    typedef Vec2<int>           Vec2i;
    typedef Vec2<double>        Vec2d;
    typedef Vec2<unsigned int>  Vec2ui;

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
        unsigned char r, g, b, a;

		Color() : r(0), g(0), b(0), a(0) {  }
		Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255) : r(_r), g(_g), b(_b), a(_a) {  }

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
		static const Color Transparent;
	};

    struct Probability {
        float prob = 0.0f;
        bool happened = false;
    };

	class Random {
	    private:
            std::random_device rd;
            std::mt19937 mt;

	    public:


	    public:
	        explicit Random() : mt(rd()) {  };

            int randomi(int _min, int _max) {
                if(_min > _max) {
                    int _aux = _min;
                    _min = _max;
                    _max = _min;
                }
                std::uniform_int_distribution<int> _dist(_min, _max);
                return _dist(this->mt);
            }

            float randomf(float _min, float _max) {
                if(_min > _max) {
                    float _aux = _min;
                    _min = _max;
                    _max = _min;
                }
                std::uniform_real_distribution<float> _dist(_min, _max);
                return _dist(this->mt);
            }

            Probability probability(float _chanceToHappen) {
                if(_chanceToHappen > 1.0f) _chanceToHappen = 1.0f;
                if(_chanceToHappen < 0.0f) _chanceToHappen = 0.0f;

                float _chance = 1.f - _chanceToHappen;
                float _leftProbability = this->randomf(0.0f, 1.0f);

                Probability _p {_leftProbability, _leftProbability >= _chance};
                return _p;
            }

            Probability probability(int _chanceToHappen) {
                if(_chanceToHappen > 100)   _chanceToHappen = 100;
                if(_chanceToHappen < 0)     _chanceToHappen = 0;
                return this->probability((float)_chanceToHappen / 100.f);
            }
	};

	class Easing {
	    public:
	        /// Linear
            static float inLinear(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                return _changeInValue * _currentTime / _duration + _startValue;
            }

	        /// Quadratic
            static float inQuadratic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                return _changeInValue * _currentTime * _currentTime + _startValue;
            }
            static float outQuadratic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                return -_changeInValue * _currentTime * (_currentTime - 2) + _startValue;
            }
            static float inOutQuadratic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration / 2;
                if (_currentTime < 1) return _changeInValue / 2 * _currentTime * _currentTime + _startValue;
                _currentTime--;
                return -_changeInValue / 2 * (_currentTime * (_currentTime - 2) - 1) + _startValue;
            }

	        /// Cubic
            static float inCubic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                return _changeInValue * _currentTime * _currentTime * _currentTime + _startValue;
            }
            static float outCubic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                _currentTime--;
                return _changeInValue * (_currentTime * _currentTime * _currentTime + 1) + _startValue;
            }
            static float inOutCubic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration / 2;
                if (_currentTime < 1) return _changeInValue / 2 * _currentTime * _currentTime * _currentTime + _startValue;
                _currentTime -= 2;
                return _changeInValue / 2 * (_currentTime * _currentTime * _currentTime + 2) + _startValue;
            }

            /// Quartic
            static float inQuartic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                return _changeInValue * _currentTime * _currentTime * _currentTime * _currentTime + _startValue;
            }
            static float outQuartic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                _currentTime--;
                return -_changeInValue * (_currentTime * _currentTime * _currentTime * _currentTime - 1) + _startValue;
            }
            static float inOutQuartic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration / 2;
                if (_currentTime < 1) return _changeInValue / 2 * _currentTime * _currentTime * _currentTime * _currentTime + _startValue;
                _currentTime -= 2;
                return -_changeInValue / 2 * (_currentTime * _currentTime * _currentTime * _currentTime - 2) + -_startValue;
            }

            /// Quintic
            static float inQuintic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                return _changeInValue * _currentTime * _currentTime * _currentTime * _currentTime * _currentTime + _startValue;
            }
            static float outQuintic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                _currentTime--;
                return _changeInValue * (_currentTime * _currentTime * _currentTime * _currentTime * _currentTime + 1) + _startValue;
            }
            static float inOutQuintic(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration / 2;
                if (_currentTime < 1) return _changeInValue / 2 * _currentTime * _currentTime * _currentTime * _currentTime * _currentTime + _startValue;
                _currentTime -= 2;
                return _changeInValue / 2 * (_currentTime * _currentTime * _currentTime * _currentTime * _currentTime + 2) + _startValue;
            }

            /// Sinusoidal
            static float inSine(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                return -_changeInValue * (float)std::cos(_currentTime / _duration * (PI / 2)) + _changeInValue + _startValue;
            }
            static float outSine(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                return _changeInValue * (float)std::sin(_currentTime / _duration * (PI / 2)) + _startValue;
            }
            static float inOutSine(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                return -_changeInValue / 2 * ((float)std::cos(PI * _currentTime / _duration) - 1) + _startValue;
            }

            /// Exponential
            static float inExponential(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                return _changeInValue * (float)std::pow( 2, 10 * (_currentTime / _duration - 1) ) + _startValue;
            }
            static float outExponential(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                return _changeInValue * ( -(float)std::pow( 2, -10 * _currentTime / _duration ) + 1 ) + _startValue;
            }
            static float inOutExponential(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration / 2;
                if (_currentTime < 1) return _changeInValue / 2 * (float)std::pow( 2, 10 * (_currentTime - 1) ) + _startValue;
                _currentTime--;
                return _changeInValue / 2 * ( -(float)std::pow( 2, -10 * _currentTime) + 2 ) + _startValue;
            }

            /// Exponential
            static float inCircular(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                return -_currentTime * ((float)std::sqrt(1 - _currentTime * _currentTime) - 1) + _startValue;
            }
            static float outCircular(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration;
                _currentTime--;
                return _changeInValue * (float)std::sqrt(1 - _currentTime * _currentTime) + _startValue;
            }
            static float inOutCircular(float _currentTime, float _startValue, float _changeInValue, float _duration) {
                _currentTime /= _duration / 2;
                if (_currentTime < 1) return -_changeInValue / 2 * ((float)std::sqrt(1 - _currentTime * _currentTime) - 1) + _startValue;
                _currentTime -= 2;
                return _changeInValue / 2 * ((float)std::sqrt(1 - _currentTime * _currentTime) + 1) + _startValue;
            }
	};

    namespace functions {

        // This method return _value if _value is between _minValue and _maxValue,
        // _minValue if _value is lower and _maxValue if _value is higher
        float clamp(float _value, float _minValue, float _maxValue);
    }
}

#endif // ENGINE_UTIL_H