#pragma once
#include "Attribute.h"

namespace Hazard::Scripting 
{
	class AuthorAttribute : public Attribute
	{
	public:
		std::string Author;
		double Version;

		ATTRIBUTE_CLASS_TYPE(Author)
	};
	class TodoAttribute : public Attribute {
	public:
		std::string Detail;
		Progress Progress;

		ATTRIBUTE_CLASS_TYPE(Todo)
	};
}