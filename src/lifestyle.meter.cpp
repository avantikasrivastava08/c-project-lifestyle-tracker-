#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOGFILE "users_log.txt"
#define MAX_ENTRIES 500
#define USERNAME_LEN 30

typedef struct
 {
    char username[USERNAME_LEN];
    int sleep;
    int productivity;
    int stress;
    int mood;
    double score;
} UserEntry;

double calculateScore(int sleep, int prod, int stress, int mood) //calculates the score of the user
{
    double s = (sleep / 8.0) * 25.0; //each element has 25% of contribution
    if (s > 25.0) s = 25.0;

    double p = (prod / 10.0) * 25.0; 
    double m = (mood / 10.0) * 25.0;
    double st = (1.0 - ((stress - 1.0) / 9.0)) * 25.0; //score works opposite for stress

    double score = s + p + m + st;
    if (score < 0) 
	score = 0;  //to avoid data leaks
    if (score > 100)
	 score = 100;
    return score;
}

// Append an entry for a user to the common log file 
int appendEntryToLog(const char *username, int sleep, int prod, int stress, int mood, double score) 
{
    FILE *fp = fopen(LOGFILE, "a");
    if (fp== NULL)
    {
	return -1;
    }
    //asks the user for their data.
    fprintf(fp, "%s %d %d %d %d %.2f\n", username, sleep, prod, stress, mood, score);
    fclose(fp);
    return 0;
}

int loadUserEntries(const char *username, UserEntry arr[], int max) // read the entries and sort them by user 
 {
    FILE *fp = fopen(LOGFILE, "r");
    if (fp==NULL)
	 return 0;  //CHECKS THE FILE POINTER TO AVOID MEMORY LEAKS

    int count = 0; //counts the score of the user
    char uname[USERNAME_LEN];
    int sleep, prod, stress, mood;
    double score;

    while (fscanf(fp, "%31s %d %d %d %d %lf", uname, &sleep, &prod, &stress, &mood, &score) == 6)
	 {
        if (strcmp(uname, username) == 0) 
		{
            if (count < max) {
                strncpy(arr[count].username, uname, USERNAME_LEN-1);
                arr[count].username[USERNAME_LEN-1] = '\0';
                arr[count].sleep = sleep;
                arr[count].productivity = prod;
                arr[count].stress = stress;
                arr[count].mood = mood;
                arr[count].score = score;
                count++;
            }
        }
    }

    fclose(fp);
    return count;
}

void AsciiGraph(const double scores[], int n) //displays ASCII graph represented by *.
 {
    printf("\n YOUR SCORE GRAPH IS HERE ^_^ :\n");  
    for (int i = 0; i < n; ++i) {
        int bars = (int)(scores[i] / 4.0 + 0.5);  //EACH * REPRESENTS 4 POINTS
        printf("Day %2d: ", i + 1);
        for (int b = 0; b < bars; ++b) 
		putchar('*');
        printf(" (%.2f)\n", scores[i]);
    }
}


void analyzeUser(const char *username, int show_last_n)
 {
    UserEntry entries[MAX_ENTRIES];
    int n = loadUserEntries(username, entries, MAX_ENTRIES); //LOADS ENTRIES.

    if (n == 0)
	 {
        printf("\n No entries found for user '%s'.\n", username); //if the user is not present, it prints an appropriate message.
        return;
    }

   
    int start = (n > show_last_n) ? (n - show_last_n) : 0;
    int m = n - start; /* number of entries considered */

    double scores[show_last_n];
    for (int i = 0; i < m; ++i) {
        scores[i] = entries[start + i].score;
    }

    
    printf("\nLast %d entries for %s:\n", m, username); //prints the last entries of the user.
    for (int i = 0; i < m; ++i)
	 {
        printf("Day %2d -> Sleep:%2d Prod:%2d Stress:%2d Mood:%2d Score: %.2f\n",
               start + i + 1,
               entries[start + i].sleep,
               entries[start + i].productivity,
               entries[start + i].stress,
               entries[start + i].mood,
               entries[start + i].score);
    }

    
    double sum = 0.0;
    double max = scores[0];
    double min = scores[0];
    for (int i = 0; i < m; ++i)
	 {
        sum += scores[i];
        if (scores[i] > max)
		 max = scores[i];
        if (scores[i] < min) 
		min = scores[i];
    }
    double avg = sum / m;
    printf("\nAverage (last %d): %.2f\n", m, avg); //average score of the user
    printf("Highest (last %d): %.2f\n", m, max); //highest
    printf("Lowest  (last %d): %.2f\n", m, min); //lowest

    if (m >= 2)  //sees trends in the routine by comparing the last entered entries.
	 {
        double last = scores[m - 1];
        double prev = scores[m - 2];
        if (last > prev) 
		printf("Trend: Improving! KEEP IT UP BLUD :P (%.2f -> %.2f)\n", prev, last);
        else if (last < prev)
		 printf("Trend: Declining! GOTTA CHANGE THE SCHEDULE DUDE OR UR COOKED *sigh* (%.2f -> %.2f)\n", prev, last);
        else 
		printf("Trend: Stable-- WELL ARENT YOU CONSISTANT!! (%.2f -> %.2f)\n", prev, last);
    }

    printf("\n");
    AsciiGraph(scores, m); //printing the score graph of the user

    
    if (avg < 40.0)  //suggestions by the system.
	 {
        printf("\nSuggestions: Your recent life score is low :( .\n");
        printf("- Try improving sleep & reducing stress.\n");
        printf("- Drink more water,maintain a time table, take short daily walks.\n");
    } 
	else if (avg < 70.0) 
	{
        printf("\nSuggestions: Room for improvement.\n");
        printf("- Strengthen hydration and consistent sleep.\n");
    } 
	else 
	{
        printf("\nGreat! Keep maintaining healthy habits.\n");
    }
}

void readUsername(char *buf, int size)
 {
    printf("Enter username (no spaces): ");
    if (scanf("%31s", buf) != 1) 
	{
        buf[0] = '\0';
    }
}

void userMenu(const char *username)  //menu avialable for the user logged in
{
    while (1)
	 {
        printf("\n--- User: %s ---\n", username);
        printf("1) Add today's data\n");
        printf("2) View analytics (last 7 entries)\n");
        printf("3) Logout\n");
        printf("Choose: ");

        int choice;
        if (scanf("%d", &choice) != 1)
		 {
            int c;
		  while ((c = getchar()) != '\n' && c != EOF) 
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (choice == 1) 
		{
            int sleep, prod, stress, mood;
            printf("Enter sleep hours (0-24): ");
            if (scanf("%d", &sleep) != 1) 
			{
			 printf("Invalid.\n");
			 continue;
			  }
            printf("Enter productivity (1-10): ");
            if (scanf("%d", &prod) != 1) 
			{ 
			printf("Invalid.\n");
			 continue;
			  }
            printf("Enter stress (1-10): ");
            if (scanf("%d", &stress) != 1)
			 {
			  printf("Invalid.\n"); 
			  continue;
			   }
            printf("Enter mood (1-10): ");
            if (scanf("%d", &mood) != 1)
			 {
			  printf("Invalid.\n");
			   continue;
			    }

           //to avoid error of out of range things.
            if (sleep < 0) 
			sleep = 0; 
			if (sleep > 24)
			sleep = 24;
            if (prod < 1)
			prod = 1;
			if (prod > 10)
		    prod = 10;
            if (stress < 1) 
			stress = 1; 
			if (stress > 10) 
			stress = 10;
            if (mood < 1) 
			mood = 1; 
			if (mood > 10)
			 mood = 10;

            double score = calculateScore(sleep, prod, stress, mood);
            if (appendEntryToLog(username, sleep, prod, stress, mood, score) == 0)
			 {
                printf("Saved entry. Today's score: %.2f\n", score);
            } 
			else
			 {
                printf("Failed to save entry.\n");
            }
        }
        else if (choice == 2) 
		{
            analyzeUser(username, 7); //data of last seven days 
        }
        else if (choice == 3)
		 {
            printf("Logging out %s.\n", username);
            break;
        }
        else 
		{
            printf("Invalid choice.\n");
        }
    }
}

int main() 
{
    printf(" YOUR LIFESTYLE STABILITY METER IS HERE ^_^ : \n");

    while (1)
	 {
        printf("\nMain Menu:\n");
        printf("1) Login as user\n");
        printf("2) Exit\n");
        printf("Choose: ");

        int mainChoice;
        if (scanf("%d", &mainChoice) != 1) {
            int c; 
			while ((c = getchar()) != '\n' && c != EOF) 
            printf("Invalid input.\n");
            continue;
        }

        if (mainChoice == 1)
		 {
            char username[USERNAME_LEN];
            readUsername(username, USERNAME_LEN);
            if (username[0] == '\0') {
                printf("Invalid username.\n");
                continue;
            }
            userMenu(username); 
        }
        else if (mainChoice == 2) {
            printf("Goodbye!\n");
            break;
        }
        else
		 {
            printf("Invalid option.\n");
        }
    }

    return 0;
}


