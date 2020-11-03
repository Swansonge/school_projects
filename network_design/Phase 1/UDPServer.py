#Eric Swanson
#Network Design
#Phase 1 - UDPServer.py: Creates server socket that receives text string from client,
#                        strips the white space from the string, and sends it back

from socket import *
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))
print("The server is ready to receive")
while True:
    message, clientAddress = serverSocket.recvfrom(2048)
    print('stripping the word "network"...')
    modifiedMessage = message.decode().replace('network','')
    serverSocket.sendto(modifiedMessage.encode(), clientAddress)
    
