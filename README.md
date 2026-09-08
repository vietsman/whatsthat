# whatsthat

C99 port of `tulir/whatsmeow` (WhatsApp Web multidevice API client library).

## Status

This repository currently contains **Milestone 1** protobuf conversion:
- Upstream whatsmeow proto inventory vendored under `third_party/protos/proto/`
- Protobuf-C generated bindings committed under `src/proto/generated/`
- Core proto helper wrappers in `src/proto/helpers.c` / `src/proto/helpers.h`
- Round-trip serialization unit tests in `tests/proto_roundtrip.c`

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

## Protobuf generation and verification

- Minimum supported versions:
  - `protoc >= 3.0.0`
  - `protobuf-c >= 1.3.0` (plugin binary: `protoc-c`)
- Pull/update upstream proto files:

```bash
./tools/proto_collect.sh main
```

- Regenerate protobuf-c bindings from vendored sources:

```bash
cmake -S . -B build -DWHATSTHAT_REGENERATE_PROTO=ON
cmake --build build --target whatsthat_proto_generate
```

- Verify generated files are committed and up-to-date:

```bash
git diff -- third_party/protos src/proto/generated
```
