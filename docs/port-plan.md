# whatsthat port plan (Milestone 0)

This document maps upstream `tulir/whatsmeow` Go modules to planned C99 modules in `whatsthat`.

## Upstream package inventory (initial)

### Top-level responsibilities
- Core client/session/auth/message flow: root Go files (`client.go`, `handshake.go`, `send.go`, `message.go`, `upload.go`, `download.go`, etc.)
- App state sync and LTHash: `appstate/`
- Binary node encoding/decoding and tokenization: `binary/`
- Noise socket + frame transport: `socket/`
- Persistence + SQL store: `store/`, `store/sqlstore/`
- Core types and events: `types/`, `types/events/`
- Utility crypto helpers/logging/keys/HKDF: `util/`

### Enumerated proto files

The following `.proto` files were identified in upstream `proto/` and are now vendored at `third_party/protos/` with matching relative paths.

- proto/waAdv/WAAdv.proto
- proto/waCert/WACert.proto
- proto/waCommon/WACommon.proto
- proto/waMmsRetry/WAMmsRetry.proto
- proto/waWinUIApi/WAWinUIApi.proto
- proto/waWeb/WAWebProtobufsWeb.proto
- proto/waAea/WAWebProtobufsAea.proto
- proto/waWa6/WAWebProtobufsWa6.proto
- proto/waE2E/WAWebProtobufsE2E.proto
- proto/waFingerprint/WAFingerprint.proto
- proto/waMultiDevice/WAMultiDevice.proto
- proto/waBotMetadata/WABotMetadata.proto
- proto/waMsgTransport/WAMsgTransport.proto
- proto/waArmadilloXMA/WAArmadilloXMA.proto
- proto/waCompanionReg/WACompanionReg.proto
- proto/waArmadilloICDC/WAArmadilloICDC.proto
- proto/waMediaTransport/WAMediaTransport.proto
- proto/waE2EGuest/WAWebProtobufsE2EGuest.proto
- proto/waMediaEntryData/WAMediaEntryData.proto
- proto/waAICommon/WAWebProtobufsAICommon.proto
- proto/waMsgApplication/WAMsgApplication.proto
- proto/waVnameCert/WAWebProtobufsVnameCert.proto
- proto/waReporting/WAWebProtobufsReporting.proto
- proto/waEphemeral/WAWebProtobufsEphemeral.proto
- proto/waSyncAction/WAWebProtobufSyncAction.proto
- proto/waServerSync/WAWebProtobufsServerSync.proto
- proto/waHistorySync/WAWebProtobufsHistorySync.proto
- proto/waRoutingInfo/WAWebProtobufsRoutingInfo.proto
- proto/waStatusAttributions/WAStatusAttributions.proto
- proto/waGroupHistory/WAWebProtobufsGroupHistory.proto
- proto/waAICommonDeprecated/WAAICommonDeprecated.proto
- proto/waWebLabyrinthWaWasm/WAWebLabyrinthWaWasm.proto
- proto/waUserPassword/WAWebProtobufsUserPassword.proto
- proto/waConsumerApplication/WAConsumerApplication.proto
- proto/waCommonParameterised/WACommonParameterised.proto
- proto/instamadilloAddMessage/InstamadilloAddMessage.proto
- proto/waArmadilloApplication/WAArmadilloApplication.proto
- proto/waArmadilloBackupCommon/WAArmadilloBackupCommon.proto
- proto/waArmadilloBackupMessage/WAArmadilloBackupMessage.proto
- proto/waChatLockSettings/WAWebProtobufsChatLockSettings.proto
- proto/instamadilloCoreTypeText/InstamadilloCoreTypeText.proto
- proto/instamadilloCoreTypeLink/InstamadilloCoreTypeLink.proto
- proto/waArmadilloTransportEvent/WAArmadilloTransportEvent.proto
- proto/instamadilloXmaContentRef/InstamadilloXmaContentRef.proto
- proto/instamadilloDeleteMessage/InstamadilloDeleteMessage.proto
- proto/instamadilloCoreTypeMedia/InstamadilloCoreTypeMedia.proto
- proto/waDeviceCapabilities/WAWebProtobufsDeviceCapabilities.proto
- proto/instamadilloTransportPayload/InstamadilloTransportPayload.proto
- proto/instamadilloSupplementMessage/InstamadilloSupplementMessage.proto
- proto/waSyncdSnapshotRecovery/WAWebProtobufsSyncdSnapshotRecovery.proto
- proto/instamadilloCoreTypeActionLog/InstamadilloCoreTypeActionLog.proto
- proto/waQuickPromotionSurfaces/WAWebProtobufsQuickPromotionSurfaces.proto
- proto/instamadilloCoreTypeCollection/InstamadilloCoreTypeCollection.proto
- proto/waLidMigrationSyncPayload/WAWebProtobufLidMigrationSyncPayload.proto
- proto/instamadilloCoreTypeAdminMessage/InstamadilloCoreTypeAdminMessage.proto
- proto/waConsumerApplicationParameterised/WAConsumerApplicationParameterised.proto
- proto/waArmadilloMiTransportAdminMessage/WAArmadilloMiTransportAdminMessage.proto

Proto origin mapping and collection command are tracked in `tools/proto_collect.sh`.

## Protobuf-C generation notes (Milestone 1)

- Minimum supported versions:
  - `protoc >= 3.0.0`
  - `protobuf-c >= 1.3.0` (`protoc-c`)
- Generated bindings are committed in `src/proto/generated/` for deterministic CI builds.
- Build-time regeneration is optional:

```bash
cmake -S . -B build -DWHATSTHAT_REGENERATE_PROTO=ON
cmake --build build --target whatsthat_proto_generate
```

- Verification command:

```bash
git diff -- third_party/protos src/proto/generated
```

## C module mapping plan

| Upstream Go area | Planned C location | Milestone |
|---|---|---|
| root client/session/auth flow | `src/api/` + `include/whatsthat/` | 2-4 |
| noise/frame socket (`socket/`) | `src/ws/` + `src/net/` | 3 |
| media upload/download | `src/http/` | 3,5 |
| protobuf schemas (`proto/`) | `src/proto/` + `third_party/protos/` | 1 |
| crypto utils (`util/*`, noise pieces) | `src/crypto/` | 2 |
| SQL store (`store/sqlstore`) | `src/store/` | 5 |
| appstate/lthash | `src/api/` + `src/store/` | 4,5 |
| types/events | `include/whatsthat/` DTO headers + `src/api/` handlers | 4 |
| binary node codec (`binary/`) | `src/util/` + `src/proto/` helpers | 4 |

## Milestone breakdown

1. **M0 (current PR):** scaffolding, CI compile checks, and this plan.
2. **M1:** bring in `third_party/protos/` + generated `protobuf-c` code and round-trip serialization tests.
3. **M2:** libsodium wrappers for X25519, HKDF, AEAD, HMAC; session key derivation and persistence format tests.
4. **M3:** websocket transport (libwebsockets), frame pump with pthreads, and HTTP media helpers (libcurl).
5. **M4:** high-level client API (`connect/login/send/on_message/disconnect`) + core message handling and ACK flow.
6. **M5:** SQLite persistence, media chunking/resume, and multi-device sync pathways.
7. **M6:** hardening, expanded tests/fuzzing/docs, packaging/install validation.

## Notes

- Crypto/TLS primitives will use existing libraries only (`libsodium`, system TLS in `libwebsockets`/`libcurl`).
- Generated protobuf C files will be committed for deterministic CI, with optional regeneration support when `protoc` is present.
- Design deviations from Go ergonomics will be documented in `docs/` as the API stabilizes.
