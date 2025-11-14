#include "ParticleEmitter.h"
#include "ParticleSceneNode.h"
#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "PrimeEngine/Scene/MeshManager.h"
#include "PrimeEngine/Scene/MeshInstance.h"
#include "PrimeEngine/Events/Component.h"
#include "PrimeEngine/Scene/CameraSceneNode.h"

namespace PE
{
    namespace Components
    {

        PE_IMPLEMENT_CLASS1(ParticleEmitter, SceneNode);

        // Constructor -------------------------------------------------------------
        ParticleEmitter::ParticleEmitter(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself)
            : SceneNode(context, arena, hMyself), m_particles(context, arena), m_timeSinceLastSpawn(0.0f), m_particlesSpawned(0)
        {
        }

        void ParticleEmitter::addDefaultComponents()
        {
            SceneNode::addDefaultComponents();

            // event handlers
            PE_REGISTER_EVENT_HANDLER(Events::Event_UPDATE, ParticleEmitter::do_UPDATE);
        }

        void ParticleEmitter::resetParticle(int index)
        {
            Particle &particle = m_particles[index];
            Vector3 initialPosition = cam->m_base.getPos();
            initialPosition.m_y = initialPosition.m_y + 0.25f;
            int coin = rand() % 2;
            if (coin) {
                initialPosition.m_x = initialPosition.m_x + rand() % 2;
                initialPosition.m_z = initialPosition.m_z - rand() % 2;
            }
            else
            {
                initialPosition.m_x = initialPosition.m_x - rand() % 2;
                initialPosition.m_z = initialPosition.m_z + rand() % 2;
            }
            if (particle.hParticleSceneNode.isValid())
            {
                particle.hParticleSceneNode.getObject<SceneNode>()->m_base.setPos(initialPosition);
                particle.lifetime = m_lifetime;
                particle.velocity = m_initialVelocity;
            }
        }

        void ParticleEmitter::emitParticles(int numParticles, const char *texture, float size, int &threadOwnershipMask, float lifetime, Vector3 initialVelocity, float spawnDuration, float turbulencePower, float gravity, float airResistance)
        {
            m_particles.reset(numParticles);
            m_lifetime = lifetime;
            m_spawnInterval = spawnDuration / numParticles;
            m_timeSinceLastSpawn = 0.0f;
            m_particlesSpawned = 0;
            m_turbulencePower = turbulencePower;
            m_initialVelocity = initialVelocity;
            m_gravity = gravity;
            m_airResistance = airResistance;

            for (int i = 0; i < numParticles; ++i)
            {
                Handle hParticleSceneNode = PE::Handle("PARTICLE_SCENE_NODE", sizeof(ParticleSceneNode));
                ParticleSceneNode *pParticleSceneNode = new (hParticleSceneNode) ParticleSceneNode(*m_pContext, m_arena, hParticleSceneNode);
                pParticleSceneNode->addDefaultComponents();
                pParticleSceneNode->loadFromTexture_needsRC(texture, size, threadOwnershipMask);
                Vector3 initialPosition = cam->m_base.getPos();
                initialPosition.m_y = initialPosition.m_y + 0.25f;
                int coin = rand() % 2;
                if (coin) {
                    initialPosition.m_x = initialPosition.m_x + rand() % 2;
                    initialPosition.m_z = initialPosition.m_z - rand() % 2;
                }
                else
                {
                    initialPosition.m_x = initialPosition.m_x - rand() % 2;
                    initialPosition.m_z = initialPosition.m_z + rand() % 2;
                }
                
                pParticleSceneNode->m_base.setPos(initialPosition);

                addComponent(hParticleSceneNode);
                Particle particle = {hParticleSceneNode, 0.0f, initialVelocity};
                m_particles.add(particle);
            }
        }

        void ParticleEmitter::createCamera(Handle HSN)
        {
            cam = HSN.getObject<CameraSceneNode>();
        }

        void ParticleEmitter::do_UPDATE(Events::Event *pEvt)
        {
            
            Events::Event_UPDATE* pRealEvt = (Events::Event_UPDATE*)(pEvt);
            float deltaTime = pRealEvt->m_frameTime;
        
            m_timeSinceLastSpawn += deltaTime;
        
            for (int i = 0; i < m_particles.m_size; ++i)
            {
                if (m_particles[i].lifetime > 0.0f)
                {
                    m_particles[i].lifetime -= deltaTime;
                    if (m_particles[i].lifetime <= 0.0f)
                    {
                        resetParticle(i);
                    }
                    else
                    {
                        float randomAngleXY = (rand() % 360) * 3.14159f / 180.0f;
                        float randomAngleZ = (rand() % 360) * 3.14159f / 180.0f;
                        Vector3 turbulence = Vector3(cos(randomAngleXY), 0, sin(randomAngleXY)) * m_turbulencePower * deltaTime;
                        m_particles[i].velocity = m_particles[i].velocity + turbulence;
                        m_particles[i].velocity.m_y -= m_gravity * deltaTime;
                        m_particles[i].velocity *= (1.0f - m_airResistance * deltaTime);
                        Vector3 newPos = m_particles[i].hParticleSceneNode.getObject<SceneNode>()->m_base.getPos() + m_particles[i].velocity * deltaTime;
                        m_particles[i].hParticleSceneNode.getObject<SceneNode>()->m_base.setPos(newPos);


                    }
                }
            }
        
            if (m_particlesSpawned < m_particles.m_size && m_timeSinceLastSpawn >= m_spawnInterval)
            {
                m_particles[m_particlesSpawned].lifetime = m_lifetime;
                m_timeSinceLastSpawn = 0.0f;
                m_particlesSpawned++;
            }
        }

    }; // namespace Components
}; // namespace PE
