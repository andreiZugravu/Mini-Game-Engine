#pragma once

#include <string>
#include <vector>

enum TrajectoryType {
	linear,
	circular
};

enum Direction {
	normal,
	alternate
};

struct Trajectory {
	TrajectoryType type;
	int iterationCount;
	Direction direction;
	float speed;
	std::vector < Vector3 > points;
};