#Eric Swanson
#Network Design
#Phase 1 - UDPClient.py: Creates client socket that sends a text string to a
#                        server socket using UDP.

from socket import *
serverName = 'localhost'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = input('Enter a string with the word "network": ') #server will strip the word network
clientSocket.sendto(message.encode(), (serverName, serverPort))
modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
print('Here is your string without the word "network":', modifiedMessage.decode())
clientSocket.close()
