#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"

#include <iostream>
#include <SDL.h>

int main(int, char**)
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize(WIDTH, HEIGHT);

	std::unique_ptr<Framebuffer> framebuffer = std::make_unique<Framebuffer>(renderer.get(), renderer->width, renderer->height);

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

        color_t color;

		framebuffer->Clear(color_t{0, 0, 0, 0});

        /*for (int i = 0; i < 100; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;
            framebuffer->DrawPoint(rand() % renderer->width, rand() % renderer->height, color);
        }
        for (int i = 0; i < 3; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;
            framebuffer->DrawTriangle(rand() % renderer->width, rand() % renderer->height, rand() % renderer->width, rand() % renderer->height, rand() % renderer->width, rand() % renderer->height, color);
        }
        for (int i = 0; i < 3; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;
            framebuffer->DrawCircle(renderer->width >> 1, renderer->height >> 1, rand() % 100, color);
        }
        */
        for (int i = 0; i < 10; i++)
        {
            color.r = (rand() % 2) * 255;
            color.g = (rand() % 2) * 255;
            color.b = (rand() % 2) * 255;
            color.a = rand() % 256;
            framebuffer->DrawRect(rand() % renderer->width, rand() % renderer->height, 50, 50, color);
        }
        /*
        for (int i = 0; i < 3; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;
            framebuffer->DrawLine(renderer->width >> 1, renderer->height >> 1, rand() % renderer->width, rand() % renderer->height, color);
        }
        for (int i = 0; i < 3; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;
            framebuffer->DrawSimpleCurve(
                rand() % renderer->width, rand() % renderer->height,
                rand() % renderer->width, rand() % renderer->height, 3, color);
        }
        for (int i = 0; i < 3; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255;
            framebuffer->DrawQuadraticCurve(
                rand() % renderer->width, rand() % renderer->height,
                rand() % renderer->width, rand() % renderer->height,
                rand() % renderer->width, rand() % renderer->height, 30, color);
        }
        for (int i = 0; i < 3; i++)
        {
            color.r = rand() % 255;
            color.g = rand() % 255;
            color.b = rand() % 255; 
            framebuffer->DrawCubicCurve(
                rand() % renderer->width, rand() % renderer->height,
                rand() % renderer->width, rand() % renderer->height,
                rand() % renderer->width, rand() % renderer->height,
                rand() % renderer->width, rand() % renderer->height,
                30, color);
        }*/

        std::unique_ptr<Image> image = std::make_unique<Image>();
        image->Load("../Resources/flower.bmp", 210);
        image->Flip();

        framebuffer->DrawImage(300, 50, image.get());

        std::unique_ptr<Image> image1 = std::make_unique<Image>(*image.get());
        PostProcess::BoxBlur(image1->colorBuffer);
        framebuffer->DrawImage(0, 300, image1.get());

        std::unique_ptr<Image> image2 = std::make_unique<Image>(*image.get());
        PostProcess::GaussianBlur(image2->colorBuffer);
        framebuffer->DrawImage(200, 300, image2.get());

        std::unique_ptr<Image> image3 = std::make_unique<Image>(*image.get());
        PostProcess::Sharpen(image3->colorBuffer);
        framebuffer->DrawImage(400, 300, image3.get());

        std::unique_ptr<Image> image4 = std::make_unique<Image>(*image.get());
        PostProcess::Monochrome(image4->colorBuffer);
        PostProcess::Edge(image4->colorBuffer, 0);
        framebuffer->DrawImage(600, 300, image4.get());

        //PostProcess::Invert(framebuffer->colorBuffer);
        //PostProcess::Monochrome(framebuffer->colorBuffer);
        //PostProcess::Noise(framebuffer->colorBuffer, 100);
        //PostProcess::Brightness(framebuffer->colorBuffer, 100);
        //PostProcess::Brightness(framebuffer->colorBuffer, -100);
        //PostProcess::ColorBalance(framebuffer->colorBuffer, 0, 0, 100);
        //PostProcess::Threshold(framebuffer->colorBuffer, 200);
        //PostProcess::ColorShift(framebuffer->colorBuffer);

		framebuffer->Update();
		renderer->CopyBuffer(framebuffer.get());

		renderer->Present();
	}

	SDL_Quit();

	return 0;
}
