//
//  MeshLoader.hpp
//  Game
//
//  Created by Nathan Thurber on 14/10/24.
//


#include <glm/glm.hpp>

#pragma once

class Material
{
public:
    //Material();   // Constructor
    //~Material();  // Destructor

    //void setTexture(const std::string& name, Texture* texture);

    inline void setColor(const glm::vec3& color) { this->albedo = color; }
    inline glm::vec3 getAlbedo() const { return albedo; }

    inline void setRoughness(float roughness) { this->roughness = roughness; }
    inline float getRoughness() const { return roughness; }
    
    inline void setEmission(glm::vec3 emission) { this->emission = emission; }
    inline glm::vec3 getEmission() const { return emission; }

    inline void setRestitution(float restituation) {this->restitution = restituation; }
    inline float getRestitution() const { return restitution; }
    
private:
    //std::unordered_map<std::string, Texture*> textures;  // Textures like diffuse, normal, etc.
    glm::vec3   albedo{1.0f};
    glm::vec3   emission = glm::vec3(0.0f, 0.0f, 0.0f) * 0.0f; //Emission color * Emission power
    //float       metallic; //0.0f dialetics - 1.0f metallic
    //float       transparency = 1.0f;
    //float       refractionIndex = 1.0f;
    float       roughness = 1.0f;
    //float       absorbance?
    
    float       restitution = 1.0f; //from 0 to 1, 1 being no energy loss in collision and 0 being all energy lost
    //float       friction; //coefficienct of friction
    //float       density;
    
    /*
     float      conductivity
     int        atomicNumber
     int        atomicMass
     float      reactivity
     float      plasticity //0.0f = brittle - 1.0f = malleable
     float      tensile strength
                shear
                compreesion
     
     
     thermal and phase properties ?
     
     */
    
};
