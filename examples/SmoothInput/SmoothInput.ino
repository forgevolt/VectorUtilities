// ---- SmoothInput ------------------------------------------------------------------------
// Taming a jumpy 2D input such as a joystick. The input here is simulated - it jumps to a
// new corner every two seconds - so the sketch runs on any board without wiring. Replace
// readInput() with your analogRead() calls.
//
// Watch it in the Serial Plotter (Tools > Serial Plotter, 115200 baud):
//   raw        the input as it arrives
//   filtered   lowPassFilter(): follows smoothly, like a mass with inertia
//   limited    moveTowards(): follows at a fixed maximum speed

#include <VectorUtilities.h>

using namespace vectorutilities;

constexpr float         cTauMs        = 300.0f;  // filter time constant; ~5 x tau to settle
constexpr float         cMaxSpeed     = 1.0f;    // full range (0 to 1) per second
constexpr unsigned long cSampleMs     = 20;
constexpr unsigned long cJumpPeriodMs = 2000;

// Simulated joystick in [-1, 1] on both axes
Vector2 readInput()
{
  static const Vector2 corners[] = { Vector2(1.0f, 1.0f),   Vector2(-1.0f, 1.0f),
                                     Vector2(-1.0f, -1.0f), Vector2(1.0f, -1.0f) };
  const unsigned long step = (millis() / cJumpPeriodMs) % 4;

  // Square stick travel reaches length 1.41 in the corners; map it onto the unit disk so
  // diagonals are not faster than straight moves.
  return circularNormalization(corners[step].x, corners[step].y);
}

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  static Vector2       filtered;
  static Vector2       limited;
  static unsigned long lastMs = millis();

  const unsigned long now = millis();
  const float         dtMs = static_cast<float>(now - lastMs);
  lastMs = now;

  const Vector2 raw = readInput();

  filtered.x = lowPassFilter(filtered.x, raw.x, cTauMs, dtMs);
  filtered.y = lowPassFilter(filtered.y, raw.y, cTauMs, dtMs);

  limited = limited.moveTowards(raw, cMaxSpeed * dtMs / 1000.0f);

  Serial.print("raw:");
  Serial.print(raw.x);
  Serial.print(" filtered:");
  Serial.print(filtered.x);
  Serial.print(" limited:");
  Serial.println(limited.x);

  delay(cSampleMs);
}
