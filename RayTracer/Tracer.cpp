#include "Tracer.h"
#include "Scene.h"
#include <iostream>
#include <iomanip>
#include <chrono>

void DisplayTime(std::chrono::steady_clock::rep milliseconds) {
	std::cout << std::setw(2) << std::setfill('0') << (milliseconds / (1000 * 60 * 60));                        // hours
	std::cout << ":" << std::setw(2) << std::setfill('0') << ((milliseconds % (1000 * 60 * 60)) / (1000 * 60));    // minutes
	std::cout << ":" << std::setw(2) << std::setfill('0') << ((milliseconds % (1000 * 60)) / 1000);                // seconds
	std::cout << ":" << std::setw(2) << std::setfill('0') << ((milliseconds % 1000) / 10);                        // 1/10 seconds
}

void Tracer::Trace(const ColorBuffer& colorBuffer, Scene* scene, Camera* camera)
{
	float aspectRatio = colorBuffer.width / (float)colorBuffer.height;
	float invSamples = 1.0f / samples;

	std::chrono::steady_clock::rep totalTime = 0;
	for (int y = 0; y < colorBuffer.height; y++)
	{
		auto start = std::chrono::steady_clock::now();
		for (int x = 0; x < colorBuffer.width; x++)
		{
			glm::vec3 color = { 0,0,0 };

			for (int sample = 0; sample < samples; sample++)
			{
				glm::vec2 viewport = camera->ScreenToViewport({ x + random01(), y + random01() });
				viewport.y = 1 - (viewport.y);
				ray_t ray = camera->ViewportToRay(viewport);

				raycastHit_t hit;
				color += scene->Trace(ray, 0.001, FLT_MAX, hit, depth);
			}
			//color /= (float)samples;

			color.r = sqrt(color.r * invSamples);
			color.g = sqrt(color.g * invSamples);
			color.b = sqrt(color.b * invSamples);

			colorBuffer.SetColor(x, y, Vec3ToColor(color));
		}
		// calculate time
		auto end = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		totalTime = totalTime + elapsedTime;
		std::chrono::steady_clock::rep averageTime = totalTime / (y + 1);
		std::chrono::steady_clock::rep estimatedTime = averageTime * colorBuffer.height;

		// display time
		system("CLS");

		std::cout << " scanline |      time       |   average time  |    total time   | estimated time  | remaining time  " << std::endl;
		std::cout << "----------|-----------------|-----------------|-----------------|-----------------|-----------------" << std::endl;
		std::cout << std::setw(4) << std::setfill('0') << (y + 1) << "/";
		std::cout << std::setw(4) << std::setfill('0') << colorBuffer.height;

		std::cout << " |   ";
		DisplayTime(elapsedTime);

		std::cout << "   |   ";
		DisplayTime(averageTime);

		std::cout << "   |   ";
		DisplayTime(totalTime);

		std::cout << "   |   ";
		DisplayTime(estimatedTime);

		std::cout << "   |   ";
		DisplayTime(estimatedTime - totalTime);

		std::cout << std::endl;
	}
}