# chipati
Chip8 interpreter in C++.

## Setup
```
conan install . --build=missing -s build_type=Debug
```

## Configure and Build
```
cmake --preset conan-debug
cmake --build --preset conan-debug
```

## Run
```
./build/Debug/chipati
```
