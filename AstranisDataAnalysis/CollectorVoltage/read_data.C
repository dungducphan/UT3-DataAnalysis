#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

std::vector<std::string> getAllTxtFiles(const std::string &folderPath) {
    std::vector<std::string> txtFiles;
    std::string searchPath = folderPath + "\\*.txt";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening directory: " << folderPath << std::endl;
        return txtFiles;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            txtFiles.push_back(findFileData.cFileName);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return txtFiles;
}

void alignDate(const std::string &date) {
    std::string folderPath = date; // Update this path accordingly
    auto list = getAllTxtFiles(folderPath);

    std::map<int, std::string> sequenceNumberToTimestamp;
    std::map<int, float> sequenceNumberToVoltage;
    std::map<int, float> sequenceNumberToSamplingLength;
    for (const auto &fileName : list) {
        std::string filePath = folderPath + "\\" + fileName;
        std::cout << "Processing file: " << filePath << std::endl;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            continue;
        }

        std::string line;
        for (int i = 0; i < 4 && std::getline(file, line); ++i) {
            // Read lines until the fourth line
        }

        if (!line.empty()) {
            std::istringstream iss(line);
            std::string token;
            int tokenIndex = 0;
            while (std::getline(iss, token, ',')) {
                if (tokenIndex == 1) {
                    std::string timestamp = token.substr(12); // Extract the time part from file's header content
                    std::string sequenceNumber = fileName.substr(14, 5); // Extract the sequence number from the file's name
                    std::cout << "Sequence number: " << sequenceNumber << ", Timestamp: " << timestamp << std::endl;
                    sequenceNumberToTimestamp[std::stoi(sequenceNumber)] = timestamp;
                    break;
                }
                tokenIndex++;
            }
        }

        float minVoltage = 9E9;
        float minTime = 9E99;
        float maxTime = -9E99;
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string firstFloat, secondFloat;
            std::getline(iss, firstFloat, ',');
            std::getline(iss, secondFloat, ',');
            float firstNumber = std::stod(firstFloat);
            float secondNumber = std::stod(secondFloat);
            minVoltage = std::min(minVoltage, secondNumber);
            minTime = std::min(minTime, firstNumber);
            maxTime = std::max(maxTime, firstNumber);
        }
        sequenceNumberToSamplingLength[std::stoi(fileName.substr(14, 5))] = maxTime - minTime;
        sequenceNumberToVoltage[std::stoi(fileName.substr(14, 5))] = minVoltage;

        file.close();
    }

    std::cout << "Size of sequenceNumberToTimestamp: " << sequenceNumberToTimestamp.size() << std::endl;

    std::ofstream csvFile(date + ".sampleLength.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Error opening CSV file: " << date << ".csv" << std::endl;
        return;
    }
    
    for (int i = 0; i < 2000; i++) {
        if (sequenceNumberToTimestamp.find(i) == sequenceNumberToTimestamp.end()) {
            continue;
        }
        csvFile << i << "," << sequenceNumberToTimestamp[i] << "," << sequenceNumberToVoltage[i] << "," << sequenceNumberToSamplingLength[i] << "\n";
    }

    csvFile.close();
}

void calculateDriftCharge(const std::string &date) {
    std::string folderPath = date; // Update this path accordingly
    auto list = getAllTxtFiles(folderPath);

    std::map<int, std::string> sequenceNumberToTimestamp;
    std::map<int, float> sequenceNumberToDriftCharge;
    std::map<int, float> sequenceNumberToVoltage;
    std::map<int, float> sequenceNumberToSamplingLength;
    for (const auto &fileName : list) {
        std::string filePath = folderPath + "\\" + fileName;
        std:;cout << "Processing file: " << filePath << std::endl;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            continue;
        }

        std::string line;
        for (int i = 0; i < 4 && std::getline(file, line); ++i) {
            // Read lines until the fourth line
        }

        if (!line.empty()) {
            std::istringstream iss(line);
            std::string token;
            int tokenIndex = 0;
            while (std::getline(iss, token, ',')) {
                if (tokenIndex == 1) {
                    std::string timestamp = token.substr(12); // Extract the time part from file's header content
                    std::string sequenceNumber = fileName.substr(14, 5); // Extract the sequence number from the file's name
                    std::cout << "Sequence number: " << sequenceNumber << ", Timestamp: " << timestamp << std::endl;
                    sequenceNumberToTimestamp[std::stoi(sequenceNumber)] = timestamp;
                    break;
                }
                tokenIndex++;
            }
        }

        float minVoltage = 9E9;
        float driftCharge = 0;
        float previousTime = 0;
        float minTime = 9E99;
        float maxTime = -9E99;
        bool ignoreFirstLine = true;
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string firstFloat, secondFloat;
            std::getline(iss, firstFloat, ',');
            std::getline(iss, secondFloat, ',');
            float firstNumber = std::stod(firstFloat);
            float secondNumber = std::stod(secondFloat);
            minVoltage = std::min(minVoltage, secondNumber);
            minTime = std::min(minTime, firstNumber);
            maxTime = std::max(maxTime, firstNumber);
            if (!ignoreFirstLine) {
                driftCharge += secondNumber * (firstNumber - previousTime);
            } else {
                ignoreFirstLine = false;
            }
            previousTime = firstNumber;
        }
        sequenceNumberToSamplingLength[std::stoi(fileName.substr(14, 5))] = maxTime - minTime;
        sequenceNumberToDriftCharge[std::stoi(fileName.substr(14, 5))] = driftCharge;
        sequenceNumberToVoltage[std::stoi(fileName.substr(14, 5))] = minVoltage;

        file.close();
    }

    std::cout << "Size of sequenceNumberToTimestamp: " << sequenceNumberToTimestamp.size() << std::endl;

    std::ofstream csvFile(date + ".driftCharge.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Error opening CSV file: " << date << ".csv" << std::endl;
        return;
    }
    
    for (int i = 0; i < 2000; i++) {
        if (sequenceNumberToTimestamp.find(i) == sequenceNumberToTimestamp.end()) {
            continue;
        }
        csvFile << i << "," << sequenceNumberToTimestamp[i] << "," << sequenceNumberToVoltage[i] << "," << sequenceNumberToDriftCharge[i] << "," << sequenceNumberToSamplingLength[i] <<"\n";
    }

    csvFile.close();
}

void read_data() {
    // alignDate("20250311");
    // alignDate("20250310");
    // alignDate("20250304");
    // alignDate("20250303");
    // alignDate("20250227");
    // alignDate("20250226");
    // alignDate("20250225");
    // alignDate("20250224");
    // alignDate("20250220");
    // alignDate("20250219");

    // calculateDriftCharge("20250311");
    // calculateDriftCharge("20250310");
    // calculateDriftCharge("20250304");
    // calculateDriftCharge("20250303");
    calculateDriftCharge("20250227");
    // calculateDriftCharge("20250226");
    // calculateDriftCharge("20250225");
    // calculateDriftCharge("20250224");
    // calculateDriftCharge("20250220");
    // calculateDriftCharge("20250219");
}