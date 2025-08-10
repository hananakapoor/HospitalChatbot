
#ifndef __TREE_H__
#define __TREE_H__

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <sstream>


enum class STRONG_CRITERION { Init_strong, Other_strong, CRITERION_TYPE_USER_RESPONSE };
enum class WEAK_CRITERION { Init_weak, Other_weak };
struct Node {
    std::string value;
    std::vector<std::string> minors;
};
class tree {
public:
    struct tree_node;

    struct auxiliary_node;
    struct Node {
        std::string value;                 
        std::vector<std::string> minors;   
    };

    
    struct tree_node {
    tree_node* parent = nullptr;
    tree_node* child = nullptr;
    tree_node* sibling = nullptr;  
    auxiliary_node* minor_child = nullptr;

    STRONG_CRITERION criterion = STRONG_CRITERION::Init_strong;
    std::string var_name;  
    std::string value = "";
    bool is_leaf = false;

    tree_node(tree_node* _parent = nullptr,
             tree_node* _child = nullptr,
             tree_node* _sibling = nullptr,  
             auxiliary_node* _minor_child = nullptr,
             STRONG_CRITERION _criterion = STRONG_CRITERION::Init_strong,
             const std::string& _var_name = "",
             const std::string& _value = "",
             bool _is_leaf = false)
        : parent(_parent),
          child(_child),
          sibling(_sibling),  
          minor_child(_minor_child),
          criterion(_criterion),
          var_name(_var_name),
          value(_value),
          is_leaf(_is_leaf) {}

    void clear() {
        criterion = STRONG_CRITERION::Init_strong;
        var_name = "";
        value = "";
        is_leaf = false;
    }
};


    struct auxiliary_node {
        auxiliary_node* minor_parent = nullptr;
        auxiliary_node* minor_child = nullptr;
        tree_node* major_parent = nullptr;

        WEAK_CRITERION criterion = WEAK_CRITERION::Init_weak;
        std::string var_name;
        std::string value = "";
        bool is_leaf = false;

        auxiliary_node(auxiliary_node* _minor_parent = nullptr,
                      auxiliary_node* _minor_child = nullptr,
                      tree_node* _major_parent = nullptr,
                      WEAK_CRITERION _criterion = WEAK_CRITERION::Init_weak,
                      const std::string& _var_name = "",
                      const std::string& _value = "",
                      bool _is_leaf = false)
            : minor_parent(_minor_parent),
              minor_child(_minor_child),
              major_parent(_major_parent),
              criterion(_criterion),
              var_name(_var_name),
              value(_value),
              is_leaf(_is_leaf) {}

        void clear() {
            criterion = WEAK_CRITERION::Init_weak;
            var_name = "";
            value = "";
            is_leaf = false;
        }
    };
    
    tree_node* root = nullptr;
    tree_node* current_major_node = nullptr;
    auxiliary_node* current_minor_node = nullptr;

    std::list<tree_node> list_of_major_nodes;
    std::list<auxiliary_node> list_of_minor_nodes;
    std::map<std::string, std::string> all_variables;
    std::map<std::string, Node> major_nodes;  

   

public:
    tree() { reset_tree(); }

    ~tree() {
        list_of_major_nodes.clear();
        list_of_minor_nodes.clear();
    }
std::map<std::string, std::string> tree_to_map() const {
    std::map<std::string, std::string> result;
    for (const auto& [key, value] : all_variables) {
        result[key] = value;
    }
    return result;
}
    void reset_tree() {
        current_minor_node = nullptr;
        list_of_major_nodes.clear();
        list_of_minor_nodes.clear();
        all_variables.clear();
        major_nodes.clear();  
        add_first_major_node();
    }

    void add_first_major_node() {
        tree_node new_node(nullptr, nullptr, nullptr,nullptr, STRONG_CRITERION::Init_strong, "var00.0", "", false);
        list_of_major_nodes.push_back(new_node);
        root = &list_of_major_nodes.back();
        current_major_node = root;
        major_nodes["var00.0"] = Node{"", {}};  
    }

    
void add_a_major_node(const std::string& var_name) {
        tree_node new_node(current_major_node, nullptr, nullptr, nullptr, STRONG_CRITERION::Init_strong, var_name, "", false);
        list_of_major_nodes.push_back(new_node);
        current_major_node->child = &list_of_major_nodes.back();
        current_major_node = current_major_node->child;
        major_nodes[var_name] = Node{"", {}};
    }

    void set_current_node(const std::string& value) {
        current_major_node->value = value;
        all_variables[current_major_node->var_name] = value;
        major_nodes[current_major_node->var_name].value = value;
    }

    void add_a_minor_node(const std::string& var_name, const std::string& value) {
        auxiliary_node new_node(nullptr, nullptr, current_major_node, WEAK_CRITERION::Init_weak, var_name, value, false);
        list_of_minor_nodes.push_back(new_node);
        all_variables[var_name] = value;
        
        if (!current_major_node->minor_child) {
            current_major_node->minor_child = &list_of_minor_nodes.back();
        } else {
            auxiliary_node* last = current_major_node->minor_child;
            while (last->minor_child) {
                last = last->minor_child;
            }
            last->minor_child = &list_of_minor_nodes.back();
        }
        
        if (current_major_node && !current_major_node->var_name.empty()) {
            major_nodes[current_major_node->var_name].minors.push_back(value);
        }
    }
   
    
  

std::string tree_to_query() const {
    std::vector<std::string> node_values;
    std::string all_queries;
    const tree_node* current = root;
    while (current) { 
        if (!current->value.empty() && current != root) {
            bool is_minor = false;
            const tree_node* minor_check = current->child;
            while (minor_check) {
                if (minor_check->value == "Minor: Yes") {
                    is_minor = true;
                    break;
                }
                minor_check = minor_check->sibling;
            }
            
            if (!is_minor) {
                node_values.push_back(current->value);
            } else {
                if (node_values.size() >= 3) {
                    for (size_t i = 1; i < node_values.size(); i += 3) {
                        if (i + 2 < node_values.size()) {
                            std::string query = "SELECT message FROM services WHERE state = '" + 
                                              node_values[i] + "' AND category = '" + 
                                              node_values[i + 1] + "' AND sub_category = '" + 
                                              node_values[i + 2] + "';\n";
                            all_queries += query;
                        }
                    }
                }
                node_values.clear();
            }
        }
        
        if (current->child) {
            current = current->child;
        } else if (current->sibling) {
            current = current->sibling;
        } else {
            while (current->parent && !current->sibling) {
                current = current->parent;
            }
            current = current->sibling;
        }
    }
    
    if (node_values.size() >= 3) {
        for (size_t i = 1; i < node_values.size(); i += 3) {
            if (i + 2 < node_values.size()) {
                std::string query = "SELECT message FROM services WHERE state = '" + 
                                  node_values[i] + "' AND category = '" + 
                                  node_values[i + 1] + "' AND sub_category = '" + 
                                  node_values[i + 2] + "';\n";
                all_queries += query;
            }
        }
    }
    
    return all_queries;
}

    



std::string print_tree() const {
    std::ostringstream oss;
    oss << "User Inputs Stored in Tree:\n";
    for (const auto& [key, node] : major_nodes) {
        if (!node.value.empty()) {
            oss << key << ": " << node.value << '\n';
        }
    }

  

    oss << "\nFull Tree Structure:\n";
    print_tree_structure(root, oss);
    return oss.str();
}

const tree_node* get_root() const {
    return root;
}

private:
void print_tree_structure(const tree_node* node, std::ostringstream& oss) const {
    if (!node) return;
    if (!node->value.empty()) {
        oss << node->var_name << ": " << node->value << '\n';
        
        auxiliary_node* minor = node->minor_child;
        while (minor) {
            oss << "  - Minor: " << minor->value << '\n';
            minor = minor->minor_child;
        }
    }
    if (node->child) {
        print_tree_structure(node->child, oss);
    }
}
};

#endif