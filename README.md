# whatsthat

C99 port of `tulir/whatsmeow` (WhatsApp Web multidevice API client library).

## Status

This repository currently contains **Milestone 0** scaffolding:
- C99 project layout and initial public header under `include/whatsthat/`
- Initial module directories under `src/`
- CMake build with pkg-config dependency detection stubs
- Example CLI scaffold (`whatsthat_cli`)
- GitHub Actions CI compile/test skeleton
- Upstream analysis and package/proto mapping in `docs/port-plan.md`

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Planned dependencies

- libsodium
- protobuf-c
- libwebsockets
- libcurl
- cJSON
- SQLite3
- zlib

Dependency checks are wired through `pkg-config` in CMake and are currently optional during scaffolding.
