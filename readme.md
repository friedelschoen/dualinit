## Directory Structure

### Minimal Directure Hierachie

```
/
├── boot/
│  ├── ...
│  ├── initramfs-x.x.img
│  └── vmlinuz-x.x
├── dev/
├── etc/
│  └── dualinit.conf
├── proc/
├── sbin/
│  └── init
├── sys/
└── <environments>/
```

By default `/boot` will be binded (mounted) to &lg;new_root&gg;/boot and 
`/sbin/init` to &lg;new_root&gg;/sbin/dualboot linked