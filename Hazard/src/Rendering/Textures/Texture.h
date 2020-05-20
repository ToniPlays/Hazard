#pragma once
#include "Core/Core.h"
#include "glad/glad.h"

namespace Hazard {
	class HAZARD_API Texture {

	public:
		std::string name;
	public:
		Texture(std::string _name) : name(_name) {}
		unsigned int GetWidth() { return width; }
		unsigned int GetHeight() { return height; }
		GLuint GetID() { return ID; }

		virtual void Bind() = 0;;
		virtual void Unbind() = 0;

	protected:
		unsigned int width;
		unsigned int height;
		GLuint ID;
	};
}
