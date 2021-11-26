#include "tiny-06.hpp"

#include <random>
#include <cstdint>
#include <stdexcept>

// Let's hope your std::random_device is implemented well.
// !! don't use as actual CSPRNG !!
class random_generator : public numbers_generator {
public:
    random_generator() = default;

    double getDouble() override {
        return double_dist(rdev);
    }
    int    getInt(int lower, int upper) override {
        return std::uniform_int_distribution<int>(lower, upper)(rdev);
    }
    bool   getBool() override {
        bool ret = bool_dist(rdev);
        return ret;
    }

    ~random_generator() override = default;

private:
    std::uniform_real_distribution<double> double_dist { 0, 1 };
    std::bernoulli_distribution bool_dist { 0.5 };
    std::random_device rdev;
};

class mersenne_twister_generator : public numbers_generator {
public:
    mersenne_twister_generator(size_t seed) :
        rng(seed)
    {}

    double getDouble() override {
        return double_dist(rng);
    }
    int    getInt(int lower, int upper) override {
        return std::uniform_int_distribution<int>(lower, upper)(rng);
    }
    bool   getBool() override {
        bool ret = bool_dist(rng);
        return ret;
    }

    ~mersenne_twister_generator() override = default;
private:
    std::uniform_real_distribution<double> double_dist { 0, 1 };
    std::bernoulli_distribution bool_dist { 0.5 };
    std::mt19937_64 rng;
};

class minstd_generator : public numbers_generator {
public:
    minstd_generator(size_t seed) :
        rng(static_cast<uint32_t>(seed)) {}

    double getDouble() override {
        return double_dist(rng);
    }
    int    getInt(int lower, int upper) override {
        return std::uniform_int_distribution<int>(lower, upper)(rng);
    }
    bool   getBool() override {
        bool ret = bool_dist(rng);
        return ret;
    }
private:
    std::uniform_real_distribution<double> double_dist { 0, 1 };
    std::bernoulli_distribution bool_dist { 0.5 };
    std::minstd_rand rng;
};

std::unique_ptr<numbers_generator> numbers_generator::create_by_kind(generator_kind kind, size_t seed) {
    switch (kind) {
        case generator_kind::random:
            return std::make_unique<random_generator>();
        case generator_kind::mersenne:
            return std::make_unique<mersenne_twister_generator>(seed);
        case generator_kind::minstd:
            return std::make_unique<minstd_generator>(seed);
        default:
            throw std::invalid_argument("Asked for unknown kind of generator!");
    }
}
