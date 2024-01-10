

#include <sstream>
#include <vector>
#include "Middleware.h"
#include <ctime>
#include <iomanip>

int convertStringToInt(std::string &str) {
    int result;
    std::stringstream ss;
    ss << str;
    ss >> result;
    return result;
}

double convertStringToDouble(std::string &str) {
    double result;
    std::stringstream ss;
    ss << str;
    ss >> result;
    return result;
}

DateTime* convertStringToDate(std::string &str) {
    std::stringstream ss {str};
    std::string word;
    std::vector<std::string> wordList;

    // Split the parameter by '/'
    while (std::getline(ss, word, '/')) {
        wordList.push_back(word);
    }

    // Get the date parameter
    int day = convertStringToInt(wordList[0]);
    int month = convertStringToInt(wordList[1]);
    int year = convertStringToInt(wordList[2]);

    // Create a day object
    auto *result = new Date(day, month, year);

    return result;
}

bool isNumber(std::string &str) {
    for (int i = 0; i < str.length(); i++) {
        if (i == 0){
            if (str[i] == '-'){
                continue;
            }
        }
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// DateTime* getCurrentDate(){
//     time_t t = std::time(nullptr);
//     tm tm = *std::localtime(&t);

//     std::stringstream ss;
//     ss << std::put_time(&tm, "%d/%m/%Y");
//     std::string str = ss.str();
//     return convertStringToDate(str);
// }

bool isDouble(std::string &str){
    int dotCount = 0;
    for (int i = 0; i < str.length(); i++){
        if (i == 0 && str[i] == '-') {
            continue;
        }
        if (str[i] == '.'){
            dotCount++;
        }
        if (str[i] != '.' && !std::isdigit(str[i])){
            return false;
        }
    }
    if (dotCount > 1){
        return false;
    }
    return true;
}