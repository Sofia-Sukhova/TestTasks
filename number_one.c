#include <stdio.h>


//aфункция распечатывает число побитово
void bit_print(unsigned long long N){
	unsigned long long tmp = 0;
	int length = 0;
	
	tmp = N;
	while (tmp > 0){
		length ++;
		tmp = tmp >> 1;
	}
	if (length == 0){
		printf("%lld\n", N);
		return;
	}

	for (int i = length - 1; i >= 0; i--){
		printf("%lld", (N >> i) & 1);
	}
	printf("\n");
}

//функция считывает число, записанное побитово
unsigned long long  bit_scan(){
	unsigned long long read = 0;
	char symbol;
	int digit = 0;

	symbol = getc(stdin);	

	while (symbol != '\n'){
		digit = symbol - '0';			
		read = (read << 1) + digit;
		// printf("%llu\n", read);
		symbol = getc(stdin);
	}	

	return read;
}	

int main(){
	unsigned long long N = 0, M = 0, New = 0, mask = 0;
	int i1 = 0, i2 = 0, length;

	//считывание входных данных задачи
	N = bit_scan();
	M = bit_scan();
	// bit_print(M);
	scanf("%d%d", &i1, &i2);
	length = i2 - i1 + 1;

	//новое число берет старшие разряды из N (c i2 и старше)
	New = N >> (i2 + 1);

	//дописываются соответствующие разряды из M (с i1 до i2)
	if (length == 64){
		New = M;
	} else {
		mask = ((unsigned long long)1 << length) - (unsigned long long)1;
		New = (New << length) + (M & (mask));
	}

	//дописываются оставшиеся младшие разряды из N (c i1 и младше)
	mask = ((unsigned long long)1 << i1) - (unsigned long long)1;
	New = (New << i1) + (N & (mask));

	bit_print(New);
	return 0;
}




















