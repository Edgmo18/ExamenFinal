#include <iostream> 
#include <cmath> 
using namespace std; 

int main() { 
    double a = 0.1 + 0.2; 
    double b = 0.3; 
    double tolerancia = 0.000001; 
    if (fabs(a - b) < tolerancia) {
	    cout << "Son aproximadamente iguales" << endl; 
    } 
	else { 
        cout << "Son diferentes" << endl; 
    } 
return 0; 
}
