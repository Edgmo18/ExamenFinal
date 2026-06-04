#include <iostream> 
#include <cstdint> 
using namespace std; 

int main() { 
   uint8_t pwm = 255; 
   uint16_t pulsos = 1200; 
   uint32_t tiempo = 50000; 
   cout << static_cast<int>(pwm) << endl; 
   cout << pulsos << endl; 
   cout << tiempo << endl; 
}
