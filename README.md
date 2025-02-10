# HA-Linux-Device
Adds any Linux client as a MQTT device in HA

## Install cmake
On Debian/Ubuntu
```
sudo apt update
sudo apt install cmake
```

On Arch
```
sudo pacman -S cmake
```

## Install Mosquitto library
For Debian/Ubuntu
```
sudo apt update
sudo apt install -y mosquitto mosquitto-clients libmosquitto-dev
```

For Arch
```
sudo pacman -S mosquitto
```
## Install the nlohmann-json library
On Debian/Ubuntu
```
sudo apt install nlohmann-json3-dev
```

On Arch
```
sudo pacman -S nlohmann-json
```

## Build the application
Generate the build system
```
mkdir build && cd build
cmake ..
```

Now compile
```
make
```

Now install
```
sudo make install
```