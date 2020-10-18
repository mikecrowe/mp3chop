#include "buffer.h"
#include <fcntl.h>
#include <unistd.h>

#include <catch2/catch.hpp>

class StringSink : public DataSink
{
    std::string m_result;

public:
    virtual int WriteOut(const uint8_t *buffer, size_t bytes_available) override
    {
        m_result.append(reinterpret_cast<const char *>(buffer), bytes_available);
        return static_cast<int>(bytes_available);
    }

    const std::string &GetResult() const
    {
        return m_result;
    }
};

TEST_CASE("OutputStreamBuffer")
{
    StringSink sink;
    OutputStreamBuffer osb;
    osb.SetSink(&sink);

    REQUIRE(osb.GetOffset() == 0);

    osb.Append(reinterpret_cast<const uint8_t *>("ABCD"), 4);
    REQUIRE(sink.GetResult().empty());
    osb.Flush();
    REQUIRE(sink.GetResult() == "ABCD");

    osb.Append(reinterpret_cast<const uint8_t *>("EFGH"), 4);
    REQUIRE(sink.GetResult() == "ABCD");

    SECTION("Bookmark")
    {
        osb.SetBookmark();
        REQUIRE(sink.GetResult() == "ABCDEFGH");
        osb.Append(reinterpret_cast<const uint8_t *>("IJKL"), 4);
        REQUIRE(sink.GetResult() == "ABCDEFGH");

        // Flush with bookmark active should have no effect
        osb.Flush();
        REQUIRE(sink.GetResult() == "ABCDEFGH");

        SECTION("ClearBookmark")
        {
            osb.ClearBookmark();
            REQUIRE(sink.GetResult() == "ABCDEFGHIJKL");
            osb.Append(reinterpret_cast<const uint8_t *>("MNOP"), 4);
            REQUIRE(sink.GetResult() == "ABCDEFGHIJKLMNOP");
        }

        SECTION("GoToBookmark")
        {
            osb.GoToBookmark();
            osb.Append(reinterpret_cast<const uint8_t *>("123"), 3);
            REQUIRE(sink.GetResult() == "ABCDEFGH123");
        }
    }

    SECTION("Large")
    {
        for(auto i = 0; i < 1000; ++i)
            osb.Append(reinterpret_cast<const uint8_t *>("0123456789"), 10);
        REQUIRE(sink.GetResult().size() == 8198);
        osb.Flush();
        REQUIRE(sink.GetResult().size() == 10008);
    }
}
