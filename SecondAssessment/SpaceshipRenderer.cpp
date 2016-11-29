#include "SpaceShipRenderer.h"


SpaceshipRenderer::SpaceshipRenderer()
{
	m_color = GREEN;
}

void SpaceshipRenderer::draw(const Mat3 & T, const Transform & ship)
{
	Mat3 glob = T * ship.getGlobalTransform();

	Vec3 F = glob * Vec3{  0,  2, 1 };
	Vec3 L = glob * Vec3{ -1, -3, 1 };
	Vec3 R = glob * Vec3{  1, -3, 1 };
	Vec3 B = glob * Vec3{  0, -1, 1 };

	sfw::drawLine(L.x, L.y, F.x, F.y, m_color);
	sfw::drawLine(F.x, F.y, R.x, R.y, m_color);
	sfw::drawLine(R.x, R.y, B.x, B.y, m_color);
	sfw::drawLine(B.x, B.y, L.x, L.y, m_color);
}