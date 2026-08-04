# Employee Management System

A desktop Employee Management & Office Automation application built with **C++** and **Qt Framework** for the Advanced Programming course. The system provides an interactive graphical interface to streamline corporate personnel management, track status logs, evaluate performance, and ensure persistent data storage across sessions.

---

## Key Features

- **Dynamic Personnel Operations:** Add, update, remove, or promote employees to Manager with dynamic UI state updates.
- **Smart Dual-Mode Search & Multi-Filtering:** 
  - Text input automatically detects and filters by **Employee Name** or **Personnel ID**.
  - Numeric search automatically filters employees based on **performance score**.
  - Additional categorical filtering by Department, Qualification, and Work Status.
- **Real-Time Statistics Dashboard:** Visual table views, custom color coding (highlighting Managers in blue), and live percentage tracking for active staff.
- **Persistent Data Storage:** Automatic serialization and deserialization of records to disk (`company_database.txt`).
- **Live System Logger:** Timestamped event tracking for all runtime actions displayed directly within the application window.

---

## System Architecture & Engineering

The application follows a modular 3-tier desktop architecture:

- **Presentation Layer (Qt UI):** Developed using Qt Widgets, custom table styles, and signal/slot architecture for event-driven user interaction.
- **Business Logic Layer (OOP Core):**
  - **Inheritance & Polymorphism:** Abstract base class `Employee` extended by `Manager` with overridden performance evaluation logic.
  - **Composition:** `Company` class managing the life cycle and runtime collection of employee instances.
  - **Memory Safety:** Explicit heap allocation management and clean object deallocation (`qDeleteAll` / virtual destructors) to guarantee zero memory leaks.
- **Persistence Layer:** Disk file I/O operations managing system state persistence upon changes and application shutdown.

---

## Technologies Used

- **Language:** C++17
- **Framework:** Qt 5.x / Qt 6.x (Qt Widgets, QObjects)
- **Design Concepts:** Object-Oriented Programming (OOP), Signal & Slot Architecture, File Serialization

---
## Screenshot

### Main Dashboard Overview
[![Main Dashboard](images/dashboard.png)]
---

## Project Report & Documentation

A detailed report covering class diagrams, UML structure, and execution workflows is available in the repository:

 [Project Report](docs/Employee-Management-System-Report.pdf)
---
## Installation & Run

### Requirements
- Qt 5/6
- C++17 Compiler
- CMake / qmake

### Steps
1. Clone the repository
2. Open project in Qt Creator
3. Build and Run
---
## Author

**Arefeh Bahramian**  
B.Sc. Computer Science Student  
Shiraz University
