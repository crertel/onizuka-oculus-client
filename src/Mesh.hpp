#pragma once

#include <ZSTL/ZArray.hpp>
#include "RenderTiny_Device.h"

#include "Vertex.hpp"
#include "Buffer.hpp"

class Mesh
{
	public:
		bool LoadFromOBJ(OVR::RenderTiny::RenderDevice* device, const void* mem, size_t len);


		Buffer* GetVertexBuffer() const { return vb; }
		Buffer* GetIndexBuffer() const { return ib; }

		uint32_t GetNumFaces() const { return nrFaces; }

	private:
		Buffer* vb;
		Buffer* ib;
		uint32_t nrFaces;

		//PrimitiveType     Type;
		//Ptr<ShaderFill>   Fill;

};

extern Mesh testMesh;