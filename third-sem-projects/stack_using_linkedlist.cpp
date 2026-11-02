/* #include <iostream>
using namespace std;

class Node {                        // Node structure
public:
int data;
Node* next;

Node(int x) {
data = x;
next = NULL;
}
};  
                                
class myStack {                     // Stack implementation using linked list
Node* top;
                                    
int count;                          // To store current size of stack
public:
myStack() {

top = NULL;                         // initially stack is empty
count = 0;
}

void push(int x) {                  // push operation
Node* temp = new Node(x);
temp->next = top;
top = temp;
count++;
}

int pop() {                         // pop operation
if (top == NULL) {
cout << "Stack Underflow" << endl;
return -1;
}

Node* temp = top;
top = top->next;
int val = temp->data;
count--;
delete temp;
return val;
}

int peek() {                        // peek operation
if (top == NULL) {
cout << "Stack is Empty" << endl;
return -1;
}
return top->data;
}


bool isEmpty() {                    // check if stack is empty
return top == NULL;
}

int size() {                        // size of stack
return count;
}
};

int main() {
myStack st;                         // pushing elements

st.push(1);
st.push(2);
st.push(3);
st.push(4);

cout << "Popped: " << st.pop() << endl;         // popping one element

cout << "Top element: " << st.peek() << endl;   // checking top element

cout << "Is stack empty: " << (st.isEmpty() ? "Yes" : "No") << endl;        // checking if stack is empty

cout << "Current size: " << st.size() << endl;  // checking current size

return 0;
} */

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;
};

bool compareEdges(const Edge& a, const Edge& b) {
    return a.w < b.w;
}

struct DSU {
    vector<int> parent, sz;

    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x)
            return x;

        return parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (sz[a] < sz[b])
            swap(a, b);

        parent[b] = a;
        sz[a] += sz[b];

        return true;
    }
};

int main() {
    int V = 4;

    vector<Edge> edges = {
        {0, 1, 1},
        {1, 3, 2},
        {0, 3, 3},
        {0, 2, 4},
        {2, 3, 5}
    };

    sort(edges.begin(), edges.end(), compareEdges);

    DSU dsu(V);
    int totalWeight = 0;
    int selectedEdges = 0;

    for (const Edge& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            cout << e.u << " - " << e.v
                 << " : " << e.w << '\n';

            totalWeight += e.w;
            selectedEdges++;

            if (selectedEdges == V - 1)
                break;
        }
    }

    cout << "Total weight: " << totalWeight << '\n';

    if (selectedEdges != V - 1)
        cout << "Graph is disconnected.\n";
}