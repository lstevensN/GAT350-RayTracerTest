#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

inline void seedRandom(uint32_t seed)
{
	srand(seed);
}

inline int random01()
{
	return rand() % RAND_MAX;
}

inline int random(int min, int max)
{
	if (min > max) std::swap(min, max);

	int num;

	do num = random01();
	while (!(num > min && num < max));

	return num;
}

inline glm::vec3 random(const glm::vec3& min, const glm::vec3& max)
{
	return glm::vec3{ random(min, max) };
}

inline glm::vec3 randomInUnitSphere()
{
	glm::vec3 p;

	do p = glm::vec3{ (float)random(-2, 2) };
	while (glm::length2(p) >= 1.0f);

	return p;
}