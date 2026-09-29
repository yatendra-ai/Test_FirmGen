/*
 * @file model_identity.h
 * @brief Immutable source ONNX identity recorded by the ST Edge AI compiler.
 */
#ifndef MODEL_IDENTITY_H
#define MODEL_IDENTITY_H

/* SHA-256 from model_artifacts/ml_c5_stai_cpu_compile_retry2/model/model_manifest.json
 * and ml_toolchain.lock.json. The ST Edge AI generated network records the same
 * source-model signature in STAI_NETWORK_ORIGIN_MODEL_SIGNATURE.
 */
#define APP_MODEL_SHA256_HEX "15deb03ca6bbc45c47b4eaec4d9a9cdeca291c2e6bf5a0e64fdaea702778d090"

#endif /* MODEL_IDENTITY_H */
