//
//  Camera.hpp
//  Game
//
//  Created by Nathan Thurber on 26/5/24.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Utils/Input/Input.hpp"

class Camera
{
public:
    Camera(float verticalFOV, float nearClip, float farClip);

    void init(bool& running);

    
    void OnUpdate(float ts);
    void OnResize(uint32_t width, uint32_t height);
    void RecalculateView();
    void RecalculateProjection();
    
    const glm::mat4& GetProjection() const { return m_Projection; }
    const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
    const glm::mat4& GetView() const { return m_View; }
    const glm::mat4& GetInverseView() const { return m_InverseView; }
    
    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec3& GetDirection() const { return m_ForwardDirection; }
    
    Input input;
    
    float GetRotationSpeed();
private:
    glm::mat4 m_Projection{ 1.0f };
    glm::mat4 m_View{ 1.0f };
    glm::mat4 m_InverseProjection{ 1.0f };
    glm::mat4 m_InverseView{ 1.0f };

    float m_VerticalFOV = 45.0f;
    float m_NearClip = 0.1f;
    float m_FarClip = 100.0f;

    glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_ForwardDirection{0.0f, 0.0f, 0.0f};

    glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };

    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    
private:
    float m_LastUpdateTime;
    float m_UpdateTime;
    float m_CurrentTime;
};
