import os

from gkterm_help import vlan_config_help
from gkterm_help import impl_nat


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
            aux_str = "ifconfig " + eth + " up"
            os.system(aux_str)
            aux_str = "ifconfig " + eth + " " + self.ip_addr + "/24"
            os.system(aux_str)

    def add_route(self, switch_ip):
        print("route add - net", switch_ip, "gw", self.ip_addr)
        print("route - n")

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


def go_to_tux(tux_name):
    print("go to ", tux_name)


def ask_debug():
    print("\nDebug mode:", end=" ")
    print("y/n?\n")
    return input()


def send_command(command, debug):
    auxstr = "debug_terminal# " + command
    if debug == 'y':
        print(auxstr)
    else:
        os.system(auxstr)


def lab3():
    print("Welcome to lab3! \n")

    workbench = 6
    switch_ip = "172.16." + str(workbench) + ".0/24"
    router_eth0 = "172.16." + str(workbench) + "1.254"
    router_eth1 = "172.16.1.29"

    computers_name_list = ["Computer L",
                           "Computer M", "Computer M", "Computer R"]
    computer_tux_number = [3, 4, 4, 2]
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
        tux_wait()

        n_step = 2
        print("passo", str(n_step))
        networking_restart(debug)
        left_tux_0.config_ip(debug)

        print("go to tux4")
        tux_wait()
        n_step = 4
        print("passo", str(n_step))
        send_command(
            "echo 0 > / proc / sys / net / ipv4 / icmp_echo_ignore_broadcasts", debug)
        n_step = 5
        print("passo", str(n_step))
        print("tux -> switch")
        left_tux_0.add_route(switch_ip)

    elif(name == "Computer M"):
        print("mid_tux!")
        mid_tux_0 = current_ip_list[0]
        mid_tux_1 = current_ip_list[1]
        ###################################
        debug = ask_debug()
        n_step = 1
        print("passo", str(n_step))
        print("\n1 Transforming mid_tux into a router")
        networking_restart(debug)
        mid_tux_0.config_ip(debug)
        mid_tux_1.config_ip(debug)
        print("\n2. Observe MAC addresses and IP addresses in tuxy4.eth0 and tuxy4.eth1")
        if(debug):
            print("\n ifconfig")
        else:
            os.system("ifconfig")

        print(
            "\nReconfigure left_tux and right_tux so that each of them can reach the other")
        n_step = 2
        print("passo", str(n_step))
        print("Go to left_tux")
        print("Go to mid_tux")
        tux_wait()

        n_step = 3
        print("passo", str(n_step))
        print("\nConfiguring vlan networks in the router")
        vlan_config_help(str(workbench*10 + 0), 0)
        vlan_config_help(str(workbench*10 + 1), 1)

        n_step = 4
        print("passo", str(n_step))

        send_command(
            "echo 1 > / proc / sys / net / ipv4 / ip_forward", debug)
        send_command(
            "echo 0 > / proc / sys / net / ipv4 / icmp_echo_ignore_broadcasts", debug)

        print("Go to left_tux")
        print("Go to right_tux")
        n_step = 5
        print("passo", str(n_step))
        tux_wait()
        #######------------------------###############

    elif(name == "Computer R"):
        print("right_tux!")
        right_tux_1 = current_ip_list[0]
        ###################################
        debug = ask_debug()
        print("Go to mid tux:")
        tux_wait()

        n_step = 2
        print("passo", str(n_step))
        networking_restart(debug)
        right_tux_1.config_ip()
        print("go to mid_tux")
        tux_wait()

        n_step = 4
        print("passo", str(n_step))
        send_command(
            "echo 0 > / proc / sys / net / ipv4 / icmp_echo_ignore_broadcasts", debug)

        n_step = 5
        print("passo", str(n_step))
        print("tux -> switch")
        left_tux_0.add_route(switch_ip)

    else:
        print("Error")

    return debug


if __name__ == "__main__":
    lab3()
    debug = ask_debug()
    impl_nat()
    send_command(
        "search netlab.fe.up.pt nameserver 172.16.1.2' > /etc/resolv.conf", debug)
    print()
    print("lab6")
    go_to_tux("tux_3")
    print("executar applicação")
    print("(começar wireshark)")
    send_command("gcc ", debug)
    send_command("./ ", debug)
