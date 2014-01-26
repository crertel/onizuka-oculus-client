#pragma once

class SimConnection
{
	public:
		//Connect to server
		bool Initialize();

		void ProcessMessages();

		void Shutdown();

	private:
		void* zmqContext;
		void* zmqSocket;
};