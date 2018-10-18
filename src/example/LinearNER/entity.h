//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_ENTITY_H
#define STATNLP_ENTITY_H

#include <iostream>
#include <unordered_map>
#include <vector>

class Entity{
public:
    Entity(std::string form, int id);
    Entity(Entity *ptr_lbl);
    ~Entity();
    static Entity *Get(std::string form);
    static Entity *Get(int index);
    static std::unordered_map<std::string, Entity*> *ptr_entities_map_;
    static std::unordered_map<int, Entity*> *ptr_entities_idx_map_;
    static std::vector<Entity *> *ptr_entity_;
    static void GenerateEntityVector();
    std::string GetForm();
private:
    std::string form_;
    int id_;
};

#endif //STATNLP_ENTITY_H

