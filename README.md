
# PDA implementation

This project is a Pushdown Automaton (PDA) simulator written in C. It allows you to define a PDA by specifying states, transitions, and final states. You can then input words to see if they are accepted by the PDA.




## Features

- Define states and transitions of a PDA.
- Specify final states.
- Input words to verify if they are accepted by the PDA.
- Display the computation steps for each word.

## Files
- 'main.c': The main program file containing the PDA logic and user interaction.
- 'Stack.c' File containing the Stack logic.
- 'Stack.h': Header file containing Stack operations.

## Getting Started

###  Prerequisites
- GCC compiler (or any C compiler).

### Compilation 
To compile the program, run the following command (on Linux):

```bash
    gcc -o pda_simulation main.c Stack.c
```
### Usage
After compiling, run the program (on Linux) with:

```bash
    ./pda_simulation
```
## Example

### Input the number of states 

```bash
    Number of states: 
    3
    Number of transitions: 
    4
```
### Input the transitions:

```bash
    Enter the transitions (format: source received pop push dest) ('&' means empty): 
    0 a & A 0
    0 & & & 1
    1 b A & 1
    1 & Z Z 2
```
### Input the final states:

```bash
    Number of final states: 
    1
    Enter which nodes will be the final states: 
    2
```
### Input words to verify:

```bash
    Enter words to verify ('*' to stop): 
    ab
    ab: yes
    (q0, ab, Z) |-
    (q0, b, AZ) |-
    (q1, b, AZ) |-
    (q1, &, Z) |-
    (q2, &, Z).

    Enter words to verify ('*' to stop): 
    aabb
    aabb: yes
    (q0, aabb, Z) |-
    (q0, abb, AZ) |-
    (q0, bb, AAZ) |-
    (q1, bb, AAZ) |-
    (q1, b, AZ) |-
    (q1, &, Z) |-
    (q2, &, Z).

    Enter words to verify ('*' to stop): 
    aab
    aab: no

    Enter words to verify ('*' to stop): 
    *
```


## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue if you have any suggestions or improvements.
