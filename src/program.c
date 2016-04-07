#include <glfw-shell/glfw-shell.h>
#include <glisy/uniform.h>
#include <glisy/geometry.h>
#include <glisy/program.h>
#include <glisy/shader.h>
#include <glisy/math.h>
#include <glisy/vao.h>
#include <stdlib.h>
#include <fs/fs.h>

GlisyGeometry geometry;
GlisyProgram program;
GlisyUniform iResolution;
GlisyUniform iGlobalTime;

GLFWwindow *window;
GLfloat globalTime = 0;
int globalWidth = 1600;
int globalHeight = 900;

void
InitializeProgram(GlisyProgram *program,
                  char *vertexPath,
                  char *fragmentPath) {
  GlisyShader vertex;
  GlisyShader fragment;
  if (!program) return;
  // initialize program
  glisyProgramInit(program);
  glisyShaderInit(&vertex, GL_VERTEX_SHADER, fs_read(vertexPath));
  glisyShaderInit(&fragment, GL_FRAGMENT_SHADER, fs_read(fragmentPath));
  glisyProgramAttachShader(program, &vertex);
  glisyProgramAttachShader(program, &fragment);
  glisyProgramLink(program);
  glisyProgramBind(program);
}

void
InitializeGeometry() {
  GlisyVAOAttribute position;
  vec2 vertices[] = {
    +4.0f, -4.0f,
    -0.0f, +4.0f,
    -4.0f, -4.0f,
  };

  glisyGeometryInit(&geometry);
  memset(&position, 0, sizeof(position));
  position.buffer.data = vertices;
  position.buffer.type = GL_FLOAT;
  position.buffer.size = sizeof(vertices);
  position.buffer.usage = GL_STATIC_DRAW;
  position.buffer.dimension = 2;
  glisyGeometryAttr(&geometry, "position", &position);
  glisyGeometryUpdate(&geometry);
}

void
UpdateUniforms() {
  GLfloat now = glfwGetTime();
  globalTime = now;
  vec3 resolution = {globalWidth, globalHeight, 0};
#define X(u, d, s) \
  glisyUniformBind(&u, &program); \
  glisyUniformSet(&u, d, s); \
  glisyUniformBind(&u, &program);

  X(iGlobalTime, &globalTime, sizeof(globalTime));
  X(iResolution, &resolution, sizeof(resolution));
#undef X
}

void
InitializeUniforms() {
  glisyUniformInit(&iGlobalTime, "iGlobalTime", GLISY_UNIFORM_FLOAT, 1);
  glisyUniformInit(&iResolution, "iResolution", GLISY_UNIFORM_VECTOR, 3);
  UpdateUniforms();
}

void
Render() {
  glisyGeometryBind(&geometry, &program);
  glisyGeometryDraw(&geometry, GL_TRIANGLES, 0, 3);
  glisyGeometryUnbind(&geometry);
}

int
main(void) {
  GLFW_SHELL_CONTEXT_INIT(3, 2);
  GLFW_SHELL_WINDOW_INIT(window, globalWidth, globalHeight);
  InitializeProgram(&program, "glsl/vertex.glsl.compiled",
                              "glsl/fragment.glsl.compiled");
  InitializeGeometry();
  InitializeUniforms();

  GLFW_SHELL_RENDER(window, {
    glDisable(GL_DEPTH_TEST);

    globalWidth = width;
    globalHeight = height;

    UpdateUniforms(); Render();
  });
  return 0;
}
