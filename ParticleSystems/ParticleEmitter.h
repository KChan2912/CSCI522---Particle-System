#ifndef __PYENGINE_2_0_PARTICLEEMITTER_H__
#define __PYENGINE_2_0_PARTICLEEMITTER_H__

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes
#include <assert.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "../Events/Component.h"
#include "../Utils/Array/Array.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "../Events/Event.h"


// #define USE_DRAW_COMPONENT

namespace PE
{
	namespace Components
	{
		struct ParticleEmitter : public SceneNode
		{
			PE_DECLARE_CLASS(ParticleEmitter);

			// Constructor -------------------------------------------------------------
			ParticleEmitter(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);

			virtual ~ParticleEmitter() {}

			// Component ------------------------------------------------------------

			virtual void addDefaultComponents();

			// Individual events -------------------------------------------------------

			PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_UPDATE);
			virtual void do_UPDATE(Events::Event* pEvt);

			void emitParticles(int numParticles, const char *texture, float size, int &threadOwnershipMask, float lifetime, Vector3 initialVelocity, float spawnDuration, float turbulencePower, float gravity, float airResistance);
			void resetParticle(int index);
			void createCamera(Handle HSN);

			float m_lifetime;
			float m_spawnInterval;
			float m_timeSinceLastSpawn;
			int m_particlesSpawned;
			float m_turbulencePower;
			Vector3 m_initialVelocity;
			float m_gravity;
			float m_airResistance;

			struct Particle
			{
				Handle hParticleSceneNode;
				float lifetime;
				Vector3 velocity;
			};
			Array<Particle> m_particles;

			SceneNode* cam;

		};

	}; // namespace Components
}; // namespace PE
#endif
