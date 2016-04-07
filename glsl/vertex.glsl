#version 400
precision mediump float;

in vec2 position;
out vec2 uv;

void
main(void) {
  uv = vec2(0.0, 1.0) + vec2(0.5, -0.5) * (position + 1.0);
  gl_Position = vec4(position, 1.0, 1.0);
}
