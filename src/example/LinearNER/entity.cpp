//
// Created by  ngs on 06/10/2018.
//

#include "entity.h"
std::unordered_map<std::string, Entity*> * Entity::ptr_entities_map_ = new std::unordered_map<std::string, Entity*>;
std::unordered_map<int, Entity*> * Entity::ptr_entities_idx_map_ = new std::unordered_map<int, Entity*>;

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
 * return the pointer of the Entity if it is existed, otherwise create an instance of Entity and
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