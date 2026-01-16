#include <stdio.h>

int main()
{
    int score300g = 0, score300 = 0, score200 = 0, score100 = 0, score50 = 0, score0 = 0; 
    printf("Enter your osu mania score: (300g 300 200 100 50 0) \n");
    scanf("%d %d %d %d %d %d", &score300g, &score300, &score200, &score100, &score50, &score0);
    if (score300g < 0 || score300 < 0 || score200 < 0 || score100 < 0 || score50 < 0 || score0 < 0)
    {
        printf("Invalid input! All scores must be non-negative integers.\n");
        printf("Press any key to exit...\n");
        while (1)
        {
            getchar();
            if (getchar() != NULL)
                break;
      }
        return 1;
    }
    if (score300g == 0 && score300 == 0 && score200 == 0 && score100 == 0 && score50 == 0 && score0 == 0)
    {
        printf("Invalid input! Scores cannot be zero.\n");
        printf("Press any key to exit...\n");
        while (1)
        {
            getchar();
            if (getchar() != NULL)
                break;
        }
        return 2;
    }

    float scorev1 = 0.0, scorev2 = 0.0;
    scorev1 = (float)(300 * (score300g + score300) + 200 * score200 + 100 * score100 + 50 * score50) /
              (float)(300 * (score300g + score300 + score200 + score100 + score50 + score0));
    scorev2 = (float)(305 * score300g + 300 * score300 + 200 * score200 + 100 * score100 + 50 * score50) /
              (float)(305 * (score300g + score300 + score200 + score100 + score50 + score0));

    printf("Your accuracy in Scorev1 is: %.4f\n", scorev1 * 100);
    printf("Your accuracy in Scorev2 is: %.4f\n", scorev2 * 100);
    printf("Press any key to exit...\n");
    while (1)
    {
        getchar();
        if (getchar() != NULL)
            break;
    }
}