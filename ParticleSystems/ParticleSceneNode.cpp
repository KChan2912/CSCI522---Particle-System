#include "ParticleSceneNode.h"
#include "../Lua/LuaEnvironment.h"
#include "ParticleMesh.h"
#include "PrimeEngine/Events/StandardEvents.h"
#include "PrimeEngine/Scene/MeshManager.h"
#include "PrimeEngine/Scene/MeshInstance.h"
#include "PrimeEngine/Scene/CameraManager.h"

namespace PE {
namespace Components {

PE_IMPLEMENT_CLASS1(ParticleSceneNode, SceneNode);

// Constructor -------------------------------------------------------------
ParticleSceneNode::ParticleSceneNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself)
: SceneNode(context, arena, hMyself)
{
}

void ParticleSceneNode::addDefaultComponents()
{
    SceneNode::addDefaultComponents();
    
    // event handlers
    PE_REGISTER_EVENT_HANDLER(Events::Event_PRE_GATHER_DRAWCALLS, ParticleSceneNode::do_PRE_GATHER_DRAWCALLS);
}

void ParticleSceneNode::loadFromTexture_needsRC(const char *texture, float size, int &threadOwnershipMask)
{
    ParticleMesh *pParticleMesh = NULL;
    if (m_hMyParticleMesh.isValid())
    {
        pParticleMesh = m_hMyParticleMesh.getObject<ParticleMesh>();
    }
    else
    {
        m_hMyParticleMesh = PE::Handle("PARTICLE_MESH", sizeof(ParticleMesh));
        pParticleMesh = new (m_hMyParticleMesh) ParticleMesh(*m_pContext, m_arena, m_hMyParticleMesh);
        pParticleMesh->addDefaultComponents();

        m_pContext->getMeshManager()->registerAsset(m_hMyParticleMesh);

        m_hMyParticleMeshInstance = PE::Handle("MeshInstance", sizeof(MeshInstance));
        MeshInstance *pInstance = new (m_hMyParticleMeshInstance) MeshInstance(*m_pContext, m_arena, m_hMyParticleMeshInstance);
        pInstance->addDefaultComponents();
        pInstance->initFromRegisteredAsset(m_hMyParticleMesh);

        addComponent(m_hMyParticleMeshInstance);
    }

    PE::IRenderer::checkForErrors("");

    pParticleMesh->loadFromTexture_needsRC(texture, size, threadOwnershipMask);
}

void ParticleSceneNode::do_PRE_GATHER_DRAWCALLS(Events::Event *pEvt)
{
}

}; // namespace Components
}; // namespace PE
