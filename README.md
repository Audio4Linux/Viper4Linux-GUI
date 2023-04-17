# Viper4Linux-GUI
[![Build Status](https://travis-ci.org/Audio4Linux/Viper4Linux-GUI.svg?branch=master)](https://travis-ci.org/ThePBone/Viper4Linux-GUI) ![GitHub](https://img.shields.io/github/license/Audio4Linux/Viper4Linux-GUI) ![GitHub release](https://img.shields.io/github/release/Audio4Linux/Viper4Linux-GUI)

Official UI for Viper4Linux [https://github.com/Audio4Linux/Viper4Linux](https://github.com/Audio4Linux/Viper4Linux)
* Telegram: @ThePBone
* Supports both Viper4Linux2 and legacy Viper4Linux

## Repository status

This project will likely not receive any more feature updates, as my focus has shifted towards [JamesDSP for Linux](https://github.com/Audio4Linux/JDSP4Linux).

[JDSP](https://github.com/Audio4Linux/JDSP4Linux) is a **free & open-source audio effects processor** with more DSP algorithms that are actively developed. It also includes an updated GUI with more features, such as AutoEQ integration, a scripting engine for custom audio effects, and **full Pipewire support**.

## Installation

* [Arch](#arch)
* [Ubuntu PPA](#ubuntu-ppa)
* [Debian](#debian)
* [Build from sources](#build-from-sources)

**IMPORTANT:** Make sure to install the Viper4Linux driver and CLI first. It is not included with this GUI: <https://github.com/Audio4Linux/Viper4Linux>


### Arch

On Arch you can use the [AUR package](https://aur.archlinux.org/packages/viper4linux-gui-git/) (using git) provided by yochananmarqos:
```bash
yay -S viper4linux-gui-git  
```

Or [this one](https://aur.archlinux.org/packages/viper4linux-gui) to get the latest stable release:
```bash
yay -S viper4linux-gui
```

![AUR version](https://img.shields.io/aur/version/viper4linux-gui?label=aur%20%28stable%29) ![AUR version](https://img.shields.io/aur/version/viper4linux-gui-git?label=aur%20%28git%29)

### Ubuntu (PPA)

Add my PPA repository if you haven't already:
```bash
curl -s --compressed "https://thepbone.github.io/PPA-Repository/KEY.gpg" | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/thepbone_ppa.gpg
sudo curl -s --compressed -o /etc/apt/sources.list.d/thepbone_ppa.list "https://thepbone.github.io/PPA-Repository/thepbone_ppa.list"
sudo apt update
```
And install the app:
```bash
sudo apt install viper4linux-gui
```
[View PPA on GitHub](https://github.com/ThePBone/PPA-Repository)

### Debian
You can find DEB packages [here](https://github.com/Audio4Linux/Viper4Linux-GUI/releases).
```bash
sudo dpkg -i <..deb-filename..>
```

### Build from sources
This GUI requires the following dependencies:
* Required Qt version: Qt 5.9.5 or higher
* Required Qt modules: core, gui, xml, dbus, network, svg and multimedia

Dependencies for Debian-based distros:
```bash
sudo apt install libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev gstreamer1.0-plugins-bad libgstreamer-plugins-bad1.0-dev
sudo apt install qtbase5-dev qtmultimedia5-dev libqt5svg5-dev  
sudo apt install libqt5core5a libqt5dbus5 libqt5gui5 libqt5multimedia5 libqt5svg5 libqt5xml5 libqt5network5
```

Clone this repository

    git clone https://github.com/Audio4Linux/Viper4Linux-GUI

Compile sources

    cd Viper4Linux-GUI
    qmake V4L_Frontend.pro
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
sudo cat <<EOT >> /usr/share/applications/viper-gui.desktop
[Desktop Entry]
Name=Viper4Linux
GenericName=Equalizer
Comment=User Interface for Viper4Linux
Keywords=equalizer
Categories=AudioVideo;Audio;
Exec=viper-gui
Icon=viper-gui
StartupNotify=false
Terminal=false
Type=Application
EOT
```
##### Download Icon
```bash
sudo wget -O /usr/share/pixmaps/viper-gui.png https://raw.githubusercontent.com/Audio4Linux/Viper4Linux-GUI/master/viper.png -q --show-progress
```

## Screenshots
![GIF](/screenshots/mainwindow.gif)
![GIF](/screenshots/eq.gif)

## Credits
* [Chinese translation by Hope2333](https://github.com/Hope2333)
* [Material Icons](https://material.io/tools/icons/)
* [QSS Stylesheets by GTRONICK (modified by @ThePBone)](https://github.com/GTRONICK/QSS)
* [Viper4Linux by noahbliss](https://github.com/noahbliss)

### Packages
* AUR packages managed by [yochananmarqos](https://github.com/yochananmarqos)
