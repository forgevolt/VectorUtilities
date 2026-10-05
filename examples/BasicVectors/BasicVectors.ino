// ---- BasicVectors -----------------------------------------------------------------------
// The everyday operations on 2D and 3D vectors, printed to the Serial Monitor.
//
// Works on any board; open the Serial Monitor at 115200 baud.

#include <VectorUtilities.h>

using namespace vectorutilities;   // or write vectorutilities::Vector2 etc.

// Built at compile time, so a table like this costs no RAM at startup.
constexpr Vector2 cCorners[] = { Vector2(0.0f, 0.0f), Vector2(4.0f, 0.0f),
                                 Vector2(4.0f, 3.0f), Vector2(0.0f, 3.0f) };

void printLine(const char* label, const Vector2& v)
{
  Serial.print(label);
  v.printTo(Serial);   // prints "x, y" without creating a String
  Serial.println();
}

void printLine(const char* label, const Vector3& v)
{
  Serial.print(label);
  v.printTo(Serial);
  Serial.println();
}

void printLine(const char* label, float value)
{
  Serial.print(label);
  Serial.println(value, 3);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);   // give the Serial Monitor a moment; does not wait for ever

  // ---- 2D
  const Vector2 a(3.0f, 4.0f);
  const Vector2 b(1.0f, 0.0f);

  Serial.println("---- Vector2");
  printLine("a              = ", a);
  printLine("b              = ", b);
  printLine("a + b          = ", a + b);
  printLine("2 * a          = ", 2.0f * a);
  printLine("|a|            = ", a.length());
  printLine("a normalized   = ", a.normalize());
  printLine("a . b          = ", a.dot(b));
  printLine("angle a to b   = ", a.angleTo(b) * rad2deg);   // degrees, signed
  printLine("b rotated 90deg= ", b.rotate(90.0f * deg2rad));
  printLine("distance       = ", a.distance(b));

  float perimeter = 0.0f;
  const size_t count = sizeof(cCorners) / sizeof(cCorners[0]);
  for (size_t i = 0; i < count; ++i)
    perimeter += cCorners[i].distance(cCorners[(i + 1) % count]);
  printLine("perimeter      = ", perimeter);

  // ---- 3D
  const Vector3 x(1.0f, 0.0f, 0.0f);
  const Vector3 y(0.0f, 1.0f, 0.0f);
  const Vector3 v(1.0f, 2.0f, 2.0f);

  Serial.println("---- Vector3");
  printLine("v              = ", v);
  printLine("|v|            = ", v.length());
  printLine("x cross y      = ", x.cross(y));
  printLine("v . x          = ", v.dot(x));
  printLine("angle v to x   = ", v.angleTo(x) * rad2deg);
  printLine("v clamped      = ", v.clamp(0.0f, 1.5f));

  // == compares with a small tolerance, so rounding errors do not matter
  const Vector3 roundTrip = v.normalize() * v.length();
  Serial.print("normalize * length == v: ");
  Serial.println(roundTrip == v ? "yes" : "no");
}

void loop()
{
}
