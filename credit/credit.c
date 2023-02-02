#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // *Get the credit card number
    long cred_num;
    do
    {
        cred_num = get_long("Credit card: ");
    }
    while (cred_num < 0);

    // *Count the number of digits
    int count = 1;
    long limit = 10;
    while (limit <= cred_num)
    {
        count++;
        limit = (long) pow(10, count);
    }
    /*
    *Calculate the sum to check the Luhn's Algorithm
    The placeholder is the value of the ith digit from the left
    */
    int sum = 0;
    int plholder;
    int frt_2_digit = 0;
    for (int i = count; i > 0; i--)
    {
        if (i % 2 != 0)
        {
            plholder = (int)(cred_num / ((long) pow( 10, i)));
            sum += plholder;
        }
        else
        {
            plholder = (int)(cred_num / ((long) pow( 10, i)));
            sum += 2 * plholder;
        }
        // Get the first two digits
        if (i == count)
        {
            frt_2_digit += 10 * plholder;
        }
        if (i == count - 1)
        {
            frt_2_digit += plholder;
        }
        // Change the cred_num for the next loop to calculate the place holder
        cred_num -= (long)(plholder * ((long) pow( 10, i)));
    }
    // *Check whether a credit card number is valid 
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        if ((frt_2_digit == 34 || frt_2_digit == 37) && (count == 15))
        {
            printf("AMERICAN EXPRESS\n");
        }
        else if ((frt_2_digit == 51 || frt_2_digit == 52 || 
        frt_2_digit == 53 || frt_2_digit == 54 || frt_2_digit == 55) && (count == 16))
        {
            printf("MASTERCARD\n");
        }
        else if ((frt_2_digit % 10 == 4) && (count == 16 || count == 13))
        {
            printf("VISA\n");
        }
        else
        {
            printf("Not a supported provider!\n");
        }
        
    }
}