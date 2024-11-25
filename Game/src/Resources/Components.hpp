//
//  Components.hpp
//  Game
//
//  Created by Nathan Thurber on 16/10/24.
//

#pragma once

#include "ResourceManager.hpp"
#include <glm/gtc/matrix_transform.hpp>

struct MeshComponent {
    MeshResource* mesh;
};

struct MaterialComponent {
    Material* material;
};

struct BVHComponent {
    BVH* bvh;
};

struct TransformComponent {
    glm::vec3 translation{0.0f};
    glm::vec3 rotation{0.0f};
    float scale = 1.0f;
    inline const glm::mat4 getTranform() { return glm::mat4(glm::translate(glm::mat4(1.0f), translation) * glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale))); }
};

struct PhysicsComponent {
    
    float Mass = 1.0f;
    glm::mat3 inertiaTensor{1.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 angularVelocity{0.0f};

    glm::vec3 acceleration{0.0f};
    glm::vec3 angularAcceleration{0.0f};
    
    glm::vec3 force{0.0f};
    
};

//struct TransformComponent {
//    Vector3 position;
//    Vector3 scale;
//    Quaternion rotation;
//};
