
class jug {
  private:
    int my_water;
    int my_capacity;

  public:
    jug(int cap) : my_water(0), my_capacity(cap){}

    int water() const {return my_water;}
    int capacity() const {return my_capacity;}

    void fill() { my_water = my_capacity; }
    void empty() { my_water = 0; }
    void pourTo(jug & other) {
        if (my_water >= other.capacity()-other.water()) {
            other.fill();
            my_water -= other.capacity()-other.water();
        }
        else {
            other.my_water += my_water;
            empty();
        }
    }
};

class solution {
  public:
    bool canMeasureWater(int x, int y, int z) {
        
    }
};
