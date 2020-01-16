# Viper4Linux-GUI
[![Build Status](https://travis-ci.org/Audio4Linux/Viper4Linux-GUI.svg?branch=master)](https://travis-ci.org/ThePBone/Viper4Linux-GUI) ![GitHub](https://img.shields.io/github/license/Audio4Linux/Viper4Linux-GUI) ![GitHub release](https://img.shields.io/github/release/Audio4Linux/Viper4Linux-GUI)

Official UI for Viper4Linux2 [https://github.com/Audio4Linux/Viper4Linux](https://github.com/Audio4Linux/Viper4Linux)
* Telegram: @ThePBone

### This repo contains the GUI for a newer Viper4Linux version which is not yet finished. Please use [Viper4Linux-GUI Legacy](https://github.com/ThePBone/Viper4Linux-GUI) for now.

#### Build from sources
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
