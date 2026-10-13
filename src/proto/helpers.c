/* SPDX-License-Identifier: MIT */
#include "proto/helpers.h"

#include <stdlib.h>

int whatsthat_proto_serialize_message(const ProtobufCMessage *message, uint8_t **out_data,
                                      size_t *out_len) {
    size_t packed_size;
    uint8_t *buffer;

    if (!message || !out_data || !out_len) {
        return -1;
    }

    packed_size = protobuf_c_message_get_packed_size(message);
    buffer = (uint8_t *)malloc(packed_size == 0 ? 1u : packed_size);
    if (!buffer) {
        return -1;
    }

    if (packed_size > 0) {
        protobuf_c_message_pack(message, buffer);
    }

    *out_data = buffer;
    *out_len = packed_size;

    return 0;
}

ProtobufCMessage *whatsthat_proto_deserialize_message(const ProtobufCMessageDescriptor *descriptor,
                                                      const uint8_t *data, size_t len) {
    if (!descriptor || (!data && len > 0)) {
        return NULL;
    }

    return protobuf_c_message_unpack(descriptor, NULL, len, data);
}

void whatsthat_proto_serialized_free(uint8_t *data) {
    free(data);
}

int whatsthat_proto_serialize_handshake_message(const WAWebProtobufsWa6__HandshakeMessage *message,
                                                uint8_t **out_data, size_t *out_len) {
    return whatsthat_proto_serialize_message((const ProtobufCMessage *)message, out_data, out_len);
}

WAWebProtobufsWa6__HandshakeMessage *whatsthat_proto_deserialize_handshake_message(
    const uint8_t *data, size_t len) {
    return (WAWebProtobufsWa6__HandshakeMessage *)whatsthat_proto_deserialize_message(
        &waweb_protobufs_wa6__handshake_message__descriptor, data, len);
}

void whatsthat_proto_free_handshake_message(WAWebProtobufsWa6__HandshakeMessage *message) {
    protobuf_c_message_free_unpacked((ProtobufCMessage *)message, NULL);
}

int whatsthat_proto_serialize_client_payload(const WAWebProtobufsWa6__ClientPayload *message,
                                             uint8_t **out_data, size_t *out_len) {
    return whatsthat_proto_serialize_message((const ProtobufCMessage *)message, out_data, out_len);
}

WAWebProtobufsWa6__ClientPayload *whatsthat_proto_deserialize_client_payload(const uint8_t *data,
                                                                              size_t len) {
    return (WAWebProtobufsWa6__ClientPayload *)whatsthat_proto_deserialize_message(
        &waweb_protobufs_wa6__client_payload__descriptor, data, len);
}

void whatsthat_proto_free_client_payload(WAWebProtobufsWa6__ClientPayload *message) {
    protobuf_c_message_free_unpacked((ProtobufCMessage *)message, NULL);
}

int whatsthat_proto_serialize_message_key(const WACommon__MessageKey *message, uint8_t **out_data,
                                          size_t *out_len) {
    return whatsthat_proto_serialize_message((const ProtobufCMessage *)message, out_data, out_len);
}

WACommon__MessageKey *whatsthat_proto_deserialize_message_key(const uint8_t *data, size_t len) {
    return (WACommon__MessageKey *)whatsthat_proto_deserialize_message(
        &wacommon__message_key__descriptor, data, len);
}

void whatsthat_proto_free_message_key(WACommon__MessageKey *message) {
    protobuf_c_message_free_unpacked((ProtobufCMessage *)message, NULL);
}

int whatsthat_proto_serialize_e2e_message(const WAWebProtobufsE2E__Message *message,
                                          uint8_t **out_data, size_t *out_len) {
    return whatsthat_proto_serialize_message((const ProtobufCMessage *)message, out_data, out_len);
}

WAWebProtobufsE2E__Message *whatsthat_proto_deserialize_e2e_message(const uint8_t *data,
                                                                     size_t len) {
    return (WAWebProtobufsE2E__Message *)whatsthat_proto_deserialize_message(
        &waweb_protobufs_e2_e__message__descriptor, data, len);
}

void whatsthat_proto_free_e2e_message(WAWebProtobufsE2E__Message *message) {
    protobuf_c_message_free_unpacked((ProtobufCMessage *)message, NULL);
}

int whatsthat_proto_serialize_web_message_info(const WAWebProtobufsWeb__WebMessageInfo *message,
                                               uint8_t **out_data, size_t *out_len) {
    return whatsthat_proto_serialize_message((const ProtobufCMessage *)message, out_data, out_len);
}

WAWebProtobufsWeb__WebMessageInfo *whatsthat_proto_deserialize_web_message_info(const uint8_t *data,
                                                                                 size_t len) {
    return (WAWebProtobufsWeb__WebMessageInfo *)whatsthat_proto_deserialize_message(
        &waweb_protobufs_web__web_message_info__descriptor, data, len);
}

void whatsthat_proto_free_web_message_info(WAWebProtobufsWeb__WebMessageInfo *message) {
    protobuf_c_message_free_unpacked((ProtobufCMessage *)message, NULL);
}
