#include "colliderRenderer.hpp"

namespace systems
{
	void ColliderRenderer::Update(Ebony::Graphics2d& graphics)
	{
		for (auto&& [id, entity] : m_Entities)
		{
			graphics.Draw(entity);
		}
	}
}

