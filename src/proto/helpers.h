/* SPDX-License-Identifier: MIT */
#ifndef WHATSTHAT_PROTO_HELPERS_H
#define WHATSTHAT_PROTO_HELPERS_H

#include <stddef.h>
#include <stdint.h>

#include <protobuf-c/protobuf-c.h>

#include "waCommon/WACommon.pb-c.h"
#include "waE2E/WAWebProtobufsE2E.pb-c.h"
#include "waWa6/WAWebProtobufsWa6.pb-c.h"
#include "waWeb/WAWebProtobufsWeb.pb-c.h"

#ifdef __cplusplus
extern "C" {
#endif

int whatsthat_proto_serialize_message(const ProtobufCMessage *message, uint8_t **out_data,
                                      size_t *out_len);
ProtobufCMessage *whatsthat_proto_deserialize_message(const ProtobufCMessageDescriptor *descriptor,
                                                      const uint8_t *data, size_t len);
void whatsthat_proto_serialized_free(uint8_t *data);

int whatsthat_proto_serialize_handshake_message(const WAWebProtobufsWa6__HandshakeMessage *message,
                                                uint8_t **out_data, size_t *out_len);
WAWebProtobufsWa6__HandshakeMessage *whatsthat_proto_deserialize_handshake_message(
    const uint8_t *data, size_t len);
void whatsthat_proto_free_handshake_message(WAWebProtobufsWa6__HandshakeMessage *message);

int whatsthat_proto_serialize_client_payload(const WAWebProtobufsWa6__ClientPayload *message,
                                             uint8_t **out_data, size_t *out_len);
WAWebProtobufsWa6__ClientPayload *whatsthat_proto_deserialize_client_payload(const uint8_t *data,
                                                                              size_t len);
void whatsthat_proto_free_client_payload(WAWebProtobufsWa6__ClientPayload *message);

int whatsthat_proto_serialize_message_key(const WACommon__MessageKey *message, uint8_t **out_data,
                                          size_t *out_len);
WACommon__MessageKey *whatsthat_proto_deserialize_message_key(const uint8_t *data, size_t len);
void whatsthat_proto_free_message_key(WACommon__MessageKey *message);

int whatsthat_proto_serialize_e2e_message(const WAWebProtobufsE2E__Message *message,
                                          uint8_t **out_data, size_t *out_len);
WAWebProtobufsE2E__Message *whatsthat_proto_deserialize_e2e_message(const uint8_t *data,
                                                                     size_t len);
void whatsthat_proto_free_e2e_message(WAWebProtobufsE2E__Message *message);

int whatsthat_proto_serialize_web_message_info(const WAWebProtobufsWeb__WebMessageInfo *message,
                                               uint8_t **out_data, size_t *out_len);
WAWebProtobufsWeb__WebMessageInfo *whatsthat_proto_deserialize_web_message_info(const uint8_t *data,
                                                                                 size_t len);
void whatsthat_proto_free_web_message_info(WAWebProtobufsWeb__WebMessageInfo *message);

#ifdef __cplusplus
}
#endif

#endif /* WHATSTHAT_PROTO_HELPERS_H */
