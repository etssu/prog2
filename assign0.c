#include <stdio.h>
#include <stdbool.h>

void primeFactors(int a, int m) {
	int tries = 0;
        for(int i = 2; i*i <= a && tries < m; i++) {
         	bool isPrime = true;
            	for(int j = 2; j*j <= i; j++) {      //checking if i is a prime num
                  if(i%j == 0) {
                  isPrime = false;
                  }
                }
            if(isPrime) {		//if i is prime and i|a
                if(a%i == 0) {
                printf("%d\n", i);
                tries++;
                	while (a % i == 0) {  //repeats this until a remains a multiple of i
                		a /= i; }
                    }
            }
        }
	if(a>1 && tries < m) {
		printf("%d\n", a);
        tries++;
	}
}


int main() {
	int a, m;
	scanf("%d %d", &a, &m);
	if(a <= 1) {
		printf("ERROR\n");
		return 0;
	}

	primeFactors(a, m);
	return 0;
}

