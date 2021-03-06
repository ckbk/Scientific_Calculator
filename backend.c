#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "backend.h"

/*----------------------- definitions --------------------------*/
/*------------- usual functions in math definition -------------*/

// factorial function
double factorial (double x) {
  if (x == 0) return 1;
  else return x*factorial(x-1);
}

// log of x to the y function. log base 9 of 81 would be log(81,9)
double logXtoY (double x, double base) {
  return log(x)/log(base);
}

// root function, returns the root order of x. square root of 7 will be root(2,7)
double root (double x, double order) {
  return pow(x,1/order);
}

// polynomial function, coeffs[i] with i integer for x^i
double polynomial (double* coeffs, int order, double x) {
  double result = 0;
  for (int i = 0; i < order+1; i++) {
    result += coeffs[i]*pow(x,i); // ai * x^i, i integer
  }
  return result;
}

/*------------------- single calculation ----------------------*/

/*
  NOTE for operations: (20 in total)
  - single operand operations: sin, cos, tan, sinh, cosh, tanh, ln, log, exp, abs, acos, asin, atan
  - double operand operations: add, minus, mult, div, pow, logx, root
*/

// return the recognizable string corresponding to each operation
char* opLiteral (OP usr_op) {
  char* result = NULL;
  switch (usr_op) {
    // double operand operations (0-6)
  case add: result = "+"; break;
  case minus: result = "m"; break;
  case mult: result = "*"; break;
  case division: result = "/"; break;
  case power: result = "pow"; break;
  case logx: result = "logx"; break;
  case rootx: result = "root"; break;

    // single operands operations (7-19)
  case sinus: result = "sin"; break;
  case cosin: result = "cos"; break;
  case tang: result = "tan"; break;
  case sinhp: result = "sinh"; break;
  case coshp: result = "cosh"; break;
  case tanhp: result = "tanh"; break;
  case ln: result = "ln"; break;
  case log_10: result = "log"; break;
  case expo: result = "exp"; break;
  case absolute: result = "abs"; break;
  case arccos: result = "ac"; break;
  case arcsin: result = "as"; break;
  case arctan: result = "at"; break;
  case fact: result = "fact"; break;
  default: printf("Operation not found.\n");
  }
  return result;
}

// function to do the calculation with 2 operands (only work on operand A if operation has only one operand), return the result
double applyOp (OP usr_op, double A, double B, int degree) {
  double result = 0;
  int error = 0;
  switch (usr_op) {
  // double operand operations (0-6)
  case add:
    result = A+B;
    break;
  case minus:
    result = A-B;
    break;
  case mult:
    result = A*B;
    break;
  case division:
    if (B == 0)  error = 1; // divide by 0 error
    else result = A/B;
    break;
  case power:
    if (A < 0)  error = 1; // power of negative number error
    else result = pow(A,B);
    break;
  case logx:
    if (B == 1 || B < 0 || A < 0) error = 1; // base = 1, negative base, log of negative error
    else result = logXtoY(A,B);
    break;
  case rootx:
    if (B < 0 || A < 0) error = 1; // root of a negative number or root order of negative number error
    else result = root(A,B);
    break;

    // single operands operations (7-20)
  case sinus:
    result = (degree) ? sin((A/180)*_PI_) : sin(A); // degree or radian
    break;
  case cosin:
    result = (degree) ? cos((A/180)*_PI_) : cos(A); // degree or radian
    break;
  case tang:
    if (degree) { // degree or radian
      if (fabs(cos((A/180.0)*_PI_)) < 0.000001) error = 1; // cos = 0 error
      else result = sin((A/180.0)*_PI_) / cos((A/180.0)*_PI_);
    }else {
      if (fabs(cos(A)) < 0.000001) error = 1;  // cos = 0 error
      else result = sin(A) / cos(A);
    }
    break;
  case sinhp:
    result = sinh(A);
    break;
  case coshp:
    result = cosh(A);
    break;
  case tanhp:
    result = tanh(A);
    break;
  case ln:
    if (A < 0) error = 1; // log of negative number error
    else result = log(A);
    break;
  case log_10:
    if (A < 0) error = 1; // log of negative number error
    else result = log10(A);
    break;
  case expo:
    result = exp(A);
    break;
  case absolute:
    result = fabs(A);
    break;
  case arccos:
    if (fabs(A) > 1)  error = 1; // arccos of a number not between (-1,1)
    else result = (degree) ? (acos(A)/_PI_)*180.0 : acos(A); // degree or radian
    break;
  case arcsin:
    if (fabs(A) > 1)  error = 1; // arcsin of a number not between (-1,1)
    else result = (degree) ? (asin(A)/_PI_)*180.0 : asin(A); // degree or radian
    break;
  case arctan:
    result = (degree) ? (atan(A)/_PI_)*180.0 : atan(A); // degree or radian
    break;
  case fact:
    if (A < 0)  error = 1;  // factorial of negative number.
    else result = factorial(floor(A)); // floor to ensure if input is not integer, truncate it
    break;
  default:
    printf("Operation not found.\n");
    break;
  }
  // print error message if error occured
  if (error) {
    printf("----------Math error----------\n");
    result = 0;
  }
  return result;
}

// single operation input by user
double singleOp (char* usr_input, double* ANS, int degree) {
  double result = 0;
  OP usr_op = -1;
  for (int i = 0; i < _TOTAL_OPS_; i++) {
    if (strstr(usr_input, opLiteral(i)) != NULL) {
      usr_op = i;
      if (usr_op == logx) break; // if logx is found, stop because log can be found again
    }
  }
  if (usr_op != -1) {
    if (usr_op >= 7) { // case of single operand operations
      // extract the operand
      char temp[50];
      char* operand;
      strcpy(temp, usr_input);
      operand = strtok(temp,"(");
      operand = strtok(NULL,")");
      // convert operand into double
      double A = (strstr(operand,"ANS") != NULL) ? *ANS : atof(operand);
      // then just apply op
      result = applyOp(usr_op,A,0,degree);
    }else if (usr_op > 3 && usr_op < 7) { // case of double operand operations
      // extract the operands
      char temp[50];
      char* operandA;
      char* operandB;
      strcpy(temp, usr_input);
      operandA = strtok(temp,"(");
      operandA = strtok(NULL,",");
      operandB = strtok(NULL,")");
      // convert operands into doubles
      double A = (strstr(operandA,"ANS") != NULL) ? *ANS : atof(operandA);
      double B = (strstr(operandB,"ANS") != NULL) ? *ANS : atof(operandB);
      // then just apply operation on operands
      result = applyOp(usr_op,A,B,degree);
    }else if (usr_op <= 3 && usr_op >= 0) { // case of +, m, *, /
      // extract the operands
      char temp[50];
      char* operandA;
      char* operandB;
      strcpy(temp, usr_input);
      switch(usr_op) {
      case add:
	operandA = strtok(temp,"+");
	break;
      case minus:
	operandA = strtok(temp,"m");
	break;
      case mult:
	operandA = strtok(temp,"*");
	break;
      case division:
	operandA = strtok(temp,"/");
	break;
      }
      operandB = strtok(NULL,"\0");
      // then convert to doubles
      double A = (strstr(operandA,"ANS") != NULL) ? *ANS : atof(operandA);
      double B = (strstr(operandB,"ANS") != NULL) ? *ANS : atof(operandB);
      // then apply operation
      result = applyOp(usr_op,A,B,degree);
    }
  }else {
    printf("Invalid operation. Please refer to \"manual\" for calculator description.\n");
  }
  *ANS = result; // set the last answer to result
  return result;
}

/*----------------------- matrix arithmetics ----------------------*/

// add 2 matrices, extract result in matrixA
void matrixAdd (size_t nrows, size_t ncols, double A[nrows][ncols], double B[nrows][ncols]) {
  // result stored directly in B
  for (int i = 0; i < nrows; i++) {
    for (int j = 0; j < ncols; j++) {
      A[i][j] += B[i][j]; 
    }
  }
}

// multiply 2 matrices, extract result in matrixC
void matrixMult (size_t nrowsA, size_t ncolsA, size_t nrowsB, size_t ncolsB, double A[nrowsA][ncolsA], double B[nrowsB][ncolsB], double C[nrowsA][ncolsB]) {
  if (ncolsA != nrowsB) { // check dimensions of matrices
    printf("Dimension error. \n");
  }else {
    // multiply the matrices, result get stored into C
    size_t nrowsC = nrowsA;
    size_t ncolsC = ncolsB;
    for (int i = 0; i < nrowsC; i++) {
      for (int j = 0; j < ncolsC; j++) {
	for (int a = 0; a < ncolsA; a++) { // ncolsA = nrowsB
	  C[i][j] += (A[i][a]*B[a][j]);
	}
      }
    }
  }
}

// a subfunction needed for det and inv
void swapRows (size_t n, double A[n][n], int rowA, int rowB) {
  for (int i = 0; i < n; i++) {
    double temp = A[rowA][i];
    A[rowA][i] = A[rowB][i];
    A[rowB][i] = temp;
  }
}

// find the determinant of a matrix
double matrixDet (size_t n, double A[n][n]) {
  double A_copy[n][n]; // copy the matrix so that this operation doesn't affect the matrix
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      A_copy[i][j] = A[i][j];
    }
  }
  int total_swaps = 0; // detect row swaps
  for (int i = 0; i < n-1; i++) {
    // handle pivot element = 0
    if (A_copy[i][i] == 0) {
      int swap = 0;
      // continue down the rows to find a row element that != 0
      for (int i_sec = i; i_sec < n; i_sec++) {
	if (A_copy[i_sec][i] != 0) {
	  swap = 1; total_swaps++;
	  swapRows(n, A_copy, i_sec, i);
	  break; // break off loop so don't swap again
	}
      }
      if (swap == 0) { // ran thru entier rows without finding a pivot
	if (i == 0) break; // break off the loop and det = 0
	else continue; // for other cases just continue  
      }
    }
    // use the new found pivot and continue to calculate
    // advance down the rows to check if all cases under pivot are null
    for (int j = i+1; j < n; j++) {
      if (A_copy[j][i] == 0);
      else {
	double coeff = -A_copy[j][i] / A_copy[i][i];
	// Rj = Rj + coeff*Ri
	for (int x = 0; x < n; x++) {
	  A_copy[j][x] += coeff*A_copy[i][x];
	}
      }
    }
  }
  // calculate the determinant
  double det = 1;
  for (int i = 0; i < n; i++) {
    det *= A_copy[i][i]; 
  }
  det = (total_swaps % 2) ? -det : det; // if swap a odd number of times, det = -det
  return det;
}

// find the inverse of a matrix, extract result in Inv
void matrixInv (size_t n, double A[n][n], double Inv[n][n]) {
  if (matrixDet(n,A) != 0) {
    printf("Matrix determinant = %lf, so matrix invertible\n",matrixDet(n,A));
    // assign the inv to the identity matrix
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
	Inv[i][j] = (i == j) ? 1 : 0;
      }
    }
    for (int i = 0; i < n; i++) { // this point A[i][i] is the pivot
      if (A[i][i] == 0) { // handle pivot = 0
	for (int i_sec = i; i_sec < n; i_sec++) {
	  if (A[i_sec][i] != 0) {
	    swapRows(n, A, i_sec, i);
	    swapRows(n, Inv, i_sec, i);
	    break;
	  }
	}
      }
      // so pivot is ok to divide
      // divide pivot to make it 1, the same for its row
      double pivot = A[i][i];
      for (int j = 0; j < n; j++) {
	A[i][j] /= pivot;
	Inv[i][j] /= pivot;
      }
      // clear that column except the pivot
      for (int a = 0; a < n; a++) {
	if (A[a][i] != 0 && a != i) {
	  double coeff = -A[a][i] / A[i][i];
	  for (int x = 0; x < n; x++) {
	    A[a][x] += coeff*A[i][x];
	    Inv[a][x] += coeff*Inv[i][x];
	  }
	}
      }
    }
  }else {
    printf("Matrix determinant null. Matrix not invertible.\n");
  }
}

// print out matrix with ease
void printMat (size_t rows, size_t cols, double mat[rows][cols]) {
  for (size_t i = 0; i < rows; i++) {
    printf("\t[");
    for (size_t j = 0; j < cols; j++) {
      printf("%.3lf\t",mat[i][j]);
    }
    printf("]\n");
  }
}

/*----------------------- complex numbers -------------------------*/
// arithmetics with complex numbers
COMPLEX cmplxAdd (COMPLEX a, COMPLEX b) { // add
  COMPLEX result = { .re = a.re + b.re, .im = a.im + b.im};
  return result;
}
COMPLEX cmplxSub (COMPLEX a, COMPLEX b) { // subtraction
  COMPLEX result = { .re = a.re - b.re, .im = a.im - b.im};
  return result;
}
COMPLEX cmplxMul (COMPLEX a, COMPLEX b) { // multiplication
  // x+iy * z+it = (xz-yt) + (yz+xt)i
  COMPLEX result = { .re = a.re*b.re - a.im*b.im, .im = a.re*b.im + b.re*a.im };
  return result;
}
COMPLEX cmplxDiv (COMPLEX a, COMPLEX b) { // division, a = numerator and b = denominator
  COMPLEX result = { .re = 0, .im = 0};
  if (b.re != 0 || b.im != 0) {
    // multiply a and b by the complement of b. So it reduces to (a*complement_of_b)/(b*complement_of_b) 
    COMPLEX complement = { .re = b.re, .im = -b.im };  // find the complement of b
    double complemented = b.re*b.re + b.im*b.im;       // b*complement_of_b which is a real number
    result = cmplxMul(a, complement);                 
    result.re /= complemented;     
    result.im /= complemented;
  }else {
    printf("Math error. Cannot divide by 0. \n"); // raise error about division for 0
  }
  return result;
}
double cmplxMag (COMPLEX a) { // magnitude of complex num
  return sqrt(a.re*a.re + a.im*a.im);
}
double cmplxArg (COMPLEX a, int degMode) { // argument of complex num
  double result = 0;
  if (a.re != 0 || a.im != 0) {
    double magnitude = cmplxMag(a);
    result = acos(a.re / magnitude);
    result = (degMode) ? (result/_PI_)*180 : result; // convert to degree if necessary
  }else {
    printf("Math error. Argument cannot be found for 0+0i\n"); // raise error for argument of 0
  }
  return result;
}

// turn the complex number string into a complex number type
COMPLEX cmplxExtract (char* usr_input, COMPLEX* ANS_CMPLX) {
  /*
    this function assumes that the overall string extraction function
    has well extracted the number part. it should look like x...x+y...yi or x...x or y...yi or x...x-y...yi
   */
  COMPLEX result = { .re = 0, .im = 0};
  if (strstr(usr_input,"ANS") == NULL) {
    if (strstr(usr_input,"i") == NULL) { // real number
      result.re = atof(usr_input);
    }else {
      if (!strcmp(usr_input,"i")) { // if the input is just i
	result.im = 1;
      }else if (!strcmp(usr_input,"-i")) { // or if the input is just -i
	result.im = -1;
      }else {
	// handle a+bi and bi
	/*-------------------- extract imaginary part --------------------*/
	int end_offset = strlen(usr_input)-2; // this assumes that i is the last character
	int offset = end_offset;
	if (usr_input[end_offset] == '-') {// this handle complex numbers like 4-i
	  result.im = -1;
	}else if (usr_input[end_offset] == '+') { // this handle complex numbers like 3+i
	  result.im = 1;
	}else { 
	  offset -= 1;
	  while (offset > 0 && (isdigit(usr_input[offset]) || usr_input[offset] == '.' )) {
	    offset--;
	  }
	  if (offset == -1) offset = 0;
	  char im_extract[end_offset-offset+2];
	  strncpy(im_extract, usr_input+offset, end_offset-offset+1);
	  im_extract[end_offset-offset+1] = '\0'; // lock string
	  result.im = atof(im_extract);
	}
	/*---------------------- extract real part -----------------------*/
	if (offset != 0) {
	  char re_extract[offset+1];
	  strncpy(re_extract, usr_input, offset);
	  re_extract[offset] = '\0'; // lock extracted string
	  result.re = atof(re_extract);
	}	
      }
    }
  }else { // retrieve the answer from last op
    result.re = ANS_CMPLX->re; 
    result.im = ANS_CMPLX->im; 
  }
  return result;
}

// inteprete the operation given by usr and then perform the operation 
COMPLEX applyOpCmplx (char* usr_input, COMPLEX* ANS_CMPLX, int degMode) { 
  char arg_or_mag[4];
  strncpy(arg_or_mag, usr_input, 3); // extract the first 3 characters that might correspond to an op
  arg_or_mag[3] = '\0';
  COMPLEX result = {.re = 0, .im = 0};
  if (!strcmp(arg_or_mag,"arg") || !strcmp(arg_or_mag,"mag")) { // handle arg() and mag() operation
    /* operand extraction */
    char temp[strlen(usr_input)+1];
    strcpy(temp, usr_input);
    char* cmplx_extract = strtok(temp,"(");
    cmplx_extract = strtok(NULL,")");
    COMPLEX operand = cmplxExtract(cmplx_extract,ANS_CMPLX);
    /* assign flag and apply operation */
    if (!strcmp(arg_or_mag,"arg")) {
      result.re = cmplxArg(operand, degMode);
    }else if (!strcmp(arg_or_mag,"mag")) {
      result.re = cmplxMag(operand);
    }
  }else { // handle basic operation on complex numbers + - * / 
    char temp[strlen(usr_input)+1];
    strcpy(temp, usr_input);
    char* operandA = strtok(temp," ");
    char* op = strtok(NULL," ");
    char* operandB = strtok(NULL,"\0");
    operandB[strlen(operandB)-1] = '\0'; // the userinput string has by default a '\n' so make it '\0'
    COMPLEX A = cmplxExtract(operandA,ANS_CMPLX);
    COMPLEX B = cmplxExtract(operandB,ANS_CMPLX);
    if (!strcmp(op,"+")) {
      result = cmplxAdd(A, B);
    }else if (!strcmp(op,"-")) {
      result = cmplxSub(A, B);
    }else if (!strcmp(op,"*")) {
      result = cmplxMul(A, B);
    }else if (!strcmp(op,"/")) {
      result = cmplxDiv(A, B);
    }
  }
  // store the returned result to answer
  ANS_CMPLX->re = result.re; ANS_CMPLX->im = result.im;
  return result;
}

// print out complex number
void printCmplx (COMPLEX input) {
  if (input.re != 0 && input.im > 0) printf("%.2lf + %.2lfi\n", input.re, input.im); // complex number with im part > 0
  else if (input.re != 0 && input.im < 0) printf("%.2lf - %.2lfi\n", input.re, -input.im); // complex number with im part < 0
  else if (input.re == 0 && input.im != 0) printf("%.2lfi\n",input.im); // pure imaginary number
  else printf("%.2lf\n",input.re); // real number
}

/*----------------------- function generator ----------------------*/

// return some recognisable literals for functions
char* functionLiteral (FT function) {
  char* result;
  switch (function) {
  case addf: result = "add"; break;
  case minusf: result = "min"; break;
  case multf: result = "mul"; break;
  case divisionf: result = "div"; break;
  case power: result = "pow"; break;
  case logxf: result = "logx"; break;
  case rootxf: result = "root"; break;
  case sinusf: result = "sin"; break;
  case cosinf: result = "cos"; break;
  case tangf: result = "tan"; break;
  case sinhpf: result = "sinh"; break;
  case coshpf: result = "cosh"; break;
  case tanhpf: result = "tanh"; break;
  case lnf: result = "ln"; break;
  case log_10f: result = "log"; break;
  case expof: result = "exp"; break;
  case absolutef: result = "abs"; break;
  case arccosf: result = "ac"; break;
  case arcsinf: result = "as"; break;
  case arctanf: result = "at"; break;
  case polf: result = "pol"; break;
  }
  return result;
}

// apply operation corresponding to the tag (derived from applyOp)
double applyFunction (FT function, double A, double B, int degree) {
  double result = 0;
  int error = 0;
  switch (function) {
  // double operand operations (0-6)
  case addf:
    result = A+B;
    break;
  case minusf:
    result = A-B;
    break;
  case multf:
    result = A*B;
    break;
  case divisionf:
    if (B == 0) error = 1; // divide by 0 error
    else result = A/B;
    break;
  case powerf:
    if (A < 0) error = 1; // power of a negative number
    else result = pow(A,B);
    break;
  case logxf:
    if (B == 1 || B < 0 || A < 0) error = 1; // log base negative, base = 1 or log of negative number
    else result = logXtoY(A,B);
    break;
  case rootxf:
    if (B < 0 || A < 0) error = 1; // root of negative number or root power negative number
    else result = root(A,B);
    break;

    // single operands operations (7-20)
  case sinusf:
    result = (degree) ? sin((A/180.0)*_PI_) : sin(A); // degree or radian
    break;
  case cosinf:
    result = (degree) ? cos((A/180.0)*_PI_) : cos(A); // degree or radian
    break;
  case tangf:
    if (degree) { // degree or radian
      if (fabs(cos((A/180.0)*_PI_)) < 0.000001)  error = 1;  // cos = 0 error
      else result = sin((A/180.0)*_PI_) / cos((A/180.0)*_PI_);
    }else {
      if (fabs(cos(A)) < 0.000001)  error = 1; // cos = 0 error
      else result = sin(A) / cos(A);
    }
    break;
  case sinhpf:
    result = sinh(A);
    break;
  case coshpf:
    result = cosh(A);
    break;
  case tanhpf:
    result = tanh(A);
    break;
  case lnf:
    if (A < 0)  error = 1; // ln of negative 
    else result = log(A);
    break;
  case log_10f:
    if (A < 0)  error = 1; // log of negative
    else result = log10(A);
    break;
  case expof:
    result = exp(A);
    break;
  case absolutef:
    result = fabs(A);
    break;
  case arccosf:
    if (fabs(A) > 1) error = 1; // arccos of input outside (-1,1)
    else result = (degree) ? (acos(A)/_PI_)*180.0 : acos(A); // degree or radian
    break;
  case arcsinf:
    if (fabs(A) > 1) error = 1; // arcsin of input outside (-1,1)
    else result = (degree) ? (asin(A)/_PI_)*180.0 : asin(A); // degree or radian
    break;
  case arctanf:
    result = (degree) ? (atan(A)/_PI_)*180.0 : atan(A); // degree or radian
    break;
  case polf: // polynomial would be handled independantly by calcFunction() later
    result = 0;
    break;
  }
  // print error message if error occured
  if (error) {
    printf("Math error. \n");
    result = 0;
  }
  return result;
}

// create the function from the user input string
FUNCT_BLOCK* createFunction (char* usr_input) {
  // extract the operation from this input string
  char temp[100];
  char* operation;
  strcpy(temp, usr_input);
  operation = strtok(temp,"(");
  // find the operation for this input
  FT funct_op;
  for (int i = 0; i < _TOTAL_FUNCTS_; i++) {
    if (strstr(operation,functionLiteral(i)) != NULL) {
      funct_op = i;
      break;
    }
  }
  // start to handle the creation steps
  FUNCT_BLOCK* new_block = (FUNCT_BLOCK*) malloc (sizeof(FUNCT_BLOCK));
  /*-------------------------- polynomial handle ---------------------------*/
  if (funct_op == 20) { // if operation is polynomial
    /* coeff extraction */
    char* coeffs = strtok(NULL,")");
    int arg_count = 1;
    for (int i = 0; i < strlen(coeffs); i++) {
      if (coeffs[i] == ',') arg_count++;
    }
    double* funct_coefs = (double*) malloc (arg_count*sizeof(double));
    for (int i = 0; i < arg_count; i++) {
      char* individual_coef;
      if (i == 0) individual_coef = strtok(coeffs, ",");
      else individual_coef = strtok(NULL, ",");
      funct_coefs[i] = atof(individual_coef);
    }
    /* function block definition */
    new_block->function_op = polf;
    new_block->coefficients = funct_coefs;
    new_block->order = arg_count-1;
    new_block->operandA = NULL;
    new_block->operandB = NULL;
  }
  /*----------------------- single operand handle ------------------------*/
  else if (funct_op < 20 && funct_op > 6) {
    /* operand extraction */
    int offset = strlen(functionLiteral(funct_op))+1;
    int end_offset = strlen(usr_input)-2;
    char operand_extract[end_offset-offset+1];
    strncpy(operand_extract, usr_input+offset, end_offset-offset+1);
    operand_extract[end_offset-offset+1] = '\0';
    /* function block definition */
    new_block->function_op = funct_op;
    new_block->coefficients = NULL;
    new_block->order = 0;
    new_block->operandA = createFunction(operand_extract);
    new_block->operandB = NULL;
  }
  /*----------------------- double operand handle ------------------------*/
  else if (funct_op < 7) {
    /* operand extraction */
    int offset = strlen(functionLiteral(funct_op))+1;
    int end_offset = strlen(usr_input)-2;
    // find the comma that seperate operandA and operandB
    int open_parentheses = 0; // the , should be located where all open '(' has closed ')'
    int comma_pos = 0;
    for (int i = offset; i < end_offset; i++) {
      if (usr_input[i] == '(') open_parentheses++;
      else if (usr_input[i] == ')') open_parentheses--;
      else if (usr_input[i] == ',' && open_parentheses == 0) {
	comma_pos = i;
	break;
      }
    }
    char operandA_extract[comma_pos-offset+1];
    char operandB_extract[end_offset-comma_pos+1];
    strncpy(operandA_extract, usr_input+offset, comma_pos-offset);
    operandA_extract[comma_pos-offset] = '\0';
    strncpy(operandB_extract, usr_input+comma_pos+1, end_offset-comma_pos);
    operandB_extract[end_offset-comma_pos] = '\0';
    /* function block definition */
    new_block->function_op = funct_op;
    new_block->coefficients = NULL;
    new_block->order = 0;
    new_block->operandA = createFunction(operandA_extract);
    new_block->operandB = createFunction(operandB_extract);
  }
  return new_block;
}

// calculate the value of the function for given x
double calcFunction (FUNCT_BLOCK* function, double X, int degree) {
  double result = 0;
  if (function != NULL) {
    if (function->function_op == polf) { // polynomials
      result = polynomial(function->coefficients, function->order, X);
    }else if (function->function_op < 20 && function->function_op > 6){ // single operand operations
      double operand = calcFunction(function->operandA, X, degree);
      result = applyFunction(function->function_op, operand, 0, degree);
    }else if (function->function_op < 7) { // double operand operations
      double operandA = calcFunction(function->operandA, X, degree);
      double operandB = calcFunction(function->operandB, X, degree);
      result = applyFunction(function->function_op, operandA, operandB, degree);
    }
  }else { // if the function pointer is for some reason a null pointer
    printf("No function found.\n");
  }
  return result;
}

void clearFunction (FUNCT_BLOCK* function) {
  if (function != NULL) {
    // clear coeffs if needed
    if (function->coefficients != NULL) free(function->coefficients);
    // continue on with operands
    if (function->operandA != NULL) clearFunction(function->operandA);
    if (function->operandB != NULL) clearFunction(function->operandB);
    // free the pointer for this function
    free(function);
  }
}

// find integral of function, find integral of a function
double integral (FUNCT_BLOCK* function, double A, double B, int degree) {
  const double step = 0.00001; // define the step for the trapezoid rule
  double result = 0; // result of integral
  int negative = 0;
  // make sure that A < B
  if (A > B) {
    negative = 1; // in case A and B are swapped, the result must also be taken as -result
    double temp = A;
    A = B;
    B = temp;
  }
  double X = A; // position of "cursor" to calculate the area under curve
  double value_at_x = 0;
  double value_next_x = 0;
  while (X < B) {
    value_at_x = calcFunction(function, X, degree);
    X = (X+step > B) ? B : X+step;
    value_next_x = calcFunction(function, X, degree);
    result += (value_at_x + value_next_x)*step/2;
  }
  return (negative) ? -result : result;
}

// find the derivative of a function at a point
double derive (FUNCT_BLOCK* function, double X, int degree) {
  const double epsilon = 0.000001;
  double result = (calcFunction(function, X+epsilon, degree) - calcFunction(function, X-epsilon, degree))/(2*epsilon);
  return result;
}
