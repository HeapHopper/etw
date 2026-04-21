#include <windows.h>
#include <TraceLoggingProvider.h>

TRACELOGGING_DEFINE_PROVIDER(
    g_hProvider,
    "MyCompany.MyFirstETWProvider",
    // Random GUID (generate your own in production)
    (0xd58c126f, 0x7c9e, 0x4a5f, 0xa3, 0x1a, 0x3f, 0x9d, 0x55, 0x11, 0x22, 0x33)
);

int main() {
    // Register provider
    TraceLoggingRegister(g_hProvider);

    // Emit a simple event
    TraceLoggingWrite(
        g_hProvider,
        "MyFirstEvent",
        TraceLoggingValue(42, "Answer"),
        TraceLoggingValue("Hello ETW!", "Message")
    );

    // Cleanup
    TraceLoggingUnregister(g_hProvider);

    return 0;
}