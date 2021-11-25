#pragma once

#include <memory>
#include <limits>

enum class generator_kind {
    random,
    mersenne,
    minstd
};


class numbers_generator {
public:
    int numberOfBools = 0;
    int numberOfTrue = 0;
    int numberOfFalse = 0;

    int numberOfDoubles = 0;
    double maxDouble = std::numeric_limits<double>::lowest();
    double minDouble = std::numeric_limits<double>::max();

    int numberOfInts = 0;
    int maxInt = std::numeric_limits<int>::lowest();
    int minInt = std::numeric_limits<int>::max();
    /**
     * Vygeneruje náhodné číslo v intervalu [0, 1)
     */
    double random_double(){
        numberOfDoubles++;
        double ret = getDouble();
        if (ret > maxDouble){
            maxDouble = ret;
        }
        if (ret < minDouble){
            minDouble = ret;
        }
        return ret;
    }
    /**
     * Vygeneruje náhodné celé číslo v intervalu [lower, upper]
     */
    int random_int(int lower, int upper){
        numberOfInts++;
        int ret = getInt(lower, upper);
        if (ret > maxInt){
            maxInt = ret;
        }
        if (ret < minInt){
            minInt = ret;
        }
        return ret;
    }
    /**
     * Vygeneruje náhodnou pravdivostní hodnotu (true/false)
     */
    bool random_bool(){
        this->numberOfBools++;
        bool ret = getBool();
        if(ret){
            this->numberOfTrue++;
        } else {
            this->numberOfFalse++;
        }
        return ret;
    }


    virtual ~numbers_generator() = default;

    static std::unique_ptr<numbers_generator> create_by_kind(generator_kind kind, size_t seed);

    double min_generated_double() const {return minDouble;}
    double max_generated_double() const {return maxDouble;}
    size_t doubles_generated() const {return numberOfDoubles;}
    int min_generated_int() const {return minInt;}
    int max_generated_int() const {return maxInt;}
    size_t ints_generated() const {return numberOfInts;}
    size_t trues_generated() const {return numberOfTrue;}
    size_t falses_generated() const {return numberOfFalse;}
    size_t bools_generated() const {return numberOfBools;}
private:
    virtual bool   getBool() = 0;
    virtual int    getInt(int lower, int upper) = 0;
    virtual double getDouble() = 0;

};
