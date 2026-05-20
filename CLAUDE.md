# EFramework (EMWare) — Claude Code Instructions

## Project Overview

EFramework is a layered, event-driven, component-based embedded C++ framework.
All code must follow EMWare conventions strictly.

- **Language**: C++17
- **Build**: CMake 3.16+, Debug/Release separated
  - Debug: `build/Debug/eframework` (`-O0 -g`)
  - Release: `build/Release/eframework` (`-O3`)
- **Dependencies**: OpenCV, pthread
- **Entry point**: `src/Main.cpp`
- **CMakeLists.txt location**: `src/CMakeLists.txt`
- **Environment**: WSL2 (Linux, x64)

## Build

```bash
# Debug
cmake -S src -B build/Debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Debug -j

# Release
cmake -S src -B build/Release -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release -j
```

Executable: `build/Debug/eframework` or `build/Release/eframework`

Include path root is `src/`, so headers use `<LayerFolder/SubDir/File.h>` syntax
(e.g., `#include <02Platform/Component/Component.h>`).

## VSCode Debug

Uses gdb-wrapper pattern to prevent `debuginfod` from breaking GDB source mapping.

**Files**:
- `.vscode/launch.json` — Debug / Release configurations
- `.vscode/tasks.json` — CMake Configure → Build → Run tasks
- `.vscode/settings.json` — cmake source/build directory settings
- `.vscode/gdb-wrapper.sh` — forces `DEBUGINFOD_URLS=` off before GDB launch
- `.gdbinit` — disables debuginfod, auto-solib-add, thread-events globally

Debug: **Run and Debug → "Debug EFramework"**
Release: **Run and Debug → "Release EFramework"**

## Layer Architecture

Folders are numbered to express dependency order. Lower number = lower layer.
A layer may only depend on layers with strictly lower numbers.

| Folder | Layer | Description |
|--------|-------|-------------|
| `01Base` | Base | RootObject, BaseObject, ValueObject, IMemory, StdLib, Aspect |
| `02Platform` | Platform | Component, ComponentPart, UId, EventQueue, Scheduler |
| `03Technical` | Technical | MemoryManager, Timer |
| `09Application` | Application | AppMain, LifecycleManager |
| `12PPlatform` | PPlatform | Platform-specific Platform implementations |
| `13PTechnical` | PTechnical | PThread, PTimer, PMemoryManager, PRemote |
| `19PApplication` | PApplication | Platform-specific Application implementations |
| `21Domain` | Domain | Domain logic (VideoManager, etc.) |

`P` prefix (12, 13, 19, 31…) = platform-specific concrete implementations.

## Naming Conventions

### Member variables

| Prefix | Type |
|--------|------|
| `m_` | member variable |
| `m_p` | member pointer |
| `m_u` | member unsigned |
| `m_n` | member int |
| `m_pc` | member `char*` |
| `m_s` | member String |
| `m_v` | member Vector |
| `m_pm` | member pointer to Map |
| `s_` | static member |

### Classes and types

| Prefix | Meaning |
|--------|---------|
| `I` | Interface class (abstract, no data) |
| `E` | Enum class |
| `P` (class prefix) | Platform-specific implementation |
| `e` | Enum value |

### Files

- `typedef.h` at each layer root defines `_ELayer_<Name>` enum and layer macros.
- Interface: `IClassName.h`
- Implementation: `ClassName.h` / `ClassName.cpp`

## Class Structure Pattern

Every non-trivial class uses this interface layout:

```cpp
class IFoo {
public:
    enum class EState      { eBegin = Foo_Id, ..., eEnd };
    enum class EException  { eBegin = Foo_Id, ..., eEnd };
    enum class EEventType  { eBegin = Foo_Id, ..., eEnd };
    enum class EParts      { eBegin = Foo_Id, ..., eEnd };
    enum class EReceivers  { eBegin = Foo_Id, ..., eEnd };
    enum class ETargetGroups { eBegin = Foo_Id, ..., eEnd };
    enum class EAttributes { eBegin = Foo_Id, ..., eEnd };

    class ParamFooEvent : public ValueObject { ... };
};
```

## Class ID and Registration

```cpp
#define Foo_Id   _GET_CLASS_UID(_ELayer_Platform::_eFoo)
#define Foo_Name "Foo"
```

Register in `RegisterEventTypes()` → `Directory::s_dirEvents`
Register in `RegisterExceptions()` → `Directory::s_dirExceptions`

## Memory Allocation

```cpp
// Correct
SomeObject* p = new("SomeObject") SomeObject(...);

// Wrong — never use plain new
SomeObject* p = new SomeObject(...);
```

## Event-Driven Communication

- Components communicate exclusively via `Event` through `EventQueue`.
- Direct calls only within the same component (Component ↔ its Parts).
- Each event has a `ParamXxx : public ValueObject` argument class.
- `ProcessAEvent(Event* pEvent)` dispatches via `switch(pEvent->GetType())`.

## Debug Macros

```cpp
LOG_HEADER(object, func, ...)   // enter scope
LOG_FOOTER(object, func, ...)   // exit scope
LOG_NEWLINE(object, func, ...)  // newline log
LOG(object, func, ...)          // inline log
MLOG_*(...)                     // memory logs
```

Never use `std::cout` or raw `printf`. Controlled by `_DEBUG_L` / `_DEBUG_M` in `src/typedef.h`.

## Key Rules

1. Never use `std::` containers — use the framework's `Vector`, `Map`, `Queue`, `String`.
2. Never use `std::cout` — use LOG macros.
3. Never use plain `new` — use `new("label") ClassName(...)`.
4. New layers must follow the numbering scheme.
5. Platform-agnostic layers (01–09) must contain zero platform-specific code.
6. Each new class must add its enum entry to that layer's `typedef.h`.
