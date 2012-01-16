#include "buffer.h"
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "file_data_source.h"

void checked_write(int fd, const void *buffer, size_t size)
{
    int rc = write(fd, buffer, size);
    assert(rc == size);
}

int main()
{
    // Generate test file
    int fd = open("testfile1", O_WRONLY|O_CREAT|O_TRUNC, 0777);

    char buffer1[26];
    for(unsigned int i = 0; i < 26; ++i)
	buffer1[i] = 'A' + i;

    checked_write(fd, buffer1, 26);
    checked_write(fd, buffer1, 26);
    checked_write(fd, buffer1, 26);
    checked_write(fd, buffer1, 26);
    close(fd);

    FileDataSource data_source;
    data_source.Open("testfile1");

    InputStreamBuffer input(10, 3);
    input.SetSource(&data_source);

    input.EnsureAvailable(4);
    assert(input.GetOffset() == 0);
    assert(memcmp(input.GetPointer(), "ABCD", 4) == 0);
    assert(memcmp(input.GetPointer(), "ABCD", 4) == 0);

    input.Advance(2);
    assert(input.GetOffset() == 2);
    input.EnsureAvailable(6);
    assert(memcmp(input.GetPointer(), "CDEFGH", 6) == 0);

    input.Advance(20);
    assert(input.GetOffset() == 22);
    input.EnsureAvailable(4);
    assert(memcmp(input.GetPointer(), "WXYZ", 4) == 0);

    input.Advance(52);
    assert(input.GetOffset() == 74);
    input.EnsureAvailable(4);
    assert(memcmp(input.GetPointer(), "WXYZ", 4) == 0);

    input.Advance(29);
    try
    {
	input.EnsureAvailable(2);
	assert(false);
    }
    catch (InsufficientDataException &e)
    {
    }

    input.EnsureAvailable(1);
    assert(memcmp(input.GetPointer(), "Z", 1) == 0);
    
    input.Advance(1);
    try
    {
	input.EnsureAvailable(1);
	assert(false);
    }
    catch (InsufficientDataException &e)
    {
    }
    
    return 0;
}
