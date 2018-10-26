//
// Created by  ngs on 19/10/2018.
//

#ifndef STATNLP_LABEL_H
#define STATNLP_LABEL_H

#include <iostream>
#include <unordered_map>
#include <vector>

enum LabelType {
    TERMINAL,
    NON_TERMINAL
};


class Label {
public:
    //rember to release these pointers.
    inline static std::unordered_map<std::string, Label *> *ptr_labels_map_ = new std::unordered_map<std::string, Label *>;
    inline static std::unordered_map<int, Label *> *ptr_id2label_map_ = new std::unordered_map<int, Label *>;
    inline static std::vector<Label *> *ptr_label_ = new std::vector<Label *>;
    //inline static std::unordered_map<Label*, int> *ptr_lebel2id_map_ = new std::unordered_map<Label*, int>;
    //inline static std::vector<std::string> *ptr_form_vec_ = new std::vector<std::string>;

    inline Label(std::string form, int id) {
        form_ = form;
        id_ = id;
        type_ = LabelType::TERMINAL;
    }

    inline ~Label() {

    }

    /**
     *
     * get the pointer of label by its form (such as NN, VP)
     *
     */
    inline static Label *Get(std::string form, bool isTerminal) {
        Label *ptr_lb = nullptr;
        auto it = ptr_labels_map_->find(form);
        if (it == ptr_labels_map_->end()) {
            int idx = ptr_labels_map_->size();
            ptr_lb = new Label(form, idx);
            if (isTerminal) {
                ptr_lb->SetType(LabelType::TERMINAL);
            } else {
                ptr_lb->SetType(LabelType::NON_TERMINAL);
            }
            ptr_labels_map_->insert(std::make_pair(form, ptr_lb));
            ptr_id2label_map_->insert(std::make_pair(ptr_lb->GetId(), ptr_lb));
            ptr_label_->push_back(ptr_lb);
        } else {
            ptr_lb = (*it).second;
        }
        return ptr_lb;
    }

    /**
     * Get the pointer of the label by its form.
     * @param form
     * @return
     */

    inline static Label *Get(std::string form){
        Label *ptr_label = ptr_labels_map_->find(form)->second;
        return ptr_label;
    }

    /**
     *
     * Get the pointer of label by its index, noted that this function is called after ptr_labels_idx_map_ is finalized.
     * @param idx
     * @return
     *
     */
    inline static Label *Get(int idx) {
        Label *ptr_label = ptr_id2label_map_->find(idx)->second;
        return ptr_label;
    }

    inline LabelType GetType() {
        return type_;
    }

    inline int GetId() {
        return id_;
    }

    inline void SetId(int id) {
        id_ = id;
    }

    inline std::string GetForm() {
        return form_;
    }

    inline void SetForm(std::string form) {
        form_ = form;
    }

    inline void SetType(LabelType type) {
        type_ = type;
    }

private:
    std::string form_;
    int id_;
    LabelType type_;

};

#endif //STATNLP_LABEL_H
