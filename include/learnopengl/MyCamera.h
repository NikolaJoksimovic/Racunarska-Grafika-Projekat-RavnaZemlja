//
// Created by joksa on 2/10/21.
//

#ifndef PROJECT_BASE_MYCAMERA_H
#define PROJECT_BASE_MYCAMERA_H

enum Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera{
private:
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float zoom = 45.0f;
    float cameraSpeed = 2.5;
    float sensitivity = 0.5;

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 6.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::normalize(glm::cross(Front, Up));

    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

public:

    Camera() {

    }

    glm::mat4 getCameraView(){
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Movement direction, float deltaTime) {
        float velocity = cameraSpeed * deltaTime;
        switch (direction) {
            case FORWARD: {
                Position += Front * velocity;
            }break;
            case BACKWARD: {
                Position -= Front * velocity;
            }break;
            case LEFT: {
                Position -= Right * velocity;
            }break;
            case RIGHT: {
                Position += Right * velocity;
            }break;
        }
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    void ProcessScroll(float yoffset){
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    float getZoom(){
        return zoom;
    }

    glm::vec3 getPosition(){
        return Position;
    }
    glm::vec3 getFront(){
        return Front;
    }

    void setPosition(glm::vec3 newPos){
        Position = newPos;
    }
};



#endif //PROJECT_BASE_MYCAMERA_H
