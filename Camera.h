#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Camera
{
public:
    // camera Attributes
    glm::vec3 Position, Front, Up, Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw, Pitch;

    // camera translation, rotation, zooming
    float MovementSpeed, MouseSensitivity, Zoom;

	Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float speed = 2.5f, float sensitivity = 0.06f, float fov = 45.0f)
	{
        Position = cameraPos;
        Front = cameraFront;
        WorldUp = cameraUp;
        Yaw = yaw;
        Pitch = pitch;
        MovementSpeed = speed;
        MouseSensitivity = sensitivity;
        Zoom = fov;

        updateCameraVectors();
		
	}

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }


    void ProcessCameraMovement(char direction, float deltaTime)
    {
        float cameraSpeed = MovementSpeed * deltaTime; // adjust accordingly
        if (direction == 'n')
            Position += Front * cameraSpeed;
        if (direction == 's')
            Position -= Front * cameraSpeed;
        if (direction == 'w')
            Position -= Right * cameraSpeed;
        if (direction == 'e')
            Position += Right * cameraSpeed;
    }


    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateCameraVectors();
    }


    void ProcessMouseScroll(float yoffset)
    {
        std::cout << yoffset << std::endl;

        float min = 1.0f, max = 45.0f; // if min less than 0, the view will filp over 

        if (Zoom >= min && Zoom <= max)
            Zoom -= yoffset;
        if (Zoom <= min)
            Zoom = min;
        if (Zoom >= max)
            Zoom = max;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};


#endif