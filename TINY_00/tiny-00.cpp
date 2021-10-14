#include "tiny-00.hpp"

#include <ostream>
#include <iostream>
#include <iomanip>
#include <limits>
// don't forget to include appropriate headers

void write_stats(std::vector<double> const& data, std::ostream& out) {
    double min = std::numeric_limits<double>::max(), max = -std::numeric_limits<double>::max(), mean = 0;
    for (double number : data) {
        if(number > max){
            max = number;
        }
        if(number < min){
            min = number;
        }
        mean+=number;
    }
    mean = mean/data.size();
    out << "min: ";
    out << std::fixed << std::setprecision(2) << min << std::endl;
    out << "max: ";
    out << std::fixed << std::setprecision(2) << max << std::endl;
    out << "mean: ";
    out << std::fixed << std::setprecision(2) << mean << std::endl;
}
