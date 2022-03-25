//
//#include "OpenGLTexture2D.h"
#ifdef HZR_INCLUDE_OPENGL
//#include "Hazard/Rendering/Loaders/TextureFactory.h"
//
//#include <glad/glad.h>
//
//namespace Hazard::Rendering::OpenGL
//{
//	namespace GLUtils {
//		static GLenum TextureWrapToGLWrapMode(ImageWrap wrap) {
//			switch (wrap)
//			{
//			case ImageWrap::Repeat:			return GL_REPEAT;
//			case ImageWrap::RepeatMirror:	return GL_MIRRORED_REPEAT;
//			case ImageWrap::ClampBorder:	return GL_CLAMP_TO_BORDER;
//			case ImageWrap::ClampEdge:		return GL_CLAMP_TO_EDGE;
//			}
//			return GL_REPEAT;
//		}
//		static GLenum TextureFilterToGLFilter(FilterMode filter) {
//			switch (filter)
//			{
//			case FilterMode::Linear:		return GL_LINEAR;
//			case FilterMode::Nearest:		return GL_NEAREST;
//			case FilterMode::LinearMip:		return GL_LINEAR_MIPMAP_LINEAR;
//			case FilterMode::NearestMip:	return GL_NEAREST_MIPMAP_NEAREST;
//			}
//			return GL_LINEAR;
//		}
//	}
//
//	OpenGLTexture2D::OpenGLTexture2D(Texture2DCreateInfo* info)
//	{
//		m_Format = info->Format;
//		glGenTextures(1, &m_ID);
//		glBindTexture(GL_TEXTURE_2D, m_ID);
//
//		if (info->Data == nullptr)
//			LoadFromFile(info->FilePath.string());
//		else
//			LoadFromData(info->Data, info->Width, info->Height);
//
//		SetFilters(info->Filter, info->WrapMode);
//
//		if (info->ClearLocalBuffer) {
//			m_LocalData.Release();
//		}
//	}
//	OpenGLTexture2D::~OpenGLTexture2D()
//	{
//		glDeleteTextures(1, &m_ID);
//	}
//	void OpenGLTexture2D::Bind(uint32_t slot) const
//	{
//		glBindTextureUnit(slot, m_ID);
//	}
//	void OpenGLTexture2D::LoadFromFile(const std::string& path)
//	{
//		TextureHeader header = TextureFactory::LoadFromCacheIfExists(path, true);
//
//		if (!header.IsValid()) {
//			HZR_CORE_INFO("[OpenGLTexture2D]: Failed to load image data from {0}", path);
//			return;
//		}
//
//		m_Width = header.Width;
//		m_Height = header.Height;
//
//		m_DataFormat = (header.Channels == 4) * GL_RGBA + (header.Channels == 3) * GL_RGB;
//		m_InternalFormat = (header.Channels == 4) * GL_RGBA8 + (header.Channels == 3) * GL_RGB8;
//
//		m_LocalData = header.ImageData;
//
//		glTextureStorage2D(m_ID, 1, m_InternalFormat, m_Width, m_Height);
//		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, header.ImageData.Data);
//		HZR_CORE_INFO("Loaded OpenGL texture from {0}", path);
//	}
//	void OpenGLTexture2D::LoadFromData(const void* data, uint32_t width, uint32_t height)
//	{
//		m_Width = width;
//		m_Height = height;
//
//		uint32_t bpp = m_Format == ImageFormat::RGBA ? 4 : 3;
//		m_DataFormat = (bpp == 4) * GL_RGBA + (bpp == 3) * GL_RGB;
//		m_InternalFormat = (bpp == 4) * GL_RGBA8 + (bpp == 3) * GL_RGB8;
//
//		glTextureStorage2D(m_ID, 1, m_InternalFormat, width, height);
//		glTextureSubImage2D(m_ID, 0, 0, 0, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);
//	}
//	void OpenGLTexture2D::SetFilters(TextureFilter* filters, ImageWrap wrap)
//	{
//		if (filters == nullptr)
//		{
//			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		}
//		else 
//		{
//			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GLUtils::TextureFilterToGLFilter(filters->MinFilter));
//			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GLUtils::TextureFilterToGLFilter(filters->MagFilter));
//		}
//
//		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GLUtils::TextureWrapToGLWrapMode(wrap));
//		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GLUtils::TextureWrapToGLWrapMode(wrap));
//	}
//}
#endif
