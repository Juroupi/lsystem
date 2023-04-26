#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <optional>
#include <bnf/grammar.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

struct State {

    float x, y, a, d;

    void rotate(float angle) {
        a = fmod(a + angle, 360);
    }

    void grow(float growth) {
        d *= growth;
    }

    void move(unsigned int alpha) {

        float rad = a / 180 * ALLEGRO_PI;

        float destx = x + cos(rad) * d;
        float desty = y + sin(rad) * d;

        al_draw_line(x, y, destx, desty, al_map_rgba(0, 0, 0, alpha), 3);

        x = destx;
        y = desty;
    }
};

class LSystem {

    float angle, growth, x, y, a, d, width, height;

    unsigned int gens, alpha;

    void exec(const string& instructions) {

        vector<State> states = { { x, y, a, d } };

        for (char c : instructions) {

            if (c == '+') {
                states.back().rotate(angle);
            } else if (c == '-') {
                states.back().rotate(-angle);
            } if (c == '>') {
                states.back().grow(growth);
            } else if (c == '<') {
                states.back().grow(1.0 / growth);
            } else if (c >= 'A' && c <= 'Z') {
                states.back().move(alpha);
            } else if (c == '[') {
                states.push_back(states.back());
            } else if (c == ']' && !states.empty()) {
                states.pop_back();
            }
        }
    }

public:

    LSystem(float angle, float growth, float x, float y, float a, float d, float width, float height, unsigned int gens, unsigned int alpha) :
        angle(angle), growth(growth), x(x), y(y), a(a), d(d), width(width), height(height), gens(gens), alpha(alpha) {}

    void display(function<string()> generateInstructions) {
        al_init();
        al_init_primitives_addon();
        al_install_keyboard();
        al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
        al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
        ALLEGRO_DISPLAY* display = al_create_display(width, height);
        ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
        ALLEGRO_EVENT event;
        al_register_event_source(queue, al_get_display_event_source(display));
        al_register_event_source(queue, al_get_keyboard_event_source());
        al_clear_to_color(al_map_rgb(255, 255, 255));
        for (unsigned int i = 0; i < gens; i++) {
            exec(generateInstructions());
        }
        al_flip_display();
        while (true) {
            al_wait_for_event(queue, &event);
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    break;
                }
            }
        }
        al_destroy_event_queue(queue);
        al_destroy_display(display);
    }
};

map<string, string> parseArgs(int argc, char* argv[]) {

    map<string, string> args;

    for (int i = 1; i < argc; i++) {

        string arg = argv[i];

        auto equal = arg.find('=');

        if (equal == string::npos) {
            throw runtime_error("Invalid argument: " + arg);
        }

        string key = arg.substr(0, equal);
        string value = arg.substr(equal + 1);

        if (key.empty() || value.empty()) {
            throw runtime_error("Invalid argument: " + arg);
        }

        args[key] = value;
    }

    return args;
}

float getFloat(const map<string, string>& args, const string& key, optional<float> defaultValue = nullopt) {

    auto it = args.find(key);

    if (it == args.end()) {

        if (!defaultValue.has_value()) {
            throw runtime_error("Missing argument: " + key);
        }

        return *defaultValue;
    }

    return stof(it->second);
}

unsigned int getUInt(const map<string, string>& args, const string& key, optional<unsigned int> defaultValue = nullopt) {

    auto it = args.find(key);

    if (it == args.end()) {

        if (!defaultValue.has_value()) {
            throw runtime_error("Missing argument: " + key);
        }

        return *defaultValue;
    }

    return (unsigned int)stoul(it->second);
}

const string& getString(const map<string, string>& args, const string& key, optional<string> defaultValue = nullopt) {

    auto it = args.find(key);

    if (it == args.end()) {

        if (!defaultValue.has_value()) {
            throw runtime_error("Missing argument: " + key);
        }

        return *defaultValue;
    }

    return it->second;
}

const big_int getBigInt(const map<string, string>& args, const string& key, optional<big_int> defaultValue = nullopt) {

    auto it = args.find(key);

    if (it == args.end()) {

        if (!defaultValue.has_value()) {
            throw runtime_error("Missing argument: " + key);
        }

        return *defaultValue;
    }

    return big_int(it->second);
}

big_int getId(big_int& id, big_int& max) {
    static rand_state state;
    if (id < 0) {
        return state.get(max);
    }
    return id;
}

int main(int argc, char* argv[]) try {

    auto args = parseArgs(argc, argv);

    auto file = getString(args, "file");
    auto start = getString(args, "start");
    auto method = getString(args, "method");
    auto angle = getFloat(args, "angle");
    auto card = getUInt(args, "card");
    auto alpha = getUInt(args, "alpha", 15);
    auto gens = getUInt(args, "gens");
    auto growth = getFloat(args, "growth", 2);
    auto x = getFloat(args, "x", 200);
    auto y = getFloat(args, "y", 200);
    auto a = getFloat(args, "a", 0);
    auto d = getFloat(args, "d", 20);
    auto width = getFloat(args, "width", 400);
    auto height = getFloat(args, "height", 400);

    LSystem lsystem(angle, growth, x, y, a, d, width, height, gens, alpha);

    Grammar grammar(file);

    if (method == "uniform") {
        lsystem.display([&]() {        
            return grammar[start].getRandomElement(card);
        }); 
    }

    else if (method == "nuniform") {
        lsystem.display([&]() {        
            return grammar[start].getNURandomElement(card);
        }); 
    }

    return 0;
}

catch (const exception& e) {
    cerr << e.what() << endl;
    return 1;
}
