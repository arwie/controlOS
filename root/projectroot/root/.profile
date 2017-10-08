#!/bin/sh


systemctl --no-block start studio.socket

mount -o remount,rw /
