#include "Camera.h"

void Camera::calculateCameraTransform(Transform playerTransform, float deltaTime)
{
	cameraTransform.m_position = lerp(cameraTransform.m_position,
		playerTransform.getGlobalPosition(),
		sfw::getDeltaTime() * cameraLerpSnap);
}

Mat3 Camera::getCameraTransform()
{
	Mat3 proj = translate(screenCenter.x, screenCenter.y) * scale(screenZoom.x, screenZoom.y);
	Mat3 view = inverse(cameraTransform.getGlobalTransform());
	return proj * view;
}