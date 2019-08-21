/*Eric Swanson
First Written: 2/1/17
edited: 2/2/17
This program will calculate the voltage and current across
three resistors in three different circuits (series, parallel,
and combination series and parallel). The printf() function
will prompt users to enter values for R1, R2, R3, and the
voltage source. The scanf() will read user inputs.
These values will be used in calculating voltage and current,
which will be displayed to the user (for each circuit type).*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{	
	//Declaring variables for R1,R2,R3,Vsource
	//and asking for user inputs
	double R1, R2, R3, Vsource;
	printf("Enter voltage source value (V): ");
	scanf("%lf", &Vsource);
	printf("Enter three resistance values (ohms): ");
	scanf("%lf %lf %lf", &R1, &R2, &R3);

	/* This code prints the voltage and current values for the series circuit.
	V1s, V2s, V3s are voltage variables, Is is current. Req is total resistance.*/
	double Req = R1 + R2 + R3;
	double V1s = (R1*Vsource) / Req;
	double V2s = (R2*Vsource) / Req;
	double V3s = (R3*Vsource) / Req;
	double Is = Vsource / Req;
	printf("\nSERIES CIRCUIT\n");
	printf("Current through circuit: %lf A\n", Is);
	printf("Voltage across R1: %lf V\n", V1s);
	printf("Voltage across R2: %lf V\n", V2s);
	printf("VOltage across R3: %lf V\n", V3s);

	/*This code prints the current and voltage values for the parallel circuit.
	I1p, I2p, I3p are current variables,Vp is voltage (which is Vsource).*/
	double I1p = Vsource / R1;
	double I2p = Vsource / R2;
	double I3p = Vsource / R3;
	printf("\nPARALLEL CIRCUIT\n");
	printf("Voltage across each resistor: %lf V\n", Vsource);
	printf("Current through R1: %lf A\n", I1p);
	printf("Current through R2: %lf A\n", I2p);
	printf("Current through R3: %lf A\n", I3p);

	/*This code prints the current and voltage values for the combination series
	and paralle circuit. V1c, V23 are voltage variables. I1c, I2c, I3c are current variables.
	Rp is parallel resistance of R2 and R3*/
	double Rp = ( R2 * R3 ) / ( R2 + R3 ); 
	double V1c = (R1*Vsource) /(R1 + Rp);
	double V23 = Vsource - V1c;
	double I1c = V1c / R1;
	double I2c = V23 / R2;
	double I3c = V23 / R3;
	printf("\nR2 AND R3 IN PARALLEL\n");
	printf("Voltage across R1: %lf V\n", V1c);
	printf("Current through R1: %lf A\n", I1c);
	printf("Voltage across R2: %lf V\n", V23);
	printf("Current through R2: %lf A\n", I2c);
	printf("Voltage across R3: %lf V\n", V23);
	printf("Current through R3: %lf A\n", I3c);
	return 0;
}
