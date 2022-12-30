#pragma once
#include "stdafx.h"
#include <soil2.h>

namespace SOF
{
	class Texture
	{
		enum Flags
		{
			Filter = 1,
			Wrap = 2
		};
		GLuint* textureObject;
		int width, height;
		int numText = 1;
	public:
		Texture(const std::string &fileName, uint32_t flags = Filter|Wrap); // constructor, load from file
		Texture(int nText); // constructor with number of texture objects to use

		void loadTexture(int num, const std::string &fileName, uint32_t flags=Filter|Wrap);
		GLuint GetObject() const { return textureObject[0]; }
		GLuint GetObject(int num) const { return textureObject[num]; }
		int GetWidth() const { return width; }
		int GetHeight() const { return height;  }
	};
}