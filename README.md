# Flight Coordinator (Currently Developing)
### Description
I developed this project as the submission for my midterm assignment of my ***OPERATING SYSTEMS*** course at my local University ***ULE*** (Universidad de León)

The project is terminal based, meaning that both input and output of the program are designed for a terminal.

The aim of the project is to be able to correctly sync parent and children processes via the use of the signals **SIGUSR1** and **SIGUSR2** (signal.h library for c)

A normal executiong goes as follow:
  1. The user selects a funcionality from the shell script.
  2. If that functionality is the execute one, then the c script will get executed.
  3. The c script forks 2 processes (flight_technician and flight_manager) and, up to as many as indicated by programm args, new processes (flight_attendants).
  4. Then through signals all this processes are executed iteratively in orden of creation.
  5. If everything went well the programm exits with 0 and if it didn´t then it exits with -1; 

### About
This project was made using vscode.

The code for the project is written in spanish since it was mandatory for the assignment.

If you are a **C** developer and have found this project interesting, please let me know by starring this project.
If by the contrary you thought something could be improved please let me know via a comment or by making a pull request with the suggested changes.

> This project has no corrected version yet (***REASON:*** This project hasn´t been marked yet)
