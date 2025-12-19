#set page(height: auto)

= Direy
To help with context switching

== Purpose
1. Make context switching _easy_
    - The system tracks intent from day to day. 
    - Easy to look back at exactly what was going through your head yesterday
    - Have goals for a project clearly laid out (Like purpose & features)
        - Kinda like this document actually

2. Complete functionality 

```bash
direy write "started implementing feature {x}"
```
Makes a new note with the current date with the inputted text \ \

```bash
diery write -a "amending to previous message"
```
Amends the message to the last note from a normal write command \ \

```bash
direy write -p "Goal: Help with context switching"
```
Goals/notes for the entire project \ \

```bash
direy read
```
Outputs the most recent note \ \

```bash
direy read -d n
```
Outputs the nth note ago (so if n were one, has the same functionality as 'direy read') \ \

```bash
direy read -p 
```
Outputs the notes for the project 

