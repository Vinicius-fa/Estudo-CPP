#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <fstream>

struct Sensor {
    std::string name;
    double value;
    double warningThreshold;
    double criticalThreshold;
    std::string unit;
};

std::string getStatus(Sensor s) {
    if(s.value >= s.criticalThreshold) {
        return "CRITICAL";
    } else if (s.value >= s.warningThreshold) {
        return "WARNING";
    } else {
        return "OK";
    }
}

void logAlert(Sensor& s) {
    time_t now = time(0);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    std::ofstream logFile("alerts.log", std::ios::app);
    logFile << "[" << timestamp << "] " << getStatus(s) << " - " << s.name << ": " << s.value << " " << s.unit << "\n";
    logFile.close();

}

int main() {

    srand(time(0));

    std::vector<Sensor> sensors;

    sensors.push_back({"Motor Temp", 72.5, 80.0, 95.0, "C"});
    sensors.push_back({"Car Temp", 43.7, 40.0, 65.0, "A"});
    sensors.push_back({"Room Temp", 67.8, 45.0, 60.0, "B"});

    std::cout << "System Monitor Starting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (true) {

        for (Sensor& s : sensors) {
            s.value += (rand() %100 - 50) / 10.0;
        }

        for (Sensor s : sensors) {
            std::cout << s.name << " | " << s.value << " " << s.unit << " | " << getStatus(s) << "\n";

            if (getStatus(s) != "OK") {
                logAlert(s);
            }
        }
        std::cout << "--- refresh ---\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    }
    return 0;
}