# Viper4Linux-GUI
[![Build Status](https://travis-ci.org/ThePBone/Viper4Linux-GUI.svg?branch=master)](https://travis-ci.org/ThePBone/Viper4Linux-GUI) ![GitHub](https://img.shields.io/github/license/ThePBone/Viper4Linux-GUI) ![Codacy branch grade](https://img.shields.io/codacy/grade/cb83ce5a7deb4130bd4b918d9e19a925/master) ![GitHub release](https://img.shields.io/github/release/ThePBone/Viper4Linux-GUI)

Official UI for Viper4Linux [https://github.com/noahbliss/Viper4Linux](https://github.com/noahbliss/Viper4Linux)
* Telegram: @ThePBone
* [Stable Builds](https://github.com/ThePBone/Viper4Linux-GUI/releases)
* [Nightly Builds](https://nightly.thebone.cf/viper-gui/?C=M;O=D)

## Usage
### Features
* Change V4L properties on the fly
* Apply automatically (Save and reload V4L after performing changes)
* Convolver Selection
* UI-Theming/Stylesheets/Color Palettes
* Import/Export Android Configs

## Installation
* [Ubuntu PPA](#ubuntu-ppa)
* [Arch](#arch)
* [Debian](#debian)
* [Debian/Ubuntu](#debianubuntu)
* [Portable/Manually](#portablemanually)
### Ubuntu PPA
Add PPA Repo
```bash
curl -s --compressed "https://thepbone.github.io/Viper4Linux-GUI-PPA/KEY.gpg" | sudo apt-key add -
sudo curl -s --compressed -o /etc/apt/sources.list.d/vipergui.list "https://thepbone.github.io/Viper4Linux-GUI-PPA/vipergui.list"
sudo apt update
```
Install from PPA
```bash
sudo apt install viper4linux-gui
```
[View PPA on GitHub](https://github.com/ThePBone/Viper4Linux-GUI-PPA)

### Arch

On Arch you also can use the [AUR package](https://aur.archlinux.org/packages/viper4linux-gui-git/) (using git) provided by yochananmarqos:
```bash
yay -S viper4linux-gui-git  
```

Or [this one](https://aur.archlinux.org/packages/viper4linux-gui) to get the latest stable release:
```bash
yay -S viper4linux-gui
```
![AUR version](https://img.shields.io/aur/version/viper4linux-gui?label=aur%20%28stable%29) ![AUR version](https://img.shields.io/aur/version/viper4linux-gui-git?label=aur%20%28git%29)

### Debian
You can find DEB packages [here](https://github.com/ThePBone/Viper4Linux-GUI/releases).
```bash
sudo dpkg -i <..deb-filename..>
```

### Debian/Ubuntu
#### Installation (script)
To simplify the installation on Debian, I have added an automatic install script.
You can also use it to update to a new stable release (if available).
If you prefer using apt, read the PPA section above.
```bash
curl -s https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/scripts/install-debian.sh | sudo bash
```
#### Uninstallation (script)
You can use this uninstall script if you want to remove the GUI (assuming you have installed it with the above automatic script)
```bash
curl -s https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/scripts/uninstall-debian.sh | sudo bash
```

### Portable/Manually
#### Install Dependencies
You will need to change this line, depending on which package manager your distribution uses.
```bash
sudo apt install libqt5widgets5 libqt5gui5 libqt5core5a libqt5xml5 libgl1-mesa-dev git
```

#### Start Executable
```bash
chmod +x viper-gui
./viper-gui
```

#### Optional: Manual Install
##### Copy to /usr/local/bin
```bash
sudo cp viper-gui /usr/local/bin
sudo chmod 755 /usr/local/bin/viper-gui
```
##### Create Menu Entry
```bash
sudo cat <<EOT >> /usr/share/applications/viper-gui.desktop
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
```
##### Download Icon
```bash
sudo wget -O /usr/share/pixmaps/viper-gui.png https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/viper.png -q --show-progress
```

## Screenshots
![Screenshot](https://github.com/ThePBone/Viper4Linux-GUI/blob/master/screenshots/tab1.png?raw=true)
![Screenshot](https://github.com/ThePBone/Viper4Linux-GUI/blob/master/screenshots/tab3_dark.png?raw=true)
![GIF](https://github.com/ThePBone/Viper4Linux-GUI/blob/master/screenshots/contexthelp.gif?raw=true)
## Credits
* [Material Icons](https://material.io/tools/icons/)
* [QSS Stylesheets - GTRONICK (modified by ThePBone)](https://github.com/GTRONICK/QSS)
* [Viper4Linux - noahbliss](https://github.com/noahbliss/Viper4Linux)

Thanks @noahbliss for declaring the GUI as official :)
