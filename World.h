#ifndef TOO_HOT_OUTSIDE_FOR_A_BIT_TOO_LONG_WORLD_H
#define TOO_HOT_OUTSIDE_FOR_A_BIT_TOO_LONG_WORLD_H

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include "Player.h"
#include "Block.h"
#include "PerlinNoise.h"
#include "Material.h"

using namespace std;

class World {
public:
    string name = "Unnamed";


    int size = 100;
    short seed = 0;

    int getSeed() { return seed; };

    int getSize() { return size; };

    void setSize(int size) { this->size = size; };

    void setSeed(int seed) { this->seed = seed; };


    vector<Player> players;
    vector<vector<unique_ptr<Block>>> map;


    Block getBlock(int x, int y) {
        return *map[x][y];
    };

    vector<vector<Block>> getMap() {
        vector<vector<Block>> map2 = vector<vector<Block>>(size, vector<Block>(size));
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                map2[i][j] = *map[i][j];
            }
        }
        return map2;
    };

    void setBlock(int x, int y, string b) {
        if (b == "-") {
            map[x][y] = make_unique<Block>(Material::Blocks::AIR);
        } else if (b == "|") {
            map[x][y] = make_unique<Block>(Material::Blocks::WOOD);
        } else if (b == "#") {
            map[x][y] = make_unique<Block>(Material::Blocks::STONE);
        } else if (b == "*") {
            map[x][y] = make_unique<Block>(Material::Blocks::SAND);
        } else if (b == ",") {
            map[x][y] = make_unique<Block>(Material::Blocks::WATER);
        } else if (b == "%") {
            map[x][y] = make_unique<Block>(Material::Blocks::WORKBENCH);
        } else if (b == "▢") {
            map[x][y] = make_unique<Block>(Material::Blocks::WATER_SCAFFOLDING);
        } else if (b == "▣") {
            map[x][y] = make_unique<Block>(Material::Blocks::MECHANICAL_SIEVE);
        } else if (b == "₪") {
            map[x][y] = make_unique<Block>(Material::Blocks::MECHANICAL_CRUSHER);
        }
    };

    void setBlock(int x, int y, Block block) {
        map[x][y] = make_unique<Block>(block);
    };

    void setMap(vector<string> Map) {
        for (int i = 0; i < Map.size(); i++) {
            for (int j = 0; j < Map[i].length(); j++) {
                setBlock(i, j, &Map[i][j]);
            }
        }
    };

    vector<Player> getPlayers() {
        return players;
    };

    Player getPlayer(int num) {
        return players[num];
    };

    void setPlayer(int num, Player player) {
        players[num] = player;
    };

    void removePlayer(string player) {
        players.erase(players.begin() + this->findPlayer(player));
    };

    void addPlayer(const Player &player) {
        players.push_back(player);
    };

    int findPlayer(string Name) {
        for (int i = 0; i < players.size(); i++) {
            if (players[i].getName() == Name) {
                return i;
            }
        }
        return -1;
    };

    void updatePlayer(Player player) {
        players[this->findPlayer(player.getName())] = player;
    }

    void printMap(Player player) {
        static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD topLeft = {0, 0};
        SetConsoleCursorPosition(hOut, topLeft);
        cout << "                                                 ";
        SetConsoleCursorPosition(hOut, topLeft);
        cout << "Coordinates: " << player.getX() << ", " << player.getY() << " Name: " << player.name << endl;
        int x, y;
        x = player.getX();
        y = player.getY();
        bool isP = false;
        for (int i = y - 15; i < y + 15; i++) {
            for (int j = x - 15; j < x + 15; j++) {
                isP = false;
                if (i < 0 || j < 0 || i >= size || j >= size) {
                    cout << "  ";
                } else {
                    for (int k = 0; k < players.size(); k++) {
                        if (players[k].getX() == j && players[k].getY() == i) {
                            SetConsoleTextAttribute(hOut, player.getColor());
                            cout << players[k].getSymbol();
                            SetConsoleTextAttribute(hOut, 7);
                            cout << " ";
                            isP = true;
                            break;
                        } else {
                            isP = false;
                        }
                    }
                    if (!isP) {
                        SetConsoleTextAttribute(hOut, map[i][j]->getColor());
                        cout << map[i][j]->getSymbol();
                        SetConsoleTextAttribute(hOut, 7);
                        cout << " ";
                    }
                }
            }
            cout << endl;
        }
    }

    void generate(unsigned int Seed) {
        map.resize(size);
        for (auto &a: map) {
            a.resize(size);
        }
        PerlinNoise pn(Seed);
        for (int i = 0; i < size; ++i) {     // y
            for (int j = 0; j < size; ++j) {  // x
                double n = pn.noise(((double) j / 10), ((double) i / 10), 0.8);
                if (n < 0.2) {
                    this->setBlock(i, j, ",");
                } else if (n >= 0.2 && n < 0.3) {
                    this->setBlock(i, j, "|");
                } else if (n >= 0.3 && n < 0.37) {
                    this->setBlock(i, j, "*");
                } else if (n >= 0.37 && n < 0.68) {
                    this->setBlock(i, j, "-");
                } else {
                    this->setBlock(i, j, "#");
                }
            }
        }
    }

    void saveFile() {
        fstream file;
        file.open("saves/" + name + ".txt", ios::out);
        if (file.is_open()) {
            file << name << " " << seed << " " << size << endl;
            file << endl;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    file << i << " " << j << " ";
                    file << map[i][j]->type << " " << map[i][j]->id;
                    if (map[i][j]->type == "Block") {
                    } else if (map[i][j]->type == "Workbench") {
                    } else if (map[i][j]->type == "MechanicalSieve") {
                    } else if (map[i][j]->type == "MechanicalCrusher") {
                    }
                    file << endl;
                }
            }
            file << players.size() << endl;
            for (int playerN = 0; playerN < players.size(); playerN++) {
                file << players[playerN].name << " " << players[playerN].x << " " << players[playerN].y << " "
                     << players[playerN].color << " "
                     << players[playerN].symbol << " " << players[playerN].selectedSlot << " " << endl;
                for (int itemsN = 0; itemsN < 9; itemsN++) {
                    file << players[playerN].inventory.items[itemsN].id << " "
                         << players[playerN].inventory.items[itemsN].quantity << endl;
                }
            }
        }
    }

    void loadFile(const string &dir) {
        fstream file;
        file.open(dir, ios::in);
        if (file.is_open()) {
            string line;
            getline(file, line);
            stringstream s(line);
            s >> name >> seed >> size;
            getline(file, line);
            map.resize(size);
            for (auto &a: map) {
                a.resize(size);
            }
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    getline(file, line);
                    stringstream ss(line);
                    string type;
                    int x, y, id;
                    ss >> x >> y >> type >> id;
                    if (type == "Block") {
                    } else if (type == "Workbench") {
                    } else if (type == "MechanicalSieve") {
                    } else if (type == "MechanicalCrusher") {
                    }
                    map[x][y] = Material::Blocks::get(id);
                }
            }
            getline(file, line);
            int pM = stoi(line);
            for (int playerN = 0; playerN < pM; playerN++) {
                Player player;
                getline(file, line);
                stringstream ss(line);
                ss >> player.name >> player.x >> player.y >> player.color >> player.symbol >> player.selectedSlot;
                for (int itemsN = 0; itemsN < 9; itemsN++) {
                    getline(file, line);
                    stringstream ss(line);
                    int id, quantity;
                    ss >> id >> quantity;
                    player.inventory.items[itemsN] = Material::Items::get(id);
                    player.inventory.items[itemsN].quantity = quantity;
                }
                players.push_back(player);
            }
        }
    }
};

#endif
