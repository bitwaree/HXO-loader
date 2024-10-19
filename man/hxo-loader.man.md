% HXO-loader(1) Commands and Usage Manual
% bitwaree <bitware@duck.com>
% September 2024

# NAME
hxo-loader -- Hybrid Xpl0it loader

# DESCRIPTION
Hybrid Xpl0it loader (HXO-loader) is an exclusive tool/framework for auto-injecting shared objects into target processes.

## UASBLE COMMANDS
- `hxo-run`      - runs the application without modifying files.
- `hxo-patch`    - patches the elf binary (executable/shared object).
- `hxo-rmpatch`  - revert patches.

# USAGE

## Local Application/Game setup
1. **Goto the directory you have your target game executable(s).**

    ```
    cd <path to elf directory>
    ```

2. **Copy `HXO.ini` to that directory.**

    or, re-download using:

      ```bash
      wget https://raw.githubusercontent.com/bitwaree/HXO-loader/master/HXO.ini
      ```

    NOTE: if this file is not copied, HXO-loader will proceed with default settings. It's always recomanded (but not mandatory) to copy this file to the directory.

3. **Create a directory and place your .hxo hacks.**

    ```bash
    mkdir modules
    ```
    NOTE: `"modules"` is the default folder name, but it can be changed by modifying `"HXO.ini"`

    ```
    cp <module1.hxo> <module2.hxo> <module3.hxo> ./modules/
    ```

    Make sure your modules have a `.hxo` filename extention,(if not: do it manually) to make it recognisable.

    ```
    #  So it will look like ...
    #./modules/module1.so -> ./modules/module1.hxo 
    #./modules/module2.so -> ./modules/module2.hxo 
    #./modules/module3.so -> ./modules/module3.hxo 
    ```

4. **Finally run the game using `hxo-run` or `hxo-patch`**
    - For a test/temporary run (recommended for testing):

        ```bash
        hxo-run ./<elf-executable>
        ```
    - or, patch the elf to load hxo automatically (recommended for finalizing/distributing):

        ```bash
        hxo-patch <elf-executable>
        ```

- ***So the tree should look something like this:***

    ```tree
      Game-Dir
      ├── game.elf
      ├── game.elf.bak
      ├── HXO.ini
      ├── modules
      │   ├── hack1.hxo
      │   └── hack2.hxo
      └──...
    ```


- NOTE: Patching will change the target file, and it's going to create a backup file with a .bak extention. To remove/revert the patch just run:

    ```bash
    hxo-rmpatch <patched-elf-executable>
    ```

# SEE ALSO

`hxo-devl(3)` - _to learn about programming and creating **.hxo** files_

# ACKNOWLEDGEMENTS

 - [INIh](https://github.com/benhoyt/inih), an awesome INI file parser by Ben Hoyt, used in this project for configuration parsing.
 - This entire project is distributed under the terms of [GPL-v3](https://www.gnu.org/licenses/quick-guide-gplv3.html).


# DISTRIBUTION
Please read https://github.com/bitwaree/HXO-loader/blob/master/DISTRIBUTION.md for additional info.
