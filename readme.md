# DUALINIT

> Work in progress

## Directory Structure

### Minimal Directure Hierachie

```
/
├── boot/ (symlinked to /<master>/boot)
│  ├── ...
│  ├── initramfs-x.x.img
│  └── vmlinuz-x.x
├── dev/ (pseudo-fs)
├── etc/
│  └── dualinit.conf
├── proc/ (pseudo-fs)
├── sbin/
│  └── init
├── sys/ (pseudo-fs)
└── <environments>/
```
