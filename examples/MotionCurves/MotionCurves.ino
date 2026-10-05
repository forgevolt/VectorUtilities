// ---- MotionCurves -----------------------------------------------------------------------
// Moves a point from A to B in four ways and prints the height of each, so the curves can
// be compared in the Serial Plotter (Tools > Serial Plotter, 115200 baud).
//
//   lerp        constant speed, abrupt start and stop
//   smoothstep  eases in and out (zero speed at both ends)
//   minjerk     minimum jerk: also zero acceleration at both ends - smooth servo moves
//   bezier3     follows a curve through two control points, e.g. a foot lifting over a step

#include <VectorUtilities.h>

using namespace vectorutilities;

constexpr Vector3 cStart(0.0f, 0.0f, 0.0f);
constexpr Vector3 cEnd(100.0f, 0.0f, 50.0f);

// Control points of the Bézier curve: rise steeply, then come down onto the end point
constexpr Vector3 cControl1(0.0f, 0.0f, 120.0f);
constexpr Vector3 cControl2(100.0f, 0.0f, 120.0f);

constexpr unsigned long cMoveDurationMs = 2000;
constexpr unsigned long cPauseMs        = 500;
constexpr unsigned long cSampleMs       = 20;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  static unsigned long moveStart = millis();

  const unsigned long elapsed = millis() - moveStart;
  if (elapsed > cMoveDurationMs + cPauseMs)
  {
    moveStart = millis();   // start the next move
    return;
  }

  // u runs from 0 to 1 over the move; the curves clamp it, so it stays at 1 in the pause
  const float u = static_cast<float>(elapsed) / static_cast<float>(cMoveDurationMs);

  Serial.print("lerp:");
  Serial.print(lerp(cStart, cEnd, u).z);
  Serial.print(" smoothstep:");
  Serial.print(smoothstep(cStart, cEnd, u).z);
  Serial.print(" minjerk:");
  Serial.print(minjerk(cStart, cEnd, u).z);
  Serial.print(" bezier3:");
  Serial.println(bezier3(cStart, cControl1, cControl2, cEnd, u).z);

  delay(cSampleMs);
}
