# Dynamic Array with Sensor CSV Input (C)

This project contains a simple dynamic array implementation in C.  
Sensor values are read from a CSV file and processed using that array.

## Features
- Manual dynamic array (malloc/realloc)
- File input: reads values from `Luftqualitaet_DEBE066_Berlin-Karlshorst_2025-01-01_00-2025-05-09_08.csv`
- Calculates statistics (min, max, average, etc.)
- Input-safe: handles variable-length data

## What I learned
- Working with pointers and dynamic memory
- File I/O in C (`fopen`, `fgets`, `strtok`)
- Data processing and memory management
