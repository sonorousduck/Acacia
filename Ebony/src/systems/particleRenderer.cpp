#include "particleRenderer.hpp"
#include "../misc/resourceManager.hpp"


namespace systems
{
	void ParticleRenderer::Update()
	{
		glDepthMask(GL_FALSE);
		for (auto&& [id, entity] : m_Entities)
		{
			if (!entity->isEnabled()) return;


			auto particleEffect = entity->getComponent<components::ParticleGroup>();

			if (particleEffect->particleCount > 0)
			{
				std::shared_ptr<Shader> s = Ebony::ResourceManager::GetShader("defaultParticle");

				if (particleEffect->shader.has_value())
				{
					s = particleEffect->shader.value();
				}
			

				//std::cout << particleEffect->particlePositionSizeData[0] << std::endl;
				//std::cout << &particleEffect->particlePositionSizeData[0] << std::endl;
				//std::cout << particleEffect->particlePositionSizeData[static_cast<unsigned long long>(particleEffect->particleCount) * 4 * sizeof(float)] << std::endl;

				glBindBuffer(GL_ARRAY_BUFFER, particleEffect->particlePositionBuffer);
				glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(particleEffect->getMaxParticles()) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<unsigned long long>(particleEffect->particleCount) * 4 * sizeof(float), &particleEffect->particlePositionSizeData[0]);

				glBindBuffer(GL_ARRAY_BUFFER, particleEffect->particleColorBuffer);
				glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(particleEffect->getMaxParticles()) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<unsigned long long>(particleEffect->particleCount) * 4 * sizeof(float), &particleEffect->particleColorData[0]);

				Ebony::Graphics2d::DrawInstanced(s, particleEffect->texture, particleEffect->instancedVAO, particleEffect->particleCount);
			}


		}
		glDepthMask(GL_TRUE);

	}


}