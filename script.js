class Elevator {
  constructor() {
    this.currentFloor = 1;
    this.queue = []; // Queue stores the floors requested
    this.pressedFloors = [];
    this.isMoving = false;
    this.isEmergency = false;
    this.direction = "up"; // Current direction
    this.displayElement = document.getElementById('display');
    this.pressedFloorsDisplay = document.getElementById('pressedFloorsDisplay');
    this.elevatorBox = document.getElementById('elevator-box');
    this.setupEventListeners();
  }

  setupEventListeners() {
    const floorButtons = document.querySelectorAll('.floor-btn');
    floorButtons.forEach(button => {
      button.addEventListener('click', () => {
        const floor = parseInt(button.getAttribute('data-floor'));
        this.addToQueue(floor);
        this.addToPressedFloors(floor);
      });
    });

    document.getElementById('run-elevator').addEventListener('click', () => {
      if (!this.isMoving) {
        this.moveElevator();
      } else {
        this.showMessage("Elevator is already moving!");
      }
    });

    document.getElementById('emergency').addEventListener('click', () => {
      this.handleEmergency();
    });
  }

  addToQueue(floor) {
    // Only add to the queue if the floor is not the current floor
    if (floor === this.currentFloor) {
      this.showMessage("You are already on this floor!");
    } else if (!this.queue.includes(floor)) {
      this.queue.push(floor);
      this.updateQueueDisplay(); // Update queue display after adding
    } else {
      this.showMessage(`Floor ${floor} is already in the queue!`);
    }
  }

  addToPressedFloors(floor) {
    // Prevent adding the same floor again to pressed floors
    if (!this.pressedFloors.includes(floor)) {
      this.pressedFloors.push(floor);
      this.updatePressedFloorsDisplay();
    } else {
      this.showMessage(`Floor ${floor} is already pressed!`);
    }
  }

  moveElevator() {
    if (this.queue.length > 0 && !this.isEmergency) {
      this.isMoving = true;

      // Filter and sort based on the current direction
      const floorsInCurrentDirection = this.queue.filter(floor => {
        return this.direction === "up" ? floor > this.currentFloor : floor < this.currentFloor;
      });

      if (floorsInCurrentDirection.length === 0) {
        // Switch direction if no floors remain in the current direction
        this.direction = this.direction === "up" ? "down" : "up";
        this.moveElevator();
        return;
      }

      const nextFloor = this.direction === "up"
        ? Math.min(...floorsInCurrentDirection)
        : Math.max(...floorsInCurrentDirection);

      this.queue = this.queue.filter(floor => floor !== nextFloor); // Remove the next floor from the queue
      const moveTime = Math.abs(nextFloor - this.currentFloor) * 1000;

      setTimeout(() => {
        if (!this.isEmergency) {
          this.currentFloor = nextFloor;
          this.updateDisplay();
          this.animateElevator();

          setTimeout(() => {
            this.moveElevator();
          }, moveTime);
        }
      }, 1000);
    } else {
      this.isMoving = false;
      if (this.isEmergency) {
        this.showMessage("Emergency mode activated. Please wait.");
      } else {
        this.showMessage("No floors in the queue!");
      }
    }
  }

  animateElevator() {
    const floorHeight = 40;
    const newPosition = (this.currentFloor - 1) * floorHeight;
    this.elevatorBox.style.bottom = `${newPosition}px`;
  }

  updateDisplay() {
    const floorName = this.currentFloor === 1 ? "Ground" : `${this.currentFloor}`;
    this.displayElement.innerText = `Elevator on Floor ${floorName}`;
  }

  updatePressedFloorsDisplay() {
    // Display only the sequence of floors pressed
    this.pressedFloorsDisplay.innerText = `Floors Pressed: ${this.pressedFloors.join(', ')}`;
  }

  updateQueueDisplay() {
    document.getElementById('queueDisplay').innerText = `Queue: ${this.queue.join(', ')}`;
  }

  handleEmergency() {
    this.isEmergency = true;
    this.queue = []; // Clear all queued floors
    this.direction = ""; // Reset direction to disregard current state
    this.updatePressedFloorsDisplay(); // Clear the displayed pressed floors
    this.showMessage("Emergency activated! Going to ground floor...");

    // Directly send the elevator to the ground floor (floor 1)
    if (!this.isMoving) {
      this.currentFloor = 1;
      this.updateDisplay();
      this.animateElevator();

      // Reset emergency state after reaching the ground floor
      setTimeout(() => {
        this.isEmergency = false;
        this.showMessage("Elevator has reached ground floor.");
      }, 1000); // Adjust this delay if needed for smoother animation
    } else {
      // Interrupt the current movement and reset to ground floor
      setTimeout(() => {
        this.currentFloor = 1;
        this.isMoving = false;
        this.updateDisplay();
        this.animateElevator();
        this.isEmergency = false;
        this.showMessage("Elevator has reached ground floor during emergency.");
      }, Math.abs(1 - this.currentFloor) * 1000); // Move immediately to ground floor
    }
  }

  showMessage(message) {
    const messageElement = document.getElementById('message-display');
    messageElement.innerText = message;
    messageElement.style.display = 'block'; // Show the message
    setTimeout(() => {
      messageElement.style.display = 'none'; // Hide after some time
    }, 3000); // Adjust time as needed
  }
}

const elevator = new Elevator();
