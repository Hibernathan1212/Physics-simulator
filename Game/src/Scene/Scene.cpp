////
////  Scene.cpp
////  Game
////
////  Created by Nathan Thurber on 25/5/24.
////
//
//#include "Scene.hpp"
//
//#include "Utils/Timer.hpp"
//
//#include <iostream>
//
//#include <glm/gtx/string_cast.hpp>
//
//#include <thread>
//
//void Scene::init(bool& Running)
//{
//    m_LastUpdateTime = duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
//
//    while (Running)
//    {
//        auto start = std::chrono::steady_clock::now();
//        //std::cout << "[Phyiscs loop] " << 1.0f/UpdateTime << " Hz" << std::endl;
//        m_CurrentTime = duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();;
//        m_UpdateTime = m_CurrentTime - m_LastUpdateTime;
//        m_LastUpdateTime = m_CurrentTime;
//        
//        //OnUpdate(m_UpdateTime);
//        OnUpdate(0.0005);
//        
//        using std::chrono::operator""us;
//        std::this_thread::sleep_until(start + 500us);
//    }
//}
//
//void Scene::AddObject(Object& object)
//{
//    Objects.push_back(object);
//}
//
//void Scene::RemoveObject(int objectIndex)
//{
//    Objects.erase(Objects.begin() + objectIndex);
//}
//
//void Scene::OnUpdate(double dt)
//{
//    const int substeps = 8;
//    const double sub_dt = dt / substeps;
//    
//    /*
//     
//     //general physics update
//     physics.update() etc
//     
//     */
//    
//    for (int i = 0; i < GetObjectCount() - 1; i++)
//    {
//        for (int j = (i+1); j < GetObjectCount(); j++)
//        {
//            Object& Object1 = Objects[i];
//            Object& Object2 = Objects[j];
//            
//            Physics::applyNewtonianGravity(Object1, Object2);
//            //applyGravity();
//            //solveCollisions();
//            //updatePositions(sub_dt);
//                    
//        }
//    }
//
//    for (int substep = 0; substep < substeps; substep++)
//    {
//        for (int i = 0; i < GetObjectCount(); i++)
//        {
//            Object& Object1 = Objects[i];
//
//            for (int j = (i+1); j < GetObjectCount(); j++)
//            {
//                Object& Object2 = Objects[j];
//
//                Physics::solveCollisions(Object1, Object2);
//            }
//            
//            Physics::updatePosition(Object1, sub_dt);
//            Physics::updateRotation(Object1, sub_dt);
//            /*
//             updates on per object basis
//             */
//        }
//
//        
//    }
//}
//
////void Scene::updatePositions(float dt)
////{
////    for (Object& object : Objects)
////    {
////        object.updatePosition(dt);
////        object.updateRotations(dt);
////    }
////}
////
////void Scene::applyGravity()
////{
////    for (Object& object : Objects)
////    {
////        object.accelerate(gravity);
////    }
////}
//
