#include "Camera.h"
#include "Canvas.h"
#include "Material.h"
#include "Plane.h"
#include "Random.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	const int width = 400;
	const int height = 300;
	const int samples = 20;
	const int depth = 15;

	std::cout << "Hello World\n";

	seedRandom((unsigned int)time(nullptr));

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", width, height);

	Canvas canvas(width, height, renderer);

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 10 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);

	Scene scene(glm::vec3{ 1.0f }, glm::vec3{ 0.5f, 0.7f, 1.0f }); // sky color could be set with the top and bottom color
	scene.SetCamera(camera);

	// create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto metal = std::make_shared<Metal>(color3_t{ 1, 1, 1 }, 1.0f);
	std::shared_ptr<Material> oldMaterial;

	// create objects -> add to scene
	/*for (int i = 0; i < 10; i++)
	{
		material = (true) ? std::dynamic_pointer_cast<Material>(lambertian) : std::dynamic_pointer_cast<Material>(metal);
		auto sphere = std::make_unique<Sphere>(glm::vec3{ random(-5, 5), random(-4, 4), -6}, 1.0f, material);
		scene.AddObject(std::move(sphere));
	}*/
	
	oldMaterial = std::make_shared<Lambertian>(color3_t{ 0.5f });
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -4, 0 }, glm::vec3{ 0, 1, 0 }, oldMaterial);
	scene.AddObject(std::move(plane));

	for (int x = -10; x < 0; x++)
	{
		for (int z = -10; z < 0; z++)
		{

			std::shared_ptr<Material> material;

			// create random material
			float r = random01();
			if (r < 0.3f)		material = std::make_shared<Lambertian>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }));
			else if (r < 0.6f)	material = std::make_shared<Metal>(color3_t{ random(0.5f, 1.0f) }, random(0, 0.5f));
			else if (r < 0.9f)	material = std::make_shared<Dielectric>(color3_t{ 1.0f }, random(1.1f, 2));
			else				material = std::make_shared<Emissive>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }), 5.0f);

			// set random sphere radius
			float radius = random(0.4f, 0.6f);
			// create sphere using random radius and material
			auto sphere = std::make_unique<Sphere>(glm::vec3{ x + random(-0.5f, 0.5f), radius, z + random(-0.5f, 0.5f) }, radius, material);
			// add sphere to the scene
			scene.AddObject(std::move(sphere));
		}
	}

	std::shared_ptr<Material> newMaterial = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto sphere = std::make_unique<Sphere>(glm::vec3{ -0.75, 0.4f, 4 }, 0.4f, newMaterial);
	scene.AddObject(std::move(sphere));

	newMaterial = std::make_shared<Metal>(color3_t{ 0.3f }, 0.4f);
	sphere = std::make_unique<Sphere>(glm::vec3{ -0.25, 0.4f, 2 }, 0.4f, newMaterial);
	scene.AddObject(std::move(sphere));

	newMaterial = std::make_shared<Dielectric>(color3_t{ 0.6f }, 1);
	sphere = std::make_unique<Sphere>(glm::vec3{ 0.25, 0.4f, 3 }, 0.4f, newMaterial);
	scene.AddObject(std::move(sphere));

	newMaterial = std::make_shared<Emissive>(glm::rgbColor(glm::vec3{ 50, 1, 1 }), 10.0f);
	sphere = std::make_unique<Sphere>(glm::vec3{ 0.75, 0.4f, 5 }, 0.4f, newMaterial);
	scene.AddObject(std::move(sphere));

	/*material = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 0.5f);
	auto triangle = std::make_unique<Triangle>(glm::vec3{ 2, -2, -3 }, glm::vec3{ 0, 2, -3 }, glm::vec3{ -2, -2, -3 }, material);
	scene.AddObject(std::move(triangle));*/

	/*
	// create material
	auto material = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });

	// create objects -> add to scene
	auto sphere = std::make_unique<Sphere>(glm::vec3{ 3.5f, 2, -5 }, 1.0f, material);
	scene.AddObject(std::move(sphere));

	sphere = std::make_unique<Sphere>(glm::vec3{ -0.5f, -4, -4 }, 1.0f, material);
	scene.AddObject(std::move(sphere));

	sphere = std::make_unique<Sphere>(glm::vec3{ -2.5f, 0, -3 }, 1.0f, material);
	scene.AddObject(std::move(sphere));
	*/

	// render scene
	canvas.Clear({ 0, 0, 0, 1 });
	scene.Render(canvas, samples, depth);
	canvas.Update();

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;	
		}

		/*canvas.Clear({ 0, 0, 0, 1 });
		for (int i = 0; i < 1000; i++) canvas.DrawPoint({ random(-1, canvas.GetSize().x), random(-1, canvas.GetSize().y) }, { color3_t{ (float)random(-1, 256) }, 1 });
		canvas.Update();

		renderer.PresentCanvas(canvas);*/

		renderer.PresentCanvas(canvas);
	}

	renderer.Shutdown();

	return 0;
}