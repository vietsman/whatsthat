/* SPDX-License-Identifier: MIT */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "proto/helpers.h"

#define CHECK_TRUE(expr)             \
    do {                             \
        if (!(expr)) {               \
            return #expr;            \
        }                            \
    } while (0)

static const char *test_serialize_rejects_null_args(void) {
    CHECK_TRUE(whatsthat_proto_serialize_message(NULL, NULL, NULL) == -1);
    return NULL;
}

static const char *test_handshake_roundtrip(void) {
    uint8_t *buffer = NULL;
    size_t buffer_len = 0;
    uint8_t eph_data[] = {1u, 2u, 3u, 4u};
    WAWebProtobufsWa6__HandshakeMessage__ClientHello client_hello =
        WAWEB_PROTOBUFS_WA6__HANDSHAKE_MESSAGE__CLIENT_HELLO__INIT;
    WAWebProtobufsWa6__HandshakeMessage handshake = WAWEB_PROTOBUFS_WA6__HANDSHAKE_MESSAGE__INIT;
    WAWebProtobufsWa6__HandshakeMessage *decoded;

    client_hello.has_ephemeral = 1;
    client_hello.ephemeral.data = eph_data;
    client_hello.ephemeral.len = sizeof(eph_data);
    client_hello.has_pqmode = 1;
    client_hello.pqmode = WAWEB_PROTOBUFS_WA6__HANDSHAKE_MESSAGE__HANDSHAKE_PQ_MODE__WA_CLASSICAL;
    handshake.clienthello = &client_hello;

    CHECK_TRUE(whatsthat_proto_serialize_handshake_message(&handshake, &buffer, &buffer_len) == 0);
    decoded = whatsthat_proto_deserialize_handshake_message(buffer, buffer_len);
    CHECK_TRUE(decoded != NULL);
    CHECK_TRUE(decoded->clienthello != NULL);
    CHECK_TRUE(decoded->clienthello->has_ephemeral == 1);
    CHECK_TRUE(decoded->clienthello->ephemeral.len == sizeof(eph_data));
    CHECK_TRUE(memcmp(decoded->clienthello->ephemeral.data, eph_data, sizeof(eph_data)) == 0);
    CHECK_TRUE(decoded->clienthello->has_pqmode == 1);
    CHECK_TRUE(decoded->clienthello->pqmode ==
               WAWEB_PROTOBUFS_WA6__HANDSHAKE_MESSAGE__HANDSHAKE_PQ_MODE__WA_CLASSICAL);

    whatsthat_proto_free_handshake_message(decoded);
    whatsthat_proto_serialized_free(buffer);
    return NULL;
}

static const char *test_client_payload_roundtrip(void) {
    uint8_t *buffer = NULL;
    size_t buffer_len = 0;
    WAWebProtobufsWa6__ClientPayload__UserAgent__AppVersion app_version =
        WAWEB_PROTOBUFS_WA6__CLIENT_PAYLOAD__USER_AGENT__APP_VERSION__INIT;
    WAWebProtobufsWa6__ClientPayload__UserAgent user_agent =
        WAWEB_PROTOBUFS_WA6__CLIENT_PAYLOAD__USER_AGENT__INIT;
    WAWebProtobufsWa6__ClientPayload client_payload = WAWEB_PROTOBUFS_WA6__CLIENT_PAYLOAD__INIT;
    WAWebProtobufsWa6__ClientPayload *decoded;

    app_version.has_primary = 1;
    app_version.primary = 2;
    app_version.has_secondary = 1;
    app_version.secondary = 3000;

    user_agent.has_platform = 1;
    user_agent.platform = WAWEB_PROTOBUFS_WA6__CLIENT_PAYLOAD__USER_AGENT__PLATFORM__WEB;
    user_agent.appversion = &app_version;

    client_payload.useragent = &user_agent;
    client_payload.pushname = (char *)"whatsthat-test";
    client_payload.has_passive = 1;
    client_payload.passive = 1;

    CHECK_TRUE(whatsthat_proto_serialize_client_payload(&client_payload, &buffer, &buffer_len) == 0);
    decoded = whatsthat_proto_deserialize_client_payload(buffer, buffer_len);
    CHECK_TRUE(decoded != NULL);
    CHECK_TRUE(decoded->useragent != NULL);
    CHECK_TRUE(decoded->useragent->has_platform == 1);
    CHECK_TRUE(decoded->useragent->platform ==
               WAWEB_PROTOBUFS_WA6__CLIENT_PAYLOAD__USER_AGENT__PLATFORM__WEB);
    CHECK_TRUE(decoded->useragent->appversion != NULL);
    CHECK_TRUE(decoded->useragent->appversion->has_primary == 1);
    CHECK_TRUE(decoded->useragent->appversion->primary == 2);
    CHECK_TRUE(strcmp(decoded->pushname, "whatsthat-test") == 0);
    CHECK_TRUE(decoded->has_passive == 1);
    CHECK_TRUE(decoded->passive == 1);

    whatsthat_proto_free_client_payload(decoded);
    whatsthat_proto_serialized_free(buffer);
    return NULL;
}

static const char *test_message_key_roundtrip(void) {
    uint8_t *buffer = NULL;
    size_t buffer_len = 0;
    WACommon__MessageKey message_key = WACOMMON__MESSAGE_KEY__INIT;
    WACommon__MessageKey *decoded;

    message_key.remotejid = (char *)"12345@s.whatsapp.net";
    message_key.has_fromme = 1;
    message_key.fromme = 1;
    message_key.id = (char *)"ABCD1234";
    message_key.participant = (char *)"67890@s.whatsapp.net";

    CHECK_TRUE(whatsthat_proto_serialize_message_key(&message_key, &buffer, &buffer_len) == 0);
    decoded = whatsthat_proto_deserialize_message_key(buffer, buffer_len);
    CHECK_TRUE(decoded != NULL);
    CHECK_TRUE(strcmp(decoded->remotejid, "12345@s.whatsapp.net") == 0);
    CHECK_TRUE(decoded->has_fromme == 1);
    CHECK_TRUE(decoded->fromme == 1);
    CHECK_TRUE(strcmp(decoded->id, "ABCD1234") == 0);
    CHECK_TRUE(strcmp(decoded->participant, "67890@s.whatsapp.net") == 0);

    whatsthat_proto_free_message_key(decoded);
    whatsthat_proto_serialized_free(buffer);
    return NULL;
}

static const char *test_e2e_message_roundtrip(void) {
    uint8_t *buffer = NULL;
    size_t buffer_len = 0;
    WAWebProtobufsE2E__ExtendedTextMessage ext = WAWEB_PROTOBUFS_E2_E__EXTENDED_TEXT_MESSAGE__INIT;
    WAWebProtobufsE2E__Message message = WAWEB_PROTOBUFS_E2_E__MESSAGE__INIT;
    WAWebProtobufsE2E__Message *decoded;

    ext.text = (char *)"hello from whatsthat";
    message.conversation = (char *)"primary conversation";
    message.extendedtextmessage = &ext;

    CHECK_TRUE(whatsthat_proto_serialize_e2e_message(&message, &buffer, &buffer_len) == 0);
    decoded = whatsthat_proto_deserialize_e2e_message(buffer, buffer_len);
    CHECK_TRUE(decoded != NULL);
    CHECK_TRUE(strcmp(decoded->conversation, "primary conversation") == 0);
    CHECK_TRUE(decoded->extendedtextmessage != NULL);
    CHECK_TRUE(strcmp(decoded->extendedtextmessage->text, "hello from whatsthat") == 0);

    whatsthat_proto_free_e2e_message(decoded);
    whatsthat_proto_serialized_free(buffer);
    return NULL;
}

static const char *test_web_message_info_roundtrip(void) {
    uint8_t *buffer = NULL;
    size_t buffer_len = 0;
    WACommon__MessageKey key = WACOMMON__MESSAGE_KEY__INIT;
    WAWebProtobufsE2E__Message message = WAWEB_PROTOBUFS_E2_E__MESSAGE__INIT;
    WAWebProtobufsWeb__WebMessageInfo web_info = WAWEB_PROTOBUFS_WEB__WEB_MESSAGE_INFO__INIT;
    WAWebProtobufsWeb__WebMessageInfo *decoded;

    key.remotejid = (char *)"group@g.us";
    key.id = (char *)"MSG-123";
    key.has_fromme = 1;
    key.fromme = 0;

    message.conversation = (char *)"group update";

    web_info.key = &key;
    web_info.message = &message;
    web_info.has_messagetimestamp = 1;
    web_info.messagetimestamp = 123456;
    web_info.has_status = 1;
    web_info.status = WAWEB_PROTOBUFS_WEB__WEB_MESSAGE_INFO__STATUS__SERVER_ACK;

    CHECK_TRUE(whatsthat_proto_serialize_web_message_info(&web_info, &buffer, &buffer_len) == 0);
    decoded = whatsthat_proto_deserialize_web_message_info(buffer, buffer_len);
    CHECK_TRUE(decoded != NULL);
    CHECK_TRUE(decoded->key != NULL);
    CHECK_TRUE(strcmp(decoded->key->id, "MSG-123") == 0);
    CHECK_TRUE(decoded->message != NULL);
    CHECK_TRUE(strcmp(decoded->message->conversation, "group update") == 0);
    CHECK_TRUE(decoded->has_messagetimestamp == 1);
    CHECK_TRUE(decoded->messagetimestamp == 123456u);
    CHECK_TRUE(decoded->has_status == 1);
    CHECK_TRUE(decoded->status == WAWEB_PROTOBUFS_WEB__WEB_MESSAGE_INFO__STATUS__SERVER_ACK);

    whatsthat_proto_free_web_message_info(decoded);
    whatsthat_proto_serialized_free(buffer);
    return NULL;
}

typedef const char *(*test_fn_t)(void);

typedef struct {
    const char *name;
    test_fn_t fn;
} test_case_t;

int main(void) {
    size_t i;
    const test_case_t tests[] = {
        {"serialize_rejects_null_args", test_serialize_rejects_null_args},
        {"handshake_roundtrip", test_handshake_roundtrip},
        {"client_payload_roundtrip", test_client_payload_roundtrip},
        {"message_key_roundtrip", test_message_key_roundtrip},
        {"e2e_message_roundtrip", test_e2e_message_roundtrip},
        {"web_message_info_roundtrip", test_web_message_info_roundtrip},
    };

    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        const char *error = tests[i].fn();
        if (error != NULL) {
            (void)fprintf(stderr, "[FAIL] %s: %s\n", tests[i].name, error);
            return EXIT_FAILURE;
        }
        (void)fprintf(stdout, "[PASS] %s\n", tests[i].name);
    }

    return EXIT_SUCCESS;
}
