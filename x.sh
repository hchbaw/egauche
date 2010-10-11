#!/bin/sh

ps axo args|grep [X]ephyr
if test $? -ne 0; then
  #Xephyr :1 -noreset -ac -br -screen 800x600 -host-cursor &
  Xephyr :1 -noreset -ac -br -screen 512x512 &
  sleep 2
fi

export DISPLAY=:1

# fiddle my own envs.
xset fp+ /pub/fonts/Pcf
xset fp rehash
setxkbmap -layout dvorak

export RXVT_SOCKET=$HOME/.rxvt-unicode-${HOST}-testing
unset WINDOW

unset DBUS_SESSION_BUS_ADDRESS
eval `dbus-launch --exit-with-session`

urxvt -name 'URxvt-ml-tiny-simple' -g 80x25+20+40 &

# start E.
unset E_RESTART E_START E_IPC_SOCKET E_START_TIME E_CONF_PROFILE
enlightenment_start \
-no-precache \
-profile testing
