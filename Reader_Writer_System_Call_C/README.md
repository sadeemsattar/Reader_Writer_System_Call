# Reader_Writer_System_Call_C
This is the fourth semester Reader Writer System Call Project implemented with AVL Tree using C Language for the Operating System Course.

# Introduction
The reader-writer problem is a famous problem in which there is a content, a file or any shared data between processes. The readers will read the data from the content and the writer will write the content.

The problems occurs when readers and writers both occur at a same time such that the readers won't be able to read all the data/message and the writers will update/manipulate the data/message simultaneously.

Therefore, the objective of the problem is to avoid the race condition and simultaneously occurrences of the readers and writers. 

# Data Structure
The data structure used is a AVL (binary tree) in which messages is stored and accessed by reader and writer for their purposes. 

# Locks
The problem is solved using semaphores. The process thread is created for readers and writers as per the user input. 

Two semaphores used are naming as mutex and write_msg. 

# System Call
A system call is also created naming sys_PrintMessage which take a string parameter and print the data/message/content on the kernel.
