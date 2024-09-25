# HXO-loader for Android - A Detailed Guide

This page documents all the things you need to know to create HXO-loaded apks.


## What's the difference?

- Android uses different approach when it comes about process handling. So, using techniques like `LD_PRELOAD` is almost impossible.
- For android no general script to do the work for you must have a little idea about what you are doing if you are the modder.
- Android applications need explicit permission to access the internal storage, so HXO stores all the required files at `/storage/emulated/0/Android/media/<APP_ID>/` directory. _(Unlike other versions, there is no elf executable directory to place the `HXO.ini` and `scripts` folder, so hxo uses the above path for basically everything.)_
- As android doesn't have a typical console, for debugging HXO-loader redirects `stdout` and `stderr` handles into a log file (`hxo_log.txt`). So one can open the log file after a run to understand if something went wrong with the `.hxo` hacks *(or, in worst case scenario with HXO-loader itself >_< )*.
- On Android it's impossible to install HXO-loader as a package, so HXO-loader must be embedded inside the apk itself.
- Android doesn't extract native libraries which don't have a `"lib"` prefix, so unlike other systems android's hxo has a name `"libhxo.so"` instead of `"hxo_loader.so"`. _(The name `"libhxo.so"` might get standardized for all platforms in future.)_

Now as we got the basic difference covered, next we will try to understand the basic idea behind it.


## How does it work?

#### In an **APK** package there are:
1. Compiled Java Dex files, 
3. ___*Native Libs___,
3. Resources.

And we are interested in the __*Native Libs__ here. Those can be found at `<package.apk>/lib/<arch>/...` _(<arch> is the corresponding architecture.)_

#### The Idea:
We are going to choose a library inside of the _native lib directory_ and add a dependency to `libhxo.so` using `patchelf`. Next, we are going to put a copy of `"libhxo.so"` in that directory. This way when we run the app the library will tell the linker to load the `libhxo.so` first and HXO-loader will be initialized.

## Step by Step guide of modding:

### Step 0: _Setup your system and the tools_

#### 1. First of all, install the required tools.
_(You can mod directly from android too using [Termux](https://termux.dev/en/), but in this tutorial we will pretend if we are in a linux machine)_
```bash
  sudo apt install patchelf aapt apksigner openjdk-17 curl
```
#### 2. Download apktool:
```bash
  curl -o apktool.jar https://github.com/iBotPeaches/Apktool/releases/download/v2.9.3/apktool_2.9.3.jar
```
and, setup a command alias for easily accessing it:
```bash
  alias apktool="java -jar $(pwd)/apktool.jar"
```
NOTE: command alias(s) are not permanent, so you have to execute the last command everytime you want to use apktool.

### Step 1: Unpack the apk you want:
- While unpacking you can use two techniques: 
      1. apktool (_recommended_),
      2. Normal `zip` archiver.
- If you use apktool to unpack, while repacking you must use apktool, and the same way around for zip.
 For unpacking use `apktool d <package.apk>` or `mkdir unpacked && unzip -d unpacked/ <package.apk>`

_Replace "<package.apk>" with the apk name._
Now apktool should create a directory in which the files will be extracted, or in case of zip it should extract the package in the `"unpacked"` folder.

Now cd in the directory:
```bash
  cd unpacked/
```

### Step 2: Locate lib folder
Generally, it should be at the parent of the directory tree. Now cd in the `lib/<arch>`. Replace <arch> with `arm64-v8a` for aarch64, `armeabi-v7a` for 32-bit arm, etc.
```bash
  cd lib/<arch>/
``` 
NOTE: It's generally recommended that one should do it manually for all of the architecture one by one. 

### Step 3: Find the correct lib and patch it
There should be one or more than one file. If more than one file you have to use some of your reverse engineering skills to find which one of the file contain major game logic. But to be honest the largest file is always a safe bet. To find the largest file run:
```bash
  ls -S | head -n 1
```
Now the output file should be the largest file in size. Now to patch it run:
```bash
  patchelf --add-needed libhxo.so <the-chossen-file.so>
```

### Step 4: Copy `libhxo.so` in the directory
You can try building the HXO-loader-android from source using [Android-ndk](https://developer.android.com/ndk/downloads). But it's recommended to use the already pre-built binaries for this job. Head over to [releases](https://github.com/bitwaree/HXO-loader/releases)  and download the latest android zip package.

Extract the zip, and copy the correct `libhxo.so` as per your directory architecture.

### Step 5: Repack the package
As previously mentioned, it's always recommended to perform the step [3](#step-3-find-the-correct-lib-and-patch-it) and [4](#step-4-copy-libhxo.so-in-the-directory) for all of the available architectures. Once done we can proceed to the repacking step.

Come back to your parent workspace directory with `cd ../../..`. 

If apktool was used to unpack the apk run:
```bash
  apktool b <directory> -o <package-hxo-loaded.apk>
```
or, in case of `zip`:
```bash
  cd <directory>
  zip ../<package-hxo-loaded.apk> *
  cd ../
```
Replace `<package-hxo-loaded.apk>` with a new apk filename.

### Step 6: Zipalign and Signing
`zipalign` comes with _aapt_ package. And it's mandatory for the important optimization of zip archives.
Use the following command to perform the zipallign:
```bash
  zipalign -v 4 <package-hxo-loaded.apk> <package-hxo-loaded-alligned.apk>
```

Now the only step left is to sign the apk. This step can be skipped and can be done on android via some `apk-signing` tools. There are plenty of tools on Play Store. But if you want to do it:
```bash
  keytool -genkeypair -v -keystore ./test-debug.keystore -keyalg rsa -sigalg SHA256withRSA -alias test-debug -keysize 2048  -validity 10000
  apksigner sign --ks ./test-debug.keystore --ks-key-alias test-debug <package-hxo-loaded-alligned.apk>
```

That's it... It's that simple. It might seem complicated but once you understand it, it's quite easy. ^.^



## For XAPK/APKM files (split apks)
For split apks, the same process should be followed with some additional steps:

1. Unpack the xapk/apkm using `mkdir unpacked_splits && unzip -d unpacked_splits <package.xapk>`. This should extract the files in a folder named `"unpacked_splits"`.

2. Locate the split apks containing the libs. This should be named something like `"split_config.<arch>.apk"` or `"config.<arch>.apk"`, now for each architecture specific split apks, follow the [Modding Steps](#step-by-step-guide-of-modding) shown above.

3. Sign all of the apks with the same keystore:
```bash
  apksigner sign --ks ./test-debug.keystore --ks-key-alias test-debug <base.apk>
```
_Execute the previous command for each and every split apks one by one._

4. After completing the modding and signing steps, the modded apks must have to be renamed into their corresponding original apk names. And any additional files and folder must be cleaned up.

5. Repack the xapk/apkm file with `zip ../<package-hxo-loaded.xapk> *`. This should create the modded xapk/apkm file in the parent folder.

6. Finally the xapk/apkm file is installed and tested. [ZArchiver](https://play.google.com/store/apps/details?id=ru.zdevs.zarchiver) and [SAI](https://f-droid.org/en/packages/com.aefyr.sai.fdroid/) are two great tools for installing the split apk packages.


## _Post_ Installation setup
These steps should be followed by the installer of the apk. You can use any file manager for this job, if you have ZArchiver, it will be enough. All the hxo files should be placed at `/storage/emulated/0/Android/media/<APP_ID>`

#### 1. Goto the target media folder `Android/media`.

#### 2. Find the directory as of your app id. If you can't find one, you can create it yourself, or just run the hxo-loaded app once then close the app. HXO-loader will start the log and the folder will be automatically created. _(For example `ru.zdevs.zarchiver` for ZArchiver)_

#### 3. Copy [HXO.ini](https://raw.githubusercontent.com/bitwaree/HXO-loader/master/HXO.ini) into that directory.

#### 4. Create a directory to place your `.hxo` hacks.
```bash
  mkdir scripts
```
NOTE: `scripts` is the default folder name, but it can be changed by modifying `HXO.ini`

#### 5: Now copy your hxo modules _(`.hxo` filename extension)_ in the scripts directory.

#### *So the tree should look something like this:*
```tree
/storage/emulated/0/Android/media/<APP_ID>
├── HXO.ini
├── scripts
│   ├── hack1.hxo
│   ├── hack2.hxo
│   └── ***.hxo
└──...
```

## Distribution of modded apks.
After completing the modding and tests. The modded apk might be wanted to be distributed. In that case, there are few terms to consider.

 Please read [DISTRIBUTION.md](https://github.com/bitwaree/HXO-loader/blob/master/DISTRIBUTION.md).
