#pragma once
#include "Core/Core.h"
#include "glad/glad.h"
#include "Texture.h"

namespace Hazard {
	class HAZARD_API RenderTexture : public Texture {
	public:
		RenderTexture();
		RenderTexture(std::string _name);
		RenderTexture(Color color);
		RenderTexture(int w, int h);
		RenderTexture(int w, int h, Color color);
		RenderTexture(std::string name, int w, int h, Color color);
		~RenderTexture();

		void Bind() override;
		void Unbind() override;
		virtual void Clear();
		uint32_t GetTextureID() { return texture; }
		Color GetClearColor() { return ClearColor; }

	protected:
		void Create();
		uint32_t texture;
		Color ClearColor = Color();
	};
}