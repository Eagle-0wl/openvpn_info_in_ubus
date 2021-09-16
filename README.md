# openvpn_info_in_ubus
Program to dispaly openvpn server clients in ubus

Program starts up when openvpn server is created. Connects to the server, create ubus instance, requiest clients list and displays it in ubus.

# openvpn
For openvpn_info_in_ubus program to work we need to enable openvpn management interface in Makefile and config files.

I edited init file to add openvpn_info_in_ubus startup, when server instance is created.
