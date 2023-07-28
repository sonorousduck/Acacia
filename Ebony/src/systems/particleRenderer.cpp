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
			
			glBindBuffer(GL_ARRAY_BUFFER, particleEffect->particlePositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, particleEffect->getMaxParticles() * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particleEffect->particleCount * sizeof(float) * 4, &particleEffect->particlePositionSizeData.at(0));

			glBindBuffer(GL_ARRAY_BUFFER, particleEffect->particleColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, particleEffect->getMaxParticles() * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particleEffect->particleCount * sizeof(GLubyte) * 4, &particleEffect->particleColorData.at(0));

			graphics.DrawInstanced(s, particleEffect->texture, particleEffect->instancedVAO, particleEffect->particleCount);


			//s.use();


			/*for (size_t i = 0; i < particleEffect->particles.size(); i++)
			{
				auto& particle = particleEffect->particles[i];
				
				if (particle.alive < particle.lifetime)
				{
					s.setVec4("color", particle.currentColor.r(), particle.currentColor.g(), particle.currentColor.b(), particle.currentAlpha);
					graphics.Draw(s, particle.texture, particle.position, particle.currentSize, particle.rotation, particle.currentColor);
				}
			}*/
		}
		glDepthMask(GL_TRUE);

	}


}