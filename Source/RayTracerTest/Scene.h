#pragma once
#include "Color.h"
#include "Camera.h"
#include "Object.h"

#include <iostream>
#include <vector>

class Scene
{
public:
	Scene() = default;
	Scene(const color3_t& topColor, const color3_t& bottomColor) :
		m_topColor{ topColor },
		m_bottomColor{ bottomColor }
	{}

	void Render(class Canvas& canvas, int numSamples, int depth);
	//color3_t Trace(const ray_t& ray);
	color3_t Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit, int depth);

	void SetCamera(std::shared_ptr<class Camera> camera) { m_camera = camera; }
	void AddObject(std::unique_ptr<Object> object) { m_objects.push_back(std::move(object)); }

private:
	std::shared_ptr<class Camera> m_camera;
	std::vector<std::unique_ptr<Object>> m_objects;

	color3_t m_topColor{ 0 };
	color3_t m_bottomColor{ 1 };
};