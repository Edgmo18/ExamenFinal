#include <iostream> 
#include <cstdint> 
using namespace std; 

int main() { 
   int edad = 18; 
   double promedio = 4.2; 
   uint8_t nivelPWM = 200; 
   uint16_t pulsos = 3500; 
   cout << edad << endl; 
   cout << promedio << endl; 
   cout << static_cast<int>(nivelPWM) << endl; 
   cout << pulsos << endl; 
return 0;
}
