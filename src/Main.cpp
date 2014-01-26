
#include "OnizukaApp.h"
#include "Mesh.hpp"
#include <curl/curl.h>


char* LoadFile(const char* path, size_t* len) {
	FILE* fp = fopen(path, "rb");

	fseek(fp, 0, SEEK_END);
	uint32_t size = (uint32_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buf = (char*)malloc(size+1);

	fread(buf, 1, size, fp);
	buf[size] = 0;
	fclose(fp);

	*len = size;
	return buf;
}

#include "Mesh.hpp"
int main()
{
	int exitCode = 0;

	size_t len;
	char* ptr = LoadFile("../model/test2.obj", &len);


	curl_global_init(CURL_GLOBAL_WIN32);

	// Initializes LibOVR. This LogMask_All enables maximum logging.
	// Custom allocator can also be specified here.
	OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

	// Scope to force application destructor before System::Destroy.
	{
		OnizukaApp app(GetModuleHandle(NULL));
		//app.hInstance = hinst;


		exitCode = app.OnStartup(NULL);
		if (!exitCode)
		{
			testMesh.LoadFromOBJ(app.GetRenderDevice(), ptr, len);
			// Processes messages and calls OnIdle() to do rendering.
			exitCode = app.Run();
		}
	}

	// No OVR functions involving memory are allowed after this.
	OVR::System::Destroy();

	curl_global_cleanup();

	OVR_DEBUG_STATEMENT(_CrtDumpMemoryLeaks());
	return exitCode;
}
