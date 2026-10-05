// Host-side tests for VectorUtilities. Build and run with:  make -C test
#include "VectorUtilities.h"

#include <cstdio>
#include <string>

using namespace vectorutilities;

namespace
{
int failures = 0;
int checks   = 0;

void check(bool ok, const char* what, int line)
{
  ++checks;
  if (!ok)
  {
    ++failures;
    printf("FAIL line %d: %s\n", line, what);
  }
}

bool near(float a, float b, float tol = 1e-5f) { return fabsf(a - b) <= tol; }

#define CHECK(expr) check((expr), #expr, __LINE__)

class StringPrint : public Print
{
  public:
    size_t write(const char* s) override { text += s; return std::string(s).size(); }
    std::string text;
};

// constexpr construction must work: tables of vectors live in flash, not RAM
constexpr Vector2 cTable2[] = { Vector2(1.0f, 2.0f), Vector2() };
constexpr Vector3 cTable3[] = { Vector3(1.0f, 2.0f, 3.0f), Vector3() };
static_assert(cTable2[0].y == 2.0f, "constexpr Vector2");
static_assert(cTable3[0].z == 3.0f, "constexpr Vector3");
} // namespace

int main()
{
  // ---- Scalars
  CHECK(clampf(-2.0f, 0.0f, 1.0f) == 0.0f);
  CHECK(clampf(0.5f, 0.0f, 1.0f) == 0.5f);
  CHECK(clampf(9.0f, 0.0f, 1.0f) == 1.0f);
  CHECK(lerpf(0.0f, 10.0f, 0.25f) == 2.5f);
  CHECK(lerpf(0.0f, 10.0f, 2.0f) == 10.0f);
  CHECK(near(mapf(5.0f, 0.0f, 10.0f, 100.0f, 200.0f), 150.0f));
  CHECK(mapf(5.0f, 1.0f, 1.0f, 7.0f, 9.0f) == 7.0f);
  CHECK(near(wrapPi(3.0f * cPI), cPI) || near(wrapPi(3.0f * cPI), -cPI));
  CHECK(near(wrap(370.0f, 0.0f, 360.0f), 10.0f));
  CHECK(near(normalize(15.0f, 10.0f, 20.0f), 0.5f));
  CHECK(smoothstep(0.5f) == 0.5f);
  CHECK(smootherstep(0.5f) == 0.5f);
  CHECK(smoothstep(-1.0f) == 0.0f && smoothstep(2.0f) == 1.0f);
  CHECK(near(lowPassFilter(0.0f, 10.0f, 100.0f, 100.0f), 5.0f));
  CHECK(lowPassFilter(0.0f, 10.0f, 0.0f, 10.0f) == 10.0f);
  CHECK(floatEquals(1.0f, 1.0f + 1e-7f));
  CHECK(!floatEquals(1.0f, 1.001f));
  CHECK(sqr(3) == 9);
  CHECK(near(90.0f * deg2rad, cPI / 2.0f));

  // ---- Vector2
  const Vector2 a(3.0f, 4.0f), b(1.0f, 0.0f);
  CHECK(a.length() == 5.0f);
  CHECK(a.lengthSqr() == 25.0f);
  CHECK(near(a.normalize().length(), 1.0f));
  CHECK(Vector2().normalize() == Vector2());
  CHECK(a.distance(Vector2()) == 5.0f);
  CHECK(a.distanceSqr(Vector2()) == 25.0f);
  CHECK(a.dot(b) == 3.0f);
  CHECK(b.cross(Vector2(0.0f, 1.0f)) == 1.0f);
  CHECK(near(Vector2(0.0f, 1.0f).angle(), cPI / 2.0f));
  CHECK(near(b.angleTo(Vector2(0.0f, 1.0f)), cPI / 2.0f));
  CHECK(near(b.angleTo(Vector2(0.0f, -1.0f)), -cPI / 2.0f));
  CHECK(b.perpendicular() == Vector2(0.0f, 1.0f));
  CHECK(b.rotate(cPI / 2.0f) == Vector2(0.0f, 1.0f));
  CHECK(Vector2(1.0f, -1.0f).reflect(Vector2(0.0f, 1.0f)) == Vector2(1.0f, 1.0f));
  CHECK(a.minimum(Vector2(5.0f, 1.0f)) == Vector2(3.0f, 1.0f));
  CHECK(a.maximum(Vector2(5.0f, 1.0f)) == Vector2(5.0f, 4.0f));
  CHECK(Vector2().moveTowards(Vector2(10.0f, 0.0f), 3.0f) == Vector2(3.0f, 0.0f));
  CHECK(Vector2().moveTowards(Vector2(1.0f, 0.0f), 3.0f) == Vector2(1.0f, 0.0f));
  CHECK(2.0f * a == a * 2.0f);
  CHECK(a.clamp(0.0f, 3.5f) == Vector2(3.0f, 3.5f));
  CHECK(lerp(Vector2(), a, 0.5f) == Vector2(1.5f, 2.0f));
  CHECK(bezier2(Vector2(), Vector2(1.0f, 2.0f), Vector2(2.0f, 0.0f), 0.5f) == Vector2(1.0f, 1.0f));
  CHECK(circularNormalization(1.0f, 1.0f).length() <= 1.0f + 1e-6f);
  CHECK(std::string(a.toString().c_str()) == "3.00, 4.00");

  Vector2 c = a;
  c += b; c -= b; c *= 2.0f; c /= 2.0f;
  CHECK(c == a);

  // ---- Vector3
  const Vector3 x(1.0f, 0.0f, 0.0f), y(0.0f, 1.0f, 0.0f), z(0.0f, 0.0f, 1.0f);
  const Vector3 v(1.0f, 2.0f, 2.0f);
  CHECK(v.length() == 3.0f);
  CHECK(v.dot(x) == 1.0f);
  CHECK(x.cross(y) == z);
  CHECK(y.cross(z) == x);
  CHECK(near(x.angleTo(y), cPI / 2.0f));
  CHECK(near(x.angleTo(-x), cPI));
  CHECK(v.project(x) == Vector3(1.0f, 0.0f, 0.0f));
  CHECK(v.reject(x) == Vector3(0.0f, 2.0f, 2.0f));
  CHECK(v.project(Vector3()) == Vector3());
  CHECK(v.project(x) + v.reject(x) == v);
  CHECK(Vector3(1.0f, -1.0f, 0.0f).reflect(y) == Vector3(1.0f, 1.0f, 0.0f));
  CHECK(x.rotateAroundAxis(z, cPI / 2.0f) == y);
  CHECK(x.rotateAroundAxis(z * 5.0f, cPI / 2.0f) == y);           // axis need not be unit
  CHECK(v.rotateAroundAxis(z, 0.7f) == rotateAroundZ(v, 0.7f));   // same convention
  CHECK(v.rotateAroundAxis(x, 0.7f) == rotateAroundX(v, 0.7f));
  CHECK(v.rotateAroundAxis(y, 0.7f) == rotateAroundY(v, 0.7f));
  CHECK(v.rotateAroundAxis(Vector3(), 1.0f) == v);
  CHECK(near(v.perpendicular().dot(v), 0.0f));
  CHECK(v.perpendicular().length() > 0.0f);
  CHECK(v.minimum(Vector3(0.0f, 5.0f, 2.0f)) == Vector3(0.0f, 2.0f, 2.0f));
  CHECK(v.maximum(Vector3(0.0f, 5.0f, 2.0f)) == Vector3(1.0f, 5.0f, 2.0f));
  CHECK(Vector3().moveTowards(Vector3(0.0f, 0.0f, 10.0f), 4.0f) == Vector3(0.0f, 0.0f, 4.0f));
  CHECK(2.0f * v == v * 2.0f);
  CHECK(v.distanceSqr(Vector3()) == 9.0f);
  CHECK(lerp(Vector3(), v, 1.0f) == v);
  CHECK(smoothstep(Vector3(), v, 0.5f) == v * 0.5f);
  CHECK(minjerk(Vector3(), v, 0.5f) == v * 0.5f);
  CHECK(minjerk(Vector3(), v, 0.0f) == Vector3());
  CHECK(bezier3(Vector3(), x, y, v, 1.0f) == v);
  CHECK(rotateXYZ(x, Vector3(0.0f, 0.0f, cPI / 2.0f)) == y);
  CHECK(v + Vector2(1.0f, 1.0f) == Vector3(2.0f, 3.0f, 2.0f));
  CHECK(std::string(v.toString().c_str()) == "1.00, 2.00, 2.00");

  // ---- printTo
  StringPrint out;
  v.printTo(out, 1);
  CHECK(out.text == "1.0, 2.0, 2.0");
  out.text.clear();
  a.printTo(out);
  CHECK(out.text == "3.00, 4.00");

  printf("%d checks, %d failed\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
