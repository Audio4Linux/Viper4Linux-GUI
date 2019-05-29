#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if (( $EUID != 0 )); then
    echo "Please run as root"
    exit
fi

dir=$(mktemp -d)
cd $dir
printf "Working Directory: $dir\n"

printf "Getting latest release tag... "
  latestrelease=$(curl --silent "https://api.github.com/repos/ThePBone/Viper4Linux-GUI/releases/latest" |grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/')

if [[ $latestrelease == "" ]]
then
    printf ${RED}"Invalid Response"${NC}"\n"  
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

printf "Installing viper-gui to /usr/bin...\n"
cp viper-gui /usr/bin/
chmod 755 /usr/bin/viper-gui

printf "Installing Desktop Entry and Icon\n"
cp viper-gui.png /usr/share/pixmaps/viper-gui.png
cat <<EOT >> /usr/share/applications/viper-gui.desktop
[Desktop Entry]
Name=Viper4Linux
GenericName=Equalizer
Comment=Unofficial UI for Viper4Linux
Keywords=equalizer
Categories=AudioVideo;Audio;
Exec=viper-gui
Icon=/usr/share/pixmaps/viper-gui.png
StartupNotify=false
Terminal=false
Type=Application

EOT

printf "${GREEN}Done!${NC}\n"
