#version 400
precision mediump float;

uniform float iGlobalTime;
uniform vec3 iResolution;

in vec2 uv;
out vec4 fragColor;

#define FOG_DENSITY 0.5

#pragma glslify: fog = require(glsl-fog/exp2)
#pragma glslify: planet = require(glsl-earth)

void main() {

  float size = 0.75;
  float fogDistance = gl_FragCoord.z / gl_FragCoord.w;
  float fogAmount = fog(fogDistance, FOG_DENSITY);

  vec2 rotation = vec2(iGlobalTime * 0.03, iGlobalTime * 0.01);
  vec3 color = planet(uv, iResolution.xy, size, rotation);
  vec4 fogColor = vec4(1/iGlobalTime, 1/iGlobalTime, 1/iGlobalTime, 1);

  fragColor = mix(vec4(color, 1.0), fogColor, fogAmount);
}
