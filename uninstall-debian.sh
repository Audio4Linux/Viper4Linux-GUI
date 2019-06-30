#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if (( $EUID != 0 )); then
    printf "${RED}Please run as root${NC}\n"
    exit
fi

if pgrep viper-gui >/dev/null 2>&1
  then
    printf "${RED}viper-gui is in use\n${NC}Please close all Viper4Linux-UI windows and try again.\n"
    exit 1
fi

printf "Removing viper-gui from /usr/local/bin or /usr/bin...\n"
rm /usr/bin/viper-gui 2> /dev/null #remove gui from oldpath if still existing there
rm /usr/local/bin/viper-gui 2> /dev/null

printf "Removing desktop entry and icon...\n"
rm /usr/share/pixmaps/viper-gui.png
rm /usr/share/applications/viper-gui.desktop

printf "${GREEN}Done!${NC}\n"
