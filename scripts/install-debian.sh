#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if ( $EUID != 0 ); then
    echo "Please run as root"
    exit
fi

if pgrep viper-gui >/dev/null 2>&1
  then
    printf "${RED}viper-gui is in use\n${NC}Please close all Viper4Linux-UI windows and try again.\n"
    exit 1
fi

dir=$(mktemp -d)
cd $dir
printf "Working Directory: $dir\n"

printf "Getting latest release tag... "
  latestrelease=$(curl --silent "https://api.github.com/repos/ThePBone/Viper4Linux-GUI/releases/latest" |grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')

if [[ $latestrelease == "" ]]
then
    printf "${RED}Invalid Response${NC}\n"  
    printf "Please check your network connection.\n"
    exit 1
fi
  
printf ${GREEN}$latestrelease${NC}"\n"

printf "Downloading build using wget... \n"
wget -O viper-gui https://github.com/ThePBone/Viper4Linux-GUI/releases/download/$latestrelease/viper-gui -q --show-progress

if [ ! -f "viper-gui" ]; then
    printf "${RED}Downloaded file not found${NC}\n"
    exit 1
fi

printf "Downloading icon using wget... \n"
wget -O viper-gui.png https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/viper.png -q --show-progress

if [ ! -f "viper-gui.png" ]; then
    printf "${RED}Downloaded icon not found${NC}\n"
fi

printf "Installing Dependencies using apt-get\n"
sudo apt-get --assume-yes install libqt5widgets5 libqt5gui5 libqt5core5a libqt5xml5 libgl1-mesa-dev

printf "Installing viper-gui to /usr/local/bin...\n"
rm /usr/bin/viper-gui 2> /dev/null #remove gui from oldpath if still existing there
cp viper-gui /usr/local/bin/
chmod 755 /usr/local/bin/viper-gui

printf "Installing Desktop Entry and Icon\n"
cp viper-gui.png /usr/share/pixmaps/viper-gui.png
cat <<EOT >> /usr/share/applications/viper-gui.desktop
[Desktop Entry]
Name=Viper4Linux
GenericName=Equalizer
Comment=Official UI for Viper4Linux
Keywords=equalizer
Categories=AudioVideo;Audio;
Exec=viper-gui
Icon=/usr/share/pixmaps/viper-gui.png
StartupNotify=false
Terminal=false
Type=Application

EOT

printf "${GREEN}Done!${NC}\n"
