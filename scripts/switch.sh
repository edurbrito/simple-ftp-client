#!/bin/bash

# reiniciair switch

enable
password:8nortel
configure terminal
no vlan 2-4094
exit
copy flash:tux2-clean startup-config
reload

# CONFIGURAR SWITCH

# EXP 2 ...

conf t
vlan 20
end

conf t
vlan 21
end

# TUX 3 ETH0

conf t
interface fastethernet 0/1
switchport mode access
switchport access vlan 20
end

# TUX 4 ETH0

conf t
interface fastethernet 0/3
switchport mode access
switchport access vlan 20
end

# TUX 2 ETH0

conf t
interface fastethernet 0/2
switchport mode access
switchport access vlan 21
end

# EXP 3

# TUX 4 ETH1

# conf t
# interface fastethernet 0/4
# switchport mode access
# switchport access vlan 21
# end

# ROUTER

# conf t
# interface gigabitethernet 0/1
# switchport mode access
# switchport access vlan 20
# end

show vlan brief

copy running-config flash:edupedro-T1B2

# reiniciair switch mas com nossas configurações

enable
password:8nortel
configure terminal
no vlan 2-4094
exit
copy flash:edupedro-T1B2 startup-config
reload