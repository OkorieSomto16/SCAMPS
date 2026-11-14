# SCAPMS

Smart Clinic Appointment & Patient Management System, implemented in C++17 with a native Windows GUI.

## Build

Requirements: Windows 10 or later, CMake 3.20+, and a C++17 desktop compiler (Visual Studio 2022 with the Desktop development with C++ workload, or MinGW-w64).

From this folder, configure and build with:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run `build/Release/scapms.exe` with Visual Studio, or `build/scapms.exe` for a single-configuration generator. No third-party libraries are required.

## Using the application

- **Patient Records:** register patients, double-click a row to load it into the edit form, update or delete records, and undo the most recent administrative change.
- **Appointments:** enter a registered patient ID, a date in `YYYY-MM-DD` format, and a reason; select a pending appointment to cancel or mark it completed.
- **Waiting Queue:** add a patient by ID and call the next patient. Service follows FIFO order.
- **Search & Sort:** choose linear ID search, binary ID search, or recursive name search. Binary search is enabled after sorting by Patient ID. Choose Selection or Insertion Sort and sort by name, ID, or age.
- **Reports:** generate completed-visit rankings, pending appointments, category totals, or a daily summary.
- Use **File > Save / Load** (Ctrl+S / Ctrl+O). Patient and appointment data are stored in a readable escaped text format.

## Design notes

The native interface is event-driven through Windows messages and themed common controls, using Segoe UI typography and a soft neutral palette. Dashboard summary cards use white surfaces and teal accents. It uses a horizontal form row above a list region, with the list anchored to the available window size; this keeps data entry compact while allowing record tables to expand as the window grows. Separate tabs keep patients, appointments, the queue, search/sort, and reports focused.

The core model and business rules live in `include/scapms/core.hpp` and `src/core.cpp`; Windows UI wiring is in `src/main_win.cpp`. A class diagram and concise project report are in `docs/`.
