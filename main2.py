import os
import sys

from sympy import init_session

#ux 1 é o 3

tux_number = input()

os.system("/etc/init.d/networking restart")

case tux_number in

  1)
    print( "Configuring tux tux_number...\n\n")
    interface="eth0"
    INT_ADDR="192.168.0.1/16"
    ROUTE="192.168.1.0/24"
    GETWAY="172.16.4.254"
    DEFAULT_GETWAY="192.168.1.1"
    print( "Default configuration for tux$tux_number: \n")

    print( "Interface: $INTERFACE \n")
    print( "Interface address: $INT_ADDR \n")
    print( "Route: $ROUTE \n")
    print( "Getway: $GETWAY \n")
    print( "Default getway: $DEFAULT_GETWAY \n")
    print( "\n")
    
    python3 test.py $INTERFACE $INT_ADDR $ROUTE $GETWAY $DEFAULT_GETWAY
    ;;

  2)
    print( "Configuring tux$tux_number...\n\n")
    interface="eth0"
    INT_ADDR="192.168.0.1/16"
    ROUTE="192.168.1.0/24"
    GETWAY="172.16.4.254"
    DEFAULT_GETWAY="192.168.1.1"
    print( "Default configuration for tux$tux_number: \n")

    print( "Interface: $INTERFACE \n")
    print( "Interface address: $INT_ADDR \n")
    print( "Route: $ROUTE \n")
    print( "Getway: $GETWAY \n")
    print( "Default getway: $DEFAULT_GETWAY \n")
    print( "\n")
    
    python3 test.py $INTERFACE $INT_ADDR $ROUTE $GETWAY $DEFAULT_GETWAY
    ;;

  3)
    print( "Configuring tux$tux_number...\n\n")
    interface="eth0"
    INT_ADDR="192.168.0.1/16"
    ROUTE="192.168.1.0/24"
    GETWAY="172.16.4.254"
    DEFAULT_GETWAY="192.168.1.1"
    print( "Default configuration for tux$tux_number: \n")

    print( "Interface: $INTERFACE \n")
    print( "Interface address: $INT_ADDR \n")
    print( "Route: $ROUTE \n")
    print( "Getway: $GETWAY \n")
    print( "Default getway: $DEFAULT_GETWAY \n")
    print( "\n")
    
    python3 test.py $INTERFACE $INT_ADDR $ROUTE $GETWAY $DEFAULT_GETWAY
    ;;

  4)

    
    print("python3 test.py" interface, int_addr, route, getway, default_getway)

  *)
    print( "Invalid tux number \n")
    ;;
esac

def tuxconfiguration(interface, int_addr):
    print( "Configuring tux tux_number...\n")
    print( "Default configuration for tux tux_number: \n")
    interface="eth1"
    int_addr="172.16.41.1/16"
    route="172.16.1.0/24"
    getway="172.16.1.254"
    default_getway="192.168.1.1"

    print( "Interface:",interface" \n")
    print( "Interface address:, "int_addr,"\n")
    print( "Route:,"route,"\n")
    print( "Getway:",getway "\n")
    print( "Default getway:", default_getway "\n")
    print( "\n")

    print('ifconfig',interface, 'up')
    print('ifconfig ',interface, int_addr)
    print('route add -net ' ,route,'gw',getway)
    print('route add default gw',default_getway)

