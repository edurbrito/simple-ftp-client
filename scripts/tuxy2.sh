#!/bin/bash

# Updateimage

echo -n "Restarting networking service... "
service networking restart
echo "done!"

ifconfig eth0 up 172.16.21.1/24
echo "IP Address set."

# EXP 3

route add default gw 172.16.21.254
echo "Default gateway route set."

route add -net 172.16.20.0/24 gw 172.16.21.253
echo "Route to VLAN 20 set."

printf "search netlab1.fe.up.pt\nnameserver 172.16.1.1\n" > /etc/resolv.conf
echo "DNS set."