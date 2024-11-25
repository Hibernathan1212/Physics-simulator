//
//  BVH.cpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//

#include "BVH.hpp"

#include "Utils/Timer.hpp"

int testtest = 0;

BVH::BVH(MeshResource& mesh)
{
    Timer timer;
    
    std::cout << "[Mesh Loader] Constructing BVH" << std::endl;
    
    AllTriangles.reserve(mesh.getIndices().size() / 3);
    //nodes = new NodeList;
    //AllTriangles = new std::vector<BVHTriangle>(mesh->getIndices().size() / 3);
    BoundingBox bounds;
    
    std::vector<uint32_t> indices = mesh.getIndices();
    std::vector<Vertex> verts = mesh.getVertices();

    for (int i = 0; i < indices.size(); i += 3)
    {
        glm::vec3 a = verts[indices[i + 0]].position;
        glm::vec3 b = verts[indices[i + 1]].position;
        glm::vec3 c = verts[indices[i + 2]].position;
        glm::vec3 centre = (a + b + c) / 3.0f;
        glm::vec3 max = glm::max(glm::max(a, b), c);
        glm::vec3 min = glm::min(glm::min(a, b), c);
        
        if (max.x > 100000 || max.y > 100000 || max.z > 100000 || min.x < -100000 || min.y < -100000 || min.z < -100000 )
            throw std::runtime_error("uh oh");
        
        AllTriangles.emplace_back(centre, min, max, i);
        bounds.GrowToInclude(min, max);
    }
    
    allNodes.emplace_back(bounds);
    Split(0, 0, AllTriangles.size());
    
    allTris.reserve(AllTriangles.size());
    
    for (int i = 0; i < AllTriangles.size(); i++)
    {
        BVHTriangle buildTri = AllTriangles[i];
        glm::vec3 a = verts[indices[buildTri.Index + 0]].position;
        glm::vec3 b = verts[indices[buildTri.Index + 1]].position;
        glm::vec3 c = verts[indices[buildTri.Index + 2]].position;
        glm::vec3 norm_a = verts[indices[buildTri.Index + 0]].normal;
        glm::vec3 norm_b = verts[indices[buildTri.Index + 1]].normal;
        glm::vec3 norm_c = verts[indices[buildTri.Index + 2]].normal;
        allTris.emplace_back(a, b, c, norm_a, norm_b, norm_c);
        //allTris[i] = Triangle(a, b, c, norm_a, norm_b, norm_c);
    }
    
    std::cout << "[Mesh Loader] Construction complete - Time taken: " << timer.ElapsedMillis() << "ms, Triangle count: " << AllTriangles.size() << ", Node count: " << allNodes.size() << std::endl;
}

BVH::~BVH()
{
    
}

void BVH::Split(int parentIndex, int triGlobalStart, int triNum, int depth)
{
    const int MaxDepth = 32;
    Node parent = allNodes[parentIndex];
    glm::vec3 size = parent.CalculateBoundsSize();
    float parentCost = NodeCost(size, triNum);
    
    std::tuple<int, float, float> splitResult = ChooseSplit(parent, triGlobalStart, triNum);
    
    int splitAxis = get<0>(splitResult);
    float splitPos = get<1>(splitResult);
    float cost = get<2>(splitResult);

    if (cost < parentCost && depth < MaxDepth)
    {
        BoundingBox boundsLeft;
        BoundingBox boundsRight;
        int numOnLeft = 0;

        for (int i = triGlobalStart; i < triGlobalStart + triNum; i++)
        {
            BVHTriangle tri = AllTriangles[i];
            if (tri.Centre[splitAxis] < splitPos)
            {
                boundsLeft.GrowToInclude(tri.Min, tri.Max);

                BVHTriangle swap = AllTriangles[triGlobalStart + numOnLeft];
                AllTriangles[triGlobalStart + numOnLeft] = tri;
                AllTriangles[i] = swap;
                numOnLeft++;
            }
            else
            {
                boundsRight.GrowToInclude(tri.Min, tri.Max);
            }
        }

        int numOnRight = triNum - numOnLeft;
        int triStartLeft = triGlobalStart;
        int triStartRight = triGlobalStart + numOnLeft;
        
        int childIndexLeft = allNodes.size();
        allNodes.emplace_back(boundsLeft, triStartLeft, 0);
        
        int childIndexRight = allNodes.size();
        allNodes.emplace_back(boundsRight, triStartRight, 0);

        parent.StartIndex = childIndexLeft;
        allNodes[parentIndex] = parent;

        Split(childIndexLeft, triGlobalStart, numOnLeft, depth + 1);
        Split(childIndexRight, triGlobalStart + numOnLeft, numOnRight, depth + 1);
    }
    else
    {
        parent.StartIndex = triGlobalStart;
        parent.TriangleCount = triNum;
        allNodes[parentIndex] = parent;
    }
}


std::tuple<int, float, float> BVH::ChooseSplit(Node node, int start, int count)
{
    if (count <= 1)
        return std::make_tuple(0, 0, MAXFLOAT);

    float bestSplitPos = 0;
    int bestSplitAxis = 0;
    const int numSplitTests = 10;

    float bestCost = MAXFLOAT;

    //std::vector<std::thread> threadGroup;
    //threadGroup.reserve(3 * numSplitTests);
    
    // Estimate best split pos
    for (int axis = 0; axis < 3; axis++)
    {
        for (int i = 1; i < numSplitTests - 1; i++) //sides are unlike to be best split
        {
            //threadGroup.emplace_back([this, i, axis, numSplitTests, node, count, start, &bestCost, &bestSplitPos, &bestSplitAxis]()
            {
                float splitT = (i + 1) / (numSplitTests + 1.0f);
                float splitPos = glm::mix(node.BoundsMin[axis], node.BoundsMax[axis], splitT);
                float cost = EvaluateSplit(axis, splitPos, start, count);
                if (cost < bestCost)
                {
                    //mtx.lock();
                    bestCost = cost;
                    bestSplitPos = splitPos;
                    bestSplitAxis = axis;
                    //mtx.unlock();
                }
            }//);
        }
    }
    
    //for (auto& thread : threadGroup)
    //{
    //    thread.join();
    //}

    return std::make_tuple(bestSplitAxis, bestSplitPos, bestCost);
}

float BVH::EvaluateSplit(int splitAxis, float splitPos, int start, int count)
{
    BoundingBox boundsLeft;
    BoundingBox boundsRight;
    int numOnLeft = 0;
    int numOnRight = 0;

    for (int i = start; i < start + count; i++)
    {      
        BVHTriangle& tri = AllTriangles[i];
        if (tri.Centre[splitAxis] < splitPos)
        {
            boundsLeft.GrowToInclude(tri.Min, tri.Max);
            numOnLeft++;
        }
        else
        {
            boundsRight.GrowToInclude(tri.Min, tri.Max);
            numOnRight++;
        }
    }

    float costA = NodeCost(boundsLeft.Size(), numOnLeft);
    float costB = NodeCost(boundsRight.Size(), numOnRight);
    return costA + costB;
}
