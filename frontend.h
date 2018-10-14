#ifndef _FRONTEND_H_
#define _FRONTEND_H_

extern int degreeMode; // defines the calculator to work at degree mode if degMode = 1

/* this header defines the 4 main modes of operation for the scientific calculator
   4 main mode of operations for the scientific calculator:
   + single operation mode: user input a single operation and the answer is returned
   + matrix manipulations: manipulate matrices like add, mult, find determinant and inverse
   + complex number handling: handle complex number operations, including argument and magnitude
   + function generator: user input a function then we can create value tables, integrate or derive
*/

/* Mode operation 0: single operation calculation
   - the user can only a single operation (syntax according to user's guide), which can be double 
   operanded or singled operanded
   - the operation get stored in ANS and can be retrieved to perform next operation
*/
void singleOpMode (double* ANS);

/* Mode operation 1: complex number handling
   - the user can only a single operation with complex numbers (syntax according to user's guide), can be
   double operanded and single operanded
   - the operation get stored in ANS and can be retrieved to perform next operation
*/
void complexMode (COMPLEX* ANS_CMPLX);

/* Mode operation 2: matrix arithmetics handling
   - the user can choose the operation to perform with matrixes
   - then there would be options to specify the size of them matrix and also to input the matrix
   - the program give user the option to confirm the matrix is properly input
   - result would be printed out according to the operation chosen
*/
void matrixMode (void);

/* Mode operation 3: function generator option
   - the user can input a function and perform operation with said function
   - input according to the guide 
*/
void functionMode (FUNCT_BLOCK* function_list[5]);


/*  general guides for calculator (1 main guide and 4 guides for specific modes) */

/* main calculator guide */
void calculatorGuide (void);

/* the user guide for single operation mode */
void singleOpGuide (void);

/* the user guide for the matrix operation mode */
void matrixModeGuide (void);

/* the user guide for the complex number mode */
void complexModeGuide (void);

/* the user guide for the function generator mode */
void functionGuide (void);

#endif
