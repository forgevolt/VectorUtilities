// Minimal stand-in for the Arduino core, so the library and its examples can be built and
// run on a PC. Only what VectorUtilities and its examples use.
#pragma once

#include <cstddef>
#include <cstdio>
#include <string>

class String
{
  public:
    String(const char* s = "") : myText(s) {}
    const char* c_str() const { return myText.c_str(); }
    bool operator==(const char* s) const { return myText == s; }

  private:
    std::string myText;
};

class Print
{
  public:
    virtual ~Print() = default;
    virtual size_t write(const char* s) = 0;

    size_t print(const char* s)          { return write(s); }
    size_t print(const String& s)        { return write(s.c_str()); }
    size_t print(int value)              { return printf_("%d", value); }
    size_t print(unsigned long value)    { return printf_("%lu", value); }
    size_t print(double value, int digits = 2)
    {
      char buffer[48];
      snprintf(buffer, sizeof(buffer), "%.*f", digits, value);
      return write(buffer);
    }

    // The real Print has no float overload: a float is converted to double at the call. GCC
    // accepts that silently even with -Wdouble-promotion, but clang (the PC compiler on a
    // Mac) warns at every Serial.print(float). Taking the float here keeps the examples as
    // they would be written for Arduino and warning-free on both compilers.
    size_t print(float value, int digits = 2) { return print(static_cast<double>(value), digits); }

    size_t println()                     { return write("\n"); }
    template<typename T>
    size_t println(const T& value)       { size_t n = print(value); return n + println(); }
    size_t println(double value, int digits) { size_t n = print(value, digits); return n + println(); }
    size_t println(float value, int digits)  { size_t n = print(value, digits); return n + println(); }

  private:
    template<typename T>
    size_t printf_(const char* format, T value)
    {
      char buffer[32];
      snprintf(buffer, sizeof(buffer), format, value);
      return write(buffer);
    }
};

class HostSerial : public Print
{
  public:
    void begin(unsigned long) {}
    size_t write(const char* s) override { return static_cast<size_t>(fputs(s, stdout) >= 0 ? std::string(s).size() : 0); }
};

extern HostSerial Serial;

unsigned long millis();
void delay(unsigned long ms);
