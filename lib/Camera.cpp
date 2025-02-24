#include "Camera.h"

void Camera::set_position()
{
	position.x = sight_distance * cosf(phi) * sinf(theta) + target.x;
	position.y = sight_distance * sinf(phi) + target.y;
	position.z = sight_distance * cosf(phi) * cosf(theta) + target.z;
}

void Camera::set_direction()
{
	forward = (target - position).normalize();
	right = cross(forward, world_up).normalize();
	up = cross(right, forward);
}
