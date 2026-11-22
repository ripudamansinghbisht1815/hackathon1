#include <stdio.h>
#include "app.h"
int main(void){initializeDefaults();puts("Student Result & Grade Analyzer");if(!adminLogin()){puts("Access denied.");return 1;}dashboard();mainMenu();puts("Logged out.");return 0;}
