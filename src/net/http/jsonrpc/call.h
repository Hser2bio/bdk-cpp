#ifndef JSONRPC_CALL_H
#define JSONRPC_CALL_H

#include "variadicparser.h"
#include "../../p2p/managernormal.h"

namespace jsonrpc {

/// @brief process a json RPC call
/// @param request the complete json RPC request
/// @param state reference to the chain state
/// @param p2p reference to the P2P manager
/// @param options reference to the global options
json call(const json& request,
  State& state,
  const Storage& storage,
  P2P::ManagerNormal& p2p,
  const Options& options) noexcept;

} // namespace jsonrpc

#endif // JSONRPC_CALL_H