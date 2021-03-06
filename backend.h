#ifndef _BACKEND_H_
#define _BACKEND_H_

#include <stddef.h>

#define _TOTAL_OPS_ 21
#define _TOTAL_FUNCTS_ 21
#define _PI_ 3.141592654

/*------------------------------- declarations ----------------------------------*/
/*------------------- usual functions in math declaration -----------------------*/

// factorial function
double factorial (double x);

// log of x to the y function. log base 9 of 81 would be log(81,9)
double logXtoY (double x, double base);

// root function, returns the root order of x. square root of 7 will be root(2,7)
double root (double x, double order);

// polynomial function, coeffs[i] with i integer for x^i
double polynomial (double* coeffs, int order, double x);

/*---------------------------- singe calculation ------------------------------*/

/*
  NOTE for operation literals: (21 in total)
  - double operand operations: add, minus, mult, div, pow, logx, root
  - single operand operations: sin, cos, tan, sinh, cosh, tanh, ln, log, exp, abs, acos, asin, atan, fact
*/
typedef enum operations OP; // list of recognizable operations 
enum operations {add, minus, mult, division, power, logx, rootx, sinus, cosin, tang, sinhp, coshp, tanhp, ln, log_10, expo, absolute, arccos, arcsin, arctan, fact};

// return the literals corresponding to each operation 
char* opLiteral (OP usr_op);

// function to do the calculation with 2 operands (only work on operand A if operation has only one operand), return the result
double applyOp (OP usr_op, double A, double B, int degree);

// single operation input by user, used when user just want to calculate something simple using 1 operation
double singleOp (char* usr_input, double* ANS, int degree);

/*-------------------------------- matrix arithmetics -----------------------------*/

// add 2 matrices, result extracted from 2nd matrix input
void matrixAdd (size_t nrows, size_t ncols, double A[*][*], double B[*][*]);

// multiply 2 matrices, result extracted from 3rd matrix input
void matrixMult (size_t nrowsA, size_t ncolsA, size_t nrowsB, size_t ncolsB, double A[*][*], double B[*][*], double C[*][*]);

// a subfunction needed for det and inv that swap the rows of a square matrix
void swapRows (size_t n, double A[*][*], int rowA, int rowB);

// find the determinant of a matrix 
double matrixDet (size_t n, double A[*][*]);

// find the inverse of a matrix
void matrixInv (size_t n, double A[*][*], double Inv[*][*]);

// print out matrix with ease
void printMat (size_t rows, size_t cols, double mat[*][*]);

/*------------------------------- complex numbers ---------------------------------*/
// declare the container for a complex number
typedef struct complex COMPLEX;
struct complex {
  // primary representation with re and im
  double re;
  double im;
};

// arithmetics with complex numbers
COMPLEX cmplxAdd (COMPLEX a, COMPLEX b);      // add
COMPLEX cmplxSub (COMPLEX a, COMPLEX b);      // subtraction
COMPLEX cmplxMul (COMPLEX a, COMPLEX b);      // multiplication
COMPLEX cmplxDiv (COMPLEX a, COMPLEX b);      // division
double cmplxMag (COMPLEX a);                  // magnitude of complex num
double cmplxArg (COMPLEX a, int degMode);     // argument of complex num

// string manipulations needed for complex operations
COMPLEX cmplxExtract (char* usr_input, COMPLEX* ANS_CMPLX); // extract the operands

// inteprete the operation given by usr and then perform the operation
// return note:
// if the operations are add/sub/multiply/division, a complex number as result would be returned
// if the operations are magnitude or argument, the returned complex result has im = 0 and re = result of the operation
COMPLEX applyOpCmplx (char* usr_input, COMPLEX* ANS_CMPLX, int degMode);

void printCmplx (COMPLEX input); // print out complex number

/*-------------------------------- function generator ------------------------------------*/

typedef enum functions FT; // list of recognizable operations 
enum functions {addf, minusf, multf, divisionf, powerf, logxf, rootxf, sinusf, cosinf, tangf, sinhpf, coshpf, tanhpf, lnf, log_10f, expof, absolutef, arccosf, arcsinf, arctanf, polf};

typedef struct function_block FUNCT_BLOCK;
struct function_block {
  // the function in this block
  FT function_op;

  // if this block is specifically a polynomial
  double* coefficients;
  int order;

  // declare the operands, which are other functions
  FUNCT_BLOCK* operandA;
  FUNCT_BLOCK* operandB; 
};

// return some recognisable literals for functions
char* functionLiteral (FT function);

// apply operation corresponding to the tag (derived from applyOp)
double applyFunction (FT function, double A, double B, int degree);

// create the function from the user input string
FUNCT_BLOCK* createFunction (char* usr_input);

// calculate the value of the function for given x
double calcFunction (FUNCT_BLOCK* function, double X, int degree);

// clear function map
void clearFunction (FUNCT_BLOCK* function);

// find integral of function, find integral of a function
double integral (FUNCT_BLOCK* function, double A, double B, int degree);

// find the derivative of a function at a point
double derive (FUNCT_BLOCK* function, double X, int degree);

#endif
