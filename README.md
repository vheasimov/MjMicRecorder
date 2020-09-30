MjMicRecorder
===================

A Simple Application for recording from default input device (microphone)

## Building

### VCPKG 
```
git clone --recursive https://github.com/Microsoft/vcpkg.git
bootstrap-vcpkg.bat(sh)
vcpkg integrate install
```

### Packages
```
vcpkg install mp3lame:x64-windows
vcpkg install qt5:x64-windows
```

### Build
```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg_dir>/scripts/buildsystems/vcpkg.cmake ../
cmake --build .

```

### Everything Else 
Everything else is untested. Submissions welcome.