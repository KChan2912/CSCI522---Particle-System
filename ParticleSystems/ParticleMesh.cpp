#define NOMINMAX
// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes
#include "PrimeEngine/FileSystem/FileReader.h"
#include "PrimeEngine/APIAbstraction/GPUMaterial/GPUMaterialSet.h"
#include "PrimeEngine/PrimitiveTypes/PrimitiveTypes.h"
#include "PrimeEngine/APIAbstraction/Texture/Texture.h"
#include "PrimeEngine/APIAbstraction/Effect/EffectManager.h"
#include "PrimeEngine/APIAbstraction/GPUBuffers/VertexBufferGPUManager.h"
#include "PrimeEngine/Scene/Light.h"
#include "PrimeEngine/GameObjectModel/Camera.h"

// Sibling/Children includes
#include "ParticleMesh.h"
#include "PrimeEngine/Scene/SceneNode.h"
#include "PrimeEngine/Scene/CameraManager.h"
#include "../Lua/LuaEnvironment.h"
#include "PrimeEngine/Events/StandardEvents.h"
namespace PE
{
	namespace Components
	{

		PE_IMPLEMENT_CLASS1(ParticleMesh, Mesh);

		void ParticleMesh::addDefaultComponents()
		{
			// add this handler before Mesh's handlers so we can intercept draw and modify transform
			PE_REGISTER_EVENT_HANDLER(Events::Event_GATHER_DRAWCALLS, ParticleMesh::do_GATHER_DRAWCALLS);
			Mesh::addDefaultComponents();
		}

		void ParticleMesh::loadFromTexture_needsRC(const char *texture, float size, int &threadOwnershipMask)
		{
			if (!m_meshCPU.isValid())
			{
				m_meshCPU = Handle("MeshCPU ParticleMesh", sizeof(MeshCPU));
				new (m_meshCPU) MeshCPU(*m_pContext, m_arena);
			}
			MeshCPU &mcpu = *m_meshCPU.getObject<MeshCPU>();
		
			if (!m_loaded)
				mcpu.createBillboardMeshWithColorTexture(texture, "Default", size, size, SamplerState_NoMips_NoMinTexelLerp_NoMagTexelLerp_Clamp);
		
			mcpu.m_manualBufferManagement = true;
		
			PositionBufferCPU *pVB = mcpu.m_hPositionBufferCPU.getObject<PositionBufferCPU>();
			TexCoordBufferCPU *pTCB = mcpu.m_hTexCoordBufferCPU.getObject<TexCoordBufferCPU>();
			NormalBufferCPU *pNB = mcpu.m_hNormalBufferCPU.getObject<NormalBufferCPU>();
			IndexBufferCPU *pIB = mcpu.m_hIndexBufferCPU.getObject<IndexBufferCPU>();
			pVB->m_values.reset(4 * 3); // 4 verts * (x,y,z)
			pTCB->m_values.reset(4 * 2);
			pNB->m_values.reset(4 * 3);
			pIB->m_values.reset(6); // 2 tris
		
			pIB->m_indexRanges[0].m_start = 0;
			pIB->m_indexRanges[0].m_end = 5;
			pIB->m_indexRanges[0].m_minVertIndex = 0;
			pIB->m_indexRanges[0].m_maxVertIndex = 3;
		
			pIB->m_minVertexIndex = pIB->m_indexRanges[0].m_minVertIndex;
			pIB->m_maxVertexIndex = pIB->m_indexRanges[0].m_maxVertIndex;
		
			float halfSize = size / 2.0f;
		
			// Set vertex positions based on the size
			pVB->m_values.add(-halfSize, halfSize, 0);    // top left
			pVB->m_values.add(halfSize, halfSize, 0);     // top right
			pVB->m_values.add(halfSize, -halfSize, 0);    // bottom right
			pVB->m_values.add(-halfSize, -halfSize, 0);   // bottom left
		
			// Add indices for the two triangles
			pIB->m_values.add(0, 1, 2);
			pIB->m_values.add(2, 3, 0);
		
			float dx = 1.0f / 512.0f / 2.0f;
			float dy = 1.0f / 512.0f / 2.0f;
		
			// Set texture coordinates
			pTCB->m_values.add(dx, dy);                   // top left
			pTCB->m_values.add(1.0f - dx, dy);            // top right
			pTCB->m_values.add(1.0f - dx, 1.0f - dy);     // bottom right
			pTCB->m_values.add(dx, 1.0f - dy);            // bottom left
		
			// Set normals (all zeros in this case)
			pNB->m_values.add(0, 0, 0);
			pNB->m_values.add(0, 0, 0);
			pNB->m_values.add(0, 0, 0);
			pNB->m_values.add(0, 0, 0);
		
			if (!m_loaded)
			{
				loadFromMeshCPU_needsRC(mcpu, threadOwnershipMask);
		
				Handle hEffect = EffectManager::Instance()->getEffectHandle("StdMesh_Diffuse_Tech");
				for (unsigned int imat = 0; imat < m_effects.m_size; imat++)
				{
					if (m_effects[imat].m_size)
						m_effects[imat][0] = hEffect;
				}
				m_loaded = true;
			}
			else
			{
				updateGeoFromMeshCPU_needsRC(mcpu, threadOwnershipMask);
			}
		}

		void ParticleMesh::do_GATHER_DRAWCALLS(Events::Event *pEvt)
		{
		}

	}; // namespace Components
}; // namespace PE
