//
//  BVH.cpp
//  Game
//
//  Created by Nathan Thurber on 5/12/24.
//

#include "BVHcollision.hpp"

BVHcollision::BVHcollision(Entity& entity1, Entity& entity2)
: m_entity1(entity1), m_entity2(entity2)
{
    
}

//const bool BVHcollision::checkCollision(const uint32_t depth)
//{
////    TriangleHitInfo result;
////    result.dst = rayLength;
//    
//    int nodeOffset = u_Data.objects[m_entity1].nodeOffset;
//    
//    int stack[depth];
//    int stackIndex = 0;
////    stack[stackIndex] = nodeOffset;
//    stack[stackIndex] = 0;
//    stackIndex++;
//    
//    while (stackIndex > 0)
//    {
//        stackIndex--;
//        int triangleCount = u_Data->nodes[stack[stackIndex]].triangleCount;
//        int startIndex = u_Data->nodes[stack[stackIndex]].startIndex;
//        
//        if (triangleCount > 0)
//        {
//            for (int i = 0; i < triangleCount; i++)
//            {
//                TriangleHitInfo triHitInfo = RayTriangle(ray, u_Data.triangles[triOffset + startIndex + i]);
//
//                if (triHitInfo.dst < result.dst)
//                {
//                    result = triHitInfo;
//                }
//            }
//        }
//        else
//        {
//            int childIndexA = nodeOffset + startIndex;
//            
//            float dstA = RayBoundingBoxDistance(ray, u_Data->nodes[childIndexA].boundsMin, u_Data->nodes[childIndexA].boundsMax);
//            float dstB = RayBoundingBoxDistance(ray, u_Data->nodes[childIndexA + 1].boundsMin, u_Data->nodes[childIndexA + 1].boundsMax);
//            
//            float dstNear;
//            float dstFar;
//            int childIndexNear;
//            int childIndexFar;
//            
//            if (dstA <= dstB)
//            {
//                dstNear = dstA;
//                dstFar  = dstB;
//                childIndexNear = childIndexA;
//                childIndexFar = childIndexA + 1;
//            }
//            else
//            {
//                dstNear = dstB;
//                dstFar  = dstA;
//                childIndexNear = childIndexA + 1;
//                childIndexFar = childIndexA;
//            }
//
//            if (dstFar < result.dst) stack[stackIndex++] = childIndexFar;
//            if (dstNear < result.dst) stack[stackIndex++] = childIndexNear;
//        }
//    }
//
//    return result;
//}
