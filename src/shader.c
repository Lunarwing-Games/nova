#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper for reading file into memory
static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if(!f) {
        fprintf(stderr, "Shader: Failed to open %s!\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

// Helper to compile a single shader stage
static GLuint compile_stage(GLenum type, const char *src, const char *path) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "Shader compile error in %s:\n%s\n", path, log);
    }
    return shader;
}

ShaderProgram shader_load(const char *vert_path, const char *frag_path) {
    ShaderProgram result = {0};
    char *vsrc = read_file(vert_path);
    char *fsrc = read_file(frag_path);
    if (!vsrc || !fsrc) {
        fprintf(stderr, "Shader load failed: missing source\n");
        free(vsrc); free(fsrc);
        return result;
    }

    GLuint vshader = compile_stage(GL_VERTEX_SHADER, vsrc, vert_path);
    GLuint fshader = compile_stage(GL_FRAGMENT_SHADER, fsrc, frag_path);
    result.id = glCreateProgram();
    glAttachShader(result.id, vshader);
    glAttachShader(result.id, fshader);
    glLinkProgram(result.id);

    GLint linked = 0;
    glGetProgramiv(result.id, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[512];
        glGetProgramInfoLog(result.id, 512, NULL, log);
        fprintf(stderr, "Shader link error:\n%s\n", log);
        glDeleteProgram(result.id);
        result.id = 0;
    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
    free(vsrc);
    free(fsrc);
    return result;
}

void shader_destroy(ShaderProgram *shader) {
    if (shader && shader->id) {
        glDeleteProgram(shader->id);
        shader->id = 0;
    }
}

void shader_use(const ShaderProgram *shader) {
    if (shader && shader->id)
        glUseProgram(shader->id);
}

void shader_set_float(const ShaderProgram *shader, const char *name, float value) {
    GLint loc = glGetUniformLocation(shader->id, name);
    glUniform1f(loc, value);
}

void shader_set_vec3(const ShaderProgram *shader, const char *name, float x, float y, float z) {
    GLint loc = glGetUniformLocation(shader->id, name);
    glUniform3f(loc, x, y, z);
}

void shader_set_mat4(const ShaderProgram *shader, const char *name, const float *mat) {
    GLint loc = glGetUniformLocation(shader->id, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}