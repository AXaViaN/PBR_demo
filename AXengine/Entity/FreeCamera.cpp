#include "AXengine/Entity/FreeCamera.h"

#include "AXengine/Tool/Debug.h"
#include "AXengine/Tool/Input.h"
#include "AXengine/Tool/Time.h"
#include <glm/gtx/rotate_vector.hpp>

namespace AX { namespace Entity {

void FreeCamera::Update()
{
	// Process rotation
	glm::ivec2 mouseDelta = Tool::Input::GetMouseDelta();
	glm::vec3 rotation = glm::vec3(mouseDelta.y, mouseDelta.x, 0);

	transform.Rotate(rotation * _rotationSpeed * 5.0f * Tool::Time::GetDeltaTime());

	if(transform.rotation.x < -90)
		transform.rotation.x = -90;
	else if(transform.rotation.x > 90)
		transform.rotation.x = 90;

	// Process movement
	glm::vec3 forward = glm::vec3(0, 0, -1);
	forward = glm::rotate(forward, glm::radians(transform.rotation.x), glm::vec3(-1, 0, 0));
	forward = glm::rotate(forward, glm::radians(transform.rotation.y), glm::vec3(0, -1, 0));

	glm::vec3 right = glm::vec3(1, 0, 0);
	right = glm::rotate(right, glm::radians(transform.rotation.x), glm::vec3(-1, 0, 0));
	right = glm::rotate(right, glm::radians(transform.rotation.y), glm::vec3(0, -1, 0));

	glm::vec3 up = glm::cross(right, forward);

	glm::vec3 movement;
	if(Tool::Input::GetKey(SDL_SCANCODE_W))
		movement += forward;
	if(Tool::Input::GetKey(SDL_SCANCODE_S))
		movement -= forward;
	if(Tool::Input::GetKey(SDL_SCANCODE_D))
		movement += right;
	if(Tool::Input::GetKey(SDL_SCANCODE_A))
		movement -= right;
	if(Tool::Input::GetKey(SDL_SCANCODE_LSHIFT))
		movement += up;
	if(Tool::Input::GetKey(SDL_SCANCODE_LCTRL))
		movement -= up;

	transform.Translate(movement * _movementSpeed * Tool::Time::GetDeltaTime());
}

} } // namespace AX::Entity
