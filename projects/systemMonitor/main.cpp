#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <random>

struct Sensor {
    std::string name;
    double value;
    double warningThreshold;
    double criticalThreshold;
    std::string unit;
    double minValue;
    double maxValue;
    double totalValue;
    int readingCount;
};

std::string getStatus(const Sensor& s) {
    if(s.value >= s.criticalThreshold) {
        return "CRITICAL";
    } else if (s.value >= s.warningThreshold) {
        return "WARNING";
    } else {
        return "OK";
    }
}

std::vector<Sensor> loadSensors(const std::string& filename) {
    std::vector<Sensor> sensors;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open " << filename << "\n";
        return sensors;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> fields;

        while (std::getline(ss, token, ',')) {
            fields.push_back(token);
        }

        if (fields.size() < 5) continue;

        try {
            std::string name  = fields[0];
            double value      = std::stod(fields[1]);
            double warning    = std::stod(fields[2]);
            double critical   = std::stod(fields[3]);
            std::string unit  = fields[4];

            sensors.push_back({name, value, warning, critical, unit, value, value, value, 1});
        } catch (const std::exception& e) {
            std::cerr << "Erro ao converter dados numéricos na linha: " << line << "\n";
            continue;
        }
    }

    file.close();
    return sensors;
}

void logAlert(const Sensor& s, std::ofstream& logFile) {
    time_t now = time(0);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    if (logFile.is_open()) {
        logFile << "[" << timestamp << "] " << getStatus(s) << " - " << s.name << ": " << s.value << " " << s.unit << "\n";
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0);


    std::vector<Sensor> sensors = loadSensors("sensors.csv");

    if (sensors.empty()) {
        std::cerr << "No sensors loaded. Check sensors.csv.\n";
        return 1;
    }

    std::ofstream logFile("alerts.log", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Falha ao abrir arquivo de log de alertas.\n";
    }

    std::cout << "System Monitor Starting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (true) {

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif


        for (Sensor& s : sensors) {
            s.value += dis(gen);
            if (s.value < s.minValue) s.minValue = s.value;
            if (s.value > s.maxValue) s.maxValue = s.value;
            s.totalValue += s.value;
            s.readingCount++;
        }

        for (const Sensor& s : sensors) {
            std::cout << s.name << " | " << s.value << " " << s.unit
                      << " | " << getStatus(s) 
                      << " | min: " << s.minValue
                      << " max: " << s.maxValue
                      << " avg: " << s.totalValue / s.readingCount << "\n";

            if (getStatus(s) != "OK") {
                logAlert(s, logFile);
            }
        }
        
        logFile.flush();

        std::cout << "--- refresh ---\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }

    return 0;
}