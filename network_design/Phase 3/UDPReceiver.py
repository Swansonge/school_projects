#Elizabeth Brown, Nolan Gray, Eric Swanson
#Network Design
#Phase 3 - UDPReceiver.py: Will receive the data stream from the UDP sender (the bytes of a BMP image), convert from from bytes back to BMP format, and display the image. Packets
#                          are sent over RDT2.2. The reciever will check the packets with a checksum to see if it is corrupt. It will also compare the sequence of the current
#                          packet received with the sequence # of the previous packet to check for duplicates.

from socket import *
import binascii #To convert between bytearray and string
import pickle #to convert between list and bytes
import sys #for command line arguments
import random #for random sample of packets to corrupt
#from PIL import Image

########### FUNCTIONS ###########
#extractData
#This method will take the data from a packet and add it to the array data
#Paramaters:
#packet - the packet from which data will be extracted
#data - the array the information from the packet will be added to	
def extractData(packet, data):
    packet = bytearray(packet)
    data.append(packet)

#deliverData
#This method will deliver the data to a new file
#Parameters
#filename - name of the new file
#data - the array containing all of the packet information
def deliverData(filename, data):
    with open(filename, 'ab+') as newImage:
        newImage.write(data)

def check_corrupt(checksum, ACKchecksum):
    #CHECK FOR CORRUPTION
    #return 1 for corrupt, 0 for not corrupt
    if (checksum == ACKchecksum):
        return 0
    else:
        return 1

def calc_checksum(packet_data):
    #variables for calculating checksum
    packet_data = bytearray(packet_data)
    checksum = 0
    temp = 0
    chunk1 = 0
    chunk2 = 0
    #constant to check if there is a carry
    two_byte_max = 65536
    #convert bytearray to string for concatinating bytes
    str_data = binascii.hexlify(packet_data)
    for i in range(0, len(str_data)-7, 8):
        #Select 2 16 bit chunks
        chunk1 = int(bytes(str_data[i:i+4]),16)
        chunk2 = int(bytes(str_data[i+4:i+8]),16)
        #Add the 16 bit chunks together
        temp = (chunk1 + chunk2) % two_byte_max
        #Check if there was a carry, if so add 1 to result
        if (temp < max(chunk1, chunk2)):
            temp = temp + 1
        #Add result to checksum and check for carry
        checksum = (checksum + temp) % two_byte_max
        if (checksum < max(checksum, temp)):
            checksum = checksum + 1
    #return final 16 bit checksum as integer
    return checksum % two_byte_max

def Receive():
    #RECEIVE DATA PACKET
    message, clientAddress = serverSocket.recvfrom(64000)
    packet = pickle.loads(message)
    #return data, checksum to check for corruption, and sequence # to check  for duplicates
    data = packet[1]
    checksum = packet[2]
    seqnum = packet[0]
    return data, checksum, seqnum

def Send(packet):
    # SEND INDIVIDUAL PACKET
    serverSocket.sendto(pickle.dumps(packet), clientAddress)
    return

#this function is used to check if the sequence numbers match up between the sent over data and the sequence number the receiver is currently looking for. This checks for duplicates.
def check_seq(seqnum, DataSeqnum):
    isDuplicate = 0 #assume not a duplicate
    if not (seqnum == DataSeqnum):
        isDuplicate = 1
    return isDuplicate

#function for making an image into a packet. Take sequence #, data (1 kB), and checksum, and put into a packet. This is packet is just an array w/ length=3
def Make_Packet(seqnum, ACK, checksum):
    Packet = [seqnum, ACK, checksum]
    return Packet #this function returns the packet to be sent

def percent_corrupt(ranPacket):
    #find percent of packets to be corrupted and place in array
    percent = int(sys.argv[1])
    decimal = percent * 0.01
    percPacket = numPacket * decimal
    percPacket = int(percPacket)
    packetList = list()
    for i in range(0, numPacket):
        packetList.append(i)
    ranPacket = random.sample(packetList, percPacket)
    ranPacket.sort()
    return ranPacket

# corrupt packets by a % given by the user.
#given array of packets
def corrupt_packet(packet):
    #for every packet
    for i in range(0, len(ranPacket)):
        if numReceived == ranPacket[i]:
            #if that numSent is in ranPacket then corrupt that packet
            ranPacket.pop(i)
            corruptPkt = bytearray([1,2,3,4,5])
            return corruptPkt
    #return regular packet if not
    return packet

############ MAIN ############
#set up server port
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))

#declare variables that will be used
#name of new file
filename = 'newLAND.bmp'
#Finite State Machine (FSM). What action will occur depends on state
FSM = "wait for 0 from below" #this is the initial state at the state of the program. Other possible state is "wait for 1 from below"
#array to put data from packets
dataArr = []

print("The server is ready to receive")
#First incoming packet will tell how many packets to expect from client
message , clientAddress = serverSocket.recvfrom(64000)
numPacket = message.decode()
numPacket = int(numPacket)

numReceived = 0 #variable indicating number of packets successfully received (no duplicates or corruption)
seqnum = 0 #seqnum used to check for duplicates. First seqnum will be 0.
sndpkt = Make_Packet(1,[0,0],0) #initial packet to send if 1st one is corrupt
ACK = [0,1] #ACK 1 if packet 0 is corrupt
checksum = calc_checksum(ACK) #checksum for ACK 1 if packet 0 is corrupt

ranPacket = list()
ranPacket = percent_corrupt(ranPacket)

#keep sending packets until all packets are sent
while numReceived < numPacket:

    #waiting on packet with seq # 0
    if FSM == "wait for 0 from below":
        data, DataChecksum, DataSeqnum = Receive() #get data, checksum, and seqnum from received packet
        oncethru = 1
        checkData = calc_checksum(data) #calculate checksum from received data
        isCorrupted = check_corrupt(checkData, DataChecksum) #check if data packet corrupted
        isDuplicate = check_seq(seqnum, DataSeqnum) #check is seqnum from data is 0. If not 0, then duplicate. store result in variable
        #if data corrupted or duplicate, don't do anything
        if isCorrupted or isDuplicate:
            if (oncethru == 1):
                sndpkt = Make_Packet(1 , corrupt_packet(ACK), checksum) #Send packet 1 again
                Send(sndpkt)
            continue
        else:
            extractData(data, dataArr)
            deliverData(filename, dataArr[numReceived])
            ACK = bytearray([0,0]) #since seqnum = 0, ACK is 2 bytes of 0
            checksum = calc_checksum(ACK)
            sndpkt = Make_Packet(seqnum, corrupt_packet(ACK), checksum) #send ACK 0
            Send(sndpkt)
            seqnum = 1 #move on to next seqnum
            numReceived = numReceived + 1
            FSM = "wait for 1 from below"

    elif FSM == "wait for 1 from below":
        data, DataChecksum, DataSeqnum = Receive()
        checkData = calc_checksum(data)
        isCorrupted = check_corrupt(checkData, DataChecksum)
        isDuplicate = check_seq(seqnum, DataSeqnum) #check is seqnum from data is 1. If not 1, then duplicate. store result in variable
        if isCorrupted or isDuplicate:
            sndpkt = Make_Packet(0, corrupt_packet(ACK), checksum) #Send packet 0 again
            Send(sndpkt)
            continue
        else:
            extractData(data, dataArr)
            deliverData(filename, dataArr[numReceived])
            ACK = bytearray([0,1]) #since seqnum = 1, ACK is 1 (x01)
            checksum = calc_checksum(ACK)
            sndpkt = Make_Packet(seqnum, corrupt_packet(ACK), checksum) #send ACK 1
            Send(sndpkt)
            seqnum = 0 #move to next seqnum
            numReceived = numReceived + 1
            FSM = "wait for 0 from below"

#im = Image.open("newLAND.bmp")  
#im.show()
