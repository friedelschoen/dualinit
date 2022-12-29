# DualINIT

A meta-init system for Linux to 'dualboot' multiple Linux distributions in user-space sharing one Linux kernel

> :warning: Work in progress!

## Installation

### Building the executable

You are required to have:

-   [Git](https://git-scm.com/)
-   [GCC for your Linux-architecture](https://www.gnu.org/software/gcc/)
-   [GNU make](https://www.gnu.org/software/make/)

> If you are building DualINIT for an other architecture than your host, grab a cross-compiler and set the environment-variable `CC` to your compiler-path.

Now you can invoke the commands to clone this repository and compile `bin/dualinit`:

```
$ git clone https://github.com/friedelschoen/dualinit.git
$ cd dualinit
$ make bin/dualinit
```

Congratulations! :tada: You have a fresh built of DualINIT, you can continue to set up a running dualinit-system.

If something went wrong, feel free to open an issue!

### Getting Started

Some dictionaries and files in the filesystem-root are required for DualINIT and Linux to work:

-   `/boot` - the Linux kernel and bootloader
-   `/dev` - an empty dictionary filled by the kernel with various devices
-   `/proc` - an empty dictionary filled by the kernel with process information
-   `/sys` - an empty dictionary filled by the kernel with various kernel files
-   `/sbin/init` - the dualinit executable
-   `/etc/dualinit.conf` - dualinit configuration
-   `/<section>` - a section to be loaded
-   `/<section>/{dev,sys,proc}` - pseudo-filesystems binded from root
-   `/<section>/{run,tmp}` - temporary dictionaries for services and applications
-   `/<section>/dualinit` - the filesystem-root to configure DualINIT
-   `/<section>/boot` - can be mounted to `/boot` to update the kernel, bootloader, etc.

Resulting in a structure like that:

```
/
├── boot/
│  ├── initramfs-x.x.img
│  ├── vmlinuz-x.x
│  └── ...
├── dev/
├── etc/
│  └── dualinit.conf
├── proc/
├── sbin/
│  └── init
├── sys/
└── <section>/
   ├── dev/
   ├── sys/
   ├── proc/
   ├── run/
   ├── tmp/
   ├── dualinit/
   └── (boot/)
```

## Configuration

Move the [basic configuration-file](/docs/dualinit.conf) to `/etc/dualinit.conf` and edit it to fit your needs.

A reference to all commands can be found [here](/docs/config.md).

## License

This whole project is licensed under the beautiful terms of the [`zlib-license`](LICENSE).
