# VectorUtilities

2D and 3D vectors and smooth motion curves for robots, games and animation.

Most projects that move something need the same few things: a vector type with proper
operators, a way to rotate and measure it, and curves that get from A to B without a jerk.
VectorUtilities is that, in one header:

- **`Vector2` and `Vector3`** with the usual operators (`a + b`, `2.0f * v`, `v /= 3.0f`), dot and
  cross products, length and distance, normalization, clamping, rotation (about the main axes
  or any axis), projection, reflection, angles, and `moveTowards()`.
- **Motion curves** between two points: `lerp`, `smoothstep`, minimum jerk (`minjerk`),
  quadratic and cubic Bézier.
- **Scalar helpers** for the same jobs: `clampf`, `lerpf`, `mapf`, `wrap`, `wrapPi`,
  `smoothstep`, `smootherstep`, and a frame-rate independent `lowPassFilter`.

It is float only, as befits microcontrollers without a hardware double. The constructors are
`constexpr`, so a constant table of vectors is built by the compiler and costs no RAM. Nothing
allocates memory except `toString()`; use `printTo()` to print without it.

The vector code is derived from [raylib](https://github.com/raysan5/raylib)'s raymath, ported
to C++ classes and extended.

## Requirements

- Any Arduino board. Tested on **ESP32** and **ESP32-S3** with Arduino-ESP32 core **3.3.12** and
  **2.0.17**, and on a PC. The code is plain C++11 with `<math.h>`, so other 32-bit boards
  (RP2040, SAMD, STM32, Renesas, …) should work too. Classic 8-bit AVR boards are not tested.
- C++11 or later. The library does not need C++17, which is why it also builds on older cores
  such as Arduino-ESP32 2.x.

No other library is needed.

## Installing

**Library Manager**: in the Arduino IDE, *Sketch → Include Library → Manage Libraries*, search for
`VectorUtilities`, install.

**From this repository**: *Code → Download ZIP*, then *Sketch → Include Library → Add .ZIP
Library*. Use this if you want a version that has not been released yet.

## Using it

```cpp
#include <VectorUtilities.h>

using namespace vectorutilities;   // or write vectorutilities::Vector3 etc.

constexpr Vector3 cStart(0.0f, 0.0f, 0.0f);
constexpr Vector3 cEnd(100.0f, 0.0f, 50.0f);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  const float u = (millis() % 2000) / 2000.0f;    // 0 → 1 every two seconds

  const Vector3 position = minjerk(cStart, cEnd, u);
  position.printTo(Serial);                       // "x, y, z"
  Serial.println();

  delay(20);
}
```

Everything lives in the namespace `vectorutilities`, so the short names (`Vector2`, `lerp`,
`wrap`, …) cannot clash with another library. `using namespace vectorutilities;` brings them back
into your sketch.

## What is in it

### Vector2 and Vector3

| | `Vector2` | `Vector3` |
| --- | --- | --- |
| Components | `x`, `y` | `x`, `y`, `z` |
| Operators | `+ -` with a vector; `* /` with a vector (per component) or a float; the compound forms `+= -= *= /=`; unary `-`; `float * v`; `==`, `!=` | the same, plus `+ -` with a `Vector2` (z unchanged) |
| Length | `length()`, `lengthSqr()`, `distance()`, `distanceSqr()` | the same |
| Direction | `normalize()`, `dot()`, `cross()` (a float), `angle()`, `angleTo()` (signed) | `normalize()`, `dot()`, `cross()`, `angleTo()` (0 to π) |
| Rotation | `rotate(angle)`, `perpendicular()` | `rotateAroundAxis(axis, angle)`, `perpendicular()`; free functions `rotateAroundX/Y/Z()`, `rotateXYZ()` |
| Other | `reflect()`, `clamp()`, `minimum()`, `maximum()`, `moveTowards()` | the same, plus `project()`, `reject()` |
| Output | `toString()`, `printTo(Print&)` | the same |

Angles are in radians. Rotations follow the right-hand rule: counterclockwise when seen from
the tip of the axis. `deg2rad` and `rad2deg` convert.

### Motion curves

All take `u` from 0 (start) to 1 (end) and clamp it, so a timer that overshoots is harmless.

| Function | Speed profile | Typical use |
| --- | --- | --- |
| `lerp(a, b, u)` | constant | straight moves, blending |
| `smoothstep(a, b, u)` | zero speed at both ends | UI and camera moves (`Vector3`; also as a float function) |
| `minjerk(a, b, u)` | zero speed and acceleration at both ends | servo and robot joint moves (`Vector3`) |
| `bezier2(p0, p1, p2, u)` | along a curve through one control point | 2D paths |
| `bezier3(p0, p1, p2, p3, u)` | along a curve through two control points | foot trajectories, 3D paths |

### Scalars and constants

`clampf`, `lerpf`, `mapf` (a float `map()` that cannot divide by zero), `normalize`, `wrap`,
`wrapPi`, `smoothstep`, `smootherstep`, `lowPassFilter`, `floatEquals`, `sqr`,
`circularNormalization` (maps square joystick travel onto the unit disk), and the
constants `cPI`, `cTwoPI`, `cEPSILON`, `deg2rad`, `rad2deg`.

## Examples

`BasicVectors`: the everyday operations on 2D and 3D vectors, printed to the Serial Monitor.

`MotionCurves`: the four ways from A to B side by side in the Serial Plotter.

`Rotation3D`: rotating a robot's foot position with its body, rotation about any axis,
projection and angles.

`SmoothInput`: taming a jumpy joystick with `lowPassFilter()` and `moveTowards()`. The input is
simulated, so it runs on a bare board.

All examples work on any board without extra hardware.

## Good to know

**`==` is approximate.** Two vectors are equal when each component matches within a small
relative tolerance (`floatEquals()`, `cEPSILON`), so rounding errors from rotations and
normalizations do not make equal vectors unequal. Compare components yourself if you need an
exact match.

**`normalize()` of a zero vector** returns the zero vector rather than dividing by zero. In the
same spirit, `mapf`, `normalize` and `wrap` return a defined value for an empty range, and
`project()` onto a zero vector gives zero.

**`toString()` allocates** an Arduino `String`. In a fast loop, prefer `printTo(Serial)`.

## Testing

The library builds and runs on a PC against a small stand-in for the Arduino core:

```bash
make -C test run examples              # C++17
make -C test clean run examples STD=c++11
```

`run` executes the checks in `test/test_main.cpp`. `examples` compiles every example and runs
it for a few simulated seconds. Both build with `-Wall -Wextra -Wdouble-promotion
-Wsign-compare -Wshadow -Werror`.

## Credits

The vector code is derived from raymath, part of [raylib](https://github.com/raysan5/raylib)
by Ramon Santamaria. VectorUtilities was developed with the help of Claude (Anthropic): the code
review, the new vector operations, the examples and the tests.

## License

MIT, see [LICENSE](LICENSE). The parts derived from raylib's raymath remain under its zlib
license, whose notice is reproduced in [LICENSE](LICENSE) as that license requires.
