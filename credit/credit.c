#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Get the credit card number
    long cred_num;
    do
    {
        cred_num = get_long("Credit card: ");
    }
    while (cred_num < 0);

    // Count the number of digits
    int count = 1;
    long limit = 10;
    while (limit <= cred_num)
    {
        count++;
        limit = (long) pow(10, count);
    }

    /*
    Calculate the sum to check the Luhn's Algorithm
    The placeholder is the value of the ith digit from the left
    */
    int sum = 0;
    int plholder;
    int frt_2_digit = 0;
    for (int i = count - 1; i >= 0; i--)
    {
        // Calculate the sum
        if ((i + 1) % 2 != 0)
        {
            plholder = (int)(cred_num / ((long) pow(10, i)));
            sum += plholder;
        }
        else
        {
            plholder = (int)(cred_num / ((long) pow(10, i)));
            sum += (int)((2 * plholder) / 10) + ((2 * plholder) % 10);
        }
        // Get the first two digits
        if (i == count - 1)
        {
            frt_2_digit += 10 * plholder;
        }
        if (i == count - 2)
        {
            frt_2_digit += plholder;
        }
        // Change the cred_num for the next loop to calculate the place holder
        cred_num -= (long)(plholder * ((long) pow(10, i)));
    }
    // Check whether a credit card number is valid
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    // Check whether a card is Mastercard, American Express or Visa
    else
    {
        if ((frt_2_digit == 34 || frt_2_digit == 37) && (count == 15))
        {
            printf("AMEX\n");
        }
        else if (((frt_2_digit >= 51) && (frt_2_digit <= 55)) && (count == 16))
        {
            printf("MASTERCARD\n");
        }
        else if ((count == 16 || count == 13) && ((int)(frt_2_digit / 10) == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
        // Other type of credit card number would not be accepted
    }
}