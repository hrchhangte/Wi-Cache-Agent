#!/bin/sh

#while true
#do

ovs-ofctl add-flow br0 idle_timeout=0,priority=1000,dl_type=0x806,nw_proto=1,actions=flood
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=a4:1f:72:63:55:b8,actions=output:1

# for C0:25:E9:2F:5E:53
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=C0:25:E9:2F:5E:53,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=C0:25:E9:2F:5E:53,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=C0:25:E9:2F:5E:53,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=C0:25:E9:2F:5E:53,actions=output:2

#for 54:35:30:D6:A4:D3
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=54:35:30:D6:A4:D3,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=54:35:30:D6:A4:D3,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=54:35:30:D6:A4:D3,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=54:35:30:D6:A4:D3,actions=output:2

#for 18:5E:0F:A1:87:E5
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=18:5E:0F:A1:87:E5,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:A1:87:E5,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:A1:87:E5,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=18:5E:0F:A1:87:E5,actions=output:2

#for 54:35:30:D6:93:FF
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=54:35:30:D6:93:FF,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=54:35:30:D6:93:FF,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=54:35:30:D6:93:FF,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=54:35:30:D6:93:FF,actions=output:2

#for 84:16:F9:0E:42:68
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=84:16:F9:0E:42:68,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=84:16:F9:0E:42:68,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=84:16:F9:0E:42:68,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=84:16:F9:0E:42:68,actions=output:2

#for D4:6E:0E:15:56:63
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=D4:6E:0E:15:56:63,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=D4:6E:0E:15:56:63,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=D4:6E:0E:15:56:63,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=D4:6E:0E:15:56:63,actions=output:2

#for 0C:60:76:36:20:FA
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=0C:60:76:36:20:FA,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=0C:60:76:36:20:FA,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=0C:60:76:36:20:FA,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=0C:60:76:36:20:FA,actions=output:2

#for 18:5E:0F:A1:88:12
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=18:5E:0F:A1:88:12,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:A1:88:12,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:A1:88:12,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=18:5E:0F:A1:88:12,actions=output:2

ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=EC:08:6B:87:B2:E9,dl_dst=F4:F2:6D:70:82:2C,actions=LOCAL      
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=F4:F2:6D:70:82:2C,dl_dst=EC:08:6B:87:B2:E9,actions=output:1

ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=D4:6E:0E:36:D8:E7,dl_dst=F4:F2:6D:70:82:2C,actions=LOCAL      
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=F4:F2:6D:70:82:2C,dl_dst=D4:6E:0E:36:D8:E7,actions=output:1

#for CC:B0:DA:A8:EE:DD
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=CC:B0:DA:A8:EE:DD,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=CC:B0:DA:A8:EE:DD,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=CC:B0:DA:A8:EE:DD,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=CC:B0:DA:A8:EE:DD,actions=output:2

#for 18:5E:0F:57:20:BA
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=18:5E:0F:57:20:BA,actions=output:2
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:57:20:BA,dl_dst=a4:1f:72:63:55:b8,actions=output:1
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:57:20:BA,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=18:5E:0F:57:20:BA,actions=output:2

#ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=EC:08:6B:87:B2:E9,dl_dst=F4:F2:6D:70:82:2C,actions=LOCAL       
#ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=F4:F2:6D:70:82:2C,dl_dst=EC:08:6B:87:B2:E9,actions=output:1

#ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=18:5E:0F:57:20:BA,dl_dst=EC:08:6B:87:B2:E9,actions=output:1

#for 74:DF:BF:88:37:9D                                                                                                                                                                                             
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=74:DF:BF:88:37:9D,actions=output:2                                                      
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=74:DF:BF:88:37:9D,dl_dst=a4:1f:72:63:55:b8,actions=output:1                                                      
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=74:DF:BF:88:37:9D,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL                                                         
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=74:DF:BF:88:37:9D,actions=output:2         

#for C0:25:E9:2F:5E:53
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=C0:25:E9:2F:5E:53,actions=output:2    
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=C0:25:E9:2F:5E:53,dl_dst=a4:1f:72:63:55:b8,actions=output:1    
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=C0:25:E9:2F:5E:53,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL       
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=C0:25:E9:2F:5E:53,actions=output:2

#for 40:88:05:69:5F:65
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=1,vlan_tci=0x0000,dl_src=a4:1f:72:63:55:b8,dl_dst=40:88:05:69:5F:65,actions=output:2                                                      
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=40:88:05:69:5F:65,dl_dst=a4:1f:72:63:55:b8,actions=output:1                                                      
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=2,vlan_tci=0x0000,dl_src=40:88:05:69:5F:65,dl_dst=f4:f2:6d:70:82:2c,actions=LOCAL                                                         
ovs-ofctl add-flow br0 importance=0,idle_timeout=0,priority=1000,in_port=LOCAL,vlan_tci=0x0000,dl_src=f4:f2:6d:70:82:2c,dl_dst=40:88:05:69:5F:65,actions=output:2


#sleep 2

#done
