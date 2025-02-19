conf t
interface gigabitethernet 0/0
ip address 172.16.21.254 255.255.255.0
no shutdown
ip nat inside
exit

interface gigabitethernet 0/1
ip address 172.16.1.29 255.255.255.0
no shutdown
ip nat outside
exit

ip nat pool ovrld 172.16.1.29 172.16.1.29 prefix 24
ip nat inside source list 1 pool ovrld overload

access-list 1 permit 172.16.20.0 0.0.0.7
access-list 1 permit 172.16.21.0 0.0.0.7

ip route 0.0.0.0 0.0.0.0 172.16.1.254
ip route 172.16.20.0 255.255.255.0 172.16.21.253
end