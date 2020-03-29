#include "buffer.h"
#include <fcntl.h>
#include <unistd.h>

#include "file_data_source.h"

#include <catch2/catch.hpp>

void checked_write(int fd, const void *buffer, size_t size)
{
    int rc = write(fd, buffer, size);
    REQUIRE(size >= 0);
    REQUIRE(static_cast<size_t>(rc) == size);
}

TEST_CASE("InputStreamBuffer")
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
    REQUIRE(input.GetOffset() == 0);
    REQUIRE(memcmp(input.GetPointer(), "ABCD", 4) == 0);
    REQUIRE(memcmp(input.GetPointer(), "ABCD", 4) == 0);

    input.Advance(2);
    REQUIRE(input.GetOffset() == 2);
    input.EnsureAvailable(6);
    REQUIRE(memcmp(input.GetPointer(), "CDEFGH", 6) == 0);

    input.Advance(20);
    REQUIRE(input.GetOffset() == 22);
    input.EnsureAvailable(4);
    REQUIRE(memcmp(input.GetPointer(), "WXYZ", 4) == 0);

    input.Advance(52);
    REQUIRE(input.GetOffset() == 74);
    input.EnsureAvailable(4);
    REQUIRE(memcmp(input.GetPointer(), "WXYZ", 4) == 0);

    input.Advance(29);
    REQUIRE_THROWS_AS(input.EnsureAvailable(2), InsufficientDataException);

    input.EnsureAvailable(1);
    REQUIRE(memcmp(input.GetPointer(), "Z", 1) == 0);
    
    input.Advance(1);
    REQUIRE_THROWS_AS(input.EnsureAvailable(1), InsufficientDataException);
}
