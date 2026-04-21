#include <windows.h>
#include <evntrace.h>
#include <tdh.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "tdh.lib")

// Your provider GUID (must match producer!)
static const GUID MyProviderGuid =
{ 0xd58c126f, 0x7c9e, 0x4a5f,{ 0xa3, 0x1a, 0x3f, 0x9d, 0x55, 0x11, 0x22, 0x33 } };

// Callback function
VOID WINAPI EventRecordCallback(PEVENT_RECORD pEvent)
{
    std::cout << "Event received!" << std::endl;

    // Basic info
    std::cout << "Provider GUID: "
        << pEvent->EventHeader.ProviderId.Data1
        << std::endl;

    std::cout << "Process ID: "
        << pEvent->EventHeader.ProcessId
        << std::endl;

    std::cout << "Thread ID: "
        << pEvent->EventHeader.ThreadId
        << std::endl;

    std::cout << "------------------------" << std::endl;
}

int main()
{
    const wchar_t* sessionName = L"MyRealtimeSession";

    // Step 1: Create session properties
    EVENT_TRACE_PROPERTIES* props =
        (EVENT_TRACE_PROPERTIES*)calloc(1, sizeof(EVENT_TRACE_PROPERTIES) + 1024);

    props->Wnode.BufferSize = sizeof(EVENT_TRACE_PROPERTIES) + 1024;
    props->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    props->LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
    props->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);

    // Copy session name
    wcscpy_s((wchar_t*)((char*)props + props->LoggerNameOffset), 512, sessionName);

    TRACEHANDLE sessionHandle = 0;

    // Step 2: Start session
    ULONG status = StartTrace(&sessionHandle, sessionName, props);
    if (status != ERROR_SUCCESS) {
        std::cout << "StartTrace failed: " << status << std::endl;
        return 1;
    }

    // Step 3: Enable provider
    status = EnableTraceEx2(
        sessionHandle,
        &MyProviderGuid,
        EVENT_CONTROL_CODE_ENABLE_PROVIDER,
        TRACE_LEVEL_VERBOSE,
        0, 0, 0,
        nullptr
    );

    if (status != ERROR_SUCCESS) {
        std::cout << "EnableTraceEx2 failed: " << status << std::endl;
        return 1;
    }

    // Step 4: Open trace for consumption
    EVENT_TRACE_LOGFILE trace = {};
    trace.LoggerName = (LPWSTR)sessionName;
    trace.ProcessTraceMode = PROCESS_TRACE_MODE_REAL_TIME |
        PROCESS_TRACE_MODE_EVENT_RECORD;
    trace.EventRecordCallback = EventRecordCallback;

    TRACEHANDLE traceHandle = OpenTrace(&trace);

    if (traceHandle == INVALID_PROCESSTRACE_HANDLE) {
        std::cout << "OpenTrace failed" << std::endl;
        return 1;
    }

    std::cout << "Listening for events..." << std::endl;

    // Step 5: Process events (in a separate thread since it is ablocking call)
    std::thread traceLoopThread([&]() {
        ProcessTrace(&traceHandle, 1, nullptr, nullptr);
		});

	std::cout << "Press Enter to stop..." << std::endl;
	std::cin.get();

    // Cleanup
    CloseTrace(traceHandle);
    ControlTrace(sessionHandle, sessionName, props, EVENT_TRACE_CONTROL_STOP);
    traceLoopThread.join();

    return 0;
}