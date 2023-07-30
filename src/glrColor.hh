#pragma once

#include "export.hh"

#include <commons/math/vec3.hh>
#include <commons/math/vec4.hh>
#include <cstdint>

namespace glr
{
	struct Color
	{
		using colorFmt = uint16_t;
		
		GLRENDER_API Color() = default;
		GLRENDER_API Color operator + (Color const &other) const;
		GLRENDER_API bool operator == (Color const &other) const;
		
		GLRENDER_API void fromRGBf(float r, float g, float b);
		GLRENDER_API void fromRGBAf(float r, float g, float b, float a);
		GLRENDER_API void fromRGBui8(uint8_t r, uint8_t g, uint8_t b);
		GLRENDER_API void fromRGBAui8(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		GLRENDER_API void fromRGBui16(uint16_t r, uint16_t g, uint16_t b);
		GLRENDER_API void fromRGBAui16(uint16_t r, uint16_t g, uint16_t b, uint16_t a);
		GLRENDER_API void fromHex(uint32_t hex);
		GLRENDER_API void fromWeb(std::string const &color);
		
		/// 0-1 float RGB representation
		GLRENDER_API vec3<float> asRGBf() const;
		
		/// 0-1 float RGBA representation
		GLRENDER_API vec4<float> asRGBAf() const;
		
		/// 0-255 RGB representation
		GLRENDER_API vec3<uint8_t> asRGBui8() const;
		
		/// 0-255 RGBA representation
		GLRENDER_API vec4<uint8_t> asRGBAui8() const;
		
		/// 0-65535 RGB representation
		GLRENDER_API vec3<uint16_t> asRGBui16() const;
		
		/// 0-65535 RGBA representation
		GLRENDER_API vec4<uint16_t> asRGBAui16() const;
		
		/// 0xAARRGGBB representation
		GLRENDER_API uint32_t asHex() const;
		
		/// #RRGGBBAA representation
		GLRENDER_API std::string asWeb() const;
	
	private:
		colorFmt p_red = 0;
		colorFmt p_green = 0;
		colorFmt p_blue = 0;
		colorFmt p_alpha = 1;
	};
}
