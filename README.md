# HA-Linux-Device
Adds any Linux client as a MQTT device in HA

## Install Mosquitto library
sudo apt update
sudo apt install -y mosquitto mosquitto-clients libmosquitto-dev

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