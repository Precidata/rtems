untar
=====

Author: Jake Janovetz 7.6.1999
untar.c contains two procedures for extracting files from a UNIX
tar file:

    int Untar_FromMemory(unsigned char *tar_buf, unsigned long size);
    int Untar_FromFile(char *tar_name);

Untar_FromMemory(...) takes its input from a chunk of allocated memory.
This is particularly useful when the tar is stored in Flash memory or
comes from the FTP daemon by way of a hook.

Untar_FromFile(...) is identical except the source is from an existing
file.  The fully qualified filename is passed through char *tar_name.



BUGS: Please email janovetz@uiuc.edu
