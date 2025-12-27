#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

enum Status { IN_PROGRESS = 0, DONE = 1, FAILED = 2 };

string statusToString(Status st) {
    if (st == IN_PROGRESS) return "Dang lam";
    if (st == DONE) return "Hoan thanh";
    return "Khong hoan thanh";
}

bool isValidDateFormat(const string& d) {

    if (d.size() != 10) return false;
    if (d[2] != '/' || d[5] != '/') return false;


    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (d[i] < '0' || d[i] > '9') return false;
    }


    int day   = (d[0] - '0') * 10 + (d[1] - '0');
    int month = (d[3] - '0') * 10 + (d[4] - '0');
    int year  = (d[6] - '0') * 1000 + (d[7] - '0') * 100 + (d[8] - '0') * 10 + (d[9] - '0');

    if (year < 1) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

void clearInputLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

class Work {
private:
    int id;
    string content;
    int hours;
    string createdAt; 
    Status status;

public:
    Work() {
        id = 0;
        content = "";
        hours = 0;
        createdAt = "01/01/2000";
        status = IN_PROGRESS;
    }

    Work(int _id, const string& _content, int _hours, const string& _createdAt) {
        id = _id;
        content = _content;
        hours = _hours;
        createdAt = _createdAt;
        status = IN_PROGRESS;
    }

    int getId() const { return id; }
    Status getStatus() const { return status; }

    void setContent(const string& c) { content = c; }

    bool setStatus(Status newStatus) {

        if (status != IN_PROGRESS) return false;
        status = newStatus;
        return true;
    }

    void printRow() const {
        cout << left
             << setw(6)  << id
             << setw(28) << content.substr(0, 27)
             << setw(8)  << hours
             << setw(12) << createdAt
             << setw(16) << statusToString(status)
             << "\n";
    }
};

class WorkManager {
private:
    Work list[100];
    int size;
    int nextId;

public:
    WorkManager() {
        size = 0;
        nextId = 1;
    }

    void showMenu() {
        cout << "\n===================MENU====================\n";
        cout << "1. Hien thi danh sach\n";
        cout << "2. Them cong viec\n";
        cout << "3. Cap nhat noi dung CV\n";
        cout << "4. Chuyen trang thai\n";
        cout << "5. Loc cong viec theo trang thai\n";
        cout << "6. Thoat\n";
        cout << "===========================================\n";
        cout << "Chon: ";
    }

    void showHeader() const {
        cout << left
             << setw(6)  << "ID"
             << setw(28) << "Noi dung"
             << setw(8)  << "Gio"
             << setw(12) << "Ngay tao"
             << setw(16) << "Trang thai"
             << "\n";
        cout << "========================================\n";
    }

    int findIndexById(int id) const {
        for (int i = 0; i < size; i++) {
            if (list[i].getId() == id) return i;
        }
        return -1;
    }

    void showList() const {
        if (size == 0) {
            cout << "Danh sach dang rong.\n";
            return;
        }
        showHeader();
        for (int i = 0; i < size; i++) {
            list[i].printRow();
        }
    }

    void addWork() {
        if (size >= 100) {
            cout << "Danh sach day, khong them duoc.\n";
            return;
        }

        string content;
        int hours;
        string date;

        cout << "Nhap noi dung: ";
        clearInputLine();
        getline(cin, content);
        while (content.size() == 0) {
            cout << "Noi dung khong duoc rong. Nhap lai: ";
            getline(cin, content);
        }

        cout << "Nhap so gio can thuc hien: ";
        while (!(cin >> hours) || hours <= 0) {
            cout << "So gio phai la so duong. Nhap lai: ";
            clearInputLine();
        }

        cout << "Nhap ngay tao (dd/MM/yyyy): ";
        clearInputLine();
        getline(cin, date);
        while (!isValidDateFormat(date)) {
            cout << "Sai dinh dang. Nhap lai (dd/MM/yyyy): ";
            getline(cin, date);
        }

        list[size] = Work(nextId, content, hours, date);
        size++;
        nextId++;

        cout << "Them cong viec thanh cong!\n";
    }

    void updateContent() {
        int id;
        cout << "Nhap ID cong viec can cap nhat: ";
        if (!(cin >> id)) {
            cout << "Nhap sai.\n";
            clearInputLine();
            return;
        }

        int idx = findIndexById(id);
        if (idx == -1) {
            cout << "Khong tim thay cong viec ID = " << id << "\n";
            return;
        }

        string newContent;
        cout << "Nhap noi dung moi: ";
        clearInputLine();
        getline(cin, newContent);
        while (newContent.size() == 0) {
            cout << "Noi dung khong duoc rong. Nhap lai: ";
            getline(cin, newContent);
        }

        list[idx].setContent(newContent);
        cout << "Cap nhat noi dung thanh cong!\n";
    }

    void changeStatus() {
        int id;
        cout << "Nhap ID cong viec can doi trang thai: ";
        if (!(cin >> id)) {
            cout << "Nhap sai.\n";
            clearInputLine();
            return;
        }

        int idx = findIndexById(id);
        if (idx == -1) {
            cout << "Khong tim thay cong viec ID = " << id << "\n";
            return;
        }

        cout << "Chon trang thai moi:\n";
        cout << "1. Hoan thanh\n";
        cout << "2. Khong hoan thanh\n";
        cout << "Chon: ";

        int opt;
        if (!(cin >> opt) || (opt != 1 && opt != 2)) {
            cout << "Lua chon khong hop le.\n";
            clearInputLine();
            return;
        }

        Status newStatus = (opt == 1) ? DONE : FAILED;
        if (!list[idx].setStatus(newStatus)) {
            cout << "Chi duoc doi trang thai khi cong viec dang lam!\n";
            return;
        }

        cout << "Doi trang thai thanh cong!\n";
    }

    void filterByStatus() const {
        cout << "Chon trang thai can loc:\n";
        cout << "0. Dang lam\n";
        cout << "1. Hoan thanh\n";
        cout << "2. Khong hoan thanh\n";
        cout << "Chon: ";

        int st;
        if (!(cin >> st) || st < 0 || st > 2) {
            cout << "Lua chon khong hop le.\n";
            clearInputLine();
            return;
        }

        Status target = (Status)st;
        int found = 0;

        showHeader();
        for (int i = 0; i < size; i++) {
            if (list[i].getStatus() == target) {
                list[i].printRow();
                found = 1;
            }
        }

        if (!found) cout << "Khong co cong viec nao theo trang thai nay.\n";
    }
};

int main() {
    WorkManager manager;
    int choice;

    while (true) {
        manager.showMenu();
        if (!(cin >> choice)) {
            cout << "Nhap sai kieu du lieu.\n";
            clearInputLine();
            continue;
        }

        if (choice == 1) manager.showList();
        else if (choice == 2) manager.addWork();
        else if (choice == 3) manager.updateContent();
        else if (choice == 4) manager.changeStatus();
        else if (choice == 5) manager.filterByStatus();
        else if (choice == 6) {
            cout << "Thoat chuong trinh.\n";
            break;
        } else {
            cout << "Lua chon khong hop le (1-6).\n";
        }
    }

    return 0;
}

