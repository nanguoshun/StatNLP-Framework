//
// Created by  ngs on 06/10/2018.
//

#include "entity.h"

std::unordered_map<std::string, Entity*> * Entity::ptr_entities_map_ = new std::unordered_map<std::string, Entity*>;
std::unordered_map<int, Entity*> * Entity::ptr_entities_idx_map_ = new std::unordered_map<int, Entity*>;
std::vector<Entity *> * Entity::ptr_entity_ = new std::vector<Entity *>;

Entity::Entity(Entity *ptr_lbl) {

}

Entity::Entity(std::string form, int id) {
    form_ = form;
    id_ = id;
}

Entity::~Entity() {

}

/***
 *
 * Return the pointer of the Entity if it is existed, otherwise create an instance of Entity and
 * insert it into ptr_entities_map_ and ptr_entities_idx_map_.
 *
 * @param form
 * @return
 *
 */
Entity* Entity::Get(std::string form) {
    auto it = ptr_entities_map_->find(form);
    if(it == ptr_entities_map_->end()){
        Entity *ptr_label = new Entity(form,ptr_entities_map_->size());
        ptr_entities_map_->insert(std::make_pair(form,ptr_label));
        ptr_entities_idx_map_->insert(std::make_pair(ptr_label->id_,ptr_label));
    }
    return ptr_entities_map_->find(form)->second;
}

Entity* Entity::Get(int index) {
   //todo:
}

std::string Entity::GetForm() {
    return form_;
}

void Entity::GenerateEntityVector() {
    ptr_entity_->push_back(Entity::Get("START_TAG"));
    //std::cout << "0 th label is "<<(*ptr_entity_)[0]->GetForm()<<std::endl;
    //int i = 1;
    for(auto it = ptr_entities_map_->begin(); it != ptr_entities_map_->end(); ++it ){
        if(0 == (*it).second->GetForm().compare("START_TAG")) { continue;}
        ptr_entity_->push_back((*it).second);
      //  std::cout << i << " th label is "<<(*it).second->GetForm()<<std::endl;
       // i++;
    }
    ptr_entity_->push_back(Entity::Get("END_TAG"));
    //std::cout << i << " th label is "<<(*ptr_entity_)[i]->GetForm()<<std::endl;
}