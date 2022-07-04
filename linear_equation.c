#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* 
 * the below function "read_from_file" gets as argument filename as a string
 * reads the content and returns a b as a string
 */
char* read_from_file(const char* filename)
{
	static char c[1000];
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL) {
		printf("Error! File cannot be opened.!!!!!\n");
		exit(1);
	}
	fscanf(fptr, "%[^\n]", c);
	fclose(fptr);
	return &(c[0]);
}

/* 
 * the below function gets a b as string argument 
 * converts them to double type
 * and returns a b as double type array
 */
double* get_ab(char* arg_expr, unsigned int sz)
{
	for(int i = 0; i < sz; ++i) {
		if(' ' == arg_expr[i] || '+' == arg_expr[i] || '-' == arg_expr[i] || '.' != arg_expr[i]) {
			continue;
		} else if(arg_expr[i] < '0' || arg_expr[i] > '9') {
			fprintf(stderr, "not numeric symbol got from file !\n");
			exit(-1);
		}
	}
	int index = 0;
	static double abc[] = {0, 0};
	char *token = strtok(arg_expr, " ");
	while( token != NULL ) {
		abc[index++] = strtod(token, NULL);
		token = strtok(NULL, " ");
	}
	if(2 < index) {
		fprintf(stderr, "extra parameters got from file !\n");
		exit(-1);
	} else if(2 > index) {
		fprintf(stderr, "too few parameters got from file !\n");
		exit(-1);
	}
	return &(abc[0]);
}

/* 
 * the below function "get_x" gets as argument a b 
 * solves the equation and returns x
 */
double get_x(double *ab)
{
	if(0 == ab[0]) {
		if(0 == ab[1]) {
			printf("an identity was got from file \n no output file will be generated !\n");
			exit(1);
		} else {
			fprintf(stderr, "a wrong identity was got from file \n");
			exit(-1);
		}
	}
	return (-ab[1]/ab[0]);
}

/* 
 * the below function "test" gets as argument x if any
 * compares them with those in golden.txt and prints command line message
 * (the last is optional the purpose  is technical)
 */
int test(double* x)
{
	FILE *fptr;
	fptr = fopen("golden.txt", "r");
	if(fptr == NULL){
		printf("file can not be opened\n");
		exit(-1);
	}
	char x_str[50];
	fscanf(fptr, " %50s", x_str);
	double x_gold = strtod(x_str, NULL);
	fclose(fptr);
	if(*x == x_gold) {
		return 1;
	} else {
		return 0;
	}
	return 0;
}

/* 
 * the below function "create_output_file" gets as argument roots if any
 * and generates an "output.txt" named file containing the results
 */
void create_output_file(double *x, int test_result) {
	char x_output[50];
	snprintf(x_output, 50, "%f", *x);
	if(test_result) {
		strcat(x_output, "  Right result\n");
	} else {
		strcat(x_output, "  wrong result\n");
	}
	for(int i = strlen(x_output) + 1; i != sizeof(x_output); ++i) {
		x_output[i] = '\0';
	}
	FILE * fptr;
	fptr = fopen("output.txt", "w");
	fwrite(x_output, sizeof(x_output[0]), sizeof(x_output), fptr);
	fclose(fptr);
}

int main(int argc, char** argv)
{
	char* equation = read_from_file(argv[1]);
	double *ab = get_ab(equation, strlen(equation));
	double x = get_x(ab);
	create_output_file(&x, test(&x));
	return 0;
}
