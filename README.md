<p align="center">
  <img src="https://github.com/bncmn/beagle-dashcam/blob/cf9881b7169a984957e73002f7d492fd23a7cf8b/bdc-title.png"/>
</p>

# BeagleDashCam
BeagleDashCam is a full-stack vehicle dashcam system developed on the BeagleBone Green. This was developed as a term project for CMPT433 (Embedded Systems).

## Features
- Continuous recording
  - The system stores ten 1-minute-long video clips into a buffer (on the on-board eMMC).
  - The oldest clip is deleted once the buffer is filled.
- Event recording
  - Accelerometer events (via I2C)
  - Motion events using a motion detector (connected via A2D)
  - Manual button-press trigger (connected through GPIO)
- Geo-tagging
  - Saved clips are tagged with latitude and longtitude sourced from a GPS module (connected via UART)

# Live Demo Videos
Continuous Recording + Button Press Trigger (links to YouTube)

[![](http://img.youtube.com/vi/MP1Bxf3zbGY/0.jpg)](http://www.youtube.com/watch?v=MP1Bxf3zbGY)

Motion Sensor Trigger (links to YouTube)

[![](http://img.youtube.com/vi/pixrTxWHzdQ/0.jpg)](http://www.youtube.com/watch?v=pixrTxWHzdQ)


## Acknowledgements
This project was developped by the BeagleDashCam team:
- Elaine Abraham
- Diego Buencamino
- Kai Feng (Kevin) Chen
- Ngan Ngoc (Lynn) Nguyen
  
The original project repository is linked [here](https://github.com/lynn9101/beagle-dashcam).
