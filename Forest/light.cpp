#include "light.h"

#include <glm/gtx/transform.hpp>

void Light::mouseButtonChanged(int button, int state, float x, float y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        arcball_on = true;
        last_mx = cur_mx = x;
        last_my = cur_my = y;
    }
    else {
        arcball_on = false;
    }
}

void Light::mouseDragged(glm::vec2 mousePosition) 
{
    if (arcball_on) 
    {  
        cur_mx = mousePosition.x;
        cur_my = mousePosition.y;
    }
}

glm::vec3 Light::get_arcball_vector(float x, float y)
{
    glm::vec3 P = glm::vec3(x * 2.0f - 1.0f, y * 2.0f - 1.0f, 0.0f);

    float OP_squared = P.x * P.x + P.y * P.y;
    if (OP_squared <= 1.0f)
        P.z = std::sqrtf(1 - OP_squared);
    else
        P = glm::normalize(P);
    return P;
}

void Light::onIdle(glm::mat4 V)
{
    if (cur_mx != last_mx || cur_my != last_my)
    {
        glm::vec3 va = get_arcball_vector(last_mx, last_my);
        glm::vec3 vb = get_arcball_vector(cur_mx, cur_my);
        float angle = acos(std::min(1.0f, glm::dot(va, vb))) * sensitivity;
        glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
        glm::mat4 camera2world = glm::inverse(V);
        glm::vec4 axis_in_world_coord = camera2world * glm::vec4(axis_in_camera_coord.x, axis_in_camera_coord.y, axis_in_camera_coord.z, 1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::degrees(angle), glm::vec3(axis_in_world_coord));
        last_mx = cur_mx;
        last_my = cur_my;
    }
}

glm::mat4 Light::GetRotationMatrix()
{
    return rotationMatrix;
}

Light::Light() : rotationMatrix(glm::mat4(1.0f)), sensitivity(0.1f) {}