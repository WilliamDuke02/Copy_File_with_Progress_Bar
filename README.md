# Copy_File_with_Progress_Bar
Copies a file one KB at a time and displays a progress bar 
gcc Copier.c -o Copier    //compiles Copier
dd if=/dev/urandom of=test.bin bs=1M count=250   //makes a binary 250mb file with random bits
./Copier test.bin test_copy.bin //runs Copier and names the copy file named test_copy.bin
