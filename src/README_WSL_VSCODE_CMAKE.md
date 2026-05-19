# EFramework - WSL + VS Code + GCC/GDB + CMake

## Required packages in WSL Ubuntu

```bash
sudo apt update
sudo apt install build-essential gdb cmake make pkg-config libopencv-dev
```

## Recommended VS Code extensions

Install these extensions in the WSL remote environment:

- C/C++
- CMake Tools
- WSL

## Build

```bash
cmake --preset linux-debug
cmake --build --preset build-debug
```

## Run

```bash
./build/debug/eframework
```

## Debug

In VS Code:

1. Open this folder through WSL: `code .`
2. Select **Run and Debug**
3. Choose **Debug EFramework with GDB**

## Notes

- The executable target name is `eframework`.
- OpenCV is detected through `find_package(OpenCV REQUIRED)`.
- Source files are collected from `src/**/*.cpp`.
- `src` is added as an include directory because the code uses includes such as `<92TestApplication/...>`.
- If `cv::imshow()` or camera access is used, WSL GUI/camera support must be configured separately.
