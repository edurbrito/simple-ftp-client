# Simple FTP Client

The implementation of this application had in counts the norms **RFC 959** (reading and analysis of server responses) and **RFC 1738** (utilization and treatment of URL addresses). 

It follows a functional organization structure and is executed sequentially, automating the communication process with the server, as a mechanized and error-resilient script.

## RUN

To download a file from a FTP server, inside the `src` folder run:

1. `make` to compile the source code
2. `./download ftp://[username:password@]ftp.server/path/filename`, with the username and password being optional parameters

## LOGS

There are some [Wireshark](https://www.wireshark.org/) logs inside the [`logs`](logs) folder. These were collected inside a lab network, to test and observe the multiple network protocols that are commonly used in modern times.

## REPORT

A full report of the experiments conducted can be found [here](report/T1G11.pdf).

## AUTHORS

* Eduardo Brito, [@edurbrito](https://github.com/edurbrito)
* Pedro Ferreira, [@pdff2000](https://github.com/pdff2000)