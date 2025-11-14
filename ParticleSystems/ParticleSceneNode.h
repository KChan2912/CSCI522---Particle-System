#ifndef __PYENGINE_2_0_PARTICLESCENENODE_H__
#define __PYENGINE_2_0_PARTICLESCENENODE_H__

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

namespace PE {
namespace Components {
struct ParticleSceneNode : public SceneNode
{
    PE_DECLARE_CLASS(ParticleSceneNode);

    // Constructor -------------------------------------------------------------
    ParticleSceneNode(PE::GameContext &context, PE::MemoryArena arena, Handle hMyself);

    virtual ~ParticleSceneNode() {}

    // Component ------------------------------------------------------------

    virtual void addDefaultComponents();

    // Individual events -------------------------------------------------------

    PE_DECLARE_IMPLEMENT_EVENT_HANDLER_WRAPPER(do_PRE_GATHER_DRAWCALLS);
    virtual void do_PRE_GATHER_DRAWCALLS(Events::Event *pEvt);

    void loadFromTexture_needsRC(const char *texture, float size, int &threadOwnershipMask);

    Handle m_hMyParticleMesh;
    Handle m_hMyParticleMeshInstance;
}; // class ParticleSceneNode

}; // namespace Components
}; // namespace PE
#endif
