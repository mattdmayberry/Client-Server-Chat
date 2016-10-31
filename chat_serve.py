#!/usr/bin/python

#************************
#	Author: Matt Mayberry
#	Date: October 30, 2016
#	CS372_400_F2016
#	Project 1
#   chat_serve.py
#************************/

# References: https://docs.python.org/2/howto/sockets.html


import sys
import signal
import socket

# signal catcher
def signal_handler(signal, frame):
	print("")
	quit()
	
signal.signal(signal.SIGINT, signal_handler)

# check number of args
if len(sys.argv) == 1:
	print("Inadequet number of Args. Correct usage: chatclient [hostname] [port]\n")
	quit()
	
handle = raw_input("\nHandle: ")

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)							# creating an INET, streaming socket
host = socket.gethostname() 														# getting local machine name
portnum = int(sys.argv[1]) 															# getting portnum

sock.bind((host, portnum)) 															# binding to the port

sock.listen(5) 																		# queue for up to 5 connections

while True: 																		# loop through accepting connections
	
	conn, addr = sock.accept() 														# connecting with a client
	
	keep_connection = True
	while keep_connection:

																					# getting the initial portnum message
		client_handle = conn.recv(10) 												# receiving from the client - the original portnum
		print("\nConnection established with " + client_handle + ".\n")
		continue_exchange = True
		while continue_exchange:
			client_message = conn.recv(513) 										# message, handle and newline
			if client_message == "\\quit":											# checking if the client wants to quit
				continue_exchange = False
				keep_connection = False
				
			else: 
				print(client_handle + " > " + client_message)
				server_message = raw_input(handle + "> ") 							# getting the server's message
				if server_message == "\\quit":
					full_server_message = server_message
					continue_exchange = False
					keep_connection = False
					
				else: 																# if the server wants to continue
					full_server_message = handle + "> " + server_message
				
				conn.send(full_server_message)

	conn.close() 																	# closing the connection
	
