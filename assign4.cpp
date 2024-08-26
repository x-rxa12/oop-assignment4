#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class Item {
protected:
    string itemID;
    string title;
    bool available;

public:
    Item(string id, string t, bool avail) : itemID(id), title(t), available(avail) {}

    virtual ~Item() {}

    virtual void getItemDetails() const = 0;
    bool checkAvailability() const { return available; }
    virtual void checkOut() = 0;
    virtual void checkIn() = 0;
};

class Book : public Item {
private:
    string author;
    string isbn;

public:
    Book(string id, string t, bool avail, string auth, string isbn)
        : Item(id, t, avail), author(auth), isbn(isbn) {}

    void getItemDetails() const override {
        cout << "Book ID: " << itemID << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Available: " << (available ? "Yes" : "No") << endl;
    }

    void checkOut() override {
        if (available) {
            available = false;
            cout << "Book checked out: " << title << endl;
        }
        else {
            cout << "Book is not available for checkout." << endl;
        }
    }

    void checkIn() override {
        available = true;
        cout << "Book checked in: " << title << endl;
    }
};

class Journal : public Item {
private:
    string publisher;
    int issueNumber;

public:
    Journal(string id, string t, bool avail, string pub, int issue)
        : Item(id, t, avail), publisher(pub), issueNumber(issue) {}

    void getItemDetails() const override {
        cout << "Journal ID: " << itemID << endl;
        cout << "Title: " << title << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "Issue Number: " << issueNumber << endl;
        cout << "Available: " << (available ? "Yes" : "No") << endl;
    }

    void checkOut() override {
        if (available) {
            available = false;
            cout << "Journal checked out: " << title << endl;
        }
        else {
            cout << "Journal is not available for checkout." << endl;
        }
    }

    void checkIn() override {
        available = true;
        cout << "Journal checked in: " << title << endl;
    }
};

class Member {
private:
    string memberID;
    string name;
    string contactInfo;
    int loanLimit;
    vector<Item*> borrowedItems;

public:
    Member(string id, string n, string contact, int limit)
        : memberID(id), name(n), contactInfo(contact), loanLimit(limit) {}

    void borrowItem(Item* item) {
        if (item->checkAvailability() && borrowedItems.size() < loanLimit) {
            item->checkOut();
            borrowedItems.push_back(item);
        }
        else {
            cout << "Cannot borrow the item." << endl;
        }
    }

    void returnItem(Item* item) {
        auto it = find(borrowedItems.begin(), borrowedItems.end(), item);
        if (it != borrowedItems.end()) {
            item->checkIn();
            borrowedItems.erase(it);
        }
        else {
            cout << "This item was not borrowed by this member." << endl;
        }
    }

    void getLoanHistory() const {
        cout << "Loan history for member: " << name << endl;
        for (const auto& item : borrowedItems) {
            item->getItemDetails();
        }
    }
};

class Loan {
private:
    string loanID;
    Item* item;
    Member* member;
    time_t dueDate;
    time_t returnDate;

public:
    Loan(string id, Item* itm, Member* mem, time_t due, time_t ret = 0)
        : loanID(id), item(itm), member(mem), dueDate(due), returnDate(ret) {}

    double calculateFine() const {
        if (returnDate > dueDate) {
            double daysOverdue = difftime(returnDate, dueDate) / (60 * 60 * 24);
            return daysOverdue * 0.5; // Assume 0.5 currency units per day overdue
        }
        return 0.0;
    }
};

int main() {
    Book book1("0001", "Game of thrones", true, "Khursheed", "9780743273565");
    Journal journal1("0002", "Nature", true, "Hafiz publicatoins", 123);

    Member member1("0003", "M.ali ", "m.ali example.com", 5);

    member1.borrowItem(&book1);
    member1.getLoanHistory();

    time_t now = time(0);
    Loan loan1("L001", &book1, &member1, now + 10 * 24 * 60 * 60, now + 15 * 24 * 60 * 60); // 15 days overdue

    cout << "Fine: " << loan1.calculateFine() << endl;

    return 0;
}
