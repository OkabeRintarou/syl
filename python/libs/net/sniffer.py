#!/usr/bin/python3
# -*- coding:utf-8 -*-

import socket
import os

def sniffing_impl(host,win,socket_prot):
	while True:
		sniffer = socket.socket(socket.AF_INET,socket.SOCK_RAW,socket_prot)
		sniffer.bind((host,0))

		sniffer.setsockopt(socket.IPPROTO_IP,socket.IP_HDRINCL,1)
		if win == 1:
			sniffer.ioctl(socket.SIO_RCVALL,socket.RCVALL_ON)
		print(sniffer.recvfrom(65535))

def main():
	host = "192.168.72.129"
	if os.name == 'nt':
		sniffing_impl(host,1,socket.IPPROTO_IP)
	else:
		sniffing_impl(host,0,socket.IPPROTO_ICMP)

if __name__ == '__main__':
	main()	