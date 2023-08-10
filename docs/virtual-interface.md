If you want to use a virtual interface for gensokyo, it should be configured as such:
- name: `gensok0`
- MAC/"hardware" address: `4e:53:4f:4b:59:4f`
- IPv4 address: `103.110.115.0/24`
- IPv6 address: `fd67:6e73:6f6b:796f::1/64`

The helper script included in this repository (`interface.pl`) already creates the interface with these parameters, but the interface will not be persistent unless you configure it to be created on boot.

Keep in mind that you can absolutely bind to a real interface as well should you want to give remote access to the virtual network. Using a virtual interface is basically like using a loopback device a-la localhost but with its own set of ports, so you can just use localhost if you don't want a separate set of ports.

To configure the interface with systemd (on a Linux system), you should first enable the `dummy` kernel module on boot. You can utilize systemd's `systemd-modules-load` service by writing the module name to `/etc/modules-load.d/modules.conf`.
You can also add the module to your ramdisk, but that depends on the distribution you're using.
For debian etc., see documentation for `initramfs-tools` and for arch-based distros see `mkinitcpio`.

Secondly, you should enable the systemd-networkd service. you can do this by running `systemctl enable systemd-networkd`.

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