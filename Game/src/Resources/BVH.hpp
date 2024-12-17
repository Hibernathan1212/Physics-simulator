//
//  BVH.hpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//


#include "MeshResource.hpp"

#include <glm/glm.hpp>

#include <vector>

#pragma once

struct Triangle
{
    glm::vec3 posA;
    glm::vec3 posB;
    glm::vec3 posC;
    
    glm::vec3 normA;
    glm::vec3 normB;
    glm::vec3 normC;

    Triangle(glm::vec3 posA, glm::vec3 posB, glm::vec3 posC, glm::vec3 normA, glm::vec3 normB, glm::vec3 normC)
    : posA(posA), posB(posB), posC(posC), normA(normA), normB(normB), normC(normC) { }
};

struct BoundingBox
{
    glm::vec3 Min;
    glm::vec3 Max;
    bool hasPoint = false;
    
    const inline glm::vec3 Size() { return  Max - Min; }
    const inline glm::vec3 Centre() { return  (Min + Max) / 2.0f; }
    
    void GrowToInclude(glm::vec3 min, glm::vec3 max)
    {
//        Min.x = min.x < Min.x ? min.x : Min.x;
//        Min.y = min.y < Min.y ? min.y : Min.y;
//        Min.z = min.z < Min.z ? min.z : Min.z;
//
//        Max.x = max.x > Max.x ? max.x : Max.x;
//        Max.y = max.y > Max.y ? max.y : Max.y;
//        Max.z = max.z > Max.z ? max.z : Max.z;
//
//        Min = glm::min(min, Min);
//        Max = glm::max(max, Max);

        [[likely]] if (hasPoint)
        {
            Min.x = min.x < Min.x ? min.x : Min.x;
            Min.y = min.y < Min.y ? min.y : Min.y;
            Min.z = min.z < Min.z ? min.z : Min.z;
            Max.x = max.x > Max.x ? max.x : Max.x;
            Max.y = max.y > Max.y ? max.y : Max.y;
            Max.z = max.z > Max.z ? max.z : Max.z;
        }
        else
        {
            hasPoint = true;
            Min = min;
            Max = max;
        }
    }
};

struct Node
{
    glm::vec3 BoundsMin;
    glm::vec3 BoundsMax;
    // Index of first child (if triangle count is negative) otherwise index of first triangle
    int32_t StartIndex;
    int32_t TriangleCount;
    
    Node(BoundingBox bounds)
    {
        BoundsMin = bounds.Min;
        BoundsMax = bounds.Max;
        StartIndex = -1;
        TriangleCount = -1;
    }
    
    Node(BoundingBox bounds, int startIndex, int triCount)
    {
        BoundsMin = bounds.Min;
        BoundsMax = bounds.Max;
        StartIndex = startIndex;
        TriangleCount = triCount;
    }
    
    const inline glm::vec3 CalculateBoundsSize() { return BoundsMax - BoundsMin; };
    const inline glm::vec3 CalculateBoundsCentre() { return (BoundsMin + BoundsMax) / 2.0f ; }
};

//struct NodeList
//{
//    std::vector<Node> Nodes;
//    //uint32_t Index = 0;
//    
//    NodeList()
//    {
//        Nodes.reserve(256);
//    }
//    
//    int32_t Add(Node node)
//    {
//        //if (Index >= Nodes.size())
//        //{
//        //    Nodes.resize(Nodes.size() * 2, );
//        //}
//        
//        Nodes.push_back(node);
//        return Nodes.size();
//    }
//    
//    int32_t Add(BoundingBox bounds, int startIndex, int triCount)
//    {
//        //if (Index >= Nodes.size())
//        //{
//        //    Nodes.resize(Nodes.size() * 2, );
//        //}
//        
//        Nodes.emplace_back(bounds, startIndex, triCount);
//        return Nodes.size();
//    }
//    
//    const inline int32_t NodeCount() { return Nodes.size(); }
//};

class BVH
{
public:
    BVH(MeshResource& mesh);
    //BVH(std::vector<glm::vec3> verts, std::vector<uint32_t> indices);
    ~BVH();
    
    const inline std::vector<Triangle> getTriangles() { return allTris; }
    const inline std::vector<Node> getNodes() { return allNodes; }
    
private:
    
    struct BVHTriangle {
    glm::vec3 Centre;
    glm::vec3 Min;
    glm::vec3 Max;
    int32_t Index;

    BVHTriangle(glm::vec3 centre, glm::vec3 min, glm::vec3 max, int index)
    : Centre(centre), Min(min), Max(max), Index(index) {}
};
    
    //void Split(int parentIndex, std::vector<glm::vec3> verts, int triGlobalStart, int triNum, int depth = 0);
    
    void Split(int parentIndex, int triGlobalStart, int triNum, int depth = 0);
    
    std::tuple<int, float, float> ChooseSplit(Node node, int start, int count);
    
    float EvaluateSplit(int splitAxis, float splitPos, int start, int count);

    const inline float NodeCost(glm::vec3 size, int numTriangles) { return (size.x * size.y + size.x * size.z + size.y * size.z) * numTriangles; }
    
    std::vector<Node> allNodes;
    std::vector<BVHTriangle> AllTriangles;
    std::vector<Triangle> allTris;
};
