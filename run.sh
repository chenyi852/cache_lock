#!/bin/bash

sudo bash -c "echo '0:0x999@0x4' > /proc/cachelock"
sudo bash -c "echo '1:0x999@0x4' > /proc/cachelock"
cat /proc/cachelock
sudo rmmod cache_lock
sudo dmesg -c

