# YOUR NAME
Daniel Dombrovsky 

# STUDENT NUMBER
1211846

# RESOURCES USED
I used the CIS 2520 slides on courselink, the CIS 2520 course .fasta files for testing, and the CIS 2520 course functions such as LLvNode.c for the assignment.

# IMPLEMENTATION STATE
The current implementation state is able to process the /home/courses/cis2520/data/uniprot_sprot-100000.fasta and /home/courses/cis2520/data/uniprot_sprot-20000.fasta files on the Linux server. No memory leaks or errors appear and 'make' does not indicate issues.

# SUMMARY
While working on this assignment, I learned how to optimize linked list operations. For example, by using a tail pointer in the linked list, the program can go directly to the end of a list and does not need to travel through the entire linked list. Executing ./llheadonly took 4.555 seconds for me (with the 20000 file), while the ./llheadtail only took 0.087 seconds for the same file. Executing ./arraydouble only took 0.078 seconds (with the 20000 file) which shows that arrays have some reasons (ex. speed) to be used over linked lists despite the benefits linked lists have.