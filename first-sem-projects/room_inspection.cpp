#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

namespace {
constexpr int ScreenWidth = 88;
constexpr int ScreenHeight = 28;
constexpr float Pi = 3.14159265359f;
constexpr float FieldOfView = Pi / 3.0f;
constexpr float MaxViewDistance = 16.0f;

struct Vec2 {
    float x;
    float y;
};

struct Inspectable {
    Vec2 position;
    char symbol;
    std::string name;
    std::string description;
};

class TerminalMode {
public:
    TerminalMode() {
        tcgetattr(STDIN_FILENO, &original_);
        termios raw = original_;
        raw.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        std::cout << "\x1b[2J\x1b[?25l";
    }

    ~TerminalMode() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_);
        std::cout << "\x1b[0m\x1b[?25h\n";
    }

    TerminalMode(const TerminalMode&) = delete;
    TerminalMode& operator=(const TerminalMode&) = delete;

private:
    termios original_{};
};

float distance(Vec2 a, Vec2 b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

float normalizeAngle(float angle) {
    while (angle < -Pi) angle += 2.0f * Pi;
    while (angle > Pi) angle -= 2.0f * Pi;
    return angle;
}

bool blocked(const std::vector<std::string>& map, Vec2 position) {
    const int x = static_cast<int>(position.x);
    const int y = static_cast<int>(position.y);
    return y < 0 || y >= static_cast<int>(map.size()) || x < 0 ||
           x >= static_cast<int>(map[y].size()) || map[y][x] == '#';
}

void movePlayer(Vec2& player, float angle, float amount,
                const std::vector<std::string>& map) {
    const Vec2 next{player.x + std::cos(angle) * amount,
                    player.y + std::sin(angle) * amount};
    if (!blocked(map, {next.x, player.y})) player.x = next.x;
    if (!blocked(map, {player.x, next.y})) player.y = next.y;
}

const Inspectable* objectInView(const Vec2& player, float angle,
                                const std::vector<Inspectable>& objects) {
    const Inspectable* selected = nullptr;
    float nearest = 1.2f;
    for (const auto& object : objects) {
        const float objectAngle =
            std::atan2(object.position.y - player.y, object.position.x - player.x);
        const float angularDistance =
            std::abs(normalizeAngle(objectAngle - angle));
        const float objectDistance = distance(player, object.position);
        if (objectDistance < nearest && angularDistance < 0.10f) {
            nearest = objectDistance;
            selected = &object;
        }
    }
    return selected;
}

void render(const std::vector<std::string>& map, const Vec2& player, float angle,
            const std::vector<Inspectable>& objects, const std::string& message) {
    std::vector<std::string> frame(
        ScreenHeight, std::string(ScreenWidth, ' '));
    const std::string shades = " .:-=+*#%@";

    for (int y = 0; y < ScreenHeight; ++y) {
        for (int x = 0; x < ScreenWidth; ++x) {
            const float rayAngle =
                angle - FieldOfView / 2.0f +
                FieldOfView * static_cast<float>(x) / ScreenWidth;
            float distanceAlongRay = 0.05f;
            while (distanceAlongRay < MaxViewDistance &&
                   !blocked(map, {player.x + std::cos(rayAngle) * distanceAlongRay,
                                  player.y + std::sin(rayAngle) * distanceAlongRay})) {
                distanceAlongRay += 0.04f;
            }
            const float correctedDistance =
                distanceAlongRay * std::cos(rayAngle - angle);
            const int wallHeight = std::min(
                ScreenHeight, static_cast<int>(ScreenHeight / correctedDistance));
            const int ceiling = (ScreenHeight - wallHeight) / 2;
            const int floor = ScreenHeight - ceiling;

            if (y < ceiling) {
                frame[y][x] = ' ';
            } else if (y < floor) {
                const float brightness =
                    std::max(0.0f, 1.0f - correctedDistance / MaxViewDistance);
                frame[y][x] = shades[static_cast<std::size_t>(
                    brightness * static_cast<float>(shades.size() - 1))];
            } else {
                const float depth = static_cast<float>(y - ScreenHeight / 2) /
                                    static_cast<float>(ScreenHeight / 2);
                frame[y][x] = depth < 0.45f ? '.' : (depth < 0.75f ? '-' : '#');
            }
        }
    }

    for (const auto& object : objects) {
        const float objectDistance = distance(player, object.position);
        const float objectAngle =
            normalizeAngle(std::atan2(object.position.y - player.y,
                                       object.position.x - player.x) - angle);
        if (objectDistance > MaxViewDistance ||
            std::abs(objectAngle) > FieldOfView / 2.0f) {
            continue;
        }
        const int screenX = static_cast<int>(
            (objectAngle / FieldOfView + 0.5f) * ScreenWidth);
        const int objectHeight = std::max(
            1, std::min(ScreenHeight / 2,
                        static_cast<int>(ScreenHeight / objectDistance)));
        const int top = ScreenHeight / 2 - objectHeight / 2;
        for (int y = std::max(0, top);
             y < std::min(ScreenHeight, top + objectHeight); ++y) {
            if (screenX >= 0 && screenX < ScreenWidth) {
                frame[y][screenX] = object.symbol;
            }
        }
    }

    std::cout << "\x1b[H";
    for (const auto& row : frame) std::cout << row << '\n';
    std::cout << "W/S move  A/D turn  E inspect  Q quit\n";
    std::cout << "Position: (" << static_cast<int>(player.x) << ", "
              << static_cast<int>(player.y) << ")  " << message << "\n"
              << std::flush;
}
}  // namespace

int main() {
    const std::vector<std::string> room{
        "########################",
        "#                      #",
        "#    ####       ####   #",
        "#    #  #       #  #   #",
        "#    #  #########  #   #",
        "#                     ##",
        "#       ##             #",
        "#       ##      ###    #",
        "#              ###     #",
        "#                      #",
        "#   ######             #",
        "#                      #",
        "########################",
    };
    const std::vector<Inspectable> objects{
        {{5.5f, 3.5f}, 'D', "Desk",
         "A wooden desk. A notebook is open beside a switched-off lamp."},
        {{16.5f, 3.5f}, 'W', "Window",
         "A closed window. Rain marks the glass and the latch is secure."},
        {{10.5f, 7.5f}, 'C', "Chair",
         "A fabric chair, slightly worn but stable. Nothing is hidden beneath it."},
        {{14.5f, 10.5f}, 'B', "Bookcase",
         "A bookcase holding maintenance manuals and a small first-aid kit."},
    };

    Vec2 player{2.5f, 9.5f};
    float angle = 0.0f;
    std::string message = "Look at an object and press E to inspect it.";
    TerminalMode terminal;

    while (true) {
        render(room, player, angle, objects, message);
        char input = '\0';
        if (read(STDIN_FILENO, &input, 1) != 1) {
            usleep(16000);
            continue;
        }
        input = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));
        if (input == 'q') break;
        if (input == 'a') angle -= 0.12f;
        if (input == 'd') angle += 0.12f;
        if (input == 'w') movePlayer(player, angle, 0.22f, room);
        if (input == 's') movePlayer(player, angle, -0.22f, room);
        if (input == 'e') {
            const auto* object = objectInView(player, angle, objects);
            message = object ? object->name + ": " + object->description
                             : "Nothing close enough is centered in your view.";
        }
    }
}
