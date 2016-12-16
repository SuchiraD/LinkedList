There are three ".c" source files. Each file implement three lists separately.
We assumed that N and M are not changing(since the assignment does has fixed values for N and M).
Hence the user only have to input "mMember, mInsert, mDelete" respectively(separated by space or new line).

"linkedList_serial.c"
----------------------
gcc linkedList_serial.c -o linkedList_serial.out
./linkedList_serial.out


"linkedList_mutex.c"
----------------------
gcc linkedList_mutex.c -o linkedList_mutex.out -lpthread -O3
./linkedList_mutex.out


"linkedList_rwLock.c"
----------------------
gcc linkedList_rwLock.c -o linkedList_rwLock.out -lpthread -O3
./linkedList_rwLock.out
