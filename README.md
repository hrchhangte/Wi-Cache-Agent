# Wi-Cache-Agent, W-Cache APs

Wi-Cache-Agent runs alongside the Odin agent on Wi-Cache APs. Currently the WiFi AP that is used as Wi-Cache AP is

* TP-Link AC1750 v2 (Archer C7). More information: https://wiki.openwrt.org/toh/tp-link/archer-c5-c7-wdr7500

It runs on OpenWrt. The detailed instruction on building the OpeWrt for the WiFi AP is provided here: [Build Odin AP OpenWrt](https://github.com/Wi5/odin-wi5/wiki/Create-OpenWRT-bin-image-including-openvswitch-and-the-ath9k-patch)

Also, to build the Odin agent follow the instructions provided here: [Build Odin agent](https://github.com/Wi5/odin-wi5/wiki/Cross-compiling-Click-Modular-Router-for-Odin)

To build and run Wi-Cache-Agent (CacheAgentMain.cpp, CacheAgentMain.h) using eclipse, following the instructions provided here ([eclipse-openwrt](https://github.com/hrchhangte/Wi-Cache-Agent/blob/master/eclipse-openwrt%20setup.pdf))

To run the Wi-Cache-Agent, Odin agent, run the [start.sh](https://github.com/hrchhangte/Wi-Cache-Agent/blob/master/scripts_start_ap_odin/start.sh) on the Wi-Cache AP.

