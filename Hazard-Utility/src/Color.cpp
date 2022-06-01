
#include "Color.h"
#include "MathCore.h"

Color Color::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
Color Color::White = { 1.0f, 1.0f, 1.0f, 1.0f };
Color Color::Red = { 1.0f, 0.0f, 0.0f, 1.0f };
Color Color::Green = { 0.0f, 1.0f, 0.0f, 1.0f };
Color Color::Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
Color Color::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
Color Color::Purple = { 1.0f, 0.0f, 1.0f, 1.0f };
Color Color::Turqoise = { 0.0f, 1.0f, 1.0f, 1.0f };


Color::Color()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 1;
}
Color::Color(const char* color)
{
	const Color& c = Color::FromHex(color);
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
}
Color::Color(int r, int g, int b, int a) {
	this->r = (float)r / 255.0f;
	this->g = (float)g / 255.0f;
	this->b = (float)b / 255.0f;
	this->a = (float)a / 255.0f;
}
Color::Color(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
Color::Color(glm::vec3 col, float a)
{
	this->r = col.r;
	this->g = col.g;
	this->b = col.b;
	this->a = a;
}

Color Color::FromGLM(const glm::vec3 color)
{
	return Color(color.r, color.g, color.b, 1.0f);
}
Color Color::FromGLM(const glm::vec4 color)
{
	return Color(color.r, color.g, color.b, color.a);
}
Color Color::FromHex(const std::string& hex) {

	if (hex[0] != '#')
		return Color();

	int r, g, b, a = 255;

	r = Math::ToDec<int>(hex.substr(1, 2));
	g = Math::ToDec<int>(hex.substr(3, 2));
	b = Math::ToDec<int>(hex.substr(5, 2));

	if (hex.length() == 9)
		a = Math::ToDec<int>(hex.substr(7, 2));

	return Color(r, g, b, a);
}
