#include "particleRenderer.hpp"
#include <resourceManager.hpp>


namespace systems
{
	void ParticleRenderer::Update(Ebony::Graphics2d& graphics)
	{
		glDepthMask(GL_FALSE);
		for (auto&& [id, entity] : m_Entities)
		{
			auto particleEffect = entity->getComponent<components::ParticleGroup>();

			Shader& s = Ebony::ResourceManager::GetShader("defaultParticle");

			if (particleEffect->shader.has_value())
			{
				s = particleEffect->shader.value();
			}
			
			s.use();

			for (size_t i = 0; i < particleEffect->particles.size(); i++)
			{
				auto& particle = particleEffect->particles[i];
				
				if (particle.alive < particle.lifetime)
				{
					s.setVec4("color", particle.currentColor.r(), particle.currentColor.g(), particle.currentColor.b(), particle.currentAlpha);
					graphics.Draw(s, particle.texture, particle.position, particle.currentSize, particle.rotation, particle.currentColor);
					//if (particle.shader.has_value())
					//{
					//	particle.shader.value().use();
					//	particle.shader.value().setVec4("color", particle.currentColor.r(), particle.currentColor.g(), particle.currentColor.b(), particle.currentAlpha);
					//	graphics.Draw(particle.shader.value(), particle.texture, particle.position, particle.currentSize, particle.rotation, particle.currentColor);
					//}
					//else
					//{
					//	Shader& defaultShader = Ebony::ResourceManager::GetShader("defaultParticle");
					//	defaultShader.use();
					//	//defaultShader.setVec4("color", particle.currentColor.r(), particle.currentColor.g(), particle.currentColor.b(), particle.currentAlpha);
					//	//graphics.Draw(defaultShader, Ebony::ResourceManager::GetTexture("face"), particle.position, particle.currentSize, particle.rotation, particle.currentColor);
					//	graphics.Draw(defaultShader, particle.texture, particle.position, particle.currentSize, particle.rotation, particle.currentColor);

					//}
				}
			}
		}
		glDepthMask(GL_TRUE);

	}


}