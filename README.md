# BEAGLEBONE DASHCAM
## Team Members
- Elaine Abraham
- Diego Buencamino
- Kai Feng (Kevin) Chen
- Ngan Ngoc (Lynn) Nguyen

## Project Description
- Building a dashcam with continuous recording capabilities, motion detection capabilities,
and accident detection (via the accelerometer). 
    - Saved clips will also be geotagged with a GPS component.
    - The dashcam saves the video for around 30 seconds before and after it detects a collision, or if the user presses a button.

- Using a webcam as the video input stream, a microphone to record audio, a motion sensor
to detect movement outside the vehicle while the vehicle is parked, and an SD card to store clips. 
    - The device should start up and start recording as soon as it gets power.

## Initial Setup
1. Install cross-compiler for g++: 

```sudo apt install g++-arm-linux-gnueabihf```