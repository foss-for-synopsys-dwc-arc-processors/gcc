extern double fma (double, double, double);
extern float fmaf (float, float, float);

float
fma32 (float x, float y, float z)
{
  return fmaf (x, y, z);
}

float
fms32 (float x, float y, float z)
{
  return fmaf (-x, y, z);
}

float
fnms32 (float x, float y, float z)
{
  return fmaf (x, y, -z);
}

float
fnma32 (float x, float y, float z)
{
  return fmaf (-x, y, -z);
}

double
fma64 (double x, double y, double z)
{
  return fma (x, y, z);
}

double
fms64 (double x, double y, double z)
{
  return fma (-x, y, z);
}

double
fnms64 (double x, double y, double z)
{
  return fma (x, y, -z);
}

double
fnma64 (double x, double y, double z)
{
  return fma (-x, y, -z);
}
