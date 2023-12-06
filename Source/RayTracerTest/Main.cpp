#include <iostream>
#include "Camera.h"
#include "Canvas.h"
#include "Material.h"
#include "Random.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"

using namespace std;

int main(int argc, char* argv[])
{
	std::cout << "Hello World\n";

	seedRandom((unsigned int)time(nullptr));

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", 400, 300);

	Canvas canvas(400, 300, renderer);

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 0, 1 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 70.0f, aspectRatio);

	Scene scene; // sky color could be set with the top and bottom color
	scene.SetCamera(camera);

	// create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto metal = std::make_shared<Metal>(color3_t{ 1, 1, 1 }, 1.0f);

	// create objects -> add to scene
	for (int i = 0; i < 10; i++)
	{
		std::shared_ptr<Material> material = (rand() % 2) ? std::dynamic_pointer_cast<Material>(lambertian) : std::dynamic_pointer_cast<Material>(metal);
		auto sphere = std::make_unique<Sphere>(glm::vec3{ random(-5, 5), random(-4, 4), -6}, 1.0f, material);
		scene.AddObject(std::move(sphere));
	}

	//// create material
	//auto material = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });

	//// create objects -> add to scene
	//auto sphere = std::make_unique<Sphere>(glm::vec3{ 3.5f, 2, -5 }, 1.0f, material);
	//scene.AddObject(std::move(sphere));

	//sphere = std::make_unique<Sphere>(glm::vec3{ -0.5f, -4, -4 }, 1.0f, material);
	//scene.AddObject(std::move(sphere));

	//sphere = std::make_unique<Sphere>(glm::vec3{ -2.5f, 0, -3 }, 1.0f, material);
	//scene.AddObject(std::move(sphere));

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

		canvas.Clear({ 0, 0, 0, 1 });
		scene.Render(canvas);
		canvas.Update();

		renderer.PresentCanvas(canvas);
	}

	renderer.Shutdown();

	return 0;
}