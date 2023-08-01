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

			if (particleEffect->particleCount > 0)
			{
				Shader& s = Ebony::ResourceManager::GetShader("defaultParticle");

				if (particleEffect->shader.has_value())
				{
					s = particleEffect->shader.value();
				}
			
				glBindBuffer(GL_ARRAY_BUFFER, particleEffect->particlePositionBuffer);
				glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(particleEffect->getMaxParticles()) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<unsigned long long>(particleEffect->particleCount) * 4 * sizeof(float), &particleEffect->particlePositionSizeData.at(0));

				glBindBuffer(GL_ARRAY_BUFFER, particleEffect->particleColorBuffer);
				glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(particleEffect->getMaxParticles()) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<unsigned long long>(particleEffect->particleCount) * 4 * sizeof(float), &particleEffect->particleColorData.at(0));

				graphics.DrawInstanced(s, particleEffect->texture, particleEffect->instancedVAO, particleEffect->particleCount);
			}


		}
		glDepthMask(GL_TRUE);

	}


}