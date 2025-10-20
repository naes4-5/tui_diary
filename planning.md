# Big overview steps for this project

1. Creating new entries
    - All to a common directory (per month)
            **Next 2 made erronious by dirinit**
        - Check for relevant directory
            - If it doesn't exist, make it
        - Check the date and go to the relevant month
    - Common formatting (Specified)
    - Project notes functionality
2. Accessing old entries
    - Create notes with metadata with the date
    - Option to have them be dated or relevant to some other directory
3. Making the entire system a cli tool
    - Adding flags that change functionality (new, daily, project-based)

## Commit planning

3. Authentication system???

#### Complete aspects
- Directories relating to the current month
-> Make sure the note is in the correct directory
-> Get the date
-> Add the time of creation at the top of the file
Make a way to create and modify the correct note. -> Take input and write it to file where the terminating character is '\n'
