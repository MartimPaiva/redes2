#!/bin/bash
# ifconfig eth0 up
ifconfig eth0 192.168.0.1/16
# route add -net 192.168.1.0/24 gw 172.16.4.254
# route add default gw 192.168.1.1

    python3 test.py eth0 192.168.1.0/24 172.16.4.254 192.168.1.1
