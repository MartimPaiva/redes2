import os


class computer_ip:
    def __init__(self, workbench, computer_name, eth_number):
        self.workbench = workbench
        self.computer_name = computer_name
        self.eth_number = eth_number
        self.vlan = str(workbench) + str(eth_number)

    def define_ip_addr(self, tux_number, ip_last_8bits):
        self.tux_number = tux_number
        self.network_number = ip_last_8bits
        self.ip_addr = "172.16." + self.vlan + "." + str(ip_last_8bits)
        print("Created:", end=" ")
        self.info()

    def config_ip(self, debug):
        eth = "eth" + str(self.eth_number)

        if debug == 'y':
            print("#tux: ifconfig", eth, "up")
            print("#tux: ifconfig", eth, self.ip_addr + "/24")
        else:
            os.system("ifconfig", eth, "up")
            os.system("ifconfig", eth, self.ip_addr + "/24")

    def getName(self):
        return str(self.computer_name)

    def info(self):
        print(self.computer_name, "(tux" + str(self.workbench) +
              str(self.tux_number) + "):", self.ip_addr)


def networking_restart(debug):
    if debug == 'y':
        print("/etc/init.d/networking restart")
    else:
        os.system("/etc/init.d/networking restart")


def tux_wait():
    print("Wating...")
    input()


def ask_debug():
    print("\nDebug mode:", end=" ")
    print("y/n?\n")
    return input()


def lab3():
    print("Welcome to lab3! \n")

    workbench = 6
    router_eth0 = "172.16." + str(workbench) + "1.254"
    router_eth1 = "172.16.1.29"

    computers_name_list = ["Computer L",
                           "Computer M", "Computer M", "Computer R"]
    computer_tux_number = [2, 3, 3, 4]
    __computers_ip_eth = [0, 0, 1, 1]
    __computer_last_8 = [1, 254, 253, 1]

    tux1_list = []
    tux2_list = []
    tux3_list = []
    tux4_list = []

    list_of_tuxes = [tux1_list, tux2_list, tux3_list, tux4_list]

    for x in range(4):
        new_computer_ip = computer_ip(
            workbench, computers_name_list[x], __computers_ip_eth[x])
        new_computer_ip.define_ip_addr(
            computer_tux_number[x], __computer_last_8[x])
        list_of_tuxes[computer_tux_number[x]-1].append(new_computer_ip)

    current_tux_number = -1

    # pergunta o tux em que estás
    while (current_tux_number < 2) | (current_tux_number > 4):
        print("Select your current tux", end=" ")
        current_tux_number = int(input())

    current_ip_list = list_of_tuxes[current_tux_number - 1]

    name = current_ip_list[0].getName()

    if(name == "Computer L"):
        print("left_tux!")
        left_tux_0 = current_ip_list[0]
        ###################################
        debug = ask_debug()
        print("Go to tux4")
        print("Wating...")
        input()
        networking_restart(debug)

        left_tux_0.config_ip(debug)
    elif(name == "Computer M"):
        print("mid_tux!")
        mid_tux_0 = current_ip_list[0]
        mid_tux_1 = current_ip_list[1]
        ###################################
        debug = ask_debug()
        print("\n1 Transforming mid_tux into a router")
        networking_restart(debug)
        mid_tux_0.config_ip(debug)
        mid_tux_1.config_ip(debug)
        print("\n2. Observe MAC addresses and IP addresses in tuxy4.eth0 and tuxy4.eth1")
        if(debug):
            print("\n ifconfig")
        else:
            os.system("ifconfig")
        print("\n3. Reconfigure tuxy1 and tuxy2 so that each of them can reach the other")
        print("Go to tux3")
        print("Go to tux2")
        tux_wait()

    elif(name == "Computer R"):
        print("right_tux!")
        right_tux_1 = current_ip_list[0]
        ###################################
        debug = ask_debug()
        print("Go to tux4")
        print("Wating...")
        input()
        networking_restart()
        right_tux_1.config_ip()
    else:
        print("Error")


if __name__ == "__main__":
    lab3()
# (no tux42)
# route add - net eth0_24_mask gw computer_2_eth1_ip
# ping computer_1_eth0_ip

# (no tux43)
# ping 172.16.41.1
# route add default gw 172.16.40.254

# (no tux42)
# route add - net 172.16.1.0/24 gw router_eth0
# route add default gw router_eth0

# (no tux44)
# route add - net 172.16.1.0/24 gw router_eth0
# route add default gw router_eth0

# route -n

# def wireshark():
#     print("Open whireshark in tux3")
#     input()
#     print("ping " + computer_2_eth0_ip)
#     print("ping " + computer_2_eth1_ip)
#     print("ping " + tux2_eth1_ip)
#     print("ping " + router_eth0)
#     print("ping " + router_eth1)

# (no tux42)
# echo 0 > / proc / sys / net / ipv4 / conf / eth0 / accept_redirects
# echo 0 > / proc / sys / net / ipv4 / conf / all / accept_redirects

# route -n
# route del - net eth0_24_mask gw 172.16.41.253
# ping computer_1_eth0_ip # eth0 tux23

# ping computer_1_eth0_ip # eth0 tux23 (fazer no wireshark)
# traceroute computer_1_eth0_ip

# route add - net eth0_24_mask gw 172.16.41.253
# traceroute 172.16.20.1

# route -n
# route del - net eth0_24_mask gw 172.16.41.253
#     print("")

#     print("echo 1 > / proc / sys / net / ipv4 / conf / eth0 / accept_redirects")
#     print("echo 1 > / proc / sys / net / ipv4 / conf / all / accept_redirects")


# configure terminal
# interface fastethernet 0/x (número da porta)
# switchport mode access
# switchport access vlan 40
# end
# show running-config interface fastethernet 0/1
# show interfaces fastethernet 0/1 switchport

# (na vlan41 fica o tux44,tux42 e o router)

# configure terminal
# vlan 41
# end
# show vlan id 41

# configure terminal
# interface fastethernet 0/x (número da porta)
# switchport mode access
# switchport access vlan 41
# end
# show running-config interface fastethernet 0/x
# show interfaces fastethernet 0/x switchport
