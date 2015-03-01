#!/bin/bash
cd /usr/src/linux/
sudo cp /mnt/hgfs/OSProject1/get_child_pids.c /usr/src/linux/kernel/
make -j2 bzImage

if [ $? -ne 0 ]; then
    echo "error"
    exit 1
fi

sudo cp /usr/src/linux/arch/x86/boot/bzImage /boot/vmlinuz-3.18.3+
make modules
sudo make modules_install
sudo update-initramfs -k 3.18.3+ -u
sudo reboot

