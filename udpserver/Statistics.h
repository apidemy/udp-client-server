#ifndef NETWORKLIB_STATISTICS
#define NETWORKLIB_STATISTICS
#include <atomic>
#include <fstream>

namespace NetworkLib {
struct Statistics {
private:
    std::atomic_uint32_t receivedMessages;
    std::atomic_uint32_t sentMessages;

    std::atomic_uint64_t receivedBytes;
    std::atomic_uint64_t sentBytes;
public:
    Statistics();
    Statistics(const Statistics& other);

    uint32_t GetReceivedMessages() const;

    uint64_t GetReceivedBytes() const;

    uint32_t GetSentMessages() const;

    uint64_t GetSentBytes() const;

    void RegisterSentMessage(int32_t size);

    void RegisterReceivedMessage(int32_t size);

};

std::ostream& operator<<(std::ostream& os, const Statistics& obj);
}
#endif // NETWORKLIB_STATISTICS