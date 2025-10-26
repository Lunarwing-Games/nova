#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GLuint id;
} ShaderProgram;

ShaderProgram shader_load(const char *vert_path, const char *frag_path);

void shader_destroy(ShaderProgram *shader);
void shader_use(const ShaderProgram *shader);
void shader_set_float(const ShaderProgram *shader, const char *name, float value);
void shader_set_vec3(const ShaderProgram *shader, const char *name, float x, float y, float z);
void shader_set_mat4(const ShaderProgram *shader, const char *name, const float *mat);

#ifdef __cplusplus
}
#endif

#endif // shader.h