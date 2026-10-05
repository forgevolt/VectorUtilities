// ---- Rotation3D -------------------------------------------------------------------------
// Rotating, projecting and measuring angles in 3D - the building blocks of kinematics,
// such as turning a robot's foot positions with its body.
//
// Works on any board; open the Serial Monitor at 115200 baud.

#include <VectorUtilities.h>

using namespace vectorutilities;

void printLine(const char* label, const Vector3& v)
{
  Serial.print(label);
  v.printTo(Serial);
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  const Vector3 foot(150.0f, 0.0f, -80.0f);   // a foot position in mm, body at the origin

  // ---- Rotating about the main axes (right-hand rule: counterclockwise seen from the tip)
  Serial.println("---- Turning the body");
  printLine("foot                  = ", foot);
  printLine("yaw 30 deg (about z)  = ", rotateAroundZ(foot, 30.0f * deg2rad));
  printLine("roll 10 deg (about x) = ", rotateAroundX(foot, 10.0f * deg2rad));
  printLine("roll, pitch, yaw      = ",
            rotateXYZ(foot, Vector3(10.0f, 5.0f, 30.0f) * deg2rad));

  // ---- Rotating about any axis: here the diagonal (1, 1, 1), by 120°.
  // A third of a turn about the diagonal swaps the axes round: x -> y -> z -> x.
  Serial.println("---- Any axis");
  const Vector3 diagonal(1.0f, 1.0f, 1.0f);
  printLine("x about (1,1,1), 120 = ",
            Vector3(1.0f, 0.0f, 0.0f).rotateAroundAxis(diagonal, 120.0f * deg2rad));

  // ---- Splitting a vector: the part along a direction, and the rest
  Serial.println("---- Project and reject");
  const Vector3 ground(1.0f, 1.0f, 0.0f);   // a direction on the ground
  printLine("along the direction   = ", foot.project(ground));
  printLine("the rest              = ", foot.reject(ground));

  // ---- Angles and a plane's normal
  Serial.println("---- Angles");
  const Vector3 down(0.0f, 0.0f, -1.0f);
  Serial.print("leg angle to vertical  = ");
  Serial.print(foot.angleTo(down) * rad2deg);
  Serial.println(" deg");

  const Vector3 a(100.0f, 0.0f, 0.0f);
  const Vector3 b(0.0f, 100.0f, 5.0f);
  printLine("normal of plane (a, b) = ", a.cross(b).normalize());
}

void loop()
{
}
