#include "camera.h"

void camera_init(Camera *cam, vec3 pos) {
    glm_vec3_copy(pos, cam->position);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, cam->world_up);

    cam->yaw = -90.0f;
    cam->pitch = 0.0f;
    cam->fov = 70.0f;
    cam->movement_speed = 5.0f;
    cam->mouse_sens = 0.1f;

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
    glm_vec3_normalize(cam->right);

    glm_vec3_crossn(cam->right, cam->front, cam->up);
    glm_vec3_normalize(cam->up);
}

void camera_get_view(Camera *cam, mat4 dest) {
    vec3 target;
    glm_vec3_add(cam->position, cam->front, target);
    glm_lookat(cam->position, target, cam->up, dest);
}

void camera_get_projection(Camera *cam, mat4 dest, float aspect, float near_plane, float far_plane) {
    glm_perspective(glm_rad(cam->fov), aspect, near_plane, far_plane, dest);
}

void camera_process_mouse(Camera *cam, float xoffset, float yoffset) {
    xoffset *= cam->mouse_sens;
    yoffset *= cam->mouse_sens;

    if (cam->yaw > 360.0f) cam->yaw -= 360.0f;
    else if (cam->yaw < 0.0f) cam->yaw += 360.0f;

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

void camera_process_vertical(Camera *cam, int up, int down, float dt) {
    const float velocity = cam->movement_speed * dt;
    vec3 tmp;

    if(up) {
        glm_vec3_scale(cam->world_up, velocity, tmp);
        glm_vec3_add(cam->position, tmp, cam->position);
    }
    if(down) {
        glm_vec3_scale(cam->world_up, velocity, tmp);
        glm_vec3_sub(cam->position, tmp, cam->position);
    }
}