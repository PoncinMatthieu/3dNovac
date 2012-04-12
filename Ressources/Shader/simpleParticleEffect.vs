#version 330 core

uniform float	Time;
uniform mat4    MVPMatrix;

in vec3         InCoord;
in vec3         InSpeed;
in vec3         InDirection;
in float        InAcceleration;
in float	InLifeTime;

out Vertice
{
  vec4	Color;
} vertice;

void main()
{
  float t = Time;
  if (t<0.0)
    t=0.0;

  vec3 direction = normalize(InDirection);

  //vec3 velocity = 30.f * normalize(InSpeed);
  vec3 velocity = 30.0 * (InLifeTime / 4.1 * length(InSpeed)) * normalize(InSpeed);
  //vec3 velocity = 30.0 * (abs(3.0 * sin(t/20.0)) * InLifeTime / 4.1 * length(InSpeed)) * normalize(InSpeed);

  float acceleration = 20.0 * InAcceleration;
  vec3 p = InCoord + velocity * t + direction * (acceleration * t * t * 0.5);
  gl_Position = MVPMatrix * vec4(p, 1.0);

  float lifeLeft = 1.0 - smoothstep(0.0, InLifeTime, t);
  gl_PointSize = min(12.0, t / InAcceleration);
  vertice.Color = vec4(1.0, pow(1.0 - InAcceleration, 6.0), pow((1.0 - InAcceleration), 14.0) - 0.3, lifeLeft / (2.0 * gl_PointSize));
}
