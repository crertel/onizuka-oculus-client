#include "Buffer.hpp"
#include "RenderTiny_Device.h"
#define D3D1x_(x)               D3D10_##x
#define ID3D1x(x)               ID3D10##x

using namespace OVR::RenderTiny;

bool Buffer::Data(int use, const void *buffer, size_t size)
{
		 	if (D3DBuffer && Size >= size)
	{
		if (Dynamic)
		{
			if (!buffer)
				return true;

			void* v = Map(0, size, Map_Discard);
			if (v)
			{
				memcpy(v, buffer, size);
				Unmap(v);
				return true;
			}
		}
		else
		{
			d3d10device->UpdateSubresource(D3DBuffer, 0, NULL, buffer, 0, 0);
			return true;
		}
	}
	if (D3DBuffer)
	{
		D3DBuffer = NULL;
		Size = 0;
		Use = 0;
		Dynamic = 0;
	}

	D3D1x_(BUFFER_DESC) desc;
	memset(&desc, 0, sizeof(desc));
	if (use & Buffer_ReadOnly)
	{
		desc.Usage = D3D1x_(USAGE_IMMUTABLE);
		desc.CPUAccessFlags = 0;
	}
	else
	{
		desc.Usage = D3D1x_(USAGE_DYNAMIC);
		desc.CPUAccessFlags = D3D1x_(CPU_ACCESS_WRITE);
		Dynamic = 1;
	}

	switch(use & Buffer_TypeMask)
	{
	case Buffer_Vertex:  desc.BindFlags = D3D1x_(BIND_VERTEX_BUFFER); break;
	case Buffer_Index:   desc.BindFlags = D3D1x_(BIND_INDEX_BUFFER);  break;
	case Buffer_Uniform:
		desc.BindFlags = D3D1x_(BIND_CONSTANT_BUFFER);
		size += ((size + 15) & ~15) - size;
		break;
	}

	desc.ByteWidth = (unsigned)size;

	D3D1x_(SUBRESOURCE_DATA) sr;
	sr.pSysMem = buffer;
	sr.SysMemPitch = 0;
	sr.SysMemSlicePitch = 0;

	HRESULT hr = d3d10device->CreateBuffer(&desc, buffer ? &sr : NULL, &D3DBuffer);
	if (SUCCEEDED(hr))
	{
		Use = use;
		Size = desc.ByteWidth;
		return 1;
	}
	return 0;
}

void*  Buffer::Map(size_t start, size_t size, int flags)
{
	OVR_UNUSED(size);

	D3D1x_(MAP) mapFlags = D3D1x_(MAP_WRITE);
	if (flags & Map_Discard)    
		mapFlags = D3D1x_(MAP_WRITE_DISCARD);    
	if (flags & Map_Unsynchronized)    
		mapFlags = D3D1x_(MAP_WRITE_NO_OVERWRITE);    

	void* map = 0;
	if (SUCCEEDED(D3DBuffer->Map(mapFlags, 0, &map)))    
		return ((char*)map) + start;        
	return NULL;
}

bool   Buffer::Unmap(void *m)
{
	OVR_UNUSED(m);

	D3DBuffer->Unmap();
	return true;
}

