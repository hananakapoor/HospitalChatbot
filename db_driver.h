
#ifndef _DB_DRIVER_H_
#define _DB_DRIVER_H_
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <mysql/mysql.h>
#include <sstream>
#include <vector>
#include "tree.h"
#include <set>


class db_driver {
private:
    const std::string Password;
    const std::string User_id;
    const std::string IP;
    const std::string Port;
    const std::string Database;
    MYSQL* conn;
    int current_state;



    void db_connect() {
        conn = mysql_init(nullptr);
        if (conn == nullptr) {
            std::cerr << "mysql_init() failed\n";
            return;
        }

        if (mysql_real_connect(conn, IP.c_str(), User_id.c_str(), Password.c_str(), nullptr, stoi(Port), nullptr, 0) == nullptr) {
            std::cerr << "Connection failed: " << mysql_error(conn) << std::endl;
            mysql_close(conn);
            conn = nullptr;
            return;
        }

        if (mysql_select_db(conn, Database.c_str())) {
            std::cerr << "Failed to select database '" << Database << "': " << mysql_error(conn) << std::endl;
            mysql_close(conn);
            conn = nullptr;
            return;
        }

        std::cout << "Connected to MySQL database '" << Database << "' successfully." << std::endl;
    }


public:
    db_driver(const std::string& password, const std::string& user_id, const std::string& ip, const std::string& port, const std::string& database)
        : Password(password), User_id(user_id), IP(ip), Port(port), Database(database), conn(nullptr) { // Initialize basic_services with max agents
        db_connect();
    }

    ~db_driver() {
        if (conn != nullptr) {
            mysql_close(conn);
            std::cout << "Database connection closed." << std::endl;
        }
    }

    void show_databases() {
        if (mysql_query(conn, "SHOW DATABASES")) {
            std::cerr << "SHOW DATABASES failed: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            std::cerr << "Failed to retrieve databases: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_ROW row;
        std::cout << "Databases:" << std::endl;
        while ((row = mysql_fetch_row(res))) {
            std::cout << row[0] << std::endl;
        }

        mysql_free_result(res);
    }

    void create_tables() {
        const char* queries[] = {
           "CREATE TABLE IF NOT EXISTS States (ID INT PRIMARY KEY, Description VARCHAR(100), Message TEXT);",
           "CREATE TABLE IF NOT EXISTS Transitions (FromState INT, Answer VARCHAR(100), ToState INT);"
        };

        for (const auto& query : queries) {
            if (mysql_query(conn, query)) {
                std::cerr << "Failed to execute query: " << query << "\nError: " << mysql_error(conn) << std::endl;
            } else {
                std::cout << "Table created successfully." << std::endl;
            }
        }
    }
    std::vector<std::string> get_all_answers() {
        std::vector<std::string> answers;
        std::string query = "SELECT DISTINCT Answer FROM Transitions";
        
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "Failed to get answers: " << mysql_error(conn) << std::endl;
            return answers;
        }
        
        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            std::cerr << "Failed to retrieve answers: " << mysql_error(conn) << std::endl;
            return answers;
        }
        
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            answers.push_back(row[0]);
        }
        
        mysql_free_result(res);
        return answers;
    }
    void show_questions_and_answers(const std::string& question_table, const std::string& answer_table) {
        std::ostringstream query;
        query << "SELECT q.que_id, q.question, a.answer FROM " << question_table << " q "
              << "LEFT JOIN " << answer_table << " a ON q.que_id = a.que_id";

        if (mysql_query(conn, query.str().c_str())) {
            std::cerr << "Query execution failed: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            std::cerr << "Failed to retrieve data: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_ROW row;
        std::cout << "Questions and Answers:" << std::endl;
        while ((row = mysql_fetch_row(res))) {
            std::cout << "Q" << row[0] << ": " << row[1] << "\n  A: " << (row[2] ? row[2] : "No answer available") << std::endl;
        }

        mysql_free_result(res);
    }


std::pair<int, std::string> first_state() {
    std::ostringstream oss;
    std::string query1 = "SELECT ID, Message FROM States WHERE ID = 1";
    
    if (mysql_query(conn, query1.c_str())) {
        oss << "Failed to get first state: " << mysql_error(conn) << '\n';
        return {-1, oss.str()};
    }

    MYSQL_RES* res1 = mysql_store_result(conn);
    if (res1 == nullptr) {
        oss << "Failed to retrieve first state: " << mysql_error(conn) << '\n';
        return {-1, oss.str()};
    }

    MYSQL_ROW row1 = mysql_fetch_row(res1);
    int state_id = -1;
    std::string message;

    if (row1 != nullptr) {
        state_id = std::stoi(row1[0]);
        message = row1[1];
        oss << "  " << message << '\n';
        // service.to_voice(message);
    }

    mysql_free_result(res1);

    return {state_id, oss.str()};
}

    

std::string sec_state() {
    std::ostringstream oss;
    current_state = 2;
    std::string query1 = "SELECT ID, Message FROM States WHERE ID = 2";

    if (mysql_query(conn, query1.c_str())) {
        oss << "Failed to get first state: " << mysql_error(conn) << '\n';
        return oss.str();
    }

    MYSQL_RES* res1 = mysql_store_result(conn);
    if (res1 == nullptr) {
        oss << "Failed to retrieve first state: " << mysql_error(conn) << '\n';
        return oss.str();
    }

    MYSQL_ROW row1 = mysql_fetch_row(res1);
    int state_id = -1;

    if (row1 != nullptr) {
        state_id = std::stoi(row1[0]);


        
        std::string message = row1[1];
        oss << "  " << row1[1] << '\n';
        // service.to_voice(message);
    }

    mysql_free_result(res1);
    return oss.str();
}

    


std::pair<int, std::string> next_state(int current_state, const std::string& answer) {
        std::ostringstream query;
        query << "SELECT t.ToState, s.Message FROM Transitions t "
              << "JOIN States s ON t.ToState = s.ID "
              << "WHERE t.FromState = " << current_state 
              << " AND t.Answer = '" << answer << "'";

        if (mysql_query(conn, query.str().c_str())) {
            std::cerr << "Failed to get next state: " << mysql_error(conn) << std::endl;
            return {-1, ""};
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) {
            std::cerr << "Failed to retrieve next state: " << mysql_error(conn) << std::endl;
            return {-1, ""};
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        int state_id = current_state ;
        std::string message;

        if (row) {
            state_id = std::stoi(row[0]);
            message = row[1];
        }

        mysql_free_result(res);
        return {state_id, message};
    }

std::string get_questionss(int state_id) {
    std::ostringstream oss;
    std::ostringstream query;
    query << "SELECT Message FROM States WHERE ID = " << state_id;

    if (mysql_query(conn, query.str().c_str())) {
        oss << "Failed to get question: " << mysql_error(conn) << '\n';
        return oss.str();
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr) {
        oss << "Failed to retrieve question: " << mysql_error(conn) << '\n';
        return oss.str();
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row != nullptr) {
        oss << " : " << row[0] << '\n';
        // service.to_voice(row[0]);
    }

    mysql_free_result(res);
    return oss.str();
}

    
   
    std::set<int> get_states_without_transitions() {
        std::set<int> allStates;
        std::set<int> fromStates;
        std::set<int> resultSet;
    
        // Fetch all states from the States table
        std::string query1 = "SELECT ID FROM States";
        if (mysql_query(conn, query1.c_str())) {
            std::cerr << "Error fetching States: " << mysql_error(conn) << std::endl;
            return resultSet;
        }
        
        MYSQL_RES* res1 = mysql_store_result(conn);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res1))) {
            allStates.insert(std::stoi(row[0]));
        }
        mysql_free_result(res1);
    
        // Fetch all FromState values from the Transitions table
        std::string query2 = "SELECT DISTINCT FromState FROM Transitions";
        if (mysql_query(conn, query2.c_str())) {
            std::cerr << "Error fetching FromStates: " << mysql_error(conn) << std::endl;
            return resultSet;
        }
    
        MYSQL_RES* res2 = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res2))) {
            fromStates.insert(std::stoi(row[0]));
        }
        mysql_free_result(res2);
    
        // Compute the difference: (allStates - fromStates)
        for (int state : allStates) {
            if (fromStates.find(state) == fromStates.end()) {
                resultSet.insert(state);
            }
        }
    
        return resultSet;
    }
   
    std::string get_question() {
        std::ostringstream query;
        query << "SELECT Message FROM States WHERE ID = " << current_state;

        if (mysql_query(conn, query.str().c_str())) {
            std::cerr << "Failed to get question: " << mysql_error(conn) << std::endl;
            return "";
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            std::cerr << "Failed to retrieve question: " << mysql_error(conn) << std::endl;
            return "";
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        std::string question = "";

        if (row != nullptr) {
            question = row[0];
        }

        mysql_free_result(res);
        return question;
    }
    int last_state() {
        std::string query1 = "SELECT MAX(ID) FROM States";
        
        if (mysql_query(conn, query1.c_str())) {
            std::cerr << "Failed to get last state: " << mysql_error(conn) << std::endl;
            return -1;
        }
    
        MYSQL_RES* res1 = mysql_store_result(conn);
        if (res1 == nullptr) {
            std::cerr << "Failed to retrieve last state: " << mysql_error(conn) << std::endl;
            return -1;
        }
    
        MYSQL_ROW row1 = mysql_fetch_row(res1);
        int state_id = -1;
    
        if (row1 != nullptr && row1[0] != nullptr) {
            state_id = std::stoi(row1[0]);
        }
    
        mysql_free_result(res1);
        
        return state_id;
    }
    


    int last_prev_state() {
        std::string query1 = "SELECT MAX(ID) - 1 FROM States";
    
        if (mysql_query(conn, query1.c_str())) {
            std::cerr << "Failed to get second last state: " << mysql_error(conn) << std::endl;
            return -1;
        }
    
        MYSQL_RES* res1 = mysql_store_result(conn);
        if (res1 == nullptr) {
            std::cerr << "Failed to retrieve second last state: " << mysql_error(conn) << std::endl;
            return -1;
        }
    
        MYSQL_ROW row1 = mysql_fetch_row(res1);
        int state_id = -1;
    
        if (row1 != nullptr && row1[0] != nullptr) {
            state_id = std::stoi(row1[0]);
        }
    
        mysql_free_result(res1);
    
        // Fetch the message for the retrieved state_id
        if (state_id != -1) {
            std::string query2 = "SELECT Message FROM States WHERE ID = " + std::to_string(state_id);
            
            if (mysql_query(conn, query2.c_str())) {
                std::cerr << "Failed to get state message: " << mysql_error(conn) << std::endl;
                return -1;
            }
    
            MYSQL_RES* res2 = mysql_store_result(conn);
            if (res2 == nullptr) {
                std::cerr << "Failed to retrieve state message: " << mysql_error(conn) << std::endl;
                return -1;
            }
    
            MYSQL_ROW row2 = mysql_fetch_row(res2);
            if (row2 != nullptr && row2[0] != nullptr) {
                std::string message = row2[0];
                std::cout << "State ID " << state_id << " Message: " << message << std::endl;
                // service.to_voice(message);
            }
    
            mysql_free_result(res2);
        }
    
        return state_id;
    }
    
    
bool has_transitions(int state_id) {
    std::ostringstream query;
    query << "SELECT COUNT(*) FROM Transitions WHERE FromState = " << state_id;

    if (mysql_query(conn, query.str().c_str())) {
        std::cerr << "Failed to check transitions: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr) {
        std::cerr << "Failed to retrieve transition count: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    bool has_trans = false;
    
    if (row != nullptr) {
        has_trans = (std::stoi(row[0]) > 0);
    }

    mysql_free_result(res);
    return has_trans;
}

    int get_current_state() const {
        return current_state;
    }





    void execute_select_query(const std::string& query) {
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "Query execution failed: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_RES* result = mysql_store_result(conn);
        if (!result) {
            std::cerr << "Failed to retrieve result set: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            std::cout << "Message: " << row[0] << std::endl;
        }

        mysql_free_result(result);
    }


};

#endif // _DB_DRIVER_H_