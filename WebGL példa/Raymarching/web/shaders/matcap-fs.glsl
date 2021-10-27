#version 300 es 
precision highp float;

in vec2 tex;
in vec4 rayDir;


precision highp sampler3D;

uniform struct {
  sampler3D volume;
  sampler2D matcapTexture;
} scene;

uniform struct {
  mat4 rayDirMatrix;
  vec3 position;
} camera;

out vec4 fragmentColor;

float threshold = 0.3;
int prec = 256;
float floatprec = 256.0;
int binprec = 10;
float offset = 0.002;


struct posnormcolor {
  vec3 position;
  vec3 normal;
  vec4 color;
};

posnormcolor foo(vec3 startpoint, vec3 d, vec3 step)
{
  
  vec3 offsetdir1 = normalize(vec3(-d.y, d.x, 0));
  vec3 offsetdir2 = cross(d, offsetdir1);

  posnormcolor ret;

  vec3 p = startpoint;

  for (int i = 0; i < prec; i++)
  {
    float h = texture(scene.volume, p).r;
    if (h > threshold)
    {
      ret.color = vec4(0.5, 0.5, 0.5, 1.0);
      break;
    }
    p += step;
  }

  vec3 smallStep = step / -2.0;
  p += smallStep;
  bool backwards = true;

  for (int i = 0; i < binprec; i++)
  {
    float h = texture(scene.volume, p).r;
    if (backwards)
    {
      if (h < threshold)
      {
        smallStep /= -2.0;
        backwards = false;
      }
      else
      {
        smallStep /= 2.0;
      }
    }
    else
    {
      if (h > threshold)
      {
        smallStep /= -2.0;
        backwards = true;
      }
      else
      {
        smallStep /= 2.0;
      }
    }
    p += smallStep;
  }
  ret.position = p;

  p = startpoint + offsetdir1 * offset;
  vec3 offsetpos1;
  for (int i = 0; i < prec; i++)
  {
    float h = texture(scene.volume, p).r;
    if (h > threshold)
    {
      break;
    }
    p += step;
  }

  smallStep = step / -2.0;
  p += smallStep;
  backwards = true;

  for (int i = 0; i < binprec; i++)
  {
    float h = texture(scene.volume, p).r;
    if (backwards)
    {
      if (h < threshold)
      {
        smallStep /= -2.0;
        backwards = false;
      }
      else
      {
        smallStep /= 2.0;
      }
    }
    else
    {
      if (h > threshold)
      {
        smallStep /= -2.0;
        backwards = true;
      }
      else
      {
        smallStep /= 2.0;
      }
    }
    p += smallStep;
  }
  offsetpos1 = p;

  p = startpoint + offsetdir2 * offset;
  vec3 offsetpos2;
  for (int i = 0; i < prec; i++)
  {
    float h = texture(scene.volume, p).r;
    if (h > threshold)
    {
      break;
    }
    p += step;
  }

  smallStep = step / -2.0;
  p += smallStep;
  backwards = true;

  for (int i = 0; i < binprec; i++)
  {
    float h = texture(scene.volume, p).r;
    if (backwards)
    {
      if (h < threshold)
      {
        smallStep /= -2.0;
        backwards = false;
      }
      else
      {
        smallStep /= 2.0;
      }
    }
    else
    {
      if (h > threshold)
      {
        smallStep /= -2.0;
        backwards = true;
      }
      else
      {
        smallStep /= 2.0;
      }
    }
    p += smallStep;
    
  }
  offsetpos2 = p;

  ret.normal = normalize(cross(ret.position - offsetpos1, ret.position - offsetpos2));
  return ret;
}



void main(void) {
  vec3 d = normalize(rayDir.xyz);
  vec3 t1 = (vec3(1.0, 1.0, 1.0) - camera.position.xyz) / d;
  vec3 t2 = (vec3(0.0, 0.0, 0.0) - camera.position.xyz) / d;

  vec3 tstartcand = vec3(max(min(t1.x, t2.x), 0.0), max(min(t1.y, t2.y), 0.0), max(min(t1.z, t2.z), 0.0));
  vec3 tendcand = vec3(max(max(t1.x, t2.x), 0.0), max(max(t1.y, t2.y), 0.0), max(max(t1.z, t2.z), 0.0));

  float tstart = max(tstartcand.x, max(tstartcand.y, tstartcand.z));
  float tend = min(tendcand.x, min(tendcand.y, tendcand.z));
  fragmentColor = vec4(0.0, 0.0, 0.0, 1.0);
  if (tend < tstart)
  {
    return;
  }

  vec3 p  = camera.position + d * tstart;
  vec3 step = d * (tend - tstart) / floatprec;

  posnormcolor point = foo(p, d, step);


  vec4 matcapColor = vec4(0.0, 0.0, 0.0, 1.0);
  if (point.color.r > threshold / 2.0)
    matcapColor = texture(scene.matcapTexture, normalize(point.normal).xy);
  fragmentColor = vec4(matcapColor);
}


