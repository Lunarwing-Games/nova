#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

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
} Camera;

void camera_init(Camera *cam, vec3 pos);
void camera_update(Camera *cam);
void camera_get_view(Camera *cam, mat4 dest);
void camera_process_mouse(Camera *cam, float xoffset, float yoffset);
void camera_process_movement(Camera *cam, int forward, int backward, int left, int right, float dt);

#endif // camera.h