#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::s_Api;
	std::vector<Shader*> RenderUtils::s_Shaders = std::vector<Shader*>();
	std::vector<Texture*> RenderUtils::s_Textures = std::vector<Texture*>();

	template<typename T>
	T* RenderUtils::Create()
	{
		static_assert(false);
	}
	template<class T, typename Arg>
	T* RenderUtils::Create(Arg args)
	{
		static_assert(false);
	}
	template<class T, typename Arg, typename C>
	T* RenderUtils::Create(Arg args, C name)
	{
		static_assert(false);
	}
	template<typename T, typename Arg>
	T* RenderUtils::Find(Arg args) {
		static_assert(false);
	}

#pragma region Find textures 

	template<>
	Shader* RenderUtils::Find(const char* file) {
		for (Shader* shader : s_Shaders) {
			if (std::strcmp(shader->GetFile(), file) == 0)
				return shader;
		}
		return nullptr;
	}

	template<>
	Texture* RenderUtils::Find(uint32_t textureID) {
		for (Texture* texture : s_Textures) {
			if (texture->GetID() == textureID) 
				return texture;
		}
		return nullptr;
	}
	template<>
	Texture* RenderUtils::Find(const char* file) {
		for (Texture* texture : s_Textures) {
			if (std::string(texture->GetFile()) == std::string(file)) 
				return texture;
		}
		return nullptr;
	}

#pragma endregion

#pragma region Rendering

	template<>
	VertexArray* RenderUtils::Create<VertexArray>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexArray();
		}
		return nullptr;
	}
	template<>
	VertexBuffer* RenderUtils::Create<VertexBuffer>(uint32_t size) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexBuffer(size);
		}
		return nullptr;
	}
	template<>
	IndexBuffer* RenderUtils::Create<IndexBuffer>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLIndexBuffer();
		}
		return nullptr;
	}
	template<>
	Shader* RenderUtils::Create<Shader>(const char* file) {

		Shader* shader = Find<Shader>(file);
		if (shader != nullptr)
			return shader;

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		shader = new OpenGL::OpenGLShader(file);
		}
		s_Shaders.push_back(shader);
		return shader;
	}
	template<>
	OcclusionQuery* RenderUtils::Create<OcclusionQuery>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLOcclusionQuery();
		}
		return nullptr;
	}
#pragma endregion

#pragma region Textures
	template<>
	RenderTexture* RenderUtils::Create<RenderTexture>(TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLRenderTexture(specs);
		}
		return nullptr;
	}

	template<>
	Texture2D* RenderUtils::Create<Texture2D>(const char* path) {

		Texture2D* texture = (Texture2D*)Find<Texture>(path);

		if (texture != nullptr) 
			return texture;

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(path, TextureSpecs()); break;
		}
		s_Textures.push_back(texture);
		return texture;
	}
	template<>
	Texture2D* RenderUtils::Create<Texture2D>(const char* name, TextureSpecs params) {

		Texture2D* texture = (Texture2D*)Find<Texture>(name);

		if (texture != nullptr)
			return texture;

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(name, params); break;
		}
		s_Textures.push_back(texture);
		return texture;
	}
	template<>
	CubemapTexture* RenderUtils::Create<CubemapTexture>() {


		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLCubemapTexture(TextureSpecs());
		}
		return nullptr;
	}
	template<>
	CubemapTexture* RenderUtils::Create<CubemapTexture>(TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLCubemapTexture(specs);
		}
		return nullptr;
	}
	template<>
	CubemapTexture* RenderUtils::Create<CubemapTexture>(std::vector<std::string> faces) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLCubemapTexture(faces);
		}
		return nullptr;
	}
	template<>
	CubemapTexture* RenderUtils::Create<CubemapTexture>(const char* name, const char* extension) {

		HZR_CORE_INFO("Loading cubemap {0}, {1}", name, extension);

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLCubemapTexture(name, extension);
		}
		return nullptr;
	}
	template<>
	EnvinronmentMap* RenderUtils::Create<EnvinronmentMap>(const char* file) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLEnvironmentMap(file, TextureSpecs());
		}
		return nullptr;
	}
	template<>
	EnvinronmentMap* RenderUtils::Create<EnvinronmentMap>(const char* file, TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLEnvironmentMap(file, specs);
		}
		return nullptr;
	}
#pragma endregion

	//Initialize white texture for batch rendering
	void RenderUtils::Init()
	{
		TextureSpecs params;
		params.width = 1;
		params.height = 1;
		params.diskImage = false;

		Texture2D* texture = Create<Texture2D>("White", params);
		uint32_t data = 0xFFFFFFFF;
		texture->SetData(&data, sizeof(uint32_t));
	}
	void RenderUtils::Flush()
	{
		for (Texture* t : s_Textures) {
			UnloadTexture(t);
		}
	}
	void RenderUtils::UnloadPending()
	{

	}
	Texture* RenderUtils::GetTexture(uint32_t textureID)
	{
		return Find<Texture>(textureID);
	}
	Texture* RenderUtils::GetFromTextures(uint32_t index) {
		if (index > s_Textures.size() - 1)
			index = 0;
		return s_Textures[index];
	}
	uint32_t RenderUtils::GetTextureStackIndex(Texture* texture) {
		for (int i = 0; i < s_Textures.size(); i++) {
			if (s_Textures[i] == texture)
				return i;
		}
		return 0;
	}
	void RenderUtils::UnloadTexture(Texture* texture)
	{
		auto i = std::find(s_Textures.begin(), s_Textures.end(), texture);
		if (i != s_Textures.end()) {
			//textures.erase(i);
			//delete texture;
		}
	}
}