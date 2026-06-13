#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/* ===== LINKED LIST ===== */

class CompletedNode {
public:
    string subject;
    float grade;
    int credits;
    CompletedNode* next;

    CompletedNode(string sub, float g, int c) {
        subject = sub;
        grade = g;
        credits = c;
        next = nullptr;
    }
};

void addCompleted(CompletedNode*& head, string sub, float grade, int credits) {
    CompletedNode* newNode = new CompletedNode(sub, grade, credits);
    newNode->next = head;
    head = newNode;
}

bool isCompleted(CompletedNode* head, string sub) {
    while (head) {
        if (head->subject == sub) return true;
        head = head->next;
    }
    return false;
}

void removeCompleted(CompletedNode*& head, string sub) {
    if (!head) return;

    if (head->subject == sub) {
        CompletedNode* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    CompletedNode* curr = head;
    while (curr->next) {
        if (curr->next->subject == sub) {
            CompletedNode* temp = curr->next;
            curr->next = temp->next;
            delete temp;
            return;
        }
        curr = curr->next;
    }
}

void displayCompleted(CompletedNode* head) {
    cout << "\nCompleted Subjects:\n";

    if (!head) {
        cout << "None\n";
        return;
    }

    float totalPoints = 0;
    int totalCredits = 0;

    while (head) {
        cout << head->subject << " GPA: " << head->grade << "\n";
        totalPoints += head->grade * head->credits;
        totalCredits += head->credits;
        head = head->next;
    }

    if (totalCredits > 0)
        cout << "CGPA: " << totalPoints / totalCredits << "\n";
}

/* ===== STACK ===== */

class StackNode {
public:
    string subject;
    float grade;
    int credits;
    StackNode* next;

    StackNode(string s, float g, int c) {
        subject = s;
        grade = g;
        credits = c;
        next = nullptr;
    }
};

class Stack {
public:
    StackNode* top = nullptr;

    void push(string s, float g, int c) {
        StackNode* n = new StackNode(s, g, c);
        n->next = top;
        top = n;
    }
};

/* ===== QUEUE ===== */

class QueueNode {
public:
    string subject;
    QueueNode* next;

    QueueNode(string s) {
        subject = s;
        next = nullptr;
    }
};

class Queue {
public:
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;

    void enqueue(string s) {
        QueueNode* n = new QueueNode(s);
        if (!rear) front = rear = n;
        else {
            rear->next = n;
            rear = n;
        }
    }

    void clear() {
        front = rear = nullptr;
    }

    void display() {
        cout << "\nRecommended Subjects:\n";
        QueueNode* temp = front;
        while (temp) {
            cout << temp->subject << "\n";
            temp = temp->next;
        }
    }
};

/* ===== GRAPH ===== */

class AdjNode {
public:
    string subject;
    AdjNode* next;

    AdjNode(string s) {
        subject = s;
        next = nullptr;
    }
};

class GraphNode {
public:
    string name;
    int credits;
    AdjNode* prereqList;
    GraphNode* next;

    GraphNode(string n, int c) {
        name = n;
        credits = c;
        prereqList = nullptr;
        next = nullptr;
    }
};

class Graph {
public:
    GraphNode* head = nullptr;

    void addSubject(string n, int c) {
        GraphNode* node = new GraphNode(n, c);
        if (!head) head = node;
        else {
            GraphNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = node;
        }
    }

    GraphNode* findSubject(string n) {
        GraphNode* temp = head;
        while (temp) {
            if (temp->name == n) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void addPrereq(string subject, string prereq) {
        GraphNode* node = findSubject(subject);
        if (!node) return;

        AdjNode* n = new AdjNode(prereq);
        n->next = node->prereqList;
        node->prereqList = n;
    }

    bool prereqsMet(string subject, CompletedNode* completed) {
        GraphNode* node = findSubject(subject);
        AdjNode* temp = node->prereqList;

        while (temp) {
            if (!isCompleted(completed, temp->subject))
                return false;
            temp = temp->next;
        }
        return true;
    }

    void generateRecommendations(Queue& q, CompletedNode* completed) {
        q.clear();
        GraphNode* temp = head;

        while (temp) {
            if (!isCompleted(completed, temp->name) &&
                prereqsMet(temp->name, completed)) {
                q.enqueue(temp->name);
            }
            temp = temp->next;
        }
    }
};

/* ===== FILE I/O ===== */

const string FILE_NAME = "progress.txt";

void saveProgress(CompletedNode* head) {
    ofstream file(FILE_NAME);
    while (head) {
        file << head->subject << " "
             << head->grade << " "
             << head->credits << "\n";
        head = head->next;
    }
}

/* ===== MAIN ===== */

Graph buildGraph() {
    Graph g;

    g.addSubject("CSE103", 3);
    g.addSubject("Math1", 3);
    g.addSubject("OOP", 3);
    g.addSubject("DSA", 3);

    g.addPrereq("OOP", "CSE103");
    g.addPrereq("DSA", "OOP");

    return g;
}

int main() {
    CompletedNode* completed = nullptr;
    Stack undo;
    Queue rec;
    Graph g = buildGraph();

    int choice;

    while (true) {
        cout << "\n1.Add 2.Show 3.Recommend 4.Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string s;
            float gpa;
            int c;

            cout << "Subject: ";
            cin >> s;

            cout << "GPA: ";
            cin >> gpa;

            cout << "Credits: ";
            cin >> c;

            addCompleted(completed, s, gpa, c);
            undo.push(s, gpa, c);
        }

        else if (choice == 2) {
            displayCompleted(completed);
        }

        else if (choice == 3) {
            g.generateRecommendations(rec, completed);
            rec.display();
        }

        else if (choice == 4) {
            break;
        }
    }

    return 0;
}