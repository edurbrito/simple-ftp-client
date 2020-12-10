#!/bin/bash

# Updateimage

echo -n "Restarting networking service... "
service networking restart
echo "done!"

ifconfig eth0 up 172.16.20.254/24

# EXP 3 ...

ifconfig eth1 up 172.16.21.253/24
echo "IP Addresses set."

route add default gw 172.16.21.254
echo "Default gateway route set."

echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts

echo "Linux router functions activated."

printf "search netlab1.fe.up.pt\nnameserver 172.16.1.1\n" > /etc/resolv.conf
echo "DNS set."