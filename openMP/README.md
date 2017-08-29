# Learning note of openMP

# Introduce:
The learning process of the openMP.

# Synchronization Constructs:

## Critical:

### Purpose:

 - The CRITICAL directive specifies a region of code that must be executed by only one thread at a time.

### Notes:

 - If a thread is currently executing inside a CRITICAL region and another thread reaches that CRITICAL region and attempts to execute it, it will block until the first thread exits that CRITICAL region.

 - The optional name enables multiple different CRITICAL regions to exist:
  1. Names act as global identifiers. Different CRITICAL regions with the same name are treated as the same region.
  2. All CRITICAL sections which are unnamed, are treated as the same section.

### Restrictions:

 - It is illegal to branch into or out of a CRITICAL block.
 - Fortran only: The names of critical constructs are global entities of the program. If a name conflicts with any other entity, the behavior of the program is unspecified.

## Atomic:

### Purpose:
 - The ATOMIC directive specifies that a specific memory location must be updated atomically, rather than letting multiple threads attempt to write to it. In essence, this directive provides a mini-CRITICAL section.

### Restrictions:
 - The directive applies only to a single, immediately following statement
 - An atomic statement must follow a specific syntax. See the most recent OpenMP specs for this.

## Reduction:

### Purpose:

 - The REDUCTION clause performs a reduction operation on the variables that appear in its list.
 - A private copy for each list variable is created and initialized for each thread. At the end of the reduction, the reduction variable is applied to all private copies of the shared variable, and the final result is written to the global shared variable.

### Restrictions:

 - The type of a list item must be valid for the reduction operator.
 - List items/variables can not be declared shared or private.
 - Reduction operations may not be associative for real numbers.
 - See the OpenMP standard API for additional restrictions.

# Data Scope Attribute Clauses:

## PRIVATE Clause:

### Purpose:
 - The PRIVATE clause declares variables in its list to be private to each thread.

 - PRIVATE variables behave as follows:
  1. A new object of the same type is declared once for each thread in the team.
  2. All references to the original object are replaced with references to the new object.
  3. Variables declared PRIVATE should be assumed to be uninitialized for each thread.

## SHARED Clause

### Purpose:
 - The SHARED clause declares variables in its list to be shared among all threads in the team.

### Notes:
 - A shared variable exists in only one memory location and all threads can read or write to that address.
 - It is the programmer's responsibility to ensure that multiple threads properly access SHARED variables (such as via CRITICAL sections).

## DEFAULT Clause

### Purpose:
The DEFAULT clause allows the user to specify a default scope for all variables in the lexical extent of any parallel region.

### Notes:
 - Specific variables can be exempted from the default using the PRIVATE, SHARED, FIRSTPRIVATE, LASTPRIVATE, and REDUCTION clauses.
 - The C/C++ OpenMP specification does not include private or firstprivate as a possible default. However, actual implementations may provide this option.
 - Using NONE as a default requires that the programmer explicitly scope all variables.

### Restrictions:
 - Only one DEFAULT clause can be specified on a PARALLEL directive.

# References:
- [Lawrence Livermore National Laboratory openMP](https://computing.llnl.gov/tutorials/openMP/#Combined)
- [sysprog 2017 B03: compute-pi](https://hackmd.io/s/HkiJpDPtx#openmp-%E7%89%88%E6%9C%AC)
- [A “Hands-on” Introduction to OpenMP](http://www.openmp.org/wp-content/uploads/omp-hands-on-SC08.pdf)
