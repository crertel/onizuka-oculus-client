#include "AssetConnection.hpp"

#include <curl/curl.h>


#define ASSET_HOST "10.0.0.119:5000"

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

bool AssetConnection::Initialize()
{
	curlHandle = curl_easy_init();

	/*
	void* ptr = NULL;
	size_t len = 0;
	PullAsset("/files/1", &ptr, &len);
	free(ptr);
	*/
	return true;
}

bool AssetConnection::PullAsset(const char* uri, void** dataReturn, size_t* lenReturn)
{
	char fullUrl[1024];

	strcpy(fullUrl, "http://" ASSET_HOST);
	strcat(fullUrl, uri);

	struct MemoryStruct chunk = { NULL, 0 };

	curl_easy_setopt(curlHandle, CURLOPT_URL, fullUrl);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");


	if(curl_easy_perform(curlHandle) != CURLE_OK)
		return false;

	*dataReturn = chunk.memory;
	*lenReturn = chunk.size;

	return true;
}

void AssetConnection::Shutdown()
{

}