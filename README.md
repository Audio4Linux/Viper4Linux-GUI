# Viper4Linux-GUI
[![Build Status](https://travis-ci.org/ThePBone/Viper4Linux-GUI.svg?branch=master)](https://travis-ci.org/ThePBone/Viper4Linux-GUI)

Official UI for Viper4Linux (https://github.com/L3vi47h4N/Viper4Linux)
  * Telegram: @ThePBone
  * [Stable Builds](https://github.com/ThePBone/Viper4Linux-GUI/releases)
  * [Nightly Builds](https://nightly.thebone.cf/viper-gui/?C=M;O=D)
## Installation
### Debian/Ubuntu
To simplify the installation on Debian, I have added an automatic install script.
You can also use it to update to a new stable release (if available).
```
curl -s https://raw.githubusercontent.com/ThePBone/Viper4Linux-GUI/master/install.sh | sudo bash
```
### Arch
On Arch you also can use the [AUR package](https://aur.archlinux.org/packages/viper4linux-gui-git/) provided by yochananmarqos
```
yay -S viper4linux-gui-git  
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
