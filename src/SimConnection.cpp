#include "SimConnection.hpp"
#include <zmq.h>
#include <assert.h>
#include <jansson.h>


#define HOST "10.0.0.148"
#define PORT "4002"

bool SimConnection::Initialize()
{
	this->zmqContext = zmq_ctx_new();
	assert(zmqContext != NULL);

	this->zmqSocket = zmq_socket(zmqContext, ZMQ_SUB);
	assert(zmqSocket != NULL);

	if(zmq_setsockopt(zmqSocket, ZMQ_SUBSCRIBE, "", 0) != 0)
		return false;

	if(zmq_connect(zmqSocket, "tcp://" HOST ":" PORT) != 0)
		return false;

	printf("ZMQ initialized\n");



	return true;
}

void SimConnection::ProcessMessages()
{
	char buf[4096];

	//Read each message
	bool done = false;
	do {

		int nrBytes = zmq_recv(zmqSocket, buf, sizeof(buf), ZMQ_DONTWAIT);

		if(nrBytes < 0) {
			if(errno != EAGAIN) {
				printf("Non-trivial error occurred.\n");
				return;
			}
			else
				done = true;
		} else {

			//If we get here, we have a valid message
			printf("Got a message, %d bytes\n", nrBytes);

			buf[nrBytes] = 0;
			json_error_t jerr;

			json_t* root = json_loads(buf, 0, &jerr);

		}


	} while(!done);




}