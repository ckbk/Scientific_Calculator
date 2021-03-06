#include <stdio.h>
#include <string.h>
#include "backend.h"
#include "frontend.h"

int main (void) {
  /* allocate variables to keep track of the operation results */
 
  // ANS store answer of single operation mode
  double ANS = 0;

  // ANS_CMPLX store answer of complex number mode
  COMPLEX ANS_CMPLX = {.re = 0, .im = 0};

  // a list of functions that was stored
  FUNCT_BLOCK* function_list[5] = {NULL, NULL, NULL, NULL, NULL};
  
  // core of the program
  char general_input[10]; // buffered at 10 chars
  calculatorGuide();
  printf("* Calculator operation: ");
  fgets(general_input, 10, stdin);
  while (strstr(general_input,"exit") == NULL) {
    if (strstr(general_input,"0") != NULL) {
      singleOpMode(&ANS);
    }else if (strstr(general_input,"1") != NULL) {
      complexMode(&ANS_CMPLX);
    }else if (strstr(general_input,"2") != NULL) {
      matrixMode();
    }else if (strstr(general_input,"3") != NULL) {
      functionMode(function_list);
    }else if (strstr(general_input,"manual") != NULL) {
      calculatorGuide();
    }else {
      printf("Please input a valid choice of operation. \n");
    }
    printf("* Calculator operation: ");
    fgets(general_input, 10, stdin);
  }
  
  // deallocate variables
  for (int i = 0; i < 5; i++) {
    clearFunction(function_list[i]);
  }
  printf("Memory has been cleared.\n");
  return 0;
}
