////
////  Scene.hpp
////  Game
////
////  Created by Nathan Thurber on 25/5/24.
////
//
//#pragma once
//
//#include <vector>
//
//#include <glm/glm.hpp>
//
//#include "Physics.hpp"
//#include "Object.hpp"
//#include "ShaderTypes.hpp"
//
//class Scene
//{
//public:
//    void AddObject(Object& object);
//    void RemoveObject(int objectIndex);
//    
//    void init(bool& Running);
//    
//    const uint32_t GetObjectCount() const { return Objects.size(); }
//    const uint32_t GetObjectVertexCount(int objectIndex) const { return Objects[objectIndex].Vertices.size(); }
//    const uint32_t GetObjectIndexCount(int objectIndex) const { return Objects[objectIndex].Indices.size(); }
//    const Material GetObjectMaterial(int objectIndex) const { return Objects[objectIndex].material; }
//    const glm::vec3 GetObjectVertex(int objectIndex, int vertexIndex) const { return Objects[objectIndex].Vertices[vertexIndex]; }
//    const glm::vec3 GetObjectindex(int objectIndex, int triangleIndex) const { return Objects[objectIndex].Indices[triangleIndex]; }
//    
//    void OnUpdate(double dt);
//    
//public:
//    std::vector<Object> Objects;
//    
//private:
//    //std::vector<Sphere> Spheres;
//    
//    //glm::vec3 gravity = {0.0f, 0.0f, 0.0f};
//    
//    //void updatePositions(float dt);
//    //void applyGravity();
//    //void solveCollisions();
//    
//    double m_UpdateTime;
//    double m_CurrentTime;
//    double m_LastUpdateTime;
//    
//};
//
