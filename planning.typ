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

== Big overview steps for this project

1. Creating new entries
    - All to a common directory (per month) \
            _Next 2 made erroneous by dirinit()_
        - Check for relevant directory
            - If it doesn't exist, make it
        - Check the date and go to the relevant month
    - Common formatting (Specified)
    - Project notes functionality
2. Accessing old entries
    - Create notes with metadata with the date
    - Option to have them be dated or relevant to some other directory
Making the entire system a cli tool
  - Adding flags that change functionality (new, daily, project-based)

== Commit planning
- Don't crate new notes when you don't need too
-> Check to see if the note already exists 

- Title formatting
-> ex. Change ':3' to ':3rd', then there is 1st, 2nd, and all the rest are xth

== Complete aspects
- Directories relating to the current month
-> Make sure the note is in the correct directory \
-> Get the date \
-> Add the time of creation at the top of the file \
Make a way to create and modify the correct note. -> Take input and write it to
file where the terminating character is '\n' _ had to make the character '|' _
- Using as a cli tool
-> using the args from argv to decide what I'm doing
-> I'm only now realizing that this has probably lost sight of the original scope, but whatever. 
