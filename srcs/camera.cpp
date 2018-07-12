#include "engine.h"
#include "camera.h"

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, Chunk *chunk)
{
        float velocity = MovementSpeed * deltaTime;
        glm::vec3 Save = Position;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        
        int x = (int)floor(Position.x) % CHUNK_X;
        int y = floor(Position.y);
        int z = (int)floor(Position.z) % CHUNK_Z;
        if (x < 0)
            x = CHUNK_X + x;
        if (z < 0)
            z = CHUNK_Z + z;

        Block *b = chunk->get_block(x,y,z);
        if (b != NULL && b->isActive())
            Position = Save; // need to change to only reverting x/y/z, not necessarily all of them

        // cout << "Block:  (" << x << ", " << y << ", " << z << ")" << endl;
        // cout << "Player: (" << Position.x << ", " << Position.y << ", " << Position.z << ")" << endl;
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