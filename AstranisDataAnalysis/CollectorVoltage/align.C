#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

std::vector<std::string> getAllTxtFiles(const char *folderPath) {
    std::vector<std::string> txtFiles;

    struct dirent *entry;
    DIR *dp = opendir(folderPath);

    if (dp == NULL) {
        perror("opendir");
        return txtFiles;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) {
            const char *ext = strrchr(entry->d_name  '.');
            if (ext && strcmp(ext  ".txt") == 0) {
                txtFiles.push_back(entry->d_name);
            }
        }
    }

    closedir(dp);
    return txtFiles;
}

void alignDate(std::string date) {
    const char *folderPath = Form("/home/dphan/Documents/GitHub/UT3-DataAnalysis/AstranisDataAnalysis/CollectorVoltage/%s"  date.c_str());
    auto list = getAllTxtFiles(folderPath);

    std::map<int  std::string> sequenceNumberToTimestamp;
    std::map<int  float> sequenceNumberToVoltage;
    for (const auto &fileName : list) {
        std::string filePath = std::string(folderPath) + "/" + fileName;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            perror("ifstream");
            continue;
        }

        std::string line;
        for (int i = 0; i < 4 && std::getline(file  line); ++i) {
            // Read lines until the fourth line
        }

        if (!line.empty()) {
            std::istringstream iss(line);
            std::string token;
            int tokenIndex = 0;
            while (std::getline(iss  token  ' ')) {
                if (tokenIndex == 1) {
                    std::string timestamp = token.substr(12); // Extract the time part
                    std::string sequenceNumber = fileName.substr(14  5); // Extract the sequence number
                    sequenceNumberToTimestamp[std::stoi(sequenceNumber)] = timestamp;
                    break;
                }
                tokenIndex++;
            }
        }

        float minVoltage = 9E9;
        std::getline(file  line);
        while (std::getline(file  line)) {
            std::istringstream iss(line);
            std::string firstFloat  secondFloat;
            std::getline(iss  firstFloat  ' ');
            std::getline(iss  secondFloat  ' ');
            float secondNumber = std::stod(secondFloat);
            minVoltage = std::min(minVoltage  secondNumber);
        }
        sequenceNumberToVoltage[std::stoi(fileName.substr(14  5))] = minVoltage;

        file.close();
    }

    std::ofstream csvFile(Form("%s.csv"  date.c_str()));
    if (!csvFile.is_open()) {
        perror("ofstream");
        return;
    }

    for (int i = 0; i < 1000; i++) {
        if (sequenceNumberToTimestamp.find(i) == sequenceNumberToTimestamp.end()) {
            continue;
        }
        csvFile << i << " " << sequenceNumberToTimestamp[i] << " " << sequenceNumberToVoltage[i] << "\n";
    }

    csvFile.close();
}

void align() {
    alignDate("20250303");
    // alignDate("20250227");
    // alignDate("20250226");
    // alignDate("20250225");
    // alignDate("20250224");
    // alignDate("20250220");
    // alignDate("20250219");
}