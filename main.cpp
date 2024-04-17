#include<iostream>
#include<bitset>
#include<iomanip>
#include<math.h>

void inputNumbers(float &x, float &y);
int verifyNumbers(float x, float y, float &z);
int separateIntegerFractionalPart(float &num);
int extractSign(float &num);
void getSignificand(float num, std::bitset<23> &bin);
void normalize(std::bitset<8> intPart, std::bitset<23> &significand,
	int &shiftCounter);
void significandMultiply(std::bitset<23> Q, std::bitset<23> M,
	std::bitset<23> &resultSignificand, std::bitset<8> &biasedExpResult);
void printCicle(std::bitset<23> Q, std::bitset<23> A, bool C);
bool bitAddition(bool bit1, bool bit2, bool &carry);
void getBiasedExpResult(std::bitset<8> biasedExponentx,
	std::bitset<8> biasedExponenty, std::bitset<8> &biasedExpResult);
void printInBinary(std::bitset<1> sign, std::bitset<8> biasedExponent,
	std::bitset<23> significand);
bool getResultSign(std::bitset<1> signx, std::bitset<1> signy);

void inputNumbers(float &x, float &y){
	std::cout<<"Ingresar valores\nNumero 1: ";
	std::cin>>x;
	std::cout<<"Numero 2: ";
	std::cin>>y;
	return;
}

int verifyNumbers(float x, float y, float &z){
	if( x==0 || y==0 ){
		z = 0;
		return 1;
	}
	return 0;
}

int separateIntegerFractionalPart(float &num){
	int intPart;
	intPart = num;
	num = num - intPart;
	return intPart;
}

int extractSign(float &num){
	if( num < 0 ){
		num *= -1;
		return 1;
	}else
		return 0;
}

void getSignificand(float num, std::bitset<23> &bin){
	//printf("\n%f\n", num);
	float tmp;
	int intTmp;
	tmp = num;
	for(int i=22; 0<=i; i--){
		tmp *= 2;
		intTmp = tmp;
		if( tmp < 1 )
			bin[i] = 0;
		else
			bin[i] = 1;
		if( intTmp )
			tmp -= intTmp;
		//std::cout<<tmp<<"\n";
		//std::cout<<bin[i];
	}
	//printf("\n");
	return;
}

void normalize(std::bitset<8> intPart, std::bitset<23> &significand,
	int &shiftCounter){

	int counter1, counter2;
	counter1 = 0;
	//printf("\nnormalize\n");
	for(int i=7; intPart[i]!=1; i--){
		counter1++;
	}
	counter2 = 8-counter1-1;
	shiftCounter = counter2;
	//printf("counter 1:%d\ncounter 2: %d\n", counter1, counter2);
	for(int i=0; i<counter2; i++){
		significand >>= 1;
		significand[22] = intPart[i];
	}
	//printf("\n");
	//for(int i=22; 0<=i; i--){
	//	std::cout<<significand[i];
	//}
	//printf("\n");
	return;
}

void significandMultiply(std::bitset<23> Q, std::bitset<23> M,
	std::bitset<23> &resultSignificand, std::bitset<8> &biasedExpResult){

	int counter1, counter2;
	std::bitset<23> A{0};
	std::bitset<24> SUM;
	std::bitset<8> one{1};
	bool C, tmp;
	C = 0;
	counter1 = 0;

	Q >>= 1;
	M >>= 1;
	Q[22] = 1;
	M[22] = 1;

	std::cout<<"\nMultiplicacion de significantes\nM: "<<M<<"\n";

	std::cout<<std::setw(3)<<"C"<<std::setw(25)<<"A"<<std::setw(25)<<"Q\n";
	printCicle(Q, A, C);
	std::cout<<"   Valores iniciales\n";

	for(int i=23; 0<i; i--){
		if(Q[0]){
			for(int j=0; j<24; j++){
				SUM[j] = bitAddition(A[j], M[j], C);
				if(j<23)
					A[j] = SUM[j];
			}
			C = SUM[23];
			printCicle(Q, A, C);
			std::cout<<"   Suma\n";
		}
		tmp = A[0];
		A >>= 1;
		A[22] = C;
		C = 0;
		Q >>= 1;
		Q[22] = tmp;
		printCicle(Q, A, C);
		std::cout<<"   Shift\n";
	}

	C = 0;

	if(A[22]){
		for(int i=0; i<8; i++){
			biasedExpResult[i] =
				bitAddition(biasedExpResult[i], one[i], C);
		}
	}

	for(int i=22; A[i]!=1; i--){
		counter1++;
	}
	for(int i=0; i<=counter1; i++){
		A <<= 1;
		A[0] = Q[22];
		Q <<= 1;
	}
	for(int i=0; i<23; i++){
		resultSignificand[i] = A[i];
	}

	return;
}

void printCicle(std::bitset<23> Q, std::bitset<23> A, bool C){
	std::cout<<std::setw(3)<<C<<std::setw(25)<<A<<
		std::setw(25)<<Q;
	return;
}

bool bitAddition(bool bit1, bool bit2, bool &carry){
	bool result = (bit1 ^ bit2) ^ carry;
	carry = (bit1 && bit2) || (bit1 && carry) || (bit2 && carry);
	return result;
}

int getBiasedExponent(int shiftCounter){
	int exponent;
	exponent = shiftCounter + (pow(2, 7) - 1);
	//printf("\n%d\n", exponent);
	return exponent;
}

void printInBinary(std::bitset<1> sign, std::bitset<8> biasedExponent,
	std::bitset<23> significand){

	std::cout<<sign<<" "<<biasedExponent<<" "<<significand<<"\n";
	return;
}

void getBiasedExpResult(std::bitset<8> biasedExponentx,
	std::bitset<8> biasedExponenty, std::bitset<8> &biasedExpResult){

	int result = (biasedExponentx.to_ulong() - 127) +
		biasedExponenty.to_ulong();
	std::bitset<8> binRes(result);
	for(int i=0; i<8; i++){
		biasedExpResult[i] = binRes[i];
	}
	return;
}

bool getResultSign(std::bitset<1> signx, std::bitset<1> signy){
	bool result;
	result = signx[0] xor signy[0];
	return result;
}

int
main()
{
	float x, y, z, result;
	int shiftCounterx, shiftCountery;
	std::bitset<23> resultSignificand;
	std::bitset<8> biasedExpResult;

	inputNumbers(x, y);

	if( verifyNumbers(x, y, z) ){
		std::cout<<"Resultado: "<<z<<"\n";
		return 0;
	}

	result = x*y;


	std::bitset<1> signx(extractSign(x));
	std::bitset<8> intPartx(separateIntegerFractionalPart(x));
	std::bitset<23> significandx(0);
	getSignificand(x, significandx);
	normalize(intPartx, significandx, shiftCounterx);
	std::bitset<8> biasedExponentx(getBiasedExponent(shiftCounterx));

	std::bitset<1> signy(extractSign(y));
	std::bitset<8> intParty(separateIntegerFractionalPart(y));
	std::bitset<23> significandy(0);
	getSignificand(y, significandy);
	normalize(intParty, significandy, shiftCountery);
	std::bitset<8> biasedExponenty(getBiasedExponent(shiftCountery));

	getBiasedExpResult(biasedExponentx, biasedExponenty, biasedExpResult);

	significandMultiply(significandx, significandy, resultSignificand,
		biasedExpResult);

	std::bitset<1> signResult(getResultSign(signx, signy));


	std::cout<<"\nNumero 1:\n";
	printInBinary(signx, biasedExponentx, significandx);
	std::cout<<"Numero 2:\n";
	printInBinary(signy, biasedExponenty, significandy);

	printf("\nResultado C++: %f\n", result);

	std::cout<<"Resultado implementado obtenido:\n";
	printInBinary(signResult, biasedExpResult, resultSignificand);


	printf("\n");

	return 0;
}
