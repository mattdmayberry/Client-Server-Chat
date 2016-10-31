## Client-Server Network Application

### Setup
Compile chat_client.c:

	$ gcc -o chatclient chat_client.c

Run chat_serve.py:

	$ python chat_serve.py <port_number>

In a seperate terminal window run chat_client:

	$ ./chatclient <hostname> <port_number>

Running chat_client example:

	$ ./chatclient flip1.engr.oregonstate.edu 30020 
		 
### Handles

You will be prompted to input handles for both the server and the client. The handle must not exceed 10 characters, respectively. 

### Communication
The client will be prompted to begin communication. The client and server will then be alternately prompted to send messages back and forth.

### Terminating the connection

Input `\quit` to terminate the connection between chat_serve and chat_client. This will stop chat_client, however chat_serve will continue running and is available to accept new connections. 

### Testing Machine

The client-server network application was tested on flip1.engr.oregonstate.edu and flip2.engr.oregonstate.edu, with success in both cases. 
