# c-project-emotion-study-assistant-
# Lifestyle Stability Meter 
It is a multi user program.
 A C-based lifestyle tracking system that allows multiple users to log in.
 Enter daily wellness data, and view personalized analytics based on their previous entries.

This program contains elements of:
- File handling  
- Struct usage  
- Multi-user data storage  
- Score calculation  
- Trend analysis  
- ASCII-based data visualization  
- Menu-driven programming  

Features:
Multi-user support  
- Each user logs in with a username  
- All data stored in a single file (`users_log.txt`)  
- Analytics shown **per user only**

Daily wellness tracking  
Users enter:
- Sleep hours  
- Productivity (1–10)  
- Stress (1–10)  
- Mood (1–10)  

Personalized analytics  
For each user:
- Last 7 entries  
- Average score  
- Highest & lowest score  
- Trend detection (Improving, Declining, Stable)  
- ASCII bar graph for visual representation  
- Suggestions based on performance .

The score uses a weighted formula (25% each):

- Sleep → 25%  
- Productivity → 25%  
- Mood → 25%  
- Stress (reverse scaled) → 25%  
  Final score is calculated from 1 to 100.


