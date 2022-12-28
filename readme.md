# DualINIT

A meta-init system for Linux to 'dualboot' multiple Linux distributions in user-space

> Work in progress!

## Directory Structure

### Minimal Directure Hierachie

```
/
├── boot/ (mounted to /<master>/boot)
│  ├── ...
│  ├── initramfs-x.x.img
│  └── vmlinuz-x.x
├── lost+found/ (mounted to /<master>/lost+found)
├── dev/ (pseudo-fs)
├── etc/
│  └── dualinit.conf
├── proc/ (pseudo-fs)
├── sbin/
│  └── init
├── sys/ (pseudo-fs)
├── <environments>/
└── ...
```

## Default Mounts

```
-           /dev        /dev        rbind
-           /           /dualinit   bind
proc        proc        /proc       relatime
tmpfs       run         /run        mode=0755
-           /sys        /sys        rbind
tmpfs       tmp         /tmp        mode=1777,strictatime
```

### Master Mounts

```
/boot       -> /<master>/boot
/lost+found -> /<master>/lost+found
```
