

#include "d3d10.h"

#pragma once

class Buffer
{
public:
	ID3D10Device*	d3d10device;
	ID3D10Buffer*	D3DBuffer;
	size_t			Size;
	int				Use;
	bool			Dynamic;

public:
	Buffer(ID3D10Device* _dev) : d3d10device(_dev), Size(0), Use(0) {}
	~Buffer() {}

	ID3D10Buffer* GetBuffer()
	{
		return D3DBuffer;
	}

	virtual size_t GetSize()
	{
		return Size;
	}
	void* Map(size_t start, size_t size, int flags = 0);
	bool Unmap(void *m);
	bool Data(int use, const void* buffer, size_t size);
};
