# Direy
To help with context switching

## Purpose
1. Make context switching _easy_
    - The system tracks intent from day to day. 
    - Easy to look back at exactly what was going through your head last session
    - Have goals for a project clearly laid out (Like purpose, features, or goals)

2. Complete functionality 

Makes a new entry with the inputted text
```bash
direy write "just finished feature $x, now starting feature $y"
```

Amends the message to the last entry from a normal write command
```bash
diery write -a "amending to previous message"
```

Uses all of the contents in message.txt as the next entry
```bash
diery write -f message.txt
```

Goals/notes for the entire project
```bash
direy write -p "Goal: Help with context switching"
```

Outputs the most recent note
```bash
direy read
```

Outputs the notes for the project 
```bash
direy read -p 
```


Outputs the nth note ago (so if n were one, has the same functionality as '`direy read`')
```bash
direy read -d n
```

