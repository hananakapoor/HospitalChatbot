// #ifndef _CALL_MANAGER_H_
// #define _CALL_MANAGER_H_

// #include <iostream>
// #include <string>
// #include <algorithm>
// #include <limits>
// #include <cctype>
// #include <vector>
// #include <set>
// #include "db_driver.h"
// #include "profiles.h"
// #include "basic_services.h"

// class CallManager {
// private:
//     db_driver* db; 
//     basic_services service;             
//     profiles profile_manager;      
//     int current_state;             
//     unsigned int profile_id;
    
//     // Predefined list of valid words
//     std::vector<std::string> keywords = {
//         "Service Selection", "Loan", "Deposit", "Gold Loan", "Housing Loan",
//         "Interest Rate", "Maximum Loan Amount", "Required Documents",
//         "Minimum Purity", "Loan Tenure", "Janamithram Gold Loan",
//         "Premium Gold Loan", "EMI Options", "Early Repayment", "Deposit Types",
//         "Fixed Deposit", "Fixed Deposit Interest Rate", "Senior Citizen Rates",
//         "Short Term Deposit Minimum", "Prize Money Deposit",
//         "Sugama Security Deposit", "Early Withdrawal",
//         "Short Term Deposit Duration", "Open Fixed Deposit Account",
//         "Deposit Documents", "Yes", "No"
//     };
    
//     // Convert a string to lowercase
//     std::string toLowerCase(const std::string &s) {
//         std::string lower = s;
//         std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
//         return lower;
//     }
    
//     // Function to compute Levenshtein Distance
//     int levenshteinDistance(const std::string &s1, const std::string &s2) {
//         int m = s1.size(), n = s2.size();
//         std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

//         for (int i = 0; i <= m; ++i) dp[i][0] = i;
//         for (int j = 0; j <= n; ++j) dp[0][j] = j;

//         for (int i = 1; i <= m; ++i) {
//             for (int j = 1; j <= n; ++j) {
//                 if (s1[i - 1] == s2[j - 1])
//                     dp[i][j] = dp[i - 1][j - 1];
//                 else
//                     dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
//             }
//         }
//         return dp[m][n];
//     }
    
//     // Function to find the closest keyword using Levenshtein Distance (Case-Insensitive)
//     std::string findClosestMatch(const std::string &input) {
//         int minDistance = std::numeric_limits<int>::max();
//         std::string bestMatch = "";
        
//         std::string inputLower = toLowerCase(input);

//         for (const auto &keyword : keywords) {
//             int dist = levenshteinDistance(inputLower, toLowerCase(keyword));
//             if (dist < minDistance) {
//                 minDistance = dist;
//                 bestMatch = keyword;
//             }
//         }

//         if (minDistance <= 2) // Only suggest if edit distance is small
//             return bestMatch;
        
//         return "No close match found!";
//     }
    
//     // Function to check if input is a substring of any keyword (Case-Insensitive)
//     std::string findPartialMatch(const std::string &input) {
//         std::string inputLower = toLowerCase(input);

//         for (const auto &keyword : keywords) {
//             if (toLowerCase(keyword).find(inputLower) != std::string::npos) { // Check case-insensitive substring match
//                 return keyword;
//             }
//         }
//         return findClosestMatch(input); // Fallback to Levenshtein distance
//     }

// public:
//     CallManager(const std::string& password, const std::string& user_id, 
//                 const std::string& ip, const std::string& port, 
//                 const std::string& database, unsigned int max_profiles, unsigned int max_agents)
//         : profile_manager(max_profiles), service(max_agents) {  
//         db = new db_driver(password, user_id, ip, port, database);
//         auto result = db->first_state();
//         current_state = result.first;
//         std::cout << result.second; 
//         // service.to_voice(result.second);  // Store the state ID

//         std::string output = db->sec_state();
//         std::cout << output;
//         // service.to_voice(output);
//         profile_id = profile_manager.init_a_profile();  
//     }

//     ~CallManager() {
//         if (db != nullptr) {
//             delete db;
//         }
//     }

//     void initializeTables() {
//         db->create_tables();
//     }

//     std::string getCurrentQuestion() {
//         return db->get_question();
//     }

//     std::string getQuestionByState(int stateId) {
//         current_state = stateId;
//         return db->get_questionss(stateId);
//     }

//     int get_next_question(const std::string& answer) {
//         profile_manager.update_a_profile(profile_id, answer);
//         std::string varName = "input" + std::to_string(profile_manager.get_tree(profile_id)->tree_to_map().size() + 1);

//         if (answer == "No" || answer == "Yes") { 
//             profile_manager.get_tree(profile_id)->add_a_minor_node(varName, answer);
//         } else {
//             profile_manager.get_tree(profile_id)->add_a_major_node(varName);
//             profile_manager.get_tree(profile_id)->set_current_node(answer);
//         }
        
//         auto result = db->next_state(current_state, answer);
//         int nextState = result.first;

//         if (nextState == -1) {
//             nextState = 27;
//         }

//         current_state = nextState;
//         return current_state;
//     }

//     std::pair<int, std::string> get_first_question() {
//         try {
//             auto result = db->first_state();
//             current_state = result.first;
//             std::string output = db->sec_state();
//             return {current_state, result.second + "\n" + output};
//         } catch (const std::exception& e) {
//             throw std::runtime_error("Error in get_first_question: " + std::string(e.what()));
//         }
//     }
    
//     bool isLastState() {
//         return current_state == db->last_state();
//     }

//     void printUserTree() const {
//         profile_manager.get_tree(profile_id)->print_tree();
//         std::cout << "KSFE Chatbot printing treeeeeee\n";
//         std::cout << "-------------------\n";
//         profile_manager.print_all_profiles();
//     }

//     void printquery() {
//         std::string queries = profile_manager.get_tree_query(profile_id);
//         std::cout << "Generated Queries for Profile ID: " << profile_id << std::endl;
//         std::cout << queries << std::endl;
        
//         std::istringstream query_stream(queries);
//         std::string single_query;
        
//         while (std::getline(query_stream, single_query)) {
//             if (single_query.empty()) {
//                 continue;
//             }
            
//             std::cout << "Executing Query: " << single_query << std::endl;
//             db->execute_select_query(single_query);
//         }
//     }

//     int getCurrentState() const {
//         return current_state;
//     }

//     void showQuestionsAndAnswers(const std::string& question_table, 
//                                   const std::string& answer_table) {
//         db->show_questions_and_answers(question_table, answer_table);
//     }

//     bool hasTransitions(int stateId) {
//         return db->has_transitions(stateId);
//     }

//     void showDatabases() {
//         db->show_databases();
//     }

//     const tree::tree_node* get_tree_root() const {
//         return profile_manager.get_tree(profile_id)->get_root();
//     }

//     std::string runChatbot(const std::string& userInput) {
//         // Process user input with keyword matching
//         std::string processedInput = findPartialMatch(userInput);
        
//         std::cout << "User Input: " << userInput << std::endl;
//         std::cout << "Processed Input: " << processedInput << std::endl;
        
//         // If no close match was found, continue with original input
//         if (processedInput == "No close match found!") {
//             processedInput = userInput;
//         }
        
//         // Initialize chatbot if needed
//         getCurrentQuestion();
//         get_next_question("Service Selection");
//         getCurrentQuestion();
    
//         int prevState = db->last_prev_state();
//         int finalState = db->last_state();
//         std::set<int> leafStates = db->get_states_without_transitions();
    
//         while (true) {
//             // Use the processed input for state transitions
//             int nextState = get_next_question(processedInput);
            
//             std::cout << "Next State: " << nextState << ", Processed Input: " << processedInput << std::endl;
    
//             if (nextState != -1) {
//                 std::string output = getQuestionByState(nextState);
//                 std::cout << "Bot: " << output << std::endl;
    
//                 if (leafStates.find(nextState) != leafStates.end()) {
//                     if (nextState == finalState) {
//                         // For the final state, return only the farewell message
//                         nextState = prevState;
//                         std::string nextOutput = getQuestionByState(nextState);
//                         std::cout << "Bot: " << nextOutput << std::endl;
//                         return output;
//                     } else {
//                         // For other leaf states, return the output without appending additional text
//                         nextState = prevState;
//                         std::string nextOutput = getQuestionByState(nextState);
//                         std::cout << "Bot: " << nextOutput << std::endl;
//                         return output + "\n\n" + nextOutput;
//                     }
//                 }
    
//                 return output;
//             } 
//             else if (nextState == prevState) { 
//                 if (nextState != finalState) { 
//                     std::cout << "Bot: Are you satisfied with the details provided?" << std::endl;
//                     if (processedInput == "No") {
//                         nextState = 2;
//                         std::string output = getQuestionByState(nextState);
//                         std::cout << "Bot: " << output << std::endl;
//                         return output;
//                     } else if (processedInput == "Yes") {
//                         nextState = finalState;
//                         std::cout << "Bot: Thank you for choosing KSFE. Have a great day!" << std::endl;
//                         break;
//                     } else {
//                         std::cout << "Bot: Please respond with 'Yes' or 'No'." << std::endl;
//                         continue;
//                     }
//                 }
//             }
    
//             if (nextState == finalState) {
//                 std::cout << "Bot: Thank you for choosing KSFE. Have a great day!" << std::endl;
//                 break;
//             }
    
//             if (nextState == -1) {
//                 std::cout << "Bot: Let me check if I've answered your question." << std::endl;
//                 nextState = prevState;
//                 std::string output = getQuestionByState(nextState);
//                 std::cout << output << std::endl;
//                 return output;
//             }
    
//             if (!hasTransitions(nextState)) {
//                 std::cout << "Bot: It seems there are no further questions for this path." << std::endl;
//                 nextState = prevState;
//                 std::string output = getQuestionByState(nextState);
//                 std::cout << output << std::endl;
//                 return output;
//             }
//         }
    
//         printquery();
//         return "Thank you for using KSFE Chatbot.";
//     }
// };
// #endif
#ifndef _CALL_MANAGER_H_
#define _CALL_MANAGER_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>
#include <vector>
#include <set>
#include "db_driver.h"
#include "profiles.h"
#include "basic_services.h"

class CallManager {
private:
    db_driver* db; 
    basic_services service;             
    profiles profile_manager;      
    int current_state;             
    unsigned int profile_id;
    std::vector<std::string> keywords; // Will be populated from database
    
    // Convert a string to lowercase
    std::string toLowerCase(const std::string &s) {
        std::string lower = s;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }
    
    // Function to compute Levenshtein Distance
    int levenshteinDistance(const std::string &s1, const std::string &s2) {
        int m = s1.size(), n = s2.size();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

        for (int i = 0; i <= m; ++i) dp[i][0] = i;
        for (int j = 0; j <= n; ++j) dp[0][j] = j;

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (s1[i - 1] == s2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
            }
        }
        return dp[m][n];
    }
    
    // Function to find the closest keyword using Levenshtein Distance (Case-Insensitive)
    std::string findClosestMatch(const std::string &input) {
        int minDistance = std::numeric_limits<int>::max();
        std::string bestMatch = "";
        
        std::string inputLower = toLowerCase(input);

        for (const auto &keyword : keywords) {
            int dist = levenshteinDistance(inputLower, toLowerCase(keyword));
            if (dist < minDistance) {
                minDistance = dist;
                bestMatch = keyword;
            }
        }

        if (minDistance <= 2) // Only suggest if edit distance is small
            return bestMatch;
        
        return "No close match found!";
    }
    
    // Function to check if input is a substring of any keyword (Case-Insensitive)
    std::string findPartialMatch(const std::string &input) {
        std::string inputLower = toLowerCase(input);

        for (const auto &keyword : keywords) {
            if (toLowerCase(keyword).find(inputLower) != std::string::npos) { // Check case-insensitive substring match
                return keyword;
            }
        }
        return findClosestMatch(input); // Fallback to Levenshtein distance
    }

    // Load keywords from database
    void loadKeywords() {
        keywords = db->get_all_answers();
    }

public:
    CallManager(const std::string& password, const std::string& user_id, 
                const std::string& ip, const std::string& port, 
                const std::string& database, unsigned int max_profiles, unsigned int max_agents)
        : profile_manager(max_profiles), service(max_agents) {  
        db = new db_driver(password, user_id, ip, port, database);
        auto result = db->first_state();
        current_state = result.first;
        std::cout << result.second; 
        // service.to_voice(result.second);  // Store the state ID

        std::string output = db->sec_state();
        std::cout << output;
        // service.to_voice(output);
        profile_id = profile_manager.init_a_profile();
        
        // Load keywords from database
        loadKeywords();
    }

    ~CallManager() {
        if (db != nullptr) {
            delete db;
        }
    }

    void initializeTables() {
        db->create_tables();
    }

    std::string getCurrentQuestion() {
        return db->get_question();
    }

    std::string getQuestionByState(int stateId) {
        current_state = stateId;
        return db->get_questionss(stateId);
    }

    int get_next_question(const std::string& answer) {
        profile_manager.update_a_profile(profile_id, answer);
        std::string varName = "input" + std::to_string(profile_manager.get_tree(profile_id)->tree_to_map().size() + 1);

        if (answer == "No" || answer == "Yes") { 
            profile_manager.get_tree(profile_id)->add_a_minor_node(varName, answer);
        } else {
            profile_manager.get_tree(profile_id)->add_a_major_node(varName);
            profile_manager.get_tree(profile_id)->set_current_node(answer);
        }
        
        auto result = db->next_state(current_state, answer);
        int nextState = result.first;

        if (nextState == -1) {
            nextState = 27;
        }

        current_state = nextState;
        return current_state;
    }

    std::pair<int, std::string> get_first_question() {
        try {
            auto result = db->first_state();
            current_state = result.first;
            std::string output = db->sec_state();
            return {current_state, result.second + "\n" + output};
        } catch (const std::exception& e) {
            throw std::runtime_error("Error in get_first_question: " + std::string(e.what()));
        }
    }
    
    bool isLastState() {
        return current_state == db->last_state();
    }

    void printUserTree() const {
        profile_manager.get_tree(profile_id)->print_tree();
        std::cout << "KSFE Chatbot printing treeeeeee\n";
        std::cout << "-------------------\n";
        profile_manager.print_all_profiles();
    }

    void printquery() {
        std::string queries = profile_manager.get_tree_query(profile_id);
        std::cout << "Generated Queries for Profile ID: " << profile_id << std::endl;
        std::cout << queries << std::endl;
        
        std::istringstream query_stream(queries);
        std::string single_query;
        
        while (std::getline(query_stream, single_query)) {
            if (single_query.empty()) {
                continue;
            }
            
            std::cout << "Executing Query: " << single_query << std::endl;
            db->execute_select_query(single_query);
        }
    }

    int getCurrentState() const {
        return current_state;
    }

    void showQuestionsAndAnswers(const std::string& question_table, 
                                  const std::string& answer_table) {
        db->show_questions_and_answers(question_table, answer_table);
    }

    bool hasTransitions(int stateId) {
        return db->has_transitions(stateId);
    }

    void showDatabases() {
        db->show_databases();
    }

    const tree::tree_node* get_tree_root() const {
        return profile_manager.get_tree(profile_id)->get_root();
    }

    // Refresh keywords from database
    void refreshKeywords() {
        loadKeywords();
    }

    std::string runChatbot(const std::string& userInput) {
        // Process user input with keyword matching
        std::string processedInput = findPartialMatch(userInput);
        
        std::cout << "User Input: " << userInput << std::endl;
        std::cout << "Processed Input: " << processedInput << std::endl;
        
        // If no close match was found, continue with original input
        if (processedInput == "No close match found!") {
            processedInput = userInput;
        }
        
        // Initialize chatbot if needed
        getCurrentQuestion();
        get_next_question("Service Selection");
        getCurrentQuestion();
    
        int prevState = db->last_prev_state();
        int finalState = db->last_state();
        std::set<int> leafStates = db->get_states_without_transitions();
    
        while (true) {
            // Use the processed input for state transitions
            int nextState = get_next_question(processedInput);
            
            std::cout << "Next State: " << nextState << ", Processed Input: " << processedInput << std::endl;
    
            if (nextState != -1) {
                std::string output = getQuestionByState(nextState);
                std::cout << "Bot: " << output << std::endl;
    
                if (leafStates.find(nextState) != leafStates.end()) {
                    if (nextState == finalState) {
                        // For the final state, return only the farewell message
                        nextState = prevState;
                        std::string nextOutput = getQuestionByState(nextState);
                        std::cout << "Bot: " << nextOutput << std::endl;
                        return output;
                    } else {
                        // For other leaf states, return the output without appending additional text
                        nextState = prevState;
                        std::string nextOutput = getQuestionByState(nextState);
                        std::cout << "Bot: " << nextOutput << std::endl;
                        return output + "\n\n" + nextOutput;
                    }
                }
    
                return output;
            } 
            else if (nextState == prevState) { 
                if (nextState != finalState) { 
                    std::cout << "Bot: Are you satisfied with the details provided?" << std::endl;
                    if (processedInput == "No") {
                        nextState = 2;
                        std::string output = getQuestionByState(nextState);
                        std::cout << "Bot: " << output << std::endl;
                        return output;
                    } else if (processedInput == "Yes") {
                        nextState = finalState;
                        std::cout << "Bot: Thank you for choosing KSFE. Have a great day!" << std::endl;
                        break;
                    } else {
                        std::cout << "Bot: Please respond with 'Yes' or 'No'." << std::endl;
                        continue;
                    }
                }
            }
    
            if (nextState == finalState) {
                std::cout << "Bot: Thank you for choosing KSFE. Have a great day!" << std::endl;
                break;
            }
    
            if (nextState == -1) {
                std::cout << "Bot: Let me check if I've answered your question." << std::endl;
                nextState = prevState;
                std::string output = getQuestionByState(nextState);
                std::cout << output << std::endl;
                return output;
            }
    
            if (!hasTransitions(nextState)) {
                std::cout << "Bot: It seems there are no further questions for this path." << std::endl;
                nextState = prevState;
                std::string output = getQuestionByState(nextState);
                std::cout << output << std::endl;
                return output;
            }
        }
    
        printquery();
        return "Thank you for using KSFE Chatbot.";
    }
};
#endif