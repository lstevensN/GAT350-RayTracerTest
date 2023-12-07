#include "Camera.h"
#include "Canvas.h"
#include "Material.h"
#include "Mesh.h"
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
	const int depth = 6;

	std::cout << "Hello World\n";

	seedRandom((unsigned int)time(nullptr));

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", width, height);

	Canvas canvas(width, height, renderer);

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 0.5f, 18 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);

	Scene scene(glm::vec3{ 1.0f }, glm::vec3{ 0.5f, 0.7f, 1.0f }); // sky color could be set with the top and bottom color
	scene.SetCamera(camera);

	// create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto metal = std::make_shared<Metal>(color3_t{ 1, 1, 1 }, 1.0f);
	std::shared_ptr<Material> oldMaterial;
	
	oldMaterial = std::make_shared<Lambertian>(color3_t{ 1 });
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -4, 0 }, glm::vec3{ 0, 1, 0 }, oldMaterial);
	scene.AddObject(std::move(plane));
	

	// walls
	auto mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	mesh->Load("models/quad.obj", glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 6, 6, 6 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0.9f, 0.9f, 0.9f }));
	mesh->Load("models/quad.obj", glm::vec3{ 0, 3, 3 }, glm::vec3{ 90, 0, 0 }, glm::vec3{ 6, 6, 16 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 0, 0 }));
	mesh->Load("models/quad.obj", glm::vec3{ -3, 0, 3 }, glm::vec3{ 0, 90, 0 }, glm::vec3{ 6, 6, 6 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0, 1, 0 }));
	mesh->Load("models/quad.obj", glm::vec3{ 3, 0, 3 }, glm::vec3{ 0, -90, 0 }, glm::vec3{ 6, 6, 6 });
	scene.AddObject(std::move(mesh));


	// objects
	mesh = std::make_unique<Mesh>(std::make_shared<Emissive>(glm::rgbColor(glm::vec3{ 1, 0, 1 }), 5.0f));
	mesh->Load("models/cube.obj", glm::vec3{ 0, 3.45f, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 1, 1 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1 }));
	mesh->Load("models/cube.obj", glm::vec3{ 1, -3, 2 }, glm::vec3{ 0, -25, 0 }, glm::vec3{ 1, 1, 1 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1 }));
	mesh->Load("models/cube.obj", glm::vec3{ -1, -3, 1 }, glm::vec3{ 0, 25, 0 }, glm::vec3{ 1, 3, 1 });
	scene.AddObject(std::move(mesh));

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

		renderer.PresentCanvas(canvas);
	}

	renderer.Shutdown();

	return 0;
}