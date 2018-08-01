#include "engine.h"
#include "camera.h"

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, Chunk *chunk)
{
    float velocity = MovementSpeed * deltaTime;
    
    // int y = Position.y;
    if (direction == FORWARD)
        Position += glm::vec3(Front.x, 0.0f, Front.z) * velocity;
    if (direction == BACKWARD)
        Position -= glm::vec3(Front.x, 0.0f, Front.z) * velocity;
    if (direction == LEFT)
        Position -= glm::vec3(Right.x, 0.0f, Right.z) * velocity;
    if (direction == RIGHT)
        Position += glm::vec3(Right.x, 0.0f, Right.z) * velocity;
    // Position.y = y;

    // DEBUGGING CAMERA MODE
    // if (direction == FORWARD)
    //   Position += glm::vec3(Front.x, Front.y, Front.z) * velocity;
    // if (direction == BACKWARD)
    //   Position -= glm::vec3(Front.x, Front.y, Front.z) * velocity;
    // if (direction == LEFT)
    //   Position -= glm::vec3(Right.x, Right.y, Right.z) * velocity;
    // if (direction == RIGHT)
    //   Position += glm::vec3(Right.x, Right.y, Right.z) * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up  = glm::normalize(glm::cross(Right, Front));
}