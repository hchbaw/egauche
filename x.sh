#!/bin/sh

Xephyr :1 -noreset -ac -br -screen 800x600 -host-cursor &

sleep 2
export DISPLAY=:1

# fiddle my own envs.
xset fp+ /pub/fonts/Pcf
xset fp rehash

export RXVT_SOCKET=$HOME/.rxvt-unicode-${HOST}-testing
unset WINDOW

unset DBUS_SESSION_BUS_ADDRESS
eval `dbus-session --exit-with-session`

# start E.
unset E_RESTART E_START E_IPC_SOCKET E_START_TIME E_CONF_PROFILE
enlightenment_start \
-no-precache \
-profile testing
