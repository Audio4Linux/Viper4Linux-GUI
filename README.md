# Viper4Linux-GUI
[![Build Status](https://travis-ci.org/Audio4Linux/Viper4Linux-GUI.svg?branch=master)](https://travis-ci.org/ThePBone/Viper4Linux-GUI) ![GitHub](https://img.shields.io/github/license/Audio4Linux/Viper4Linux-GUI) ![GitHub release](https://img.shields.io/github/release/Audio4Linux/Viper4Linux-GUI)

Official UI for Viper4Linux2 [https://github.com/Audio4Linux/Viper4Linux](https://github.com/Audio4Linux/Viper4Linux)
* Telegram: @ThePBone

#### This repo contains the GUI for a newer Viper4Linux version. Please use [Viper4Linux-GUI Legacy](https://github.com/ThePBone/Viper4Linux-GUI) for now, unless you use Arch Linux.

### Arch

On Arch you can use the [AUR package](https://aur.archlinux.org/packages/viper4linux-gui-git/) (using git) provided by yochananmarqos:
```bash
yay -S viper4linux-gui-git  
```
![AUR version](https://img.shields.io/aur/version/viper4linux-gui-git?label=aur%20%28git%29)

### Build from sources
Clone this repository

    git clone https://github.com/Audio4Linux/Viper4Linux-GUI

Compile sources

    cd Viper4Linux-GUI
    qmake
    make
    
```bash
./V4L_Frontend
```

#### Optional: Manual Install
##### Copy to /usr/local/bin
```bash
sudo cp V4L_Frontend /usr/local/bin/viper-gui
sudo chmod 755 /usr/local/bin/viper-gui
```
##### Create Menu Entry
```bash
sudo cat <<EOT >> /usr/share/applications/viper-gui2.desktop
[Desktop Entry]
Name=Viper4Linux Beta
GenericName=Equalizer
Comment=User Interface for Viper4Linux
Keywords=equalizer
Categories=AudioVideo;Audio;
Exec=viper-gui
StartupNotify=false
Terminal=false
Type=Application
EOT
```
##### Download Icon
```bash
sudo wget -O /usr/share/pixmaps/viper-gui.png https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/viper.png -q --show-progress
```

## Screenshots
![GIF](/screenshots/mainwindow.gif)
![GIF](/screenshots/eq.gif)

## Credits
* [Material Icons](https://material.io/tools/icons/)
* [QSS Stylesheets - GTRONICK (modified by @ThePBone)](https://github.com/GTRONICK/QSS)
* [Viper4Linux - noahbliss](https://github.com/noahbliss/Viper4Linux)
### Packages
* [yochananmarqos - AUR packages](https://github.com/yochananmarqos)
