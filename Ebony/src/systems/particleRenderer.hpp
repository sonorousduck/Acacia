#pragma once

#include "system.hpp"
#include <graphics2d.hpp>
#include "../components/particle.hpp"


namespace systems
{
	class ParticleRenderer : public System
	{
	public:
		ParticleRenderer() : System({ ctti::unnamed_type_id<components::ParticleGroup>() })
		{
		}


		void Update(std::shared_ptr<Ebony::Graphics2d>);

	private:
		using System::Update;

	};
}