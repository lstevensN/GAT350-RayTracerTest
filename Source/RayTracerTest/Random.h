#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

inline void seedRandom(uint32_t seed)
{
	srand(seed);
}

inline float random01()
{
	return rand() / (float)RAND_MAX;
}

inline float random(int min, int max)
{
	if (min > max) std::swap(min, max);

	return min + (max - min) * random01();
}

inline glm::vec3 random(const glm::vec3& min, const glm::vec3& max)
{
	return glm::vec3{ random(min, max) };
}

inline glm::vec3 randomInUnitSphere()
{
	glm::vec3 p;

	do p = glm::vec3{ random(-1, 1) };
	while (glm::length2(p) >= 1.0f);

	return p;
}
