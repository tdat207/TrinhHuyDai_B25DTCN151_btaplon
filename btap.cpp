#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Status {
    DANG_LAM = 1,
    HOAN_THANH,
    KHONG_HOAN_THANH
};

string statusToString(Status st) {
    if (st == DANG_LAM) return "Dang lam";
    if (st == HOAN_THANH) return "Hoan thanh";
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

class Work {
private:
    int id;
    string content;
    int hours;
    string createDate;
    Status status;

public:
    // Ham tao
    Work(int _id, string _content, int _hours, string _date) {
        id = _id;
        content = _content;
        hours = _hours;
        createDate = _date;
        status = DANG_LAM;
    }

   
    int getId() { return id; }
    Status getStatus() { return status; }

    void setContent(string newContent) { content = newContent; }

    bool changeStatus(Status newStatus) {
        if (status != DANG_LAM) return false;
        status = newStatus;
        return true;
    }

    void display() {
        cout << "ID: " << id
             << " | Noi dung: " << content
             << " | Gio: " << hours
             << " | Ngay tao: " << createDate
             << " | Trang thai: " << statusToString(status)
             << "\n";
    }
};

class WorkManager {
private:
    vector<Work> list;
    int nextId = 1;

    int findIndexById(int id) {
        for (int i = 0; i < (int)list.size(); i++) {
            if (list[i].getId() == id) return i;
        }
        return -1;
    }

public:
    void showMenu() {
        cout << "\n-------------------MENU----------------------\n";
        cout << "1. Hien thi danh sach\n";
        cout << "2. Them cong viec\n";
        cout << "3. Cap nhat noi dung CV\n";
        cout << "4. Chuyen trang thai\n";
        cout << "5. Loc cong viec theo trang thai\n";
        cout << "6. Thoat\n";
        cout << "--------------------------------------------\n";
        cout << "Nhap lua chon: ";
    }

    void showList() {
        if (list.size() == 0) {
            cout << "Danh sach cong viec dang rong!\n";
            return;
        }
        for (int i = 0; i < (int)list.size(); i++) {
            list[i].display();
        }
    }

    void addWork() {
        string content, date;
        int hours;

        cout << "Nhap noi dung cong viec: ";
        getline(cin, content);
        while (content.size() == 0) {
            cout << "Noi dung khong duoc de trong. Nhap lai: ";
            getline(cin, content);
        }

        cout << "Nhap so gio can thuc hien: ";
        while (!(cin >> hours) || hours <= 0) {
            cout << "So gio phai la so nguyen > 0. Nhap lai: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');

        cout << "Nhap ngay tao (dd/MM/yyyy): ";
        getline(cin, date);
        while (!isValidDateFormat(date)) {
            cout << "Sai dinh dang. Nhap lai (dd/MM/yyyy): ";
            getline(cin, date);
        }

        Work w(nextId, content, hours, date);
        list.push_back(w);
        nextId++;

        cout << "Them cong viec thanh cong!\n";
    }

    void updateContent() {
        int id;
        cout << "Nhap ID cong viec can cap nhat: ";
        while (!(cin >> id)) {
            cout << "Nhap sai. Nhap lai ID: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');

        int idx = findIndexById(id);
        if (idx == -1) {
            cout << "Khong tim thay cong viec ID = " << id << "\n";
            return;
        }

        string newContent;
        cout << "Nhap noi dung moi: ";
        getline(cin, newContent);
        while (newContent.size() == 0) {
            cout << "Noi dung khong duoc de trong. Nhap lai: ";
            getline(cin, newContent);
        }

        list[idx].setContent(newContent);
        cout << "Cap nhat noi dung thanh cong!\n";
    }

    void changeStatus() {
        int id;
        cout << "Nhap ID cong viec can chuyen trang thai: ";
        while (!(cin >> id)) {
            cout << "Nhap sai. Nhap lai ID: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        int idx = findIndexById(id);
        if (idx == -1) {
            cout << "Khong tim thay cong viec ID = " << id << "\n";
            cin.ignore(10000, '\n');
            return;
        }

        cout << "Chon trang thai moi:\n";
        cout << "2. Hoan thanh\n";
        cout << "3. Khong hoan thanh\n";
        cout << "Nhap (2/3): ";

        int c;
        while (!(cin >> c) || (c != 2 && c != 3)) {
            cout << "Chi duoc nhap 2 hoac 3. Nhap lai: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');

        Status newSt = (c == 2) ? HOAN_THANH : KHONG_HOAN_THANH;

        if (!list[idx].changeStatus(newSt)) {
            cout << "Chi duoc chuyen trang thai khi cong viec dang o trang thai 'Dang lam'!\n";
        } else {
            cout << "Chuyen trang thai thanh cong!\n";
        }
    }

    void filterByStatus() {
        cout << "Chon trang thai can loc:\n";
        cout << "1. Dang lam\n";
        cout << "2. Hoan thanh\n";
        cout << "3. Khong hoan thanh\n";
        cout << "Nhap (1-3): ";

        int s;
        while (!(cin >> s) || (s < 1 || s > 3)) {
            cout << "Chi duoc nhap 1-3. Nhap lai: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cin.ignore(10000, '\n');

        bool found = false;
        for (int i = 0; i < (int)list.size(); i++) {
            if ((int)list[i].getStatus() == s) {
                list[i].display();
                found = true;
            }
        }

        if (!found) {
            cout << "Khong co cong viec nao theo trang thai nay.\n";
        }
    }
};

int main() {
    WorkManager manager;
    int choice;

    while (true) {
        manager.showMenu();

        if (!(cin >> choice)) {
            cout << "Nhap sai kieu du lieu!\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 1) manager.showList();
        else if (choice == 2) manager.addWork();
        else if (choice == 3) manager.updateContent();
        else if (choice == 4) manager.changeStatus();
        else if (choice == 5) manager.filterByStatus();
        else if (choice == 6) {
            cout << "Thoat chuong trinh.\n";
            break;
        } else {
            cout << "Lua chon khong hop le (1-6)!\n";
        }
    }

    return 0;
}
