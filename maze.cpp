#include <iostream>
#include <string>

using namespace std;

const int MAX_FLOORS = 10; // Maximum number of floors
const int MAX_QUEUE_SIZE = 10; // Maximum size of the queue

class Elevator {
private:
    int currentFloor;
    int queue[MAX_QUEUE_SIZE];
    int queueSize;
    int pressedFloors[MAX_FLOORS]; // Pressed floors
    int pressedFloorsSize;
    bool isMoving;
    bool isEmergency;
    string direction; 

public:
    Elevator() : currentFloor(1), queueSize(0), pressedFloorsSize(0), isMoving(false), isEmergency(false), direction("up") {
        for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
            queue[i] = 0; // Initialize queue
        }
        for (int i = 0; i < MAX_FLOORS; i++) {
            pressedFloors[i] = 0; // Initialize pressed floors
        }
    }

    void addToQueue(int floor) {
        if (floor == currentFloor) {
            showMessage("You are already on this floor!");
        } else if (floor < 1 || floor > MAX_FLOORS) {
            showMessage("Invalid floor number!");
        } else if (!isInQueue(floor)) {
            if (queueSize < MAX_QUEUE_SIZE) {
                queue[queueSize++] = floor;
                updateQueueDisplay();
            } else {
                showMessage("Queue is full! Cannot add more floors.");
            }
        } else {
            showMessage("Floor " + to_string(floor) + " is already in the queue!");
        }
    }

    bool isInQueue(int floor) {
        for (int i = 0; i < queueSize; i++) {
            if (queue[i] == floor) {
                return true;
            }
        }
        return false;
    }

    void addToPressedFloors(int floor) {
        if (floor < 1 || floor > MAX_FLOORS) {
            showMessage("Invalid floor number!");
            return;
        }
        if (!isInPressedFloors(floor)) {
            if (pressedFloorsSize < MAX_FLOORS) {
                pressedFloors[pressedFloorsSize++] = floor;
                updatePressedFloorsDisplay();
            } else {
                showMessage("Cannot add more pressed floors!");
            }
        } else {
            showMessage("Floor " + to_string(floor) + " is already pressed!");
        }
    }

    bool isInPressedFloors(int floor) {
        for (int i = 0; i < pressedFloorsSize; i++) {
            if (pressedFloors[i] == floor) {
                return true;
            }
        }
        return false;
    }

    void moveElevator() {
        if (queueSize > 0 && !isEmergency) {
            isMoving = true;

            // Find the next floor in the current direction
            int nextFloor = 0;
            bool found = false;
            for (int i = 0; i < queueSize; i++) {
                if ((direction == "up" && queue[i] > currentFloor) ||
                    (direction == "down" && queue[i] < currentFloor)) {
                    if (!found || (direction == "up" && queue[i] < nextFloor) ||
                        (direction == "down" && queue[i] > nextFloor)) {
                        nextFloor = queue[i];
                        found = true;
                    }
                }
            }

            if (!found) {
                // Switch direction if no floors remain in the current direction
                direction = (direction == "up") ? "down" : "up";
                moveElevator();
                return;
            }

            // Simulate the movement of the elevator
            cout << "Elevator is moving from floor " << currentFloor << " to floor " << nextFloor << endl;
            currentFloor = nextFloor;
            updateDisplay();
            removeFromQueue(nextFloor);
            isMoving = false;
            moveElevator();
        } else {
            isMoving = false;
            if (isEmergency) {
                showMessage("Emergency mode activated. Please wait.");
            } else {
                showMessage("No floors in the queue!");
            }
        }
    }

    void removeFromQueue(int floor) {
        for (int i = 0; i < queueSize; i++) {
            if (queue[i] == floor) {
                // Shift remaining elements to the left
                for (int j = i; j < queueSize - 1; j++) {
                    queue[j] = queue[j + 1];
                }
                queueSize--;
                break; 
            }
        }
    }

    void updateDisplay() {
        string floorName = (currentFloor == 1) ? "Ground" : to_string(currentFloor);
        cout << "Elevator on Floor " << floorName << endl;
    }

    void updateQueueDisplay() {
        cout << "Queue: ";
        for (int i = 0; i < queueSize; i++) {
            cout << queue[i] << " ";
        }
        cout << endl;
    }

    void updatePressedFloorsDisplay() {
        cout << "Pressed Floors: ";
        for (int i = 0; i < pressedFloorsSize; i++) {
            cout << pressedFloors[i] << " ";
        }
        cout << endl;
    }

    void activateEmergencyMode() {
        isEmergency = true;
        showMessage("Emergency mode activated! Returning to ground floor.");
        currentFloor = 1; // Return to ground floor
        isEmergency = false;
        updateDisplay();
    }

    void showMessage(const string& message) {
        cout << message << endl;
    }
};

int main() {
    Elevator elevator;

    elevator.addToQueue(3); // Example requests
    elevator.addToQueue(5);
    elevator.addToQueue(1);
    elevator.addToQueue(7);

    elevator.moveElevator(); // Start moving the elevator

    return 0;
}

