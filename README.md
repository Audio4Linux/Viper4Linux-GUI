# Viper4Linux-GUI
[![Build Status](https://travis-ci.org/ThePBone/Viper4Linux-GUI.svg?branch=master)](https://travis-ci.org/ThePBone/Viper4Linux-GUI)

Official UI for Viper4Linux (https://github.com/L3vi47h4N/Viper4Linux)
  * Telegram: @ThePBone
  * [Stable Builds](https://github.com/ThePBone/Viper4Linux-GUI/releases)
  * [Nightly Builds](https://nightly.thebone.cf/viper-gui/?C=M;O=D)
## Installation
### Ubuntu PPA
#### Add PPA Repo
```
curl -s --compressed "https://thepbone.github.io/Viper4Linux-GUI-PPA/KEY.gpg" | sudo apt-key add -
sudo curl -s --compressed -o /etc/apt/sources.list.d/vipergui.list "https://thepbone.github.io/Viper4Linux-GUI-PPA/vipergui.list"
sudo apt update
```
#### Installation
```
sudo apt install viper4linux-gui
```

### Debian/Ubuntu
To simplify the installation on Debian, I have added an automatic install script.
You can also use it to update to a new stable release (if available).
If you prefer using apt, read the PPA section above.
```
curl -s https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/install.sh | sudo bash
```
Starting with version 1.10 I will provide Deb-Packages [here](https://github.com/ThePBone/Viper4Linux-GUI/releases).
### Arch
On Arch you also can use the [AUR package](https://aur.archlinux.org/packages/viper4linux-gui-git/) (using git) provided by yochananmarqos:
```
yay -S viper4linux-gui-git  
```
Or [this one](https://aur.archlinux.org/packages/viper4linux-gui) to get the latest stable release:
```
yay -S viper4linux-gui 
```
### Manually
#### Install Dependencies

```
sudo apt install libqt5widgets5 libqt5gui5 libqt5core5a libqt5xml5 libgl1-mesa-dev git
```

#### Start Executable
```
chmod +x viper-gui
./viper-gui
```

## Usage
#### Features
  * Change V4L Config on the fly 
  * Auto-Apply (Save and reload V4L after performing changes)
  * Convolver Selection
  * UI-Theming/Stylesheets
  * Import Android Configs

#### Screenshots
![Screenshot](https://github.com/ThePBone/Viper4Linux-GUI/blob/master/screenshots/tab1.png?raw=true)
![Screenshot](https://github.com/ThePBone/Viper4Linux-GUI/blob/master/screenshots/tab3_dark.png?raw=true)
#### Credits
  * [Material Icons](https://material.io/tools/icons/)
  * [QSS Stylesheets - GTRONICK (modified by ThePBone)](https://github.com/GTRONICK/QSS)
  * [Viper4Linux - L3vi47h4N](https://github.com/L3vi47h4N/Viper4Linux)

Thanks @L3vi47h4N for declaring the GUI as official :)
