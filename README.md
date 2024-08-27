
# HXO-loader

A shared object (.so) auto-injector, particularly made for game hacking/modding in mind.
 


## Features

- Automatically injects target shared object modules.
- Tweakable settings for effective module injection.
- Works on every linux system.

## Installation

#### 1. First of all, install the required dependencies.

*for Debian based systems*:
```bash
  sudo apt install gcc binutils make patchelf
```
**or, if you are in Arch based systems*:
```bash
  sudo pacman -S gcc binutils make patchelf
```
#### 2. Clone this repo.
```bash
  git clone https://github.com/bitwaree/HXO-loader.git
  cd HXO-loader
```
#### 3. Build and install the library.
```
  make
  sudo make install
```

## Local Application/Game setup
#### 1. Goto the directory you have your target game executable(s).

```
  cd <path to elf directory>
```

#### 2. Copy `HXO.ini` to that directory. 
or, re-download using
```bash
  wget https://raw.githubusercontent.com/bitwaree/HXO-loader/master/HXO.ini
```
NOTE: if this file is not copied, HXO-loader will proceed with default settings. It's always recomanded (but not mandatory) to copy this file to the directory.
#### 3. Create a directory and place your .hxo hacks.
```bash
  mkdir scripts
```
NOTE: `scripts` is the default folder name, but it can be changed by modifying `HXO.ini`
```
  cp <module1.so> <module2.so> <module3.so> ./scripts/
```
Now make sure your modules have a `.hxo` filename extention,(if not: do it manually) to make it recognisable.
```
#  So it will look like ...
#./scripts/module1.so -> ./scripts/module1.hxo 
#./scripts/module2.so -> ./scripts/module2.hxo 
#./scripts/module3.so -> ./scripts/module3.hxo 
```
#### 4. Finally run the game using `hxo-run` or `hxo-patch`
 - For a test/temporary run (recommended for testing):
```bash
  hxo-run ./<elf-executable>
```
- or, patch the elf to load hxo automatically (recommended for finalizing/distributing):
```bash
  hxo-patch <elf-executable>
```
#### *So the tree should look something like this:*
```tree
Game-Dir
├── game.elf
├── game.elf.bak
├── HXO.ini
├── scripts
│   ├── hack1.hxo
│   └── hack2.hxo
└──...
```
- NOTE: Patching will change the target file, and it's going to create a backup file with a .bak extention. To remove/revert the patch just run:
```bash
  hxo-rmpatch <patched-elf-executable>
```

## Acknowledgements

 - [INIh](https://github.com/benhoyt/inih), an awesome INI file parser by Ben Hoyt, used in this project for configuration parsing.
 - This entire project is distributed under the terms of [GPL-v3](https://www.gnu.org/licenses/quick-guide-gplv3.html).
 


## Terms of Distribution
Please read [DISTRIBUTION.md](https://github.com/bitwaree/HXO-loader/blob/master/DISTRIBUTION.md) for additional info.
