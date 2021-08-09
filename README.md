# Wi-Cache-Agent, W-Cache APs

Wi-Cache-Agent runs alongside the [Odin agent](https://github.com/Wi5/odin-wi5-agent) on Wi-Cache APs. Currently the WiFi AP that is used as Wi-Cache AP is

* TP-Link AC1750 v2 (Archer C7). More information: https://wiki.openwrt.org/toh/tp-link/archer-c5-c7-wdr7500

It runs on OpenWrt. The detailed instruction on building the OpeWrt for the WiFi AP is provided here: [Build Odin AP OpenWrt](https://github.com/Wi5/odin-wi5/wiki/Create-OpenWRT-bin-image-including-openvswitch-and-the-ath9k-patch)

Also, to build the Odin agent follow the instructions provided here: [Build Odin agent](https://github.com/Wi5/odin-wi5/wiki/Cross-compiling-Click-Modular-Router-for-Odin)

To build and run Wi-Cache-Agent (CacheAgentMain.cpp, CacheAgentMain.h) using eclipse, following the instructions provided here ([eclipse-openwrt](https://github.com/hrchhangte/Wi-Cache-Agent/blob/master/eclipse-openwrt%20setup.pdf))

To run the Wi-Cache-Agent, Odin agent, run the scripts provided in [scripts_start_ap_Wi-Cache](https://github.com/hrchhangte/Wi-Cache-Agent/tree/master/scripts_start_ap_Wi-Cache) on the Wi-Cache AP. An example scripts that can be run on two different Wi-Cache APs with IP addresses 192.168.1.10 and 192.168.1.12 are provided for reference.

Run the scripts in the following order.

1. flow.sh (This adds the necessary flows on the vSwitch, should be adapted for the MACs of the clients) <br />
2. start.sh (This makes additional configurations and start the Click router and WiCacheAP)

**Photo of our setup with 2 Wi-Cache APs**

![Setup with 2 TP-Link AC1750 v2 (Archer C7) routers](https://github.com/hrchhangte/Wi-Cache-Agent/blob/master/IMG_20160714_001115.jpg)

