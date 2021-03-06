#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "frontend.h"

int degreeMode = 0;

/* Mode operation 0: single operation calculation
   - the user can only a single operation (syntax according to user's guide), which can be double 
   operanded or singled operanded
   - the operation get stored in ANS and can be retrieved to perform next operation
*/
void singleOpMode (double* ANS) { // answer is on the heap to keep access scope
  char usr_input[100]; // user input get buffered up to 100 characters
  singleOpGuide(); // the guide before using this mode
  double result; // store result of current op
  // preprompt
  printf("\tPrompt: ");
  fgets(usr_input,100,stdin);
  while (strstr(usr_input,"exit") == NULL) {
    if (strstr(usr_input,"degMode") != NULL) { // setting the degree mode for trigo functions
      degreeMode = 1;
      printf("\t\tSet trigonometric functions to DEGREE mode.\n");
    }else if (strstr(usr_input,"radMode") != NULL) { // setting the radian mode for trigo functions
      degreeMode = 0;
      printf("\t\tSet trigonometric functions to RADIAN mode.\n");
    }else if (strstr(usr_input,"manual") != NULL) { // print manual if the user forgets
      singleOpGuide();
    }else {
      result = singleOp(usr_input, ANS, degreeMode);
      printf("* ANS = %lf \n", result);
    }
    printf("\tPrompt: ");
    fgets(usr_input,100,stdin);
  }
}

/* Mode operation 1: complex number handling
   - the user can only a single operation with complex numbers (syntax according to user's guide), can be
   double operanded and single operanded
   - the operation get stored in ANS and can be retrieved to perform next operation
*/
void complexMode (COMPLEX* ANS_CMPLX) {
  char usr_input[100]; // user input get buffered up to 100 characters
  complexModeGuide(); // the guide before using this mode
  COMPLEX result; // store result of current op
  // preprompt
  printf("\tPrompt: ");
  fgets(usr_input,100,stdin);
  while (strstr(usr_input,"exit") == NULL) {
    if (strstr(usr_input,"degMode") != NULL) { // setting the degree mode for trigo functions
      degreeMode = 1;
      printf("\t\tSet argument function to DEGREE mode.\n");
    }else if (strstr(usr_input,"radMode") != NULL) { // setting the radian mode for trigo functions
      degreeMode = 0;
      printf("\t\tSet argument function to RADIAN mode.\n");
    }else if (strstr(usr_input,"manual") != NULL) { // print manual if the user forgets
      complexModeGuide();
    }else {
      result = applyOpCmplx(usr_input, ANS_CMPLX, degreeMode);
      printf("* ANS = ");
      printCmplx(result);
    }
    printf("\tPrompt: ");
    fgets(usr_input,100,stdin);
  }
}

/* Mode operation 2: matrix arithmetics handling
   - the user can choose the operation to perform with matrixes
   - then there would be options to specify the size of them matrix and also to input the matrix
   - the program give user the option to confirm the matrix is properly input
   - result would be printed out according to the operation chosen
*/
void matrixMode (void) {
  char usr_input[100]; // user input get buffered up to 100 characters
  char num_input[20]; // double number buffered upto 20 characters
  matrixModeGuide();
  printf("\tPrompt: ");
  fgets(usr_input,100,stdin);
  while (strstr(usr_input,"exit") == NULL) {
    /*------------------ handle matrix inversion ---------------------*/
    if (strstr(usr_input,"inverse") != NULL) { 
      printf("You have chosen inverse mode. Please input the matrix size: ");
      // take in the matrix size
      size_t matrix_size;
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      matrix_size = atof(num_input);
      // make sure that matrix size is not 1
      while (matrix_size < 2 || matrix_size > 10) { // 10 by 10 matrix would be too time consuming anyway
	printf("Please reinput a valid matrix size: ");
        fgets(num_input,20,stdin);
	if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	matrix_size = atof(num_input);
      }
      double input_matrix[matrix_size][matrix_size];
      // take in the input for the matrix
      for (size_t i = 0; i < matrix_size; i++) {
	for (size_t j = 0; j < matrix_size; j++) {
	  printf("Input for {%ld,%ld} = ",i+1,j+1); // for user convenience 
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  input_matrix[i][j] = atof(num_input);
	}
      }
      printf("You have input matrix: \n");
      printMat(matrix_size, matrix_size, input_matrix);
      // perform inverse operation
      double result_matrix[matrix_size][matrix_size];
      matrixInv(matrix_size, input_matrix, result_matrix);
      printf("* ANS = \n");
      for (size_t i = 0; i < matrix_size; i++) {
	printf("\t[");
	for (size_t j = 0; j < matrix_size; j++) {
	  printf("%.3lf\t",result_matrix[i][j]);
	}
	printf("]\n");
      }
      /*-------------------- handle matrix determinant -----------------*/
    }else if (strstr(usr_input,"determinant") != NULL) { 
      printf("You have chosen determinant mode. Please input the matrix size: ");
      // take in the matrix size
      size_t matrix_size;
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      matrix_size = atof(num_input);
      // make sure that matrix size is not 1
      while (matrix_size < 2 || matrix_size > 10) { // 10 by 10 matrix would be too time consuming anyway
	printf("Please reinput a valid matrix size: ");
        fgets(num_input,20,stdin);
	if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	matrix_size = atof(num_input);
      }
      double input_matrix[matrix_size][matrix_size];
      // take in the input for the matrix
      for (size_t i = 0; i < matrix_size; i++) {
	for (size_t j = 0; j < matrix_size; j++) {
	  printf("Input for {%ld,%ld} = ",i+1,j+1); // for user convenience 
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  input_matrix[i][j] = atof(num_input);
	}
      }
      printf("You have input matrix: \n");
      for (size_t i = 0; i < matrix_size; i++) {
	printf("\t[");
	for (size_t j = 0; j < matrix_size; j++) {
	  printf("%.3lf\t",input_matrix[i][j]);
	}
	printf("]\n");
      }
      // perform determinant operation
      double result = matrixDet(matrix_size,input_matrix);
      printf("* ANS = %lf\n",result);
      /*----------------------------- handle matrix addition ---------------------------------*/
    }else if (strstr(usr_input,"add") != NULL) { 
      printf("You have chosen matrix addition mode. \n");
      // input the size of the matrices
      size_t rows, cols;
      /* input rows for matrix*/
      printf("Please input the size of number of rows for first matrix = ");
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      rows = atof(num_input);
      /* input cols for matrix */
      printf("Please input the size of number of column for first matrix = ");
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      cols = atof(num_input);
      /* matrix declartion */
      double A[rows][cols], B[rows][cols];
      /* input for first matrix */
      /* start input for matrix A */
      printf(" * Please start with the input for the first matrix. \n");
      for (size_t i = 0; i < rows; i++) {
	for (size_t j = 0; j < cols; j++) {
	  printf("Input number for {%ld,%ld}: ",i+1,j+1);
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  A[i][j] = atof(num_input);
	}
      }
      /* start input for matrix B */
      printf(" * Please start with the input for the second matrix. \n");
      for (size_t i = 0; i < rows; i++) {
	for (size_t j = 0; j < cols; j++) {
	  printf("Input number for {%ld,%ld}: ",i+1,j+1);
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  B[i][j] = atof(num_input);
	}
      }
      /* perform matrix addition */
      matrixAdd(rows, cols, A, B);
      printf("* ANS = \n");
      printMat(rows, cols, A);
      /*------------------------ handle matrix multiplication -------------------------*/
    }else if (strstr(usr_input,"multiply") != NULL) { 
      printf("You have chosen matrix multiplication. \n");
      // input the size of the matrices
      size_t rowsA, colsA, rowsB, colsB;
      /* input rows for A */
      printf("Please input the size of number of rows for first matrix = ");
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      rowsA = atof(num_input);
      /* input cols for A */
      printf("Please input the size of number of column for first matrix = ");
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      colsA = atof(num_input);
      /* input rows for B */
      printf("Please input the size of number of rows for second matrix = ");
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      rowsB = atof(num_input);
      /* input cols for B */
      printf("Please input the size of number of column for second matrix = ");
      fgets(num_input,20,stdin);
      if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      colsB = atof(num_input);
      /* create the input matrices A,B and result matrix C */
      double A[rowsA][colsA], B[rowsB][colsB], C[rowsA][colsB];
      /* tie C to 0 */
      for (size_t i = 0; i < rowsA; i++) {
	for (size_t j = 0; j < colsB; j++) {
	  C[i][j] = 0;
	}
      }
      /* start input for matrix A */
      printf(" * Please start with the input for the first matrix. \n");
      for (size_t i = 0; i < rowsA; i++) {
	for (size_t j = 0; j < colsA; j++) {
	  printf("Input number for {%ld,%ld}: ",i+1,j+1);
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  A[i][j] = atof(num_input);
	}
      }
      /* start input for matrix B */
      printf(" * Please start with the input for the second matrix. \n");
      for (size_t i = 0; i < rowsB; i++) {
	for (size_t j = 0; j < colsB; j++) {
	  printf("Input number for {%ld,%ld}: ",i+1,j+1);
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  B[i][j] = atof(num_input);
	}
      }
      /* calculate the result for multiplication*/
      matrixMult(rowsA, colsA, rowsB, colsB, A, B, C);
      printf("* ANS = \n");
      printMat(rowsA, colsB, C);
      /*----------------------------- show manual ----------------------------*/
    }else if (strstr(usr_input,"manual") != NULL) { 
      matrixModeGuide();
    }
    /*------------- main loop continue ---------------------*/
    if (0) { // this block is inaccessible, except for a direct access to premature_exit
    premature_exit: {
	/* when user want to change mode prematurely */
	printf("\t* You have canceled the current operation.\n");
	printf("\t* If you are having any trouble, please refer to the \"manual\" \n");
      }
    }
    printf("\tPrompt: ");
    fgets(usr_input, 100, stdin);
  }
}

/* Mode operation 3: function generator option
   - the user can input a function and perform operation with said function
   - input according to the guide 
*/
void functionMode (FUNCT_BLOCK* function_list[5]) {
  char usr_input[100]; // usr_input buffered at 100 characters
  char num_input[20]; // double number buffered upto 20 characters  
  functionGuide(); // the guide before using this mode
  // preprompt
  printf("\tPrompt: ");
  fgets(usr_input,100,stdin);
  while (strstr(usr_input,"exit") == NULL) {
    if (strstr(usr_input,"degMode") != NULL) { // setting the degree mode for trigo functions
      degreeMode = 1;
      printf("\t\tSet argument function to DEGREE mode.\n");
    }else if (strstr(usr_input,"radMode") != NULL) { // setting the radian mode for trigo functions
      degreeMode = 0;
      printf("\t\tSet argument function to RADIAN mode.\n");
    }else if (strstr(usr_input,"manual") != NULL) { // print manual if the user forgets
      functionGuide();
    }/* user type in input to start typing and storing function */
    else if(strstr(usr_input,"input") != NULL) {
      /*----------- generate function -----------*/
      printf("Please start input your function: ");
      fgets(usr_input, 100, stdin);
      if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
      FUNCT_BLOCK * new_function = createFunction(usr_input);
      /*----------- store function ------------*/
      int correct_storage = 1;
      do {
	printf("Store your new function in variable: ");
	fgets(usr_input, 100, stdin);
	if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	if (strstr(usr_input,"A") != NULL) { // store in A
	  correct_storage = 1;
	  clearFunction(function_list[0]);
	  function_list[0] = new_function;
	}else if (strstr(usr_input,"B") != NULL) { // store in B
 	  correct_storage = 1;
	  clearFunction(function_list[1]);
	  function_list[1] = new_function;
	}else if (strstr(usr_input,"C") != NULL) { // store in C
	  correct_storage = 1;
	  clearFunction(function_list[2]);
	  function_list[2] = new_function;
	}else if (strstr(usr_input,"D") != NULL) { // store in D
	  correct_storage = 1;
	  clearFunction(function_list[3]);
	  function_list[3] = new_function;
	}else if (strstr(usr_input,"E") != NULL) { // store in E
	  correct_storage = 1;
	  clearFunction(function_list[4]);
	  function_list[4] = new_function;
	}else {
	  correct_storage = 0;
	  printf("Invalid storage. \n");
	}
      }while (!correct_storage);
    }/* user type in calc to start working with the functions */
    else if(strstr(usr_input,"calc") != NULL) {
      /*--------------------- start with operations -------------------*/
      while (strstr(usr_input,"exit") == NULL) {
	printf("Please choose function (single, table, integrate, derive): ");
	fgets(usr_input, 100, stdin);
	if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	/*----------------------------- single operation on function -----------------------*/
	if (strstr(usr_input,"single") != NULL) {
	  printf("You have chosen to work in single calculation mode. \n");
	  double X = 0, result = 0;
	  FUNCT_BLOCK * function; 
	  printf("\t Please input x = ");
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  X = atof(num_input);
	  printf("Apply to function (A,B,C,D,E): ");
	  fgets(usr_input, 100, stdin);
	  if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  if (strstr(usr_input,"A") != NULL) function = function_list[0];
	  else if (strstr(usr_input,"B") != NULL) function = function_list[1];
	  else if (strstr(usr_input,"C") != NULL) function = function_list[2];
	  else if (strstr(usr_input,"D") != NULL) function = function_list[3];
	  else if (strstr(usr_input,"E") != NULL) function = function_list[4];
	  result = calcFunction(function, X, degreeMode);
	  printf("* ANS = %lf\n", result);
	}/*----------------------------- value table on function -----------------------*/
	else if (strstr(usr_input,"table") != NULL) {
	  printf("You have chosen to work in value table mode. \n");
	  double begin, end, step, X;
	  FUNCT_BLOCK* function;
	  // begin point input
	  printf("\tBegin = ");
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  begin = atof(num_input);
	  // end point input, check if end is bigger than begin to make sure operation is valid
	  int valid_end = 1;
	  do {
	    printf("\tEnd = ");
	    fgets(num_input,20,stdin);
	    if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	    end = atof(num_input);
	    if (end < begin) {
	      printf("Please reenter a valid end point bigger than %lf \n",begin);
	      valid_end = 0;
	    }else {
	      valid_end = 1;
	    }
	  }while(!valid_end);
	  // step input, make sure that step input is in the correct range
	  int valid_step = 1;
	  do {
	    printf("\tStep = ");
	    fgets(num_input,20,stdin);
	    if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	    step = atof(num_input);
	    if (step < 0.01 || step > 1000) {
	      printf("Please reenter a valid step between 0.01 and 1000. \n");
	      valid_step = 0;
	    }else {
	      valid_step = 1;
	    }
	  }while(!valid_step);
	  // choose function to apply table
	  printf("Apply to function (A,B,C,D,E): ");
	  fgets(usr_input, 100, stdin);
	  if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  if (strstr(usr_input,"A") != NULL) function = function_list[0];
	  else if (strstr(usr_input,"B") != NULL) function = function_list[1];
	  else if (strstr(usr_input,"C") != NULL) function = function_list[2];
	  else if (strstr(usr_input,"D") != NULL) function = function_list[3];
	  else if (strstr(usr_input,"E") != NULL) function = function_list[4];
	  // then perform the calculations and print out value table
	  X = begin;
	  printf("Value table for f(x): \n");
	  printf("\t\t X\t\t f(X) \n");
	  while (X <= end) {
	    printf("\t\t %lf\t %lf \n",X,calcFunction(function,X,degreeMode));
	    X += step;
	    X = (X>end && X != end+step) ? end : X; // if above end, send back to end. Avoid if X is right at end.
	  }
	}/* -------------------------- integration handle -----------------------------*/
	else if (strstr(usr_input,"integrate") != NULL) {
	  printf("You have chosen to work in integration mode. \n");
	  double a = 0, b = 0, result = 0;
	  FUNCT_BLOCK * function;
	  // get a 
	  printf("\t Please input a = ");
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  a = atof(num_input);
	  // get b
	  printf("\t Please input b = ");
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  b = atof(num_input);
	  // get function to apply
	  printf("Apply to function (A,B,C,D,E): ");
	  fgets(usr_input, 100, stdin);
	  if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  if (strstr(usr_input,"A") != NULL) function = function_list[0];
	  else if (strstr(usr_input,"B") != NULL) function = function_list[1];
	  else if (strstr(usr_input,"C") != NULL) function = function_list[2];
	  else if (strstr(usr_input,"D") != NULL) function = function_list[3];
	  else if (strstr(usr_input,"E") != NULL) function = function_list[4];
	  // calculate and print integral
	  result = integral(function, a, b, degreeMode);
	  printf("* ANS = %lf \n", result);
	}/*------------------------- derivative handle ----------------------------*/
	else if (strstr(usr_input,"derive") != NULL) {
	  printf("You have chosen to work in derivation mode. \n");
	  double x = 0, result = 0;
	  FUNCT_BLOCK * function;
	  // get x
	  printf("\t Please input a = ");
	  fgets(num_input,20,stdin);
	  if (strstr(num_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  x = atof(num_input);
	  // get function to apply
	  printf("Apply to function (A,B,C,D,E): ");
	  fgets(usr_input, 100, stdin);
	  if (strstr(usr_input,"exit") != NULL) goto premature_exit; // handle a premature exit
	  if (strstr(usr_input,"A") != NULL) function = function_list[0];
	  else if (strstr(usr_input,"B") != NULL) function = function_list[1];
	  else if (strstr(usr_input,"C") != NULL) function = function_list[2];
	  else if (strstr(usr_input,"D") != NULL) function = function_list[3];
	  else if (strstr(usr_input,"E") != NULL) function = function_list[4];
	  // calculate and print integral
	  result = derive(function, x, degreeMode);
	  printf("* ANS = %lf \n", result);
	}else { // invalid input
	  printf("Please input a valid command. Refer to manual in prompt for further details.\n");
	}
      }
    }
    /*------------------- main loop continue ---------------------*/
    if (0) { // this block is inaccessible, except for a direct access to premature_exit
    premature_exit: {
	/* when user want to change mode prematurely */
	printf("\t* You have canceled the current operation.\n");
	printf("\t* If you are having any trouble, please refer to the \"manual\" \n");
      }
    }
    printf("\tPrompt: ");
    fgets(usr_input,100,stdin);
  }
}

/*  general guides for calculator (1 main guide and 4 guides for specific modes) */

/* main calculator guide */
void calculatorGuide (void) {
  printf("-----------------------------------------------------------------------------------------\n");
  printf("*                      *** WELCOME TO THE SCIENTIFIC CALCULATOR ***                     *\n");
  printf("*  Please choose from the following modes of operations:                                *\n");
  printf("*  - for single operation mode, where user can input a math operation and get result    *\n");
  printf("*    * operation code: 0                                                                *\n");
  printf("*  - for complex number mode, where user can manipulate complex numbers                 *\n");
  printf("*    * operation code: 1                                                                *\n");
  printf("*  - for matrix calculations, where user can perform matrix arithmetics                 *\n");
  printf("*    * operation code: 2                                                                *\n");
  printf("*  - for function mode, where user can input and store function used for table values   *\n");
  printf("*    or integration or derive                                                           *\n");
  printf("*    * operation code: 3                                                                *\n");
  printf("*  - to exit calculator, type exit. for manual type manual and type operation code to   *\n");
  printf("*    use calculator                                                                     *\n");
  printf("-----------------------------------------------------------------------------------------\n");
}

/* the user guide for single operation mode */
void singleOpGuide (void) {
  printf("-----------------------------------------------------------------------------------------\n");
  printf("* You are using mode single operation.                                                  *\n");
  printf("* There are 21 commands that can be understood and executed by the calculator:          *\n");
  printf("* - basic operations such as + m * /, e.g. a*b. (m replaces - as operation              *\n");
  printf("* - pow(a,b) = a^b                                                                      *\n");
  printf("* - logx(a,b) = log b of a                                                              *\n");
  printf("* - root(a,b) = root b of a                                                             *\n");
  printf("* - single operand functions: sin(x), cos(x), tan(x), sinh(x), cosh(x), tanh(x), fact(x)*\n");
  printf("*  ln(x), log(x), exp(x), abs(x), ac(x) (arccos), as(x) (arcsin), at(x) (arctan)        *\n");
  printf("* Use ANS as operand to recall the last answer                                          *\n");
  printf("* CONFIGURATION OPTIONS:                                                                *\n");
  printf("* + Type \"exit\" if you want to get out of single operation mode.                        *\n");
  printf("* + Type degMode to use trigonometric function in degree mode.                          *\n");
  printf("* + Type radMode to use trigonometric function in radian mode.                          *\n");
  printf("-----------------------------------------------------------------------------------------\n");
}

/* the user guide for the matrix operation mode */
void matrixModeGuide (void) {
  printf("-----------------------------------------------------------------------------------------\n");
  printf("* You are using the matrix operation mode.                                              *\n");
  printf("* Operations that the calculator can perform                                            *\n");
  printf("*    - inverse                                                                          *\n");
  printf("*    - determinant                                                                      *\n");
  printf("*    - add                                                                              *\n");
  printf("*    - multiply                                                                         *\n");
  printf("* CONFIGURATION OPTIONS:                                                                *\n");
  printf("* + Type \"exit\" if you want to get out of matrix mode.                                  *\n");
  printf("-----------------------------------------------------------------------------------------\n");
}

/* the user guide for the complex number mode */
void complexModeGuide (void) { 
  printf("-----------------------------------------------------------------------------------------\n");
  printf("* You are using the complex number mode.                                                *\n");
  printf("* In this mode of operation, you can perform operations on complex numbers.             *\n");
  printf("* Valid operations: +(add), -(subtract), *(multiply), /(divide)                         *\n");
  printf("*                   mag() (magnitude), arg() (argument)                                 *\n");
  printf("* When writing the complex numbers, please don't leave spaces between them, but dont't  *\n");
  printf("* forget to add space between the complex number operand and the basic operations.      *\n");
  printf("* Don't leave space however in the operand for mag() and arg() function.                *\n");
  printf("* Input examples: 1+2i - -7+8i        (valid)                                           *\n");
  printf("*                 1+2i*3-i            (invalid)                                         *\n");
  printf("*                 mag(1+4i)           (valid)                                           *\n");
  printf("*                 arg( -78+i )        (invalid)                                         *\n");
  printf("* Use ANS as operand to recall the last answer                                          *\n");
  printf("* CONFIGURATION OPTIONS:                                                                *\n");
  printf("* + Type \"exit\" if you want to get out of complex number mode.                          *\n");
  printf("* + Type degMode to use trigonometric function in degree mode.                          *\n");
  printf("* + Type radMode to use trigonometric function in radian mode.                          *\n");
  printf("-----------------------------------------------------------------------------------------\n");
}

/* the user guide for the function generator mode */
void functionGuide (void) {
  printf("-----------------------------------------------------------------------------------------\n");
  printf("* You have chosen function generator mode.                                              *\n");
  printf("* Up to 5 functions could be stored and invoked by characters A,B,C,D,E                 *\n");
  printf("* There are 21 functions that function generator can understand:                        *\n");
  printf("* + Double operand functions: add(a,b)(a+b), min(a,b)(a-b), mul(a,b)(a*b), div(a,b)(a/b)*\n");
  printf("*   pow(a,b)(a^b), logx(a,b) (log of a to power of b), root(a,b) (root a of power b),   *\n");
  printf("* + Single operand functions: sin(x), cos(x), tan(x), sinh(x), cosh(x), tanh(x), ln(x), *\n");
  printf("*   log(x), exp(x), abs(x), ac(x) (arccos), as(x) (arcsin), at(x) (arctan)              *\n");
  printf("* + Polynomial: pol(a0,a1,a2,...,an) = a0 + a1*x + a2*x^2 + ... + an*x^n                *\n");
  printf("*   Input example: div(exp(pol(1,2,3)),cos(pol(0,1))) represent an exponetial of a      *\n");
  printf("*                 polyomial with coeff 1,2,3 divide by cos of polynomial with coeff 0,1 *\n");
  printf("* After function input, user can store it in A or B or C or D or E                      *\n");
  printf("* User can then choose from 4 options after typing calc                                 *\n");
  printf("* - single : calculate single value input by user. exit to return from this mode.       *\n");
  printf("* - table : calculate a value of table with starting point, ending point and step. Steps*\n");
  printf("*           are limited between 0.01 and 1000. exit to return from this mode.           *\n");
  printf("* - integrate: integrate function from a to b. exit to return from this mode.           *\n");
  printf("* - derive: take the derivative of function at point a. exit to return from this mode   *\n");
  printf("* CONFIGURATION OPTIONS: (in prompt)                                                    *\n");
  printf("* + Type exit if you want to exit function generator mode.                              *\n");
  printf("* + Type degMode to use trigonometric function in degree mode.                          *\n");
  printf("* + Type radMode to use trigonometric function in radian mode.                          *\n");
  printf("* + Type input to start input function.                                                 *\n");
  printf("* + Type calc to start working with the functions you have input                        *\n");
  printf("-----------------------------------------------------------------------------------------\n");
}

