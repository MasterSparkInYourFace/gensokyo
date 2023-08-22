# Virtual interface

If you want to use a virtual interface for gensokyo for some reason instead of a real one, it should be configured as such:
- name: `gensok0`
- MAC/"hardware" address: `4e:53:4f:4b:59:4f`
- IPv4 address: `103.110.115.0/24`
- IPv6 address: `fd67:6e73:6f6b:796f::1/64`

To be able to create dummy interfaces, you should enable the `dummy` kernel module, preferably on boot. You can utilize systemd's `systemd-modules-load` service by writing the module name to `/etc/modules-load.d/modules.conf`.
You can also add the module to your ramdisk, but that depends on the distribution you're using.
For debian etc., see documentation for `initramfs-tools` and for arch-based distros see `mkinitcpio`.
To load the module temporarily, `modprobe dummy` as root will do the trick.

To configure using systemd, you should enable the systemd-networkd service. you can do this by running `systemctl enable systemd-networkd`.

You should then make two files called `10-gensokyo.netdev` and `11-gensokyo.network` under `/etc/systemd/network` .

In the former, write
```ini
[NetDev]
Name=gensok0
MACAddress=4e:53:4f:4b:59:4f
Kind=dummy
```
and in the latter, write
```ini
[Match]
Name=gensok0
MACAddress=4e:53:4f:4b:59:4f
Kind=dummy

[Network]
Description=Gensokyo virtual network interface
Address=103.110.115.0/24
Address=fd67:6e73:6f6b:796f::1/64
```

To configure using NetworkManager instead of systemd, run
```sh
nmcli conn add con-name gensokyo-interface \
	ifname gensok0 \
	type dummy \
	ip4 103.110.115.0/24 \
	ip6 fd67:6e73:6f6b:796f::1/64 \
	mac 4e:53:4f:4b:59:4f
```
