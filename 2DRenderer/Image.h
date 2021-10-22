#pragma once
#include "SDL.h"
#include "ColorBuffer.h"
#include <string>

using color_t = SDL_Color;

class Image
{
public:
	bool Load(const std::string& filename, uint8_t alpha = 255);
	void Flip();

	friend class Framebuffer;
	ColorBuffer colorBuffer;
private:
};