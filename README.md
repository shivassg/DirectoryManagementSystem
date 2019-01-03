# DirectoryManagementSystem

Course: Object Oriented Programming with Design Pattern Language: C++ Semester: Fall 2018

Description of the project:

This project is about designing and implementing a DMS that can create a directory, run queries on it and simulate “load balancer” for web servers in an object-oriented manner. The system first processes raw data from multiple sources to generate the directory. The raw data consists of contact information such as name, phone numbers, addresses, web information etc. Also, the contacts can be ‘People’ contacts or ‘Business’ contacts. The raw data comes from files. Once the directory is successfully created, multiple concurrent queries can be run. The Load balancers ensure low latency of the system.

Directory Management System:

A Directory usually consists of a collection of contacts where each ‘Contact’ is information about someone.

In this project, the directory can contain ‘People Contacts’ as well as ‘Business Contacts’

The system processes raw data from multiple sources to generate the directory. In particular, the DMS will read the files, create appropriate ‘Contact’ objects and populate the directory.

Query and Results: Once the directory is created, queries can be made. For example, a search query could be to find the number of ‘John’ in the directory ordered by State. The result of such a query can be TN – 4, CT – 6, NY – 9 etc.

Multiple Concurrent Queries and Load Balancer: In this part, we’ll scale the design to accommodate multiple concurrent queries. Because multiple queries can potentially cause a high increase in wait time, load balancers are used. Load balancers ensure low latency of the system by adding a new web server in case of accumulation of too many queries.

Event-driven Algorithm: Now, one would potentially need to check the availability of the web server to service the next query. A more efficient way would be for each web server to ‘communicate’ when it becomes available. Hence, we’ll use an event driven algorithm where the processing time of the query that is allotted to the server is used as the delay as to when the server would be available.
