#pragma once

#include <curl/curl.h>

class AssetConnection {

	public:

		bool Initialize();

		bool PullAsset(const char* uri, void** dataReturn, size_t* lenReturn);

		void Shutdown();

	private:
		CURL* curlHandle;
	
};