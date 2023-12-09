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

void InitScene03(Scene& scene, const Canvas& canvas);     // Cornell Room
void InitSceneFinal(Scene& scene, const Canvas& canvas);  // Final Scene

int main(int argc, char* argv[])
{
	const int width = 400;
	const int height = 300;
	const int samples = 1000;
	const int depth = 25;

	std::cout << "Hello World\n";

	seedRandom((unsigned int)time(nullptr));

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", width, height);

	Canvas canvas(width, height, renderer);

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 10, 0, 18 }, glm::vec3{ 0, 5, -6 }, glm::vec3{ 0, 1, 0 }, 30.0f, aspectRatio);

	Scene scene(glm::vec3{ 0 }, glm::vec3{ 0.5f, 0.7f, 1.0f }); // sky color could be set with the top and bottom color
	scene.SetCamera(camera);

	// create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto metal = std::make_shared<Metal>(color3_t{ 1, 1, 1 }, 1.0f);
	std::shared_ptr<Material> oldMaterial;
	
	oldMaterial = std::make_shared<Lambertian>(color3_t{ 1 });
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -4, 0 }, glm::vec3{ 0, 1, 0 }, oldMaterial);
	scene.AddObject(std::move(plane));

	InitSceneFinal(scene, canvas);

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

void InitSceneFinal(Scene& scene, const Canvas& canvas)
{
	// central cube structure
	auto mesh = std::make_unique<Mesh>(std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1));
	mesh->Load("models/cube.obj", glm::vec3{ 0, 5, -6 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 4, 4, 4 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 1));
	mesh->Load("models/cube.obj", glm::vec3{ -1.5f, 2, -9.5f }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 3, 3, 3 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1));
	mesh->Load("models/cube.obj", glm::vec3{ -2.5f, -0.5f, -12 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 2, 2, 2 });
	scene.AddObject(std::move(mesh));


	// moon
	auto sphere = std::make_unique<Sphere>(glm::vec3{ 7.5f, 15, -18 }, 2.0f, std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 1));
	scene.AddObject(std::move(sphere));


	// background buildings
	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0, 0, 0 }));
	mesh->Load("models/cube.obj", glm::vec3{ -30, 0, -45 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 17, 58, 17 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0, 0, 0 }));
	mesh->Load("models/cube.obj", glm::vec3{ -10, 0, -45 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 17, 58, 17 });
	scene.AddObject(std::move(mesh));


	// buildings
	mesh = std::make_unique<Mesh>(std::make_shared<Metal>(color3_t{ 0.5f, 0.5f, 0.5f }, 0.5f));
	mesh->Load("models/cube.obj", glm::vec3{ -10, 0, -25 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 10, 38, 10 });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0.5f, 0.5f, 0.5f }));
	mesh->Load("models/cube.obj", glm::vec3{ 2, 0, -30 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 8, 28, 8 });
	scene.AddObject(std::move(mesh));
}

void InitScene03(Scene& scene, const Canvas& canvas)
{
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
}