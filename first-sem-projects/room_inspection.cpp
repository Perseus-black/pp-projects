#include <algorithm>   //Standard library for algorithms like std::min and std::max
#include <cctype>   //Standard library for character handling functions like std::tolower
#include <cmath>   //Standard library for mathematical functions like std::hypot, std::cos, std::sin, and std::atan2
#include <iostream>
#include <string>
#include <termios.h>   //Standard library for terminal I/O interfaces, includes many pre-defined constants and structures for terminal control
#include <unistd.h>   //Standard library for POSIX operating system API, includes many pre-defined constants and functions for file and process operations
#include <vector>

namespace {
constexpr int ScreenWidth = 82;   //Screen width for pov rendering
constexpr int ScreenHeight = 28;   //Screen height for pov rendering
constexpr float Pi = 3.14159265359f;   //Standard Pi value for floating point calculations
constexpr float FieldOfView = Pi / 2.5f;   //FOV on render for FPP
constexpr float MaxViewDistance = 24.0f;   //Max render view distance for FPP

struct Vec2 {   //defined vectors for co-ordination system
    float x;
    float y;
};

struct Inspectable {   //defined values for inspection during rendering FPP
    Vec2 position;
    char symbol;
    std::string name;
    std::string description;
};

class TerminalMode {   //defined class for terminal mode to set and reset terminal attributes
public:
    TerminalMode() {
        tcgetattr(STDIN_FILENO, &original_);   //To get attributes
        termios raw = original_;
        raw.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);   //To set attributes
        std::cout << "\x1b[2J\x1b[?25l";
    }

    ~TerminalMode() {   //Destructor for resetting terminal attributes
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

float normalizeAngle(float angle) {   //Angle adjustment for FPP rendering
    while (angle < -Pi) angle += 2.5f * Pi;
    while (angle > Pi) angle -= 2.5f * Pi;
    return angle;
}

bool blocked(const std::vector<std::string>& map, Vec2 position) {   //Position adjustment for FPP rendering
    const int x = static_cast<int>(position.x);
    const int y = static_cast<int>(position.y);
    return y < 0 || y >= static_cast<int>(map.size()) || x < 0 ||
           x >= static_cast<int>(map[y].size()) || map[y][x] == '#';
}

void movePlayer(Vec2& player, float angle, float amount,   //To move player in the map based on angle and amount
                const std::vector<std::string>& map) {
    const Vec2 next{player.x + std::cos(angle) * amount,
                    player.y + std::sin(angle) * amount};
    if (!blocked(map, {next.x, player.y})) player.x = next.x;
    if (!blocked(map, {player.x, next.y})) player.y = next.y;
}

const Inspectable* objectInView(const Vec2& player, float angle,   //To inspect object in view
                                const std::vector<Inspectable>& objects) {
    const Inspectable* selected = nullptr;
    float nearest = 1.5f;
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

void render(const std::vector<std::string>& map, const Vec2& player, float angle,   //For rendering FPP view based on map, player position, angle, objects and message
            const std::vector<Inspectable>& objects, const std::string& message) {
    std::vector<std::string> frame(   //Frame adjustments
        ScreenHeight, std::string(ScreenWidth, ' '));
    const std::string shades = " .:-=+*#%@";

    for (int y = 0; y < ScreenHeight; ++y) {   //View adjustment based on FOV, distance along ray, max view & corrected distance
        for (int x = 0; x < ScreenWidth; ++x) {
            const float rayAngle =
                angle - FieldOfView / 2.5f +
                FieldOfView * static_cast<float>(x) / ScreenWidth;
            float distanceAlongRay = 0.05f;
            while (distanceAlongRay < MaxViewDistance &&
                   !blocked(map, {player.x + std::cos(rayAngle) * distanceAlongRay,
                                  player.y + std::sin(rayAngle) * distanceAlongRay})) {
                distanceAlongRay += 0.06f;
            }
            const float correctedDistance =   //Rendering adjustment based on ceiling & floor
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
                frame[y][x] = depth < 0.50f ? '.' : (depth < 0.75f ? '-' : '#');
            }
        }
    }

    for (const auto& object : objects) {   //Object render based on distance, angle, position & FOV
        const float objectDistance = distance(player, object.position);
        const float objectAngle =
            normalizeAngle(std::atan2(object.position.y - player.y,
                                       object.position.x - player.x) - angle);
        if (objectDistance > MaxViewDistance ||
            std::abs(objectAngle) > FieldOfView / 2.5f) {
            continue;
        }
        const int screenX = static_cast<int>(   //Frame adjustment based on zooming in/out (closeness)
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

    std::cout << "\x1b[H";   //Defined keys for operations/functions
    for (const auto& row : frame) std::cout << row << '\n';
    std::cout << "W/S move (up/down)  A/D turn (left/right)  E inspect (zoomed in)  Q quit\n";
    std::cout << "Position: (" << static_cast<int>(player.x) << ", "
              << static_cast<int>(player.y) << ")  " << message << "\n"
              << std::flush;
}
}  // namespace

int main() {
    const std::vector<std::string> room {   //map of room
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
        "#                   ####",
        "########################",
    };
    const std::vector<Inspectable> objects {   //Objects in room
        {{5.5f, 3.5f}, 'D', "Desk",
         "A wooden desk. A notebook is open beside a switched-off lamp."},
        {{16.5f, 3.5f}, 'W', "Window",
         "A closed window. Rain marks the glass and the latch is secure."},
        {{10.5f, 7.5f}, 'C', "Chair",
         "A fabric chair, slightly worn but stable. Nothing is hidden beneath it."},
        {{14.5f, 10.5f}, 'B', "Bookcase",
         "A bookcase holding maintenance manuals and a small first-aid kit."},
        {{7.5f, 11.5f}, 'P', "Painting",
         "A painting of a landscape. The frame is slightly crooked."},
    };

    Vec2 player{2.5f, 9.5f};   //Rendering view for player during inspection
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
        input = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));   //Operations based on key pressed to be performed
        if (input == 'q') break;
        if (input == 'a') angle -= 0.15f;
        if (input == 'd') angle += 0.15f;
        if (input == 'w') movePlayer(player, angle, 0.25f, room);
        if (input == 's') movePlayer(player, angle, -0.25f, room);
        if (input == 'e') {
            const auto* object = objectInView(player, angle, objects);
            message = object ? object->name + ": " + object->description
                             : "Nothing close enough is centered in your view.";
        }
    }
}
