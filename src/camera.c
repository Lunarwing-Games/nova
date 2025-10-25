#include "camera.h"

void camera_init(Camera *cam, vec3 pos) {
    glm_vec3_copy(pos, cam->position);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, cam->world_up);
    cam->yaw = -90.0f;
    cam->pitch = 0.0f;
    cam->fov = 70.0f;

    camera_update(cam);
}

void camera_update(Camera *cam) {
    vec3 front = {
        cosf(glm_rad(cam->yaw)) * cosf(glm_rad(cam->pitch)),
        sinf(glm_rad(cam->pitch)),
        sinf(glm_rad(cam->yaw)) * cosf(glm_rad(cam->pitch))
    };
    glm_vec3_normalize_to(front, cam->front);

    glm_vec3_crossn(cam->front, cam->world_up, cam->right);
    glm_vec3_crossn(cam->right, cam->front, cam->up);
}

void camera_get_view(Camera *cam, mat4 dest) {
    vec3 target;
    glm_vec3_add(cam->position, cam->front, target);
    glm_lookat(cam->position, target, cam->up, dest);
}

void camera_process_mouse(Camera *cam, float xoffset, float yoffset) {
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->yaw   += xoffset;
    cam->pitch += yoffset;

    cam->pitch = CLAMP(cam->pitch, -89.0f, 89.0f);

    camera_update(cam);
}

void camera_process_movement(Camera *cam, int forward, int backward, int left, int right, float dt) {
    const float speed = 5.0f * dt;
    vec3 tmp;

    if (forward) {
        glm_vec3_scale(cam->front, speed, tmp);
        glm_vec3_add(cam->position, tmp, cam->position);
    }
    if (backward) {
        glm_vec3_scale(cam->front, speed, tmp);
        glm_vec3_sub(cam->position, tmp, cam->position);
    }
    if (left) {
        glm_vec3_scale(cam->right, speed, tmp);
        glm_vec3_sub(cam->position, tmp, cam->position);
    }
    if (right) {
        glm_vec3_scale(cam->right, speed, tmp);
        glm_vec3_add(cam->position, tmp, cam->position);
    }
}