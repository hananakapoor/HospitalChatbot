#ifndef _CALL_MANAGER_H_
#define _CALL_MANAGER_H_

#include <iostream>
#include <string>
#include "db_driver1.h"
#include "profiles.h"
#include "basic_services.h"


class CallManager {
private:
    db_driver* db; 
    basic_services service;             
    profiles profile_manager;      
    int current_state;             
    unsigned int profile_id;      
    int tockenCounter;
public:
    CallManager(const std::string& password, const std::string& user_id, 
                const std::string& ip, const std::string& port, 
                const std::string& database, unsigned int max_profiles, unsigned int max_agents)
        : profile_manager(max_profiles), service(max_agents),tockenCounter(1000) {
        db = new db_driver(password, user_id, ip, port, database);
        auto result = db->first_state();
current_state = result.first;
std::cout << result.second; 
// service.to_voice(result.second);  // Store the state ID

    std::string output =db->sec_state();
    std::cout << output;
    // service.to_voice(output);
        profile_id = profile_manager.init_a_profile();  
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


std::pair<bool, int> bookDoctor(const std::string& patientName, const std::string& patientPhone, int doctorId, const std::string& bookingDate, const std::string& bookingTime) {
    // Check if the doctor exists
    std::ostringstream checkQuery;
    checkQuery << "SELECT doctor_id FROM Doctors WHERE doctor_id = " << doctorId;

    if (mysql_query(db->getConnection(), checkQuery.str().c_str())) {
        std::cerr << "Failed to check doctor: " << mysql_error(db->getConnection()) << std::endl;
        return {false, 0};  // Return failure and token number 0
    }

    MYSQL_RES* res = mysql_store_result(db->getConnection());
    if (!res || mysql_num_rows(res) == 0) {
        std::cerr << "Doctor with ID " << doctorId << " does not exist." << std::endl;
        mysql_free_result(res);
        return {false, 0};  // Return failure and token number 0
    }
    mysql_free_result(res);

    // Get the current token number for the doctor
    std::ostringstream tokenQuery;
    tokenQuery << "SELECT current_token FROM token_counter WHERE doctor_id = " << doctorId << " FOR UPDATE";

    if (mysql_query(db->getConnection(), tokenQuery.str().c_str())) {
        std::cerr << "Failed to get current token: " << mysql_error(db->getConnection()) << std::endl;
        return {false, 0};  // Return failure and token number 0
    }

    MYSQL_RES* tokenRes = mysql_store_result(db->getConnection());
    if (!tokenRes) {
        std::cerr << "Failed to retrieve current token: " << mysql_error(db->getConnection()) << std::endl;
        return {false, 0};  // Return failure and token number 0
    }

    MYSQL_ROW tokenRow = mysql_fetch_row(tokenRes);
    int tokenNumber = 1;  // Default token number if no entry exists

    if (tokenRow) {
        tokenNumber = std::stoi(tokenRow[0]);
    } else {
        // If no entry exists, insert a new row with the default token number
        std::ostringstream insertQuery;
        insertQuery << "INSERT INTO token_counter (doctor_id, current_token) VALUES (" << doctorId << ", 1)";

        if (mysql_query(db->getConnection(), insertQuery.str().c_str())) {
            std::cerr << "Failed to insert token counter: " << mysql_error(db->getConnection()) << std::endl;
            mysql_free_result(tokenRes);
            return {false, 0};  // Return failure and token number 0
        }
    }

    mysql_free_result(tokenRes);

    // Increment the token number for the next booking
    std::ostringstream updateQuery;
    updateQuery << "UPDATE token_counter SET current_token = current_token + 1 WHERE doctor_id = " << doctorId;

    if (mysql_query(db->getConnection(), updateQuery.str().c_str())) {
        std::cerr << "Failed to update token counter: " << mysql_error(db->getConnection()) << std::endl;
        return {false, 0};  // Return failure and token number 0
    }

    // Insert the booking with the token number
    std::ostringstream query;
    query << "INSERT INTO PatientBookings (patient_name, patient_phone, doctor_id, booking_date, booking_time, token_number) VALUES ('"
          << patientName << "', '" << patientPhone << "', " << doctorId << ", '" << bookingDate << "', '" << bookingTime << "', " << tokenNumber << ")";

    std::cout << "Executing query: " << query.str() << std::endl; // Log the query

    if (mysql_query(db->getConnection(), query.str().c_str())) {
        std::cerr << "Failed to book doctor: " << mysql_error(db->getConnection()) << std::endl; // Log the error
        return {false, 0};  // Return failure and token number 0
    }

    std::cout << "Doctor booked successfully! Token Number: " << tokenNumber << std::endl;
    return {true, tokenNumber};  // Return success and the token number
}
int get_next_question(const std::string& answer) {
        profile_manager.update_a_profile(profile_id, answer);
        std::string varName = "input" + std::to_string(profile_manager.get_tree(profile_id)->tree_to_map().size() + 1);

        if (answer == "No" || answer=="Yes")
        { 
            profile_manager.get_tree(profile_id)->add_a_minor_node(varName, answer);

        }
        else{
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
        std::cout << "Chatbot printing treeeeeee\n";
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
        //db->show_databases();
        std::cout<<"print ";
    }

    const tree::tree_node* get_tree_root() const {
        return profile_manager.get_tree(profile_id)->get_root();
    }
 std::string runChatbot(const std::string& userInput) {
        getCurrentQuestion();
        get_next_question("Service Selection");
        getCurrentQuestion();

    
        if (current_state == 29) {
        // Capture patient details
        std::vector<std::string> details;
        std::istringstream iss(userInput);
        std::string detail;

        // Split the input by comma
        while (std::getline(iss, detail, ',')) {
            // Remove leading and trailing whitespace from each detail
            detail.erase(0, detail.find_first_not_of(' '));
            detail.erase(detail.find_last_not_of(' ') + 1);
            details.push_back(detail);
        }

        // Validate the input
        if (details.size() == 5) {
            std::string patientName = details[0];
            std::string patientPhone = details[1];
            int doctorId = 0;
            std::string bookingDate = details[3];
            std::string bookingTime = details[4];

            // Validate phone number (10 digits)
            if (patientPhone.length() != 10 || patientPhone.find_first_not_of("0123456789") != std::string::npos) {
                return "Invalid phone number. Please provide a valid 10-digit phone number.";
            }

            // Validate doctor ID (must be a positive integer)
            try {
                doctorId = std::stoi(details[2]);
                if (doctorId <= 0) {
                    return "Invalid Doctor ID. Please provide a valid positive integer.";
                }
            } catch (const std::exception& e) {
                return "Invalid Doctor ID. Please provide a valid integer.";
            }

            // Validate booking date (YYYY-MM-DD format)
            if (bookingDate.length() != 10 || bookingDate[4] != '-' || bookingDate[7] != '-') {
                return "Invalid booking date. Please provide the date in YYYY-MM-DD format.";
            }

            // Validate booking time (HH:MM:SS format)
            if (bookingTime.length() != 8 || bookingTime[2] != ':' || bookingTime[5] != ':') {
                return "Invalid booking time. Please provide the time in HH:MM:SS format.";
            }

            // Book the doctor and capture the token number
            auto bookingResult = bookDoctor(patientName, patientPhone, doctorId, bookingDate, bookingTime);
            if (bookingResult.first) {
                return "Booking successful! Your token number is: " + std::to_string(bookingResult.second) + ". Thank you for using our services.";
            } else {
                return "Booking failed. Please try again.";
            }
        } else {
            return "Invalid input. Please provide all details in the following format:\nName, Phone Number, Doctor ID, Booking Date (YYYY-MM-DD), Booking Time (HH:MM:SS)";
        }
    }

        while (true) {
        

            std::string varName = "input" + std::to_string(profile_manager.get_tree(profile_id)->tree_to_map().size() + 1);

            int nextState = get_next_question(userInput);
            std::cout<<"433333";
            std::cout << nextState << userInput << std::endl;
if (nextState != -1) {
    std::string output = getQuestionByState(nextState);
    std::cout << "Bot: " << output << std::endl;

    /*if (nextState == 6 || nextState == 7 || nextState == 8 || nextState == 9 ||
        nextState == 10 || nextState == 11 || nextState == 12 || nextState == 13 ||
        nextState == 14 || nextState == 15 || nextState == 17 || nextState == 18 ||
        nextState == 19 || nextState == 20 || nextState == 21 || nextState == 22 ||
        nextState == 23 || nextState == 24 || nextState == 25 || nextState == 26) {*/
        
        if(nextState == 18 || nextState == 19 || nextState == 20 || nextState == 23 || nextState == 24 || nextState == 22) {
        
        /*if(nextState == 18 || nextState == 19 || nextState == 20 || nextState == 21 || nextState == 22) {*/
        
        nextState = 27;
        std::string nextOutput = getQuestionByState(nextState);
        std::cout << "Bot: " << nextOutput << std::endl;
        // return nextOutput;
        return output + "\n \n \n" + nextOutput;
    }

    return output;
}
            else if (nextState == 27) {
               std:: cout<<"23333";
                std::cout << "Bot: Are you satisfied with the details provided?" << std::endl;


                if (userInput == "No") {
                    nextState = 2; 
                    std::cout << nextState<< std::endl;;

                     std::string output =getQuestionByState(nextState);
                 std::cout << output;
                 std::cout<<"3333";
                return output;

                    continue;
                } else if (userInput == "Yes") {
                    nextState = 28; 
                    std::cout << nextState<< std::endl;;

                    std::cout << "Bot: Thank you for choosing our chatbot. Have a great day!" << std::endl;
                    break;
                } else {
                    std::cout << "Bot: Please respond with 'Yes' or 'No'." << std::endl;
                    continue;
                }
            }

            if (nextState == 28) {
                            std::cout << nextState<< std::endl;;

                std::cout << "Bot1: Thank you for choosing our chatbot. Have a great day!" << std::endl;
                break;
            }

            if (nextState == -1) {
                std::cout << "Bot2: Let me check if I've answered your question." << std::endl;
                nextState = 27;
                std::cout << nextState<< std::endl;;

                // getQuestionByState(nextState);
                 std::string output =getQuestionByState(nextState);
                 std::cout << output;
                return output;

                //  service.to_voice(output);
                continue;
            }

            if (!hasTransitions(nextState)) {
                std::cout << "Bot: It seems there are no further questions for this path." << std::endl;
                nextState = 27; 
                // getQuestionByState(nextState);
                 std::string output =getQuestionByState(nextState);
                 std::cout << output;
                return output;

                //  service.to_voice(output);
                continue;
            }
        }

        printquery();
    }
   
};
#endif  
