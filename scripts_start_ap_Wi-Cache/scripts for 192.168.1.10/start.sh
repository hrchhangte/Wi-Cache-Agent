#!/bin/sh

## Variables
echo "Setting variables"
CTLIP=192.168.1.145 # Controller IP
SW=br0
DPPORTS="eth1" # Ports for data plane
VSCTL="ovs-vsctl"

## Setting interfaces
echo "Setting interfaces"
ifconfig wlan0 down # Main interface
ifconfig wlan1 down # Auxiliary interface
iw phy phy0 interface add mon0 type monitor
iw phy phy1 interface add mon1 type monitor
ifconfig mon0 up
ifconfig mon1 up
#ifconfig mon0 mtu 1532
#ifconfig mon1 mtu 1532
ifconfig wlan0 up
ifconfig wlan1 up

# add this route in order to permit control from Unizar networks
# traffic from these networks will not go through the default gateway
#route add -net 155.210.158.0 netmask 255.255.255.0 gw 155.210.157.254 eth0
#route add -net 155.210.156.0 netmask 255.255.255.0 gw 155.210.157.254 eth0
# set the default gateway where masquerading is being performed
#route del default gw 155.210.157.254
#route add default gw 192.168.1.131

## We assume that OpenvSwitch runs when OpenWRT starts up
echo "Restarting OpenvSwitch daemon"
/etc/init.d/openvswitch stop
sleep 1

# The next line is added in order to start the controller after stopping openvswitch
read -p "Now you can launch the Wi-5 odin controller and press Enter" pause

#echo "Cleaning DB"
#if [ -d "/etc/openvswitch" ]; then
#  rm -r /etc/openvswitch
#fi
#if [ -f "/var/run/db.sock" ]; then
#  rm /var/run/db.sock
#fi
#if [ -f "/var/run/ovsdb-server.pid" ]; then
#  rm /var/run/ovsdb-server.pid
#fi
#if [ -f "/var/run/ovs-vswitchd.pid" ]; then
#  rm /var/run/ovs-vswitchd.pid
#fi

/etc/init.d/openvswitch start

echo "Configuring OpenvSwitch"
$VSCTL add-br $SW # Create the bridge
ifconfig $SW up # In OpenWrt 15.05 the bridge is created down

$VSCTL set-controller $SW tcp:$CTLIP:6633 # Configure the OpenFlow Controller.
for i in $DPPORTS ; do # Including ports to OVS
    PORT=$i
    ifconfig $PORT up
    $VSCTL add-port $SW $PORT
done

#This is our DataPath ID
#DPID=00000000000000010

#Ensure that the switch has the correct DataPath ID
#$VSCTL set bridge $SW other-config:datapath-id=$DPID

## Launch click
#echo "Mounting USB" # Because we have click into the USB
#if [ ! -d "/mnt/usb" ]; then
#  mkdir -p /mnt/usb
#fi
#mount /dev/sda1 /mnt/usb/ 
#cd /mnt/usb

sleep 3
./click router.cli &
sleep 1

ifconfig ap up # Adding ap interface (click Interface) to OVS
$VSCTL add-port $SW ap
sleep 1

## OpenVSwitch Rules
# OpenFlow rules needed to make it possible for DHCP traffic to arrive to the Wi-5 odin controller
# It may happen that 'eth1.2' is port 1 and 'ap' is port 2
ovs-ofctl add-flow br0 in_port=2,dl_type=0x0800,nw_proto=17,tp_dst=67,actions=output:1,CONTROLLER
ovs-ofctl add-flow br0 in_port=1,dl_type=0x0800,nw_proto=17,tp_dst=68,actions=output:CONTROLLER,2
# It may happen that 'eth1.2' is port 2 and 'ap' is port 1
ovs-ofctl add-flow br0 in_port=1,dl_type=0x0800,nw_proto=17,tp_dst=67,actions=output:2,CONTROLLER
ovs-ofctl add-flow br0 in_port=2,dl_type=0x0800,nw_proto=17,tp_dst=68,actions=output:CONTROLLER,1

ifconfig br0 192.168.1.10 netmask 255.255.255.0 broadcast 192.168.1.255       
ifconfig br0 up                                                               
ifconfig eth1 0.0.0.0                                                       
                                                                              
ip link set dev ap mtu 1400                                                   
ip link set dev br0 mtu 1400                                                  
                                                                              
chmod -R 777 /mnt      

./WicacheAP &
sleep 1

