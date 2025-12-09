#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

// OS-independent clear
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

// ====================== STRUCT ====================================
struct Student {
    string id, name, department, email, password, club, gpa;

    int attCoding = 0, attCPP = 0, attDAA = 0;
    int markCoding = 0, markCPP = 0, markDAA = 0;
};

// ====================== UI HELPERS =================================
void printTitle(const string &title) {
    cout << "\n============================================\n";
    cout << "            " << title << "\n";
    cout << "============================================\n";
}

void pauseClear() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    system(CLEAR);
}

// ====================== LOGGING & BACKUP ==========================
void logActivity(const string &userType, const string &action) {
    ofstream log("activity_log.txt", ios::app);
    if (!log) return;

    time_t now = time(0);
    string timestamp = ctime(&now);
    timestamp.pop_back();

    log << "[" << timestamp << "] (" << userType << ") " << action << "\n";
}

void backupStudent(const Student &s) {
    ofstream out("backup_" + s.id + ".txt");
    if (out) out << s.id << "|" << s.name << "|" << s.department << "|" << s.email << "|"
                 << s.password << "|" << s.club << "|" << s.gpa << "|"
                 << s.attCoding << "|" << s.attCPP << "|" << s.attDAA << "|"
                 << s.markCoding << "|" << s.markCPP << "|" << s.markDAA;
}

// ====================== SERIALIZATION ==============================
string serialize(const Student &s) {
    stringstream ss;
    ss << s.id << "|" << s.name << "|" << s.department << "|" << s.email << "|"
       << s.password << "|" << s.club << "|" << s.gpa << "|"
       << s.attCoding << "|" << s.attCPP << "|" << s.attDAA << "|"
       << s.markCoding << "|" << s.markCPP << "|" << s.markDAA;
    return ss.str();
}

Student deserialize(const string &line) {
    Student s;
    string token;
    stringstream ss(line);

    getline(ss, s.id, '|');
    getline(ss, s.name, '|');
    getline(ss, s.department, '|');
    getline(ss, s.email, '|');
    getline(ss, s.password, '|');
    getline(ss, s.club, '|');
    getline(ss, s.gpa, '|');

    getline(ss, token, '|'); if (!token.empty()) s.attCoding = stoi(token);
    getline(ss, token, '|'); if (!token.empty()) s.attCPP = stoi(token);
    getline(ss, token, '|'); if (!token.empty()) s.attDAA = stoi(token);

    getline(ss, token, '|'); if (!token.empty()) s.markCoding = stoi(token);
    getline(ss, token, '|'); if (!token.empty()) s.markCPP = stoi(token);
    getline(ss, token, '|'); if (!token.empty()) s.markDAA = stoi(token);

    return s;
}

bool fetchStudent(const string &id, Student &out) {
    ifstream file("students.txt");
    string line;

    while (getline(file, line)) {
        Student s = deserialize(line);
        if (s.id == id) {
            out = s;
            return true;
        }
    }
    return false;
}

void updateStudent(const Student &s) {
    ifstream file("students.txt");
    ofstream temp("temp.txt");
    string line;

    while (getline(file, line)) {
        Student st = deserialize(line);
        if (st.id == s.id)
            temp << serialize(s) << "\n";
        else
            temp << line << "\n";
    }

    file.close();
    temp.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");
}

bool deleteStudentById(const string &id) {
    ifstream file("students.txt");
    ofstream temp("temp.txt");

    string line;
    bool found = false;

    while (getline(file, line)) {
        Student s = deserialize(line);
        if (s.id == id) {
            found = true;   // skip writing → deletes record
            continue;
        }
        temp << line << "\n";
    }

    file.close();
    temp.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    return found;
}

// ====================== GPA CALC ====================================
void calculateGPA(Student &s) {
    int total = s.markCoding + s.markCPP + s.markDAA;
    float gpa = total / 30.0;

    stringstream ss;
    ss << fixed << setprecision(2) << gpa;
    s.gpa = ss.str();
}

// ====================== DISPLAY ===================================
void showBasic(const Student &s) {
    cout << "ID: " << s.id << "\nName: " << s.name
         << "\nDepartment: " << s.department
         << "\nEmail: " << s.email << endl;
}

void showFull(const Student &s) {
    cout << "\n---------------- STUDENT DETAILS ----------------\n";
    cout << "ID      : " << s.id << "\n";
    cout << "Name    : " << s.name << "\n";
    cout << "Dept    : " << s.department << "\n";
    cout << "Email   : " << s.email << "\n";
    cout << "Club    : " << s.club << "\n";
    cout << "GPA     : " << s.gpa << "\n\n";

    cout << "******** Attendance (%) ********\n";
    cout << "+---------+-------+-----+-----+\n";
    cout << "| Subject | Coding| C++ | DAA |\n";
    cout << "+---------+-------+-----+-----+\n";
    cout << "| Value   |  " << setw(3) << s.attCoding
         << "  | " << setw(3) << s.attCPP
         << " | " << setw(3) << s.attDAA << " |\n";
    cout << "+---------+-------+-----+-----+\n\n";

    cout << "******** Marks ********\n";
    cout << "+---------+-------+-----+-----+\n";
    cout << "| Subject | Coding| C++ | DAA |\n";
    cout << "+---------+-------+-----+-----+\n";
    cout << "| Value   |  " << setw(3) << s.markCoding
         << "  | " << setw(3) << s.markCPP
         << " | " << setw(3) << s.markDAA << " |\n";
    cout << "+---------+-------+-----+-----+\n";
}


// ====================== FEATURES ===================================
void viewSingleStudent() {
    string id;
    cout << "Enter Student ID: ";
    cin >> id;

    Student s;
    if (!fetchStudent(id, s)) {
        cout << "Student Not Found!\n";
        return;
    }

    cout << "\n--- Full Student Details ---\n";
    showFull(s);
}

void addStudent() {
    Student s;

    cout << "\nEnter Student ID: ";
    cin >> s.id;
    cin.ignore();

    cout << "Name: ";
    getline(cin, s.name);

    cout << "Department: ";
    getline(cin, s.department);

    cout << "Email: ";
    getline(cin, s.email);

    cout << "Password: ";
    cin >> s.password;

    cin.ignore();
    cout << "Club: ";
    getline(cin, s.club);

    cout << "Attendance (Coding C++ DAA): ";
    cin >> s.attCoding >> s.attCPP >> s.attDAA;

    cout << "Marks (Coding C++ DAA): ";
    cin >> s.markCoding >> s.markCPP >> s.markDAA;

    calculateGPA(s);

    ofstream out("students.txt", ios::app);
    out << serialize(s) << "\n";

    backupStudent(s);
    logActivity("ADMIN", "Added student " + s.id);

    cout << "Added Successfully!\n";
}

void editStudent() {
    string id;
    cout << "Enter ID to edit: ";
    cin >> id;

    Student s;
    if (!fetchStudent(id, s)) {
        cout << "Student Not Found!\n";
        return;
    }

    int ch;
    while (true) {
        cout << "\n--- Edit Menu for Student " << s.id << " ---\n";
        cout << "1. Name\n2. Department\n3. Email\n4. Club\n";
        cout << "5. Attendance (Coding/C++/DAA)\n6. Marks (Coding/C++/DAA)\n";
        cout << "7. Password\n8. Save & Exit\nChoice: ";
        cin >> ch;
        cin.ignore();

        if (ch == 1) {
            cout << "New Name: ";
            getline(cin, s.name);
        }
        else if (ch == 2) {
            cout << "New Department: ";
            getline(cin, s.department);
        }
        else if (ch == 3) {
            cout << "New Email: ";
            getline(cin, s.email);
        }
        else if (ch == 4) {
            cout << "New Club: ";
            getline(cin, s.club);
        }
        else if (ch == 5) {
            cout << "New Attendance: ";
            cin >> s.attCoding >> s.attCPP >> s.attDAA;
        }
        else if (ch == 6) {
            cout << "New Marks: ";
            cin >> s.markCoding >> s.markCPP >> s.markDAA;
            calculateGPA(s);
        }
        else if (ch == 7) {
            cout << "New Password: ";
            cin >> s.password;
        }
        else if (ch == 8) {
            updateStudent(s);
            backupStudent(s);
            logActivity("ADMIN", "Edited student " + s.id);
            cout << "Changes Saved!\n";
            break;
        }
        else cout << "Invalid Choice!\n";
    }
}

void staffEditAttendance() {
    string id;
    cout << "Enter ID: ";
    cin >> id;

    Student s;
    if (!fetchStudent(id, s)) {
        cout << "Not Found!\n";
        return;
    }

    int ch;
    while (true) {
        cout << "\n--- Staff Edit Menu for " << s.id << " ---\n";
        cout << "1. Update Attendance Only\n";
        cout << "2. Update Marks Only\n";
        cout << "3. Update Both Attendance + Marks\n";
        cout << "4. Save & Exit\nChoice: ";
        cin >> ch;

        if (ch == 1) {
            cout << "Enter Attendance: ";
            cin >> s.attCoding >> s.attCPP >> s.attDAA;
        }
        else if (ch == 2) {
            cout << "Enter Marks: ";
            cin >> s.markCoding >> s.markCPP >> s.markDAA;
            calculateGPA(s);
        }
        else if (ch == 3) {
            cout << "Enter Attendance: ";
            cin >> s.attCoding >> s.attCPP >> s.attDAA;
            cout << "Enter Marks: ";
            cin >> s.markCoding >> s.markCPP >> s.markDAA;
            calculateGPA(s);
        }
        else if (ch == 4) {
            updateStudent(s);
            backupStudent(s);
            logActivity("STAFF", "Updated attendance/marks for " + s.id);
            cout << "Changes Saved!\n";
            break;
        }
        else cout << "Invalid Choice!\n";
    }
}

void viewAllStudents() {
    ifstream file("students.txt");
    string line;

    cout << "\n--- All Students ---\n";
    while (getline(file, line)) {
        Student s = deserialize(line);
        cout << s.id << " | " << s.name << " | " << s.department << endl;
    }
}

void deleteStudent() {
    string id;
    cout << "Enter Student ID to delete: ";
    cin >> id;

    Student s;
    if (!fetchStudent(id, s)) {
        cout << "Student Not Found!\n";
        return;
    }

    char confirm;
    cout << "\nAre you sure you want to delete this student? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (deleteStudentById(id)) {
            logActivity("ADMIN", "Deleted student " + id);
            cout << "Student Deleted Successfully!\n";
        }
    } else {
        cout << "Delete Cancelled.\n";
    }
}

void exportCSV() {
    ifstream file("students.txt");
    ofstream csv("students_export.csv");

    csv << "ID,Name,Dept,Email,Club,GPA,CodingAtt,CPPAtt,DAAAtt,CodingMark,CPPMark,DAAMark\n";

    string line;
    while (getline(file, line)) {
        Student s = deserialize(line);
        csv << s.id << "," << s.name << "," << s.department << "," << s.email 
            << "," << s.club << "," << s.gpa << "," << s.attCoding << "," 
            << s.attCPP << "," << s.attDAA << "," << s.markCoding << "," 
            << s.markCPP << "," << s.markDAA << "\n";
    }

    cout << "Exported to students_export.csv\n";
    logActivity("ADMIN", "Exported CSV");
}

// ====================== DASHBOARD ==================================
void showDashboard() {
    ifstream file("students.txt");
    string line;

    int count = 0, sumC = 0, sumCPP = 0, sumDAA = 0;
    float totalGPA = 0;

    while (getline(file, line)) {
        Student s = deserialize(line);
        count++;

        sumC += s.attCoding;
        sumCPP += s.attCPP;
        sumDAA += s.attDAA;
        totalGPA += stof(s.gpa);
    }

    if (count == 0) {
        cout << "No Data Available.\n";
        return;
    }

    cout << "\n========== DASHBOARD SUMMARY ==========\n";
    cout << "Total Students: " << count << "\n";
    cout << "Avg Coding Attendance: " << sumC / count << "%\n";
    cout << "Avg C++ Attendance: " << sumCPP / count << "%\n";
    cout << "Avg DAA Attendance: " << sumDAA / count << "%\n";
    cout << "Avg GPA: " << fixed << setprecision(2) << (totalGPA / count) << "\n";
}

// ====================== LOGIN & MENUS ==============================
void studentLogin() {
    string id, pass;

    printTitle("STUDENT LOGIN");
    cout << "ID: ";
    cin >> id;

    cout << "Password: ";
    cin >> pass;

    Student s;
    if (!fetchStudent(id, s) || s.password != pass) {
        cout << "Invalid login!\n";
        return;
    }

    system(CLEAR);
    printTitle("YOUR PROFILE");
    showFull(s);
}

// -------- ADMIN --------
void adminMenu();

void adminLogin() {
    string u, p;
    printTitle("ADMIN LOGIN");

    cout << "Username: ";
    cin >> u;

    cout << "Password: ";
    cin >> p;

    if (u == "admin" && p == "admin123") {
        system(CLEAR);
        adminMenu();
    } else {
        cout << "Invalid Login!\n";
    }
}

void adminMenu() {
    int c;

    while (true) {
        printTitle("ADMIN MENU");
        showDashboard();

        cout << "\n1. Add Student\n2. View All Students\n3. View Single Student\n";
        cout << "4. Edit Student\n5. Delete Student\n6. Export CSV\n7. Back\n";

        cout << "Choice: ";
        cin >> c;

        system(CLEAR);

        if (c == 1) addStudent();
        else if (c == 2) viewAllStudents();
        else if (c == 3) viewSingleStudent();
        else if (c == 4) editStudent();
        else if (c == 5) deleteStudent();
        else if (c == 6) exportCSV();
        else if (c == 7) break;
        else cout << "Invalid Choice!\n";

        pauseClear();
    }
}

// -------- STAFF --------
void staffMenu();

void staffLogin() {
    string u, p;
    printTitle("STAFF LOGIN");

    cout << "Username: ";
    cin >> u;

    cout << "Password: ";
    cin >> p;

    if (u == "staff" && p == "staff123") {
        system(CLEAR);
        staffMenu();
    } else {
        cout << "Invalid Login!\n";
    }
}

void staffMenu() {
    int c;

    while (true) {
        printTitle("STAFF MENU");
        showDashboard();

        cout << "\n1. Edit Attendance/Marks\n2. View All Students\n3. View Single Student\n4. Back\n";
        cout << "Choice: ";
        cin >> c;

        system(CLEAR);

        if (c == 1) staffEditAttendance();
        else if (c == 2) viewAllStudents();
        else if (c == 3) viewSingleStudent();
        else if (c == 4) break;
        else cout << "Invalid Choice!\n";

        pauseClear();
    }
}

// -------- GUEST --------
void guestLogin() {
    string id, pass;
    printTitle("GUEST LOGIN");

    cout << "Guest ID: ";
    cin >> id;

    cout << "Password: ";
    cin >> pass;

    if (id == "guest" && pass == "guest123") {
        cout << "Enter Student ID to View: ";
        cin >> id;

        Student s;
        if (fetchStudent(id, s)) showBasic(s);
        else cout << "Not Found!\n";
    } else {
        cout << "Invalid Guest Login!\n";
    }
}

// ====================== MAIN ======================================
int main() {
    int c;

    while (true) {
        printTitle("STUDENT MANAGEMENT SYSTEM");

        cout << "1. Admin\n2. Staff\n3. Student\n4. Guest\n5. Exit\n";
        cout << "Choice: ";
        cin >> c;

        system(CLEAR);

        if (c == 1) adminLogin();
        else if (c == 2) staffLogin();
        else if (c == 3) studentLogin();
        else if (c == 4) guestLogin();
        else if (c == 5) break;
        else cout << "Invalid Choice!\n";

        pauseClear();
    }

    return 0;
}