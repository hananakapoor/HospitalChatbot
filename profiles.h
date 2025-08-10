#ifndef __PROFILES_H__
#define __PROFILES_H__

#include <map>
#include <string>
#include "tree.h" 

enum class CRITERION_TYPE { Strong, Weak };

struct criterion_state {
    CRITERION_TYPE active;
    STRONG_CRITERION strong;
    WEAK_CRITERION weak;
};


class profiles {
private:
    unsigned int last_profile_id;
    unsigned int max_number_of_profiles;
    std::map<unsigned int, tree*> profile_id_to_tree;

public:
    profiles(unsigned int _max_number_of_profiles) 
        : max_number_of_profiles(_max_number_of_profiles), last_profile_id(0) {
        clear_all_profiles();
    }

    ~profiles() {
        for (auto& pair : profile_id_to_tree) {
            delete pair.second;
        }
        profile_id_to_tree.clear();
    }

    void clear_all_profiles() {
        last_profile_id = 0;
        for (auto& pair : profile_id_to_tree) {
            delete pair.second;
        }
        profile_id_to_tree.clear();
    }

    unsigned int init_a_profile() {
        last_profile_id = (last_profile_id + 1) % max_number_of_profiles;
        tree* obj_tree = new tree();
        profile_id_to_tree[last_profile_id] = obj_tree;
        return last_profile_id;
    }



void update_a_profile(unsigned int profile_id, const std::string& _value = "") {
    auto it = profile_id_to_tree.find(profile_id);
    if (it != profile_id_to_tree.end()) {
        tree* user_tree = it->second;
        
        std::string varName = "input" + std::to_string(user_tree->tree_to_map().size() + 1);
        
        // Check if node already exists
        if (user_tree->tree_to_map().find(varName) != user_tree->tree_to_map().end()) {
            return;  // Node already exists, so do nothing
        }

        if (_value == "No" || _value == "Yes") {
            user_tree->add_a_minor_node(varName, _value);
        } else {
            user_tree->add_a_major_node(varName);
            user_tree->set_current_node(varName);
        }
    }
}


// Add method to print all profiles and their trees
void print_all_profiles() const {
    for (const auto& [pid, t] : profile_id_to_tree) {
        std::cout << "Profile ID: " << pid << std::endl;
        t->print_tree();
        std::cout << "-------------------" << std::endl;
    }
}
    void remove_a_profile(unsigned int profile_id) {
        auto it = profile_id_to_tree.find(profile_id);
        if (it != profile_id_to_tree.end()) {
            delete it->second;
            profile_id_to_tree.erase(it);
        }
    }

    std::map<std::string, std::string> get_profile(unsigned int profile_id) const {
        auto it = profile_id_to_tree.find(profile_id);
        if (it != profile_id_to_tree.end()) {
            return it->second->tree_to_map();
        }
        return {};
    }

    std::string get_tree_query(unsigned int profile_id) const {
        auto it = profile_id_to_tree.find(profile_id);
        if (it != profile_id_to_tree.end()) {
            return it->second->tree_to_query();
        }
        return "";
    }

    const tree* get_tree(unsigned int profile_id) const {
        auto it = profile_id_to_tree.find(profile_id);
        if (it != profile_id_to_tree.end()) {
            return it->second;
        }
        return nullptr;
    }

    tree* get_tree(unsigned int profile_id) {
        auto it = profile_id_to_tree.find(profile_id);
        if (it != profile_id_to_tree.end()) {
            return it->second;
        }
        return nullptr;
    }
// 
//     unsigned int init_a_profile() {
//         last_profile_id = (last_profile_id + 1) % max_number_of_profiles;
//         tree* obj_tree = new tree();  // Create a new tree for this profile
//         profile_id_to_tree[last_profile_id] = obj_tree;  // Store the tree in the map
//         std::cout << "Initialized Profile ID: " << last_profile_id << " with new tree at address: " << obj_tree << std::endl;
//         return last_profile_id;
//     }



    void print_profile_trees(unsigned int profile_id) const {
        auto it = profile_id_to_tree.find(profile_id);
        if (it != profile_id_to_tree.end()) {
            std::cout << "Tree for Profile ID: " << profile_id << std::endl;
            it->second->print_tree();
        } else {
            std::cerr << "Error: Profile ID " << profile_id << " not found!" << std::endl;
        }
    }

//     
// // // Add method to print all profiles and their trees
// // void print_all_profiles() const {
// //     for (const auto& [pid, t] : profile_id_to_tree) {
// //         std::cout << "Profile ID: " << pid << std::endl;
// //         t->print_tree();
// //         std::cout << "-------------------" << std::endl;
// //     }
// // }
    // void print_profile_trees(unsigned int profile_id) const {
    //     auto it = profile_id_to_tree.find(profile_id);
    //     if (it != profile_id_to_tree.end()) {
    //         std::cout << "Trees for Profile ID: " << profile_id << std::endl;
    //         int tree_count = 1;
    //         for (const tree* t : it->second) {
    //             std::cout << "Tree " << tree_count++ << " at address: " << t << std::endl;
    //             t->print_tree();
    //             std::cout << "-------------------" << std::endl;
    //         }
    //     } else {
    //         std::cerr << "Error: Profile ID " << profile_id << " not found!" << std::endl;
    //     }
    // }

//     std::string get_tree_query(unsigned int profile_id) const {
//         auto it = profile_id_to_trees.find(profile_id);
//         if (it != profile_id_to_trees.end()) {
//             std::string all_queries;
//             for (const tree* t : it->second) {
//                 all_queries += t->tree_to_query() + "\n";
//             }
//             return all_queries;
//         }
//         return "";
//     }



//     void print_profile_trees(unsigned int profile_id) const {
//         auto it = profile_id_to_trees.find(profile_id);
//         if (it != profile_id_to_trees.end()) {
//             std::cout << "Trees for Profile ID: " << profile_id << std::endl;
//             int tree_count = 1;
//             for (const tree* t : it->second) {
//                 std::cout << "Tree " << tree_count++ << " at address: " << t << std::endl;
//                 t->print_tree();
//                 std::cout << "-------------------" << std::endl;
//             }
//         } else {
//             std::cerr << "Error: Profile ID " << profile_id << " not found!" << std::endl;
//         }
//     }

//     std::string get_tree_query(unsigned int profile_id) const {
//         auto it = profile_id_to_trees.find(profile_id);
//         if (it != profile_id_to_trees.end()) {
//             std::string all_queries;
//             for (const tree* t : it->second) {
//                 all_queries += t->tree_to_query() + "\n";
//             }
//             return all_queries;
//         }
//         return "";
//     }
// tree* get_tree(unsigned int profile_id) {
//     auto it = profile_id_to_trees.find(profile_id);
//     if (it != profile_id_to_trees.end() && !it->second.empty()) {
//         return it->second.back();  // Return the latest tree
//     }
//     return nullptr;
// }
//     const tree* get_tree(unsigned int profile_id) const {
//         auto it = profile_id_to_trees.find(profile_id);
//         if (it != profile_id_to_trees.end() && !it->second.empty()) {
//             return it->second.back();
//         }
//         return nullptr;
//     }


};
#endif 
