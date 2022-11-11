#include "glrTexture.hh"

#include <glad/gl.h>

namespace GLRender
{
	Texture::Texture(uint32_t width, uint32_t height, TextureColorFormat colorFormat, TextureInterpolationMode mode, bool sRGB)
	{
		this->m_fmt = colorFormat;
		this->m_width = width;
		this->m_height = height;
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_handle);
		int32_t f = 0;
		if(colorFormat == TextureColorFormat::RGB) //It's more efficient to use an extra 8 bits of VRAM per pixel
		{
			if(sRGB) f = GL_SRGB8;
			else f = GL_RGB8;
		}
		else if(colorFormat == TextureColorFormat::RGBA)
		{
			if(sRGB) f = GL_SRGB8_ALPHA8;
			else f = GL_RGBA8;
		}
		else if(colorFormat == TextureColorFormat::GREY)
		{
			f = GL_R8;
		}
		glTextureStorage2D(this->m_handle, 1, f, (GLsizei)width, (GLsizei)height);
		this->clear();
		this->setInterpolation(mode, mode);
		this->setAnisotropyLevel(1);
	}
	
	Texture::Texture(uint8_t *data, uint32_t width, uint32_t height, TextureColorFormat colorFormat, TextureInterpolationMode mode, bool sRGB)
	{
		this->m_fmt = colorFormat;
		this->m_width = width;
		this->m_height = height;
		int32_t f = 0, cf = 0;
		if(colorFormat == TextureColorFormat::RGB) //TODO It's more efficient to use an extra 8 bits of VRAM per pixel
		{
			if(sRGB) f = GL_SRGB8;
			else f = GL_RGB8;
			cf = GL_RGB;
		}
		else if(colorFormat == TextureColorFormat::RGBA)
		{
			if(sRGB) f = GL_SRGB8_ALPHA8;
			else f = GL_RGBA8;
			cf = GL_RGBA;
		}
		else if(colorFormat == TextureColorFormat::GREY)
		{
			f = GL_R8;
			cf = GL_RED;
		}
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_handle);
		glTextureStorage2D(this->m_handle, 1, f, (GLsizei)width, (GLsizei)height);
		glTextureSubImage2D(this->m_handle, 0, 0, 0, (GLsizei)this->m_width, (GLsizei)this->m_height, cf, GL_UNSIGNED_BYTE, data);
		this->setInterpolation(mode, mode);
		this->setAnisotropyLevel(1);
	}
	
	Texture::Texture(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, bool sRGB)
	{
		this->m_fmt = TextureColorFormat::RGBA;
		this->m_width = 1;
		this->m_height = 1;
		uint8_t **data = new uint8_t *;
		data[0] = new uint8_t[4];
		data[0][0] = red;
		data[0][1] = green;
		data[0][2] = blue;
		data[0][3] = alpha;
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_handle);
		glTextureStorage2D(this->m_handle, 1, sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8, (GLsizei)this->m_width, (GLsizei)this->m_height);
		glTextureSubImage2D(this->m_handle, 0, 0, 0, (GLsizei)this->m_width, 1, GL_RGBA, GL_UNSIGNED_BYTE, data[0]);
		this->setInterpolation(TextureInterpolationMode::LINEAR, TextureInterpolationMode::LINEAR);
		this->setAnisotropyLevel(1);
	}
	
	Texture::~Texture()
	{
		glDeleteTextures(1, &this->m_handle);
	}
	
	Texture::Texture(Texture &other)
	{
		this->m_handle = other.m_handle;
		other.m_handle = 0;
	}
	
	Texture& Texture::operator=(Texture other)
	{
		this->m_handle = other.m_handle;
		other.m_handle = 0;
		return *this;
	}
	
	Texture::Texture(Texture &&other) noexcept
	{
		this->m_handle = other.m_handle;
		other.m_handle = 0;
	}
	
	Texture& Texture::operator=(Texture &&other) noexcept
	{
		this->m_handle = other.m_handle;
		other.m_handle = 0;
		return *this;
	}
	
	void Texture::use(uint32_t target) const
	{
		int32_t curTex;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &curTex);
		glBindTextureUnit(target, this->m_handle);
	}
	
	void Texture::setInterpolation(TextureInterpolationMode min, TextureInterpolationMode mag) const
	{
		GLenum glMin = GL_NEAREST, glMag = GL_NEAREST;
		switch(min)
		{
			case TextureInterpolationMode::NEAREST: glMin = GL_NEAREST; break;
			case TextureInterpolationMode::LINEAR: glMin = GL_LINEAR; break;
			default: break;
		}
		
		switch(mag)
		{
			case TextureInterpolationMode::NEAREST: glMag = GL_NEAREST; break;
			case TextureInterpolationMode::LINEAR: glMag = GL_LINEAR; break;
			default: break;
		}
		glTextureParameteri(this->m_handle, GL_TEXTURE_MIN_FILTER, (GLint)glMin);
		glTextureParameteri(this->m_handle, GL_TEXTURE_MAG_FILTER, (GLint)glMag);
	}
	
	void Texture::setAnisotropyLevel(uint32_t level) const
	{
		glTextureParameterf(this->m_handle, GL_TEXTURE_MAX_ANISOTROPY, (GLfloat)level);
	}
	
	void Texture::subImage(uint8_t *data, uint32_t w, uint32_t h, uint32_t xPos, uint32_t yPos, TextureColorFormat format) const
	{
		int32_t f = 0;
		switch(format)
		{
			case TextureColorFormat::RGB:
				f = GL_RGB;
				break;
			case TextureColorFormat::RGBA:
				f = GL_RGBA;
				break;
			case TextureColorFormat::GREY:
				f = GL_RED;
				break;
		}
		glTextureSubImage2D(this->m_handle, 0, (GLint)xPos, (GLint)yPos, (GLint)w, (GLint)h, f, GL_UNSIGNED_BYTE, data);
	}
	
	void Texture::clear() const
	{
		int32_t f = 0;
		switch(this->m_fmt)
		{
			case TextureColorFormat::RGB:
				f = GL_RGB;
				break;
			case TextureColorFormat::RGBA:
				f = GL_RGBA;
				break;
			case TextureColorFormat::GREY:
				f = GL_RED;
				break;
		}
		glClearTexImage(this->m_handle, 0, f, GL_UNSIGNED_BYTE, "\0\0\0\0");
	}
}