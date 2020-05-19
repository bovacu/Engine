#include "EngineUtil.h"

namespace engine {

    const Color Color::White(255, 255, 255, 255);
	const Color Color::Black(0, 0, 0, 255);
	const Color Color::Blue(0, 0, 255, 255);
	const Color Color::Red(255, 0, 0, 255);
	const Color Color::Yellow(255, 255, 0, 255);
	const Color Color::Magenta(255, 0, 255, 255);
	const Color Color::Orange(255, 69, 0, 255);
	const Color Color::Green(0, 255, 0, 255);
	const Color Color::Brown(139, 69, 19, 255);
	const Color Color::Purple(128, 0, 128, 255);
	const Color Color::Gray(128, 128, 128, 255);

    namespace functions {

        float clamp(float _value, float _minValue, float _maxValue) {
            return (_value < _minValue) ? _minValue : (_value > _maxValue) ? _maxValue : _value;
        }

    }

}