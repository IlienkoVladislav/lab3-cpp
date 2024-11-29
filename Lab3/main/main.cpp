#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <random>

// Структура для збереження інформації про процес
struct Process {
    int id;             // ID процесу
    int arrivalTime;    // Час прибуття
    int burstTime;      // Час виконання
    int priority;       // Пріоритет
    int remainingTime;  // Залишковий час виконання
    int startTime;      // Час початку виконання
    int endTime;        // Час завершення виконання
    int waitingTime;    // Час очікування
    int age;            // Лічильник для механізму старіння
};

// Функція для генерації випадкових процесів
std::vector<Process> generateProcesses(int numProcesses) {
    std::vector<Process> processes;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> arrivalDist(0, 30); // Час прибуття
    std::uniform_int_distribution<> burstDist(1, 21);   // Час виконання
    std::uniform_int_distribution<> priorityDist(1, 6); // Пріоритет

    for (int i = 0; i < numProcesses; ++i) {
        processes.push_back({
            i + 1,
            arrivalDist(gen),
            burstDist(gen),
            priorityDist(gen),
            0,
            0,
            0,
            0,
            0
            });
        processes.back().remainingTime = processes.back().burstTime; // Ініціалізація залишкового часу
    }

    return processes;
}

// Функція для виведення результатів
void printProcesses(const std::vector<Process>& processes) {
    std::cout << std::setw(5) << "ID" << std::setw(15) << "Arrival Time"
        << std::setw(15) << "Burst Time" << std::setw(10) << "Priority"
        << std::setw(15) << "Waiting Time" << std::setw(15) << "Turnaround Time" << std::endl;

    for (const auto& p : processes) {
        std::cout << std::setw(5) << p.id
            << std::setw(15) << p.arrivalTime
            << std::setw(15) << p.burstTime
            << std::setw(10) << p.priority
            << std::setw(15) << p.waitingTime
            << std::setw(15) << (p.endTime - p.arrivalTime) // час завершення - час прибуття
            << std::endl;
    }
}

// Shortest Job First (SJF) алгоритм
void shortestJobFirst(std::vector<Process> processes) {
    std::cout << "\n=== Shortest Job First Scheduling ===\n";

    int currentTime = 0;
    std::vector<Process*> readyQueue;

    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
        });

    while (!processes.empty() || !readyQueue.empty()) {
        for (auto& process : processes) {
            if (process.arrivalTime <= currentTime) {
                readyQueue.push_back(&process);
            }
        }
        processes.erase(std::remove_if(processes.begin(), processes.end(), [currentTime](const Process& process) {
            return process.arrivalTime <= currentTime;
            }), processes.end());

        if (!readyQueue.empty()) {
            std::sort(readyQueue.begin(), readyQueue.end(), [](const Process* a, const Process* b) {
                return a->burstTime < b->burstTime;
                });

            // Виконуємо процес
            Process* currentProcess = readyQueue.front();
            readyQueue.erase(readyQueue.begin());

            currentProcess->startTime = currentTime;
            currentTime += currentProcess->burstTime;
            currentProcess->endTime = currentTime;
            currentProcess->waitingTime = currentProcess->startTime - currentProcess->arrivalTime;
        }
        else {
            currentTime++;
        }
    }

    printProcesses(processes);
}

void prioritySchedulingWithAging(std::vector<Process> processes) {
    std::cout << "\n=== Priority Scheduling with Aging ===\n";

    int currentTime = 0;
    std::vector<Process*> readyQueue;

    // Сортуємо процеси за часом прибуття
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
        });

    while (!processes.empty() || !readyQueue.empty()) {
        for (auto& process : processes) {
            if (process.arrivalTime <= currentTime) {
                readyQueue.push_back(&process);
            }
        }
        processes.erase(std::remove_if(processes.begin(), processes.end(), [currentTime](const Process& process) {
            return process.arrivalTime <= currentTime;
            }), processes.end());

        for (auto& process : readyQueue) {
            process->age++;
            if (process->age >= 6) { 
                process->priority = std::max(1, process->priority - 1); // Пріоритет зменшується
                process->age = 0; // Скидаємо лічильник
            }
        }

        if (!readyQueue.empty()) {
            std::sort(readyQueue.begin(), readyQueue.end(), [](const Process* a, const Process* b) {
                return a->priority < b->priority;
                });

            // процес
            Process* currentProcess = readyQueue.front();
            readyQueue.erase(readyQueue.begin());

            currentProcess->startTime = currentTime;
            currentTime += currentProcess->burstTime;
            currentProcess->endTime = currentTime;
            currentProcess->waitingTime = currentProcess->startTime - currentProcess->arrivalTime;
        }
        else {
           
            currentTime++;
        }
    }

    printProcesses(processes);
}

// Головна функція
int main() {
    int numProcesses = 5;  // Кількість процесів

    auto processes = generateProcesses(numProcesses);

    std::cout << "=== Generated Processes ===\n";
    printProcesses(processes);

    shortestJobFirst(processes);

    prioritySchedulingWithAging(processes);

    return 0;
}
