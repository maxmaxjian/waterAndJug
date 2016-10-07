#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

class jug {
private:
  int my_capacity;
  int my_water;

public:
  jug(int cap, int wat = 0) : my_capacity(cap), my_water(wat){}
  jug(const jug & other) {
    if (this != &other) {
      my_capacity = other.my_capacity;
      my_water = other.my_water;
    }
  }
  
  int water() const {return my_water;}
  int capacity() const {return my_capacity;}

  void fill() { my_water = my_capacity; }
  void empty() { my_water = 0; }
  void pourTo(jug & other) {
    if (my_water >= other.capacity()-other.water()) {
      my_water -= other.capacity()-other.water();
      other.fill();
    }
    else {
      other.my_water += my_water;
      empty();
    }
  }

  bool equals(const jug & other) const {
    return my_water == other.my_water && my_capacity == other.my_capacity;
  }
};

bool operator==(const jug & j1, const jug & j2) {
  return j1.equals(j2);
}

using state = std::pair<jug,jug>;

bool operator==(const state & s1, const state & s2) {
  return s1.first == s2.first && s1.second == s2.second;
}

void print(const state & s) {
  std::cout << "(" << s.first.water() << "," << s.second.water() << ")";
}

class solution {
  public:
    bool canMeasureWater(int x, int y, int z) {
      jug small(std::min(x,y)), large(std::max(x,y));
      auto start = std::make_pair(small, large);
      auto finish = std::make_pair(jug(std::min(x,y)), jug(std::max(x,y), z));
      std::vector<std::vector<state>> paths = addOne(std::vector<state>{start}, finish);

      size_t i = 0;
      for (auto path : paths) {
	std::cout << "Possible process " << i++ << ":" << std::endl;
	for (auto s : path)
	  print(s);
	std::cout << std::endl;
      }

      return !paths.empty();
    }

private:
  std::vector<state> nextStates(const std::vector<state> & path) {
    std::vector<state> next;
    state curr = path.back();
    if (curr.first.water() > 0) {
      auto cpy = curr;
      cpy.first.empty();
      if (!exist(path, cpy) && !exist(next, cpy))
	next.push_back(cpy);
    }

    if (curr.first.water() != curr.first.capacity()) {
      auto cpy = curr;
      cpy.first.fill();
      if (!exist(path, cpy) && !exist(next, cpy))
	next.push_back(cpy);
    }

    if (curr.first.water() > 0 && curr.second.water() < curr.second.capacity()) {
      auto cpy = curr;
      cpy.first.pourTo(cpy.second);
      if (!exist(path, cpy) && !exist(next, cpy))
	next.push_back(cpy);
    }

    if (curr.second.water() > 0) {
      auto cpy = curr;
      cpy.second.empty();
      if (!exist(path, cpy) && !exist(next, cpy))
	next.push_back(cpy);
    }

    if (curr.second.water() != curr.second.capacity()) {
      auto cpy = curr;
      cpy.second.fill();
      if (!exist(path, cpy) && !exist(next, cpy))
	next.push_back(cpy);
    }

    if (curr.second.water() > 0 && curr.first.water() < curr.first.capacity()) {
      auto cpy = curr;
      cpy.second.pourTo(cpy.first);
      if (!exist(path, cpy) && !exist(next, cpy))
	next.push_back(cpy);
    }

    return next;
  }

  bool exist(const std::vector<state> & path, const state & s) {
    auto it = std::find(path.begin(), path.end(), s);
    return it != path.end();
  }

  std::vector<std::vector<state>> addOne(const std::vector<state> & path, const state & last) {
    std::vector<std::vector<state>> results;
    if (path.back() == last) {
      results.push_back(path);
    }
    else {
      std::vector<std::vector<state>> cands;
      std::vector<state> next = nextStates(path);
      for (auto nx : next) {
	auto cpy = path;
	cpy.push_back(nx);
	auto temp = addOne(cpy, last);
	for (auto tmp : temp)
	  cands.push_back(tmp);
      }

      // shortest path
      // if (!cands.empty()) {
      // 	size_t idx = 0;
      // 	for (size_t i = idx+1; i < cands.size(); i++)
      // 	  if (cands[i].size() < cands[idx].size())
      // 	    idx = i;

      // 	for (size_t i = 0; i < cands.size(); i++)
      // 	  if (cands[i].size() == cands[idx].size())
      // 	    results.push_back(cands[i]);
      // }

      // all paths
      results = cands;
    }
    return results;
  }
};

int main() {
  int x{3}, y{5}, z{4};
  // int x{2}, y{6}, z{5};

  solution soln;
  bool isMeasureable = soln.canMeasureWater(x,y,z);
  if (isMeasureable)
    std::cout << z << " liters of water can be measured from jugs of " << x << " and " << y << std::endl;
  else
    std::cout << z << " liters of water can NOT be measured from jugs of " << x << " and " << y << std::endl;
}
