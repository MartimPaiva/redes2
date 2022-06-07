def gkterm_config_help(router_eth0, eth0_network_ip, computer_2_eth1_ip):
    print("GKTERM COMMANDS:")
    print("(Interface config)\n")

    print("interface gigabitethernet 0/0")
    print("ip address", router_eth0, " 255.255.255.0")
    print("no shutdown")
    print("exit")
    print("show interface gigabitethernet 0/0")

    print("interface gigabitethernet 0/1")
    print("ip address 172.16.1.254 255.255.255.0")
    print("no shutdown")
    print("exit")
    print("show interface gigabitethernet 0/1 \n")

    print("Static route")
    print("ip route 0.0.0.0 0.0.0.0 172.16.1.254")
    print("ip route", eth0_network_ip, "255.255.255.0 " + computer_2_eth1_ip)

def vlan_config_help(vlan, port_number):
    print("configure terminal")
    print("vlan", vlan)
    print("end")
    print("show vlan id", vlan)

    print("configure terminal")
    print("interface fastethernet 0/" + port_number)
    print("switchport mode access")
    print("switchport access vlan", vlan)
    print("end")
    print("show running-config interface fastethernet 0/" + port_number)
    print("show interfaces fastethernet 0/" + port_number, "switchport")