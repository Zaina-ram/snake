import socket
from _thread import *
import sys

server = "130.229.179.86"
port = 5555

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM) #AF_INET is the Internet address family for IPv4, and  SOCK_STREAM is the socket type for TCP : the protocol

try:
    s.bind((server,port))
except socket.error as e:
    str(e)

s.listen(2) #agument 2 in case we one want 2 players to connect, remove argument for unlimitied
print("[Server Started]")

def read_pos(str):
    str = str.split(",")
    return int(str[0]), int(str[1])


def make_pos(tup):
    return str(tup[0]) + "," + str(tup[1])

pos = [(20,2),(20,3)]
def threaded_client (conn,player):
    conn.send(str.encode(make_pos(pos[player])))
    reply = ""
    while True:
        try: 
            data = read_pos(conn.recv(2048).decode())
            pos[player] = data
            if not data:
                print("[Client Disconnected]")
                break
            else:
                if pos[0] == pos[1]:
                   pass
                if player == 1:
                    reply = pos[0]
                else:
                    reply = pos[1]
                print("Received: ", reply)
                print("Sending: ", reply)
            conn.sendall(str.encode(make_pos(reply)))
        except:
            break
    print("[connection lost]")
    conn.close()

currentPlayer = 0
while True: 
    conn, addr = s.accept()      #accept any incoming connection 
    print("connected to: ", addr)

    start_new_thread(threaded_client, (conn, currentPlayer))
    currentPlayer += 1