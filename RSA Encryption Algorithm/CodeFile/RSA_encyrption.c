#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
//Import necessary header files
#include "functions.h"



int* extendedEuclidean(int m, int e) {
    int s = 0;
    int old_s = 1;
    int t = 1;
    int old_t = 0;
    int r = e;
    int old_r = m;
    int temp;
    int quotient;
    int ret[2];

    while (r != 0) {
				// Quotient of old_r divided by r
        quotient = old_r / r;
        temp = r;
				// Remainder of old_r divided by r
        r = old_r - r * quotient;
        old_r = temp;
        temp = s;
				// Substract quotient times s from old_s
        s = old_s - s * quotient;
        old_s = temp;
        temp = t;
				// Substract quotient times t from old_t
        t = old_t - t * quotient;
        old_t = temp;
    }
		
    ret[0] = old_t; //d
    ret[1] = old_r; //GCD
    return ret;  // pointer of retrun d and gcd

}

int f_encrypt(int e, int n, int nipple) { // fraction = nipple ^ e
    int fraction = 1;
    nipple = nipple % n;
    while (e != 0) {
        if (e % 2 == 0) { //if e is tek
            nipple = (nipple * nipple) % n;
        }
        else {
					fraction = (fraction * nipple) % n;
					nipple = (nipple * nipple) % n;
				}
				e /= 2;
			
    }
    return fraction;
}

int f_decrypt(int d, int n, int nipple) { // fraction = nipple ^ e
    int fraction = 1;
    nipple = nipple % n;
    while (d != 0) {
        if (d % 2 == 0) { //if e is tek
            nipple = (nipple * nipple) % n;
        }
        else {
					fraction = (fraction * nipple) % n;
					nipple = (nipple * nipple) % n;
				}
				d /= 2;
			
    }
    return fraction;
}

//This is where the code runs. Programs start from here
int main(void){
	  int p = 59;
    int q = 61;
    int m = (p - 1) * (q - 1);
    int n = p * q;
    int e = 2;
    int i;
		char LUT[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
		
     for (i = 2; i <= m; i++) {
        if (extendedEuclidean(m, i)[1] == 1) { // GCD is 1 m and i coprime
            e = i; // update e to make it coprime with m
           
            break;

        }
    }

    int d = *extendedEuclidean(m, e);
    while (d < 0) {
        d += m;
    }  // we got positive d

	//Initialize LCD and keypad
	LCD_init();
	keypad_init();
	print_fnc("Press A or D:                           ");
	int input = get_number_from_keypad();
	clear_lcd();
		
	if (input == 17){ // encryption is selected
		print_fnc("Enter a Number:                         ");
		int number = get_number_from_keypad();
		int HSB = number/ 10000;
		int LSB = number %10000;
		clear_lcd();
		int output = f_encrypt(e, n, HSB)* 10000 + f_encrypt(e, n, LSB);
		print_fnc("Encrypted Number:                       ");
		int i;
		int divider = 10000000;

		for (i = 0; i<8; i++){
			int index = output / divider;
			output = output - index*divider;
			divider /= 10;
			LCD_data(*(LUT + index));
			
	}	
}		
	 else if (input == 20){ 
		print_fnc("Enter a Number:                         ");
		int number = get_number_from_keypad();
		 
		int HSB = number / 10000;
		int LSB = number % 10000;
		clear_lcd();
		int output = f_decrypt(d, n, HSB)* 10000 + f_decrypt(d, n, LSB);
		print_fnc("Decrypted Number:                       ");
		int i;
		int divider = 10000000;

		for (i = 0; i<8; i++){
			int index = output / divider;
			output = output - index*divider;
			divider /= 10;
			LCD_data(*(LUT + index));
			
	}	
		
} 
	else {
			print_fnc("Invalid Input!");
	}

}