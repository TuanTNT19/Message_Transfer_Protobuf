#pragma once

#include "Message.h"

class ReceiveMessage : public Message{
	private:
	char buffer[1024];
	const size_t buffer_size = 1024;
	ssize_t bytesRead ;

	public:
	bool Receive(int fd);
	int Parse ();
};