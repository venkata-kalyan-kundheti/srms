Student Management System (C++ CLI)

This project is a console-based Student Management System built using C++. It provides a complete role-based access system for Admin, Staff, Students, and Guests. All data is stored using simple text files, making it lightweight and easy to run on any machine without external dependencies. The system includes full CRUD operations, attendance and marks management, GPA calculation, activity logging, and automatic backups.

Features

Role-based login system: Admin, Staff, Student, Guest

Admin can add, edit, view, delete students, and export data as CSV

Staff can update attendance and marks

Students can view their full personal profile

Guests can view limited student information

Automatic GPA calculation based on marks

Activity logging with timestamps

Per-student backup file generation

Dashboard summary showing total students, average attendance, and average GPA

Student Data Stored

Student ID

Name

Department

Email

Password

Club

Attendance for Coding, C++, and DAA

Marks for Coding, C++, and DAA

Auto-calculated GPA

Project Structure

main.cpp — main source code

students.txt — primary database file

activity_log.txt — log of all actions

backup_<id>.txt — individual student backup

students_export.csv — exported CSV file

Running the Program

Compile using: g++ main.cpp -o sms

Run using: ./sms (Linux/Mac) or sms.exe (Windows)

Default Credentials

Admin: admin / admin123

Staff: staff / staff123

Guest: guest / guest123

Project Summary

This Student Management System is designed for academic use and learning purposes. It demonstrates file handling, serialization, user authentication, menu-driven interfaces, and basic data analytics. It is a strong base for expanding into a fully OOP or database-backed system.
