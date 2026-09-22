# AutoCar Sign-Aware Road Follower

Arduino sketch for an autonomous car that follows a road using left/right distance sensors and reacts to detected traffic signs. It displays status messages on a 16x2 RGB LCD and controls four motor channels through an H-bridge motor driver.

## Features

- Road following using left and right distance sensors
- Normal road-following speed and slow road-following speed
- Traffic sign detection via the `AutoCar` library
- LCD status display for detected signs
- Sign actions for Stop, Speed 60, Left, Right, and Speed 50
- Re-trigger prevention using software flags and timestamps
- Initialization of `AutoCar` modules:
  - MLP module
  - Edge detection module
  - Hough transform module
  - Driverless configuration

## Hardware Requirements

- Arduino board compatible with the `AutoCar` library
- AutoCar chassis / shield or compatible motor driver
- Four motor channels connected to Arduino pins
- RGB LCD compatible with `rgb_lcd.h`
- Left and right distance sensors supported by `AutoCar`
- Camera / sensor module for sign detection, as required by `AutoCar`

## Software Dependencies

- Arduino IDE
- `Wire.h` — built-in Arduino I2C library
- `AutoCar.h` — custom AutoCar control and sensor library
- `rgb_lcd.h` — RGB LCD library

## Pin Mapping

The sketch uses the following Arduino pins for motor control:

| Motor Channel | Direction Pin | PWM Pin |
|---|---:|---:|
| Channel 1 | 7 | 6 |
| Channel 2 | 8 | 9 |
| Channel 3 | 4 | 5 |
| Channel 4 | 2 | 10 |

The LCD and `AutoCar` modules use I2C and the library-managed sensor/camera interfaces.

## Sign Types and Actions

| Sign Type | LCD Text | Action |
|---:|---|---|
| 1 | `Stop` | Stops the car indefinitely using `while (true);` |
| 7 | `speed 60` | Goes straight for 2 seconds, then stops for 1 second |
| 8 | `Left` | Turns left for 2 seconds, then stops for 1 second |
| 9 | `Right` | Turns right for 2 seconds, then stops for 1 second |
| 15 | `speed 50` | Follows the road slowly for 3 seconds, then stops for 1 second |

## How It Works

1. `setup()` initializes the motor pins, Serial, LCD, detection flags, and `AutoCar` modules.
2. `loop()` repeatedly calls `car.probe()` to update sensor and sign data.
3. If a sign is detected, `car.getSignType()` is checked.
4. The matching case performs the required action and updates the LCD.
5. Software flags such as `Left_Detected`, `Right_Detected`, `Forward_Detected`, and `Life_Detected` prevent the same sign from being handled repeatedly within a short time window.
6. Road-following logic is provided in `Move_along_road()` and `Main_Programme()`, but in the current `loop()` the main behavior is sign handling.

## Function Overview

| Function | Purpose |
|---|---|
| `Walk_Along_Road_Slowly()` | Follows the road at slow speed using left/right distance difference |
| `Go_Straight()` | Drives forward at speed 100 |
| `Turn_left()` | Turns left |
| `Turn_right()` | Turns right |
| `Move_along_road()` | Normal road following using left/right distance sensors |
| `Stop()` | Stops all motors |
| `Temporary_storage()` | Earlier/unused sign-handling function |
| `Main_Programme()` | Probes sensors and calls `Move_along_road()` when a distance is detected |
| `setup()` | Arduino initialization |
| `loop()` | Main sign-detection and response loop |

## Getting Started

1. Wire the motor driver, LCD, and sensors according to your AutoCar hardware.
2. Install the required Arduino libraries: `AutoCar`, `rgb_lcd`, and any dependencies.
3. Open the sketch in the Arduino IDE.
4. Select the correct board and port.
5. Upload the sketch.
6. Open the Serial Monitor at `9600` baud if needed.
7. Place the car on the track and power it on.

## Known Issues / Notes

- `Main_Programme()` and `Move_along_road()` are defined but not called in `loop()`. For continuous road following, add a call to `Main_Programme()` in `loop()` when no sign is active.
- `Temporary_storage()` appears to be an older version and is unused.
- In the `Life_Detected` timeout block, `Left_Detected` is reset instead of `Life_Detected`. This is likely a bug.
- Sign type `1` uses `while (true);`, which blocks the program forever.
- The sketch uses blocking `delay()` calls, which can prevent sensor updates during delays.
- `lcd.setCursor(1, 2)` may be invalid for a 16x2 LCD. Rows are usually `0` and `1`.
- `if (car.signDetected() && true)` can be simplified to `if (car.signDetected())`.
- The distance threshold `43` is hard-coded and may need tuning for different tracks and sensors.
- Comments are mixed between English and Chinese.

## Possible Improvements

- Call `Main_Programme()` in `loop()` for continuous lane following.
- Replace blocking delays with a `millis()`-based state machine.
- Fix the `Life_Detected` flag reset bug.
- Add configurable constants for speeds and distance thresholds.
- Add Serial debug output for sensor values and detected signs.
- Replace `while (true);` with a controlled stop state.
- Improve LCD cursor usage for a 16x2 display.

## License

No license is specified in the project. Add a license if you plan to distribute or reuse the code.
