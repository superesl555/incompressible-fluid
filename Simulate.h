#include "Fixed.h"

constexpr int N = 36;
constexpr int M = 84;

int N_field;
int M_field;

constexpr std::array<std::pair<int, int>, 4> deltas{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

template <typename T_f>
struct VectorField {
    std::array<T_f, deltas.size()> v[N][M];
    T_f &add(int x, int y, int dx, int dy, T_f dv) {
        return get(x, y, dx, dy) += dv;
    }

    T_f &get(int x, int y, int dx, int dy) { 
        auto it = std::find(deltas.begin(), deltas.end(), std::make_pair(dx, dy)); 
        assert(it != deltas.end()); 
        size_t i = std::distance(deltas.begin(), it);
        return v[x][y][i]; 
    } 
};

template <class T,class U,class V>
class Simulate {
public:
    Simulate() : rnd(1337), field(N_field, std::vector<char>(M_field + 1)){ initialize_field(); }

    void execute(size_t Tike);

    bool propagate_move(int x, int y, bool is_first);
    std::tuple<T, bool, std::pair<int, int>> propagate_flow(int x, int y, T lim);
    void propagate_stop(int x, int y, bool force = false);
    T move_prob(int x, int y);
    T random01() {
        std::uniform_real_distribution<double> dist(0.0, 1.0); 
        return static_cast<T>(dist(rnd));
    }

    T p[N][M];
    VectorField<U> velocity{}; 
    VectorField<V> velocity_flow{};

    T rho[256];
    T old_p[N][M];
    int last_use[N][M]{};
    int UT = 0;
    int dirs[N][M]{};
    std::mt19937 rnd;

    void initialize_field() {
        for (int i = 0; i < N_field; ++i) {
            for (int j = 0; j < M_field; ++j) {
                if (i == 0 || i == N_field - 1 || j == 0 || j == M_field - 1) {
                    field[i][j] = '#';
                } else {
                    field[i][j] = ' ';
                }
            }
        }

        for (int i = 6; i <= 9; ++i) {
            for (int j = 37; j <= 45; ++j) {
                field[i][j] = '.';
            }
        }
    }

    std::vector<std::vector<char>> field;

    // char field[N][M + 1] = {
    // "####################################################################################",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                       .........                                  #",
    // "#..............#            #           .........                                  #",
    // "#..............#            #           .........                                  #",
    // "#..............#            #           .........                                  #",
    // "#..............#            #                                                      #",
    // "#..............#            #                                                      #",
    // "#..............#            #                                                      #",
    // "#..............#            #                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............#                                                      #",
    // "#..............#............################                     #                 #",
    // "#...........................#....................................#                 #",
    // "#...........................#....................................#                 #",
    // "#...........................#....................................#                 #",
    // "##################################################################                 #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "#                                                                                  #",
    // "####################################################################################",
    // };

    struct ParticleParams {
        char type;
        T cur_p;
        std::array<U, deltas.size()> v;

        void swap_with(Simulate& sim, int x, int y) {
            std::swap(sim.field[x][y], type);
            std::swap(sim.p[x][y], cur_p);
            std::swap(sim.velocity.v[x][y], v);
        }
    };
};

template <class T, class U, class V>
std::tuple<T, bool, std::pair<int, int>> Simulate<T, U, V>::propagate_flow(int x, int y, T lim) {
    last_use[x][y] = UT - 1;
    T ret = 0;
    for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT) {
            auto cap = velocity.get(x, y, dx, dy);
            auto flow = velocity_flow.get(x, y, dx, dy);
            if (flow == cap) {
                continue;
            }
            // assert(v >= velocity_flow.get(x, y, dx, dy));
            auto vp = std::min(lim, static_cast<decltype(lim)>(cap - flow));
            if (last_use[nx][ny] == UT - 1) {
                velocity_flow.add(x, y, dx, dy, static_cast<V>(vp));
                last_use[x][y] = UT;
                // cerr << x << " " << y << " -> " << nx << " " << ny << " " << vp << " / " << lim << "\n";
                return {vp, 1, {nx, ny}};
            }
            auto [t, prop, end] = propagate_flow(nx, ny, vp);
            ret += t;
            if (prop) {
                velocity_flow.add(x, y, dx, dy, static_cast<V>(t));
                last_use[x][y] = UT;
                // cerr << x << " " << y << " -> " << nx << " " << ny << " " << t << " / " << lim << "\n";
                return {t, prop && end != std::pair(x, y), end};
            }
        }
    }
    last_use[x][y] = UT;
    return {ret, 0, {0, 0}};
}


template <class T, class U, class V>
void Simulate<T,U,V>::propagate_stop(int x, int y, bool force) {
    if (!force) {
    bool stop = true;
    for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) > 0) {
            stop = false;
            break;
        }
    }
    if (!stop) {
        return;
    }
    }
    last_use[x][y] = UT;
    for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT || velocity.get(x, y, dx, dy) > 0) {
            continue;
        }
        propagate_stop(nx, ny);
    }
}

template <class T, class U, class V>
T Simulate<T,U,V>::move_prob(int x, int y) {
    T sum = 0;
    for (size_t i = 0; i < deltas.size(); ++i) {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
            continue;
        }
        auto v = velocity.get(x, y, dx, dy);
        if (v < 0) {
            continue;
        }
        sum += v;
    }
    return sum;
}

template <class T, class U, class V>
bool Simulate<T,U,V>::propagate_move(int x, int y, bool is_first) {
    last_use[x][y] = UT - is_first;
    bool ret = false;
    int nx = -1, ny = -1;
    do {
        std::array<T, deltas.size()> tres;
        T sum = 0;
        for (size_t i = 0; i < deltas.size(); ++i) {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                tres[i] = sum;
                continue;
            }
            auto v = velocity.get(x, y, dx, dy);
            if (v < 0) {
                tres[i] = sum;
                continue;
            }
            sum += v;
            tres[i] = sum;
        }

        if (sum == 0) {
            break;
        }

        T p = random01() * sum;
        size_t d = std::ranges::upper_bound(tres, p) - tres.begin();

        auto [dx, dy] = deltas[d];
        nx = x + dx;
        ny = y + dy;
        assert(velocity.get(x, y, dx, dy) > 0 && field[nx][ny] != '#' && last_use[nx][ny] < UT);

        ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
    } while (!ret);
    last_use[x][y] = UT;
    for (size_t i = 0; i < deltas.size(); ++i) {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) < 0) {
            propagate_stop(nx, ny);
        }
    }
    if (ret) {
        if (!is_first) {
            ParticleParams pp{};
            pp.swap_with(*this,x, y);
            pp.swap_with(*this,nx, ny);
            pp.swap_with(*this,x, y);
        }
    }
    return ret;
}

template <class T, class U, class V>
void Simulate<T,U,V>::execute(size_t Tike) {
    rho[' '] = 0.01;
    rho['.'] = 1000;
    T g = 0.1;
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
            if (field[x][y] == '#')
                continue;
            for (auto [dx, dy] : deltas) {
                dirs[x][y] += (field[x + dx][y + dy] != '#');
            }
        }
    }

    for (size_t i = 0; i < Tike; ++i) {
        
        // Apply external forces
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] == '#')
                    continue;
                if (field[x + 1][y] != '#')
                    velocity.add(x, y, 1, 0, static_cast<U>(g));
            }
        }

        // Apply forces from p
        memcpy(old_p, p, sizeof(p));
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] == '#')
                    continue;
                for (auto [dx, dy] : deltas) {
                    int nx = x + dx, ny = y + dy;
                    if (field[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y]) {
                        auto delta_p = old_p[x][y] - old_p[nx][ny];
                        auto force = delta_p;
                        auto &contr = velocity.get(nx, ny, -dx, -dy);
                        if (contr * rho[(int) field[nx][ny]] >= force) {
                            auto cur = force / rho[(int) field[nx][ny]];
                            contr -= cur;
                            continue;
                        }
                        auto cur = contr * rho[(int) field[nx][ny]];
                        force -= cur;
                        contr = 0;
                        velocity.add(x, y, dx, dy, static_cast<U>(force / rho[(int) field[x][y]]));
                        p[x][y] -= force / dirs[x][y];
                    }
                }
            }
        }

        // Make flow from velocities
        velocity_flow = {};
        bool prop = false;
        do {
            UT += 2;
            prop = 0;
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] != '#' && last_use[x][y] != UT) {
                        auto [t, local_prop, _] = propagate_flow(x, y, 1);
                        if (t > 0) {
                            prop = 1;
                        }
                    }
                }
            }
        } while (prop);

        // Recalculate p with kinetic energy
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] == '#')
                    continue;
                for (auto [dx, dy] : deltas) {
                    auto old_v = velocity.get(x, y, dx, dy);
                    auto new_v = velocity_flow.get(x, y, dx, dy);
                    if (old_v > 0) {
                        // assert(new_v <= old_v);
                        velocity.get(x, y, dx, dy) = new_v;
                        auto force = (old_v - new_v) * rho[(int) field[x][y]];
                        if (field[x][y] == '.')
                            force *= 0.8;
                        if (field[x + dx][y + dy] == '#') {
                            auto cur = force / dirs[x][y];
                            p[x][y] += cur;
                        } else {
                            auto cur = force / dirs[x + dx][y + dy];
                            p[x + dx][y + dy] += cur;
                        }
                    }
                }
            }
        }

        UT += 2;
        prop = false;
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] != '#' && last_use[x][y] != UT) {
                    if (random01() < move_prob(x, y)) {
                        prop = true;
                        propagate_move(x, y, true);
                    } else {
                        propagate_stop(x, y, true);
                    }
                }
            }
        }

        if (prop) {
            std::cout << "Tick " << i << ":\n";
            for (size_t x = 0; x < N_field; ++x) {
                for (char c : field[x]) {
                    std::cout << c; 
                }
                std::cout << "\n";
            }
        }

        // Запись в файл
        std::ofstream file("output.txt");
        file << "Тик: " << i << ", g: " << g << "\n";
        for (size_t x = 0; x < N_field; ++x) {
            for (char c : field[x]) {
                file << c;
            }
            file << "\n";
        }
    }
}
