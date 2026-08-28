#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

struct Vec2 {
    float x, y;
};

struct Node {
    Vec2 pos;
    Vec2 prev;
    bool fixed = false;
};

struct Edge {
    int a, b;
    float rest = 1.0f;
};

struct System {
    std::vector<Node> nodes;
    std::vector<Edge> edges;
};

// physics step (Verlet-style)
void step(System& s) {
    for (auto& n : s.nodes) {
        if (n.fixed) continue;

        Vec2 vel = {n.pos.x - n.prev.x, n.pos.y - n.prev.y};

        n.prev = n.pos;
        n.pos.x += vel.x;
        n.pos.y += vel.y;
    }

    // spring constraints
    for (auto& e : s.edges) {
        Node& A = s.nodes[e.a];
        Node& B = s.nodes[e.b];

        float dx = B.pos.x - A.pos.x;
        float dy = B.pos.y - A.pos.y;

        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist == 0) continue;

        float diff = (dist - e.rest) / dist;

        float adjustX = dx * 0.5f * diff;
        float adjustY = dy * 0.5f * diff;

        if (!A.fixed) {
            A.pos.x += adjustX;
            A.pos.y += adjustY;
        }
        if (!B.fixed) {
            B.pos.x -= adjustX;
            B.pos.y -= adjustY;
        }
    }
}

// move operations (like “string grabs”)
void applyMove(System& s, int move) {
    switch (move) {

        case 0: // pull center
            s.nodes[2].pos.y -= 0.5f;
            break;

        case 1: // widen
            s.nodes[1].pos.x -= 0.5f;
            s.nodes[3].pos.x += 0.5f;
            break;

        case 2: // twist
            std::swap(s.nodes[1].pos, s.nodes[3].pos);
            break;

        case 3: // tighten structure (add constraint)
            if (s.edges.size() < 6)
                s.edges.push_back({1, 3, 1.0f});
            break;

        case 4: // release
            if (!s.edges.empty())
                s.edges.pop_back();
            break;
    }
}

// simple ASCII render
void render(const System& s) {
    const int W = 20, H = 10;
    char grid[H][W];

    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            grid[y][x] = '.';

    for (int i = 0; i < s.nodes.size(); i++) {
        int x = (int)(s.nodes[i].pos.x * 5 + 10);
        int y = (int)(s.nodes[i].pos.y * 2 + 5);

        if (x >= 0 && x < W && y >= 0 && y < H)
            grid[y][x] = 'A' + i;
    }

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++)
            std::cout << grid[y][x];
        std::cout << "\n";
    }
}

int main() {
    System s;

    // 4 “finger points”
    s.nodes = {
        {{0,0},{0,0},true},
        {{-1,1},{-1,1},false},
        {{0,1},{0,1},false},
        {{1,1},{1,1},false}
    };

    for (int t = 0; t < 30; t++) {

        std::cout << "\nTurn " << t << "\n";
        render(s);

        std::cout << "Move (0 pull,1 widen,2 twist,3 bind,4 release): ";
        int m;
        std::cin >> m;

        applyMove(s, m);

        // physics relaxation loop
        for (int i = 0; i < 5; i++)
            step(s);
    }

    return 0;
}