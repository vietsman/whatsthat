# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added
- Milestone 0 scaffolding for C99 project layout, CMake build, CI skeleton, and initial public header.
- Initial `docs/port-plan.md` mapping upstream whatsmeow packages/protos to target C modules.
- Milestone 1 protobuf assets: vendored upstream `.proto` schemas in `third_party/protos/` and committed protobuf-c bindings in `src/proto/generated/`.
- Core proto helper wrappers in `src/proto/helpers.c` and `src/proto/helpers.h` for connect/auth and message serialization paths.
- `tests/proto_roundtrip.c` round-trip coverage for handshake, auth payload, message key, E2E message, and Web message info message types.
