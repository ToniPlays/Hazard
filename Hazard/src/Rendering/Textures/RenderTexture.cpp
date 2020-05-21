#include "hzrpch.h"
#include "RenderTexture.h"

namespace Hazard {
	RenderTexture::RenderTexture(std::string _name) : Texture(_name)
	{
		Create();
	}
	RenderTexture::RenderTexture() : Texture("RenderTexture")
	{
		Create();
	}
	RenderTexture::RenderTexture(Color color) : Texture("RenderTexture")
	{
		ClearColor = color;
		Create();
	}
	RenderTexture::RenderTexture(int w, int h) : Texture("RenderTexture")
	{
		width = w;
		height = h;
		Create();
	}
	RenderTexture::RenderTexture(int w, int h, Color color) : Texture("RenderTexture")
	{
		width = w;
		height = h;
		ClearColor = color;
		Create();
	}
	RenderTexture::RenderTexture(std::string _name, int w, int h, Color color) : Texture(_name)
	{
		width = w;
		height = h;
		ClearColor = color;
		Create();
	}
	RenderTexture::~RenderTexture() {
		glDeleteFramebuffers(1, &ID);
	}
	void RenderTexture::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

	}
	void RenderTexture::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void RenderTexture::Clear()
	{
		Color c = GetClearColor();
		glClearColor(c.r, c.g, c.b, c.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void RenderTexture::Create()
	{
		glGenFramebuffers(1, &ID);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	}
}
