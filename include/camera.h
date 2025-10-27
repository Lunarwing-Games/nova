#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 world_up;
    float yaw;
    float pitch;
    float fov;
    float movement_speed;
    float mouse_sens;
} Camera;

// Initalize a new camera
void camera_init(Camera *cam, vec3 pos);

// Update camera (this takes a lot of math)
void camera_update(Camera *cam);

//
void camera_get_view(Camera *cam, mat4 dest);

// A wrapper for `glm_perspective`
void camera_get_projection(Camera *cam, mat4 dest, float aspect, float near_plane, float far_plane);

// Take in mouse input and process
void camera_process_mouse(Camera *cam, float xoffset, float yoffset);

// WASD style movement within the 3D space
void camera_process_movement(Camera *cam, int forward, int backward, int left, int right, float dt);

// up/down movement
void camera_process_vertical(Camera *cam, int up, int down, float dt);

#ifdef __cplusplus
}
#endif

#endif // camera.h
