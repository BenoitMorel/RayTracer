#pragma once
#include <limits>

struct Interval {
  Interval(): min(std::numeric_limits<double>::infinity()), 
    max(-std::numeric_limits<double>::infinity()) {}
  Interval(double min, double max): min(min), max(max) {}
  double min;
  double max;
  double getCenter() const { return (min + max) * 0.5; }
  void unionWith(const Interval &interval) {
    min = std::min(min, interval.min);
    max = std::max(max, interval.max);
  }
  friend std::ostream& operator<<(std::ostream &os, const Interval &v) {os << v.min << " " << v.max; return os;}
};