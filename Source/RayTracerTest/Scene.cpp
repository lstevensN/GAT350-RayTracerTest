#include "Scene.h"
#include "Canvas.h"
#include "MathUtils.h"

void Scene::Render(Canvas& canvas)
{
	// cast ray for each point (pixel) on the canvas
	for (int y = 0; y < canvas.GetSize().y; y++)
	{
		for (int x = 0; x < canvas.GetSize().x; x++)
		{
			// create vec2 pixel from canvas x,y
			glm::vec2 pixel = glm::vec2{ x, y };
			// get normalized (0 - 1) point coordinates from pixel
			glm::vec2 point = glm::vec2{ pixel.x / canvas.GetSize().x, pixel.y / canvas.GetSize().y }; // < divide pixel by canvas size to normalize(0 - 1)
			// flip y
			point.y = 1.0f - point.y;

			// create ray from camera
			ray_t ray = m_camera->GetRay(point); 

			// cast ray into scene
			// set color value from trace
			raycastHit_t raycastHit;
			color3_t color = Trace(ray, 0, 100, raycastHit);

			// draw color to canvas point (pixel)
			canvas.DrawPoint(pixel, color4_t(color, 1));
		}
	}
}

//color3_t Scene::Trace(const ray_t& ray)
//{
//	glm::vec3 direction = glm::normalize(ray.direction);
//
//	// set scene sky color
//	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
//	color3_t color = lerp(m_bottomColor, m_topColor, t); // <lerp between bottom and top color using t>
//
//	return color;
//}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit)
{
	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : m_objects)
	{
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))
		{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;
		}
	}

	// if ray hit object, scatter (bounce) ray and check for next hit
	if (rayHit)
	{
		ray_t scattered;
		color3_t color;

		if (raycastHit.material->Scatter(ray, raycastHit, color, scattered))
		{
			return raycastHit.normal;
			//return color3_t{ raycastHit.distance / 255.0f };
		}
		else
		{
			return color3_t{ 0, 0, 0 };
		}
	}

	// if ray not hit, return scene sky color
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
	color3_t color = lerp(m_bottomColor, m_topColor, t);

	return color;
}
