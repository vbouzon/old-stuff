#include <stdio.h>
#include <stdlib.h>

double x = 0;
double h = 0;
double integral = 0;
double pi = 0;
double pas = 0.00000001;

int main(int argc, char *argv[])
{
  while (x < 1)
  {
    h = sqrt(1 - ((1 - x) * (1 - x)));
    integral += h * pas;
    x = x + pas;
    printf("x = %.8f , h = %.8f\n", x, h);
  }
  pi = integral * 4;
  printf("%.8f", pi);
  system("pause");
}
