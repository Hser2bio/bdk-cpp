/*
Copyright (c) [2023-2024] [Sparq Network]

This software is distributed under the MIT License.
See the LICENSE.txt file in the project root for more information.
*/

#ifndef P2P_ENCODING_H
#define P2P_ENCODING_H

#include <future>

#include "../../utils/utils.h"
#include "../../utils/safehash.h"
#include "../../utils/tx.h"
#include "../../utils/block.h"
#include "../../utils/options.h"

namespace P2P {
  // Forward declarations.
  class Message;

  /// Enum for identifying which type of connection is being made.
  enum ConnectionType { INBOUND, OUTBOUND };

  /**
   * Messaging concepts:
   *
   * Request: a point-to-point message that requires an Answer;
   * Answer: a message that fulfills a Request;
   * Broadcast: use only for messages that must be routed to all nodes
   *   automatically by the networking engine;
   * Notification: one-way message between two peers.
   *
   * "NotifyAll" methods mean sending a notification to all peers
   *   (this is not routed; the routed version is a Broadcast).
   */

  /**
   * Enum for identifying from which type is a given node.
   *
   * "Normal" P2P nodes follow all protocol rules, can answer any request,
   * and will broadcast requests to other nodes if the broadcast flag is used.
   *
   * "Discovery" P2P nodes only answer requests related to connection/discovery,
   * and will not broadcast requests to other nodes.
   */
  enum NodeType { NORMAL_NODE, DISCOVERY_NODE };

  /// Enum for identifying the type of a request.
  enum RequestType { Requesting, Answering, Broadcasting, Notifying };

  /// Enum for identifying the type of a command.
  enum CommandType {
    Ping,
    Info,
    RequestNodes,
    RequestValidatorTxs,
    BroadcastValidatorTx,
    BroadcastTx,
    BroadcastBlock,
    BroadcastInfo, // FIXME/TODO: Remove this message/command if it's not going to be broadcasted (routed)
    RequestTxs,
    NotifyInfo
  };

  /**
   * List of type prefixes (as per RequestType) for easy conversion.
   * Reference is as follows:
   * - "00" = Request
   * - "01" = Answer
   * - "02" = Broadcast
   * - "03" = Notification
   */
  inline extern const std::vector<Bytes> typePrefixes {
    Bytes(1, 0x00), // Request
    Bytes(1, 0x01), // Answer
    Bytes(1, 0x02), // Broadcast
    Bytes(1, 0x03)  // Notification
  };

  /**
   * List of command prefixes (as per CommandType) for easy conversion.
   * Reference is as follows:
   * - "0000" = Ping
   * - "0001" = Info
   * - "0002" = RequestNodes
   * - "0003" = RequestValidatorTxs
   * - "0004" = BroadcastValidatorTx
   * - "0005" = BroadcastTx
   * - "0006" = BroadcastBlock
   * - "0007" = BroadcastInfo
   * - "0008" = RequestTxs
   * - "0009" = NotifyInfo
   */
  inline extern const std::vector<Bytes> commandPrefixes {
    Bytes{0x00, 0x00}, // Ping
    Bytes{0x00, 0x01}, // Info
    Bytes{0x00, 0x02}, // RequestNodes
    Bytes{0x00, 0x03}, // RequestValidatorTxs
    Bytes{0x00, 0x04}, // BroadcastValidatorTx
    Bytes{0x00, 0x05}, // BroadcastTx
    Bytes{0x00, 0x06}, // BroadcastBlock
    Bytes{0x00, 0x07}, // BroadcastInfo
    Bytes{0x00, 0x08}, // RequestTxs
    Bytes{0x00, 0x09}  // NotifyInfo
  };

  /**
   * Get the type of a request within a message.
   * @param message The message to parse.
   * @return The request type.
   */
  RequestType getRequestType(const BytesArrView message);

  /**
   * Get the 1-byte prefix of a given request inside typePrefixes.
   * @param type The request type to parse.
   * @return The prefix string.
   */
  const Bytes& getRequestTypePrefix(const RequestType& type);

  /**
   * Get the type of a command within a message.
   * @param message The message to parse.
   * @return The command type.
   */
  CommandType getCommandType(const BytesArrView message);

  /**
   * Get the 2-byte prefix of a given command inside commandPrefixes.
   * @param commType The command type to parse.
   * @return The prefix string.
   */
  const Bytes& getCommandPrefix(const CommandType& commType);

  /// Abstraction of an 8-byte/64-bit hash that represents a unique ID for a request. Inherits `FixedBytes<8>`.
  class RequestID : public FixedBytes<8> {
    public:
      // Using parent constructor and operator=.
      using FixedBytes<8>::FixedBytes;
      using FixedBytes<8>::operator=;

      /**
       * Constructor.
       * @param value The unsigned number to convert into a hash string.
       */
      explicit RequestID(const uint64_t& value);

      /// Convert the hash string back to an unsigned number.
      uint64_t toUint64() const;

      /// Generate a random hash.
      static RequestID random();
  };

  using NodeID = std::pair<boost::asio::ip::address, uint16_t>;

  /// Struct with information about a given node.
  class NodeInfo {
    private:
      /// Node version.
      uint64_t nodeVersion_;

      /// Current node epoch timestamp, in microseconds.
      /// This is the timestamp that the node answered us
      uint64_t currentNodeTimestamp_;

      /// Current epoch timestamp, in seconds.
      /// Timestamp for when we parsed the NodeInfo
      uint64_t currentTimestamp_;

      /// Difference between the current node timestamp and the current timestamp, in seconds.
      /// int because the node clock can be ahead or behind our system clock.
      /// This **does not** determine latency.
      int64_t timeDifference_;

      /// Height of the latest block the node is at.
      uint64_t latestBlockHeight_;

      /// %Hash of the latest block the node is at.
      Hash latestBlockHash_;

    public:
      NodeInfo() : nodeVersion_(0), currentNodeTimestamp_(0), currentTimestamp_(0),
        timeDifference_(0), latestBlockHeight_(0), latestBlockHash_(Hash()) {};

      /// Default constructor.
      NodeInfo(const uint64_t& nodeVersion, const uint64_t& currentNodeTimestamp,
        const uint64_t& currentTimestamp, const int64_t& timeDifference,
        const uint64_t& latestBlockHeight, const Hash& latestBlockHash
      ) : nodeVersion_(nodeVersion), currentNodeTimestamp_(currentNodeTimestamp),
          currentTimestamp_(currentTimestamp), timeDifference_(timeDifference),
          latestBlockHeight_(latestBlockHeight), latestBlockHash_(latestBlockHash) {};

      /// Equality operator. Checks if all members are the same.
      bool operator==(const NodeInfo& other) const {
        return (
          this->nodeVersion_ == other.nodeVersion_ &&
          this->currentNodeTimestamp_ == other.currentNodeTimestamp_ &&
          this->currentTimestamp_ == other.currentTimestamp_ &&
          this->timeDifference_ == other.timeDifference_ &&
          this->latestBlockHeight_ == other.latestBlockHeight_ &&
          this->latestBlockHash_ == other.latestBlockHash_
        );
      }

      /// Assignment operator.
      NodeInfo& operator=(const NodeInfo& other) {
        if (this != &other) {
          this->nodeVersion_ = other.nodeVersion_;
          this->currentTimestamp_ = other.currentTimestamp_;
          this->currentNodeTimestamp_ = other.currentNodeTimestamp_;
          this->timeDifference_ = other.timeDifference_;
          this->latestBlockHeight_ = other.latestBlockHeight_;
          this->latestBlockHash_ = other.latestBlockHash_;
        }
        return *this;
      }

      /// Getter functions
      const uint64_t& nodeVersion() const { return this->nodeVersion_; }
      const uint64_t& currentNodeTimestamp() const { return this->currentNodeTimestamp_; }
      const uint64_t& currentTimestamp() const { return this->currentTimestamp_; }
      const int64_t& timeDifference() const { return this->timeDifference_; }
      const uint64_t& latestBlockHeight() const { return this->latestBlockHeight_; }
      const Hash& latestBlockHash() const { return this->latestBlockHash_; }

  };

  /// Helper class used to create requests.
  class RequestEncoder {
    public:
      /**
       * Create a `Ping` request.
       * @return The formatted request.
       */
      static Message ping();

      /**
       * Create a `Info` request.
       * @param latestBlock Pointer to the node's latest block.
       * @param options Pointer to the node's options singleton.
       * @return The formatted request.
       */
      static Message info(
        const std::shared_ptr<const Block>& latestBlock,
        const Options& options
      );

      /**
       * Create a `RequestNodes` request.
       * @return The formatted request.
       */
      static Message requestNodes();

      /**
       * Create a `RequestValidatorTxs` request.
       * @return The formatted request.
       */
      static Message requestValidatorTxs();

      /**
       * Create a `RequestTxs` request.
       * @return The formatted request.
       */
      static Message requestTxs();
  };

  /// Helper class used to parse requests.
  class RequestDecoder {
    public:
      /**
       * Parse a `Ping` message.
       * @param message The message to parse.
       * @return `true` if the message is valid, `false` otherwise.
       */
      static bool ping(const Message& message);

      /**
       * Parse a `Info` message.
       * Throws if message is invalid.
       * @param message The message to parse.
       * @return A struct with the node's information.
       */
      static NodeInfo info(const Message& message);

      /**
       * Parse a `RequestNodes` message.
       * @param message The message to parse.
       * @return `true` if the message is valid, `false` otherwise.
       */
      static bool requestNodes(const Message& message);

      /**
       * Parse a `RequestValidatorTxs` message.
       * @param message The message to parse.
       * @return `true` if the message is valid, `false` otherwise.
       */
      static bool requestValidatorTxs(const Message& message);

      /**
       * Parse a `RequestTxs` message.
       * @param message The message to parse.
       * @return `true` if the message is valid, `false` otherwise.
       */
      static bool requestTxs(const Message& message);
  };

  /// Helper class used to create answers to requests.
  class AnswerEncoder {
    public:
      /**
       * Create a `Ping` answer.
       * @param request The request message.
       * @return The formatted answer.
       */
      static Message ping(const Message& request);

      /**
       * Create a `Info` answer.
       * @param request The request message.
       * @param latestBlock Pointer to the node's latest block.
       * @param options Pointer to the node's options singleton.
       * @return The formatted answer.
       */
      static Message info(const Message& request,
        const std::shared_ptr<const Block>& latestBlock,
        const Options& options
      );

      /**
       * Create a `RequestNodes` answer.
       * @param request The request message.
       * @param nodes The list of nodes to use as reference.
       * @return The formatted answer.
       */
      static Message requestNodes(const Message& request,
        const std::unordered_map<NodeID, NodeType, SafeHash>& nodes
      );

      /**
       * Create a `RequestValidatorTxs` answer.
       * @param request The request message.
       * @param txs The list of transactions to use as reference.
       * @return The formatted answer.
       */
      static Message requestValidatorTxs(const Message& request,
        const std::unordered_map<Hash, TxValidator, SafeHash>& txs
      );

      /**
       * Create a `RequestTxs` answer.
       * @param request The request message.
       * @param txs The list of transactions to use as reference.
       * @return The formatted answer.
       */
      static Message requestTxs(const Message& request,
        const std::unordered_map<Hash, TxBlock, SafeHash>& txs
      );
  };

  /// Helper class used to parse answers to requests.
  class AnswerDecoder {
    public:
      /**
       * Parse a `Ping` answer.
       * @param message The answer to parse.
       * @return `true` if the message is valid, `false` otherwise.
       */
      static bool ping(const Message& message);

      /**
       * Parse a `Info` answer.
       * @param message The answer to parse.
       * @return A struct with the node's information.
       */
      static NodeInfo info(const Message& message);

      /**
       * Parse a `RequestNodes` answer.
       * @param message The answer to parse.
       * @return A list of requested nodes.
       */
      static std::unordered_map<
        NodeID, NodeType, SafeHash
      > requestNodes(const Message& message);

      /**
       * Parse a `RequestValidatorTxs` answer.
       * @param message The answer to parse.
       * @param requiredChainId The chain ID to use as reference.
       * @return A list of requested Validator transactions.
       */
      static std::vector<TxValidator> requestValidatorTxs(
        const Message& message, const uint64_t& requiredChainId
      );

      /**
       * Parse a `RequestTxs` answer.
       * @param message The answer to parse.
       * @param requiredChainId The chain ID to use as reference.
       * @return A list of requested Validator transactions.
       */
      static std::vector<TxBlock> requestTxs(
        const Message& message, const uint64_t& requiredChainId
      );
  };

  /// Helper class used to create broadcast messages.
  class BroadcastEncoder {
    public:
      /**
       * Create a message to broadcast a Validator transaction.
       * @param tx The transaction to broadcast.
       * @return The formatted message.
       */
      static Message broadcastValidatorTx(const TxValidator& tx);

      /**
       * Create a message to broadcast a block transaction.
       * @param tx The transaction to broadcast.
       * @return The formatted message.
       */
      static Message broadcastTx(const TxBlock& tx);

      /**
       * Create a message to broadcast a whole block.
       * @param block The block to broadcast.
       * @return The formatted message.
       */
      static Message broadcastBlock(const std::shared_ptr<const Block>& block);

      /**
       * Create a message to broadcast the node's information.
       * @param nodeInfo The node's information.
       * @return The formatted message.
       */
      static Message broadcastInfo(const std::shared_ptr<const Block>& latestBlock, const Options& options);
  };

  /// Helper class used to parse broadcast messages.
  class BroadcastDecoder {
    public:
      /**
       * Parse a broadcasted message for a Validator transaction.
       * @param message The message that was broadcast.
       * @param requiredChainId The chain ID to use as reference.
       * @return The built Validator transaction object.
       */
      static TxValidator broadcastValidatorTx(const Message& message, const uint64_t& requiredChainId);

      /**
       * Parse a broadcasted message for a block transaction.
       * @param message The message that was broadcast.
       * @param requiredChainId The chain ID to use as reference.
       * @return The built block transaction object.
       */
      static TxBlock broadcastTx(const Message& message, const uint64_t& requiredChainId);

      /**
       * Parse a broadcasted message for a whole block.
       * @param message The message that was broadcast.
       * @param requiredChainId The chain ID to use as reference.
       * @return The build block object.
       */
      static Block broadcastBlock(const Message& message, const uint64_t& requiredChainId);

      /**
       * Parse a broadcasted message for a node's information.
       * @param message The message that was broadcast.
       * @return The node's information.
       */
      static NodeInfo broadcastInfo(const Message& message);
  };

  /// Helper class used to create notification messages.
  class NotificationEncoder {
    public:
      /**
       * Create a message to notify the node's information.
       * @param nodeInfo The node's information.
       * @return The formatted message.
       */
      static Message notifyInfo(const std::shared_ptr<const Block>& latestBlock, const Options& options);
  };

  /// Helper class used to parse notification messages.
  class NotificationDecoder {
    public:
      /**
       * Parse a notification message for a node's information.
       * @param message The message that was broadcast.
       * @return The node's information.
       */
      static NodeInfo notifyInfo(const Message& message);
  };

  /**
   * Abstraction of a %P2P message.
   * The structure is a bytes string (1 byte = 2 chars), as follows:
   * ```
   * 0x            00         0000000000000000       0000        00000000000000000000000000000000...
   * What:    Request Flag        Random ID       Command ID                Data...
   * Chars:        2                 16               4                       X
   * Bytes:        1                 8                2                      X/2
   * ```
   */
  class Message {
    private:
      /// The internal message data to be read/written, stored as bytes.
      /// Sessions has directly access to it
      /// As it can use the vector for its buffer.
      Bytes rawMessage_;

      /// Raw string move constructor. Throws on invalid size.
      explicit Message(Bytes&& raw) : rawMessage_(std::move(raw)) {
        if (rawMessage_.size() < 11) throw DynamicException("Invalid message size.");
      }

      /// Assignment operator.
      Message& operator=(const Message& message) {
        this->rawMessage_ = message.rawMessage_; return *this;
      }

    public:
      /// Default constructor.
      Message() = default;

      /// Copy constructor.
      Message(const Message& message) { this->rawMessage_ = message.rawMessage_; }

      /// Move constructor.
      Message(Message&& message) { this->rawMessage_ = std::move(message.rawMessage_); }

      /// Get the request type of the message.
      RequestType type() const { return getRequestType(BytesArrView(rawMessage_).subspan(0,1)); }

      /// Get the request ID of the message.
      RequestID id() const { return RequestID(BytesArrView(rawMessage_).subspan(1, 8)); }

      /// Get the command type of the message.
      CommandType command() const { return getCommandType(BytesArrView(rawMessage_).subspan(9,2)); }

      /// Get the message data (without the flags and IDs).
      BytesArrView message() const { return BytesArrView(rawMessage_).subspan(11); }

      /// Get the whole message.
      BytesArrView raw() const { return this->rawMessage_; }

      /// Get the message's size.
      size_t size() const { return this->rawMessage_.size(); }

      friend class RequestEncoder;
      friend class AnswerEncoder;
      friend class BroadcastEncoder;
      friend class NotificationEncoder;
      friend class Session;
      friend class Request;
  };

  /// Abstraction of a %P2P request, passed through the network.
  class Request {
    private:
      const CommandType command_;                                 ///< Command type.
      const RequestID id_;                                        ///< Request ID.
      const NodeID nodeId_;                                       ///< Host node ID.
      std::promise<const std::shared_ptr<const Message>> answer_; ///< Answer to the request.
      const std::shared_ptr<const Message> message_;              ///< The request message. Used if we need to ask another node.
      bool isAnswered_ = false;                                   ///< Indicates whether the request was answered.

    public:
      /**
       * Constructor.
       * @param command The request's command type.
       * @param id The request's ID.
       * @param nodeId The request's host node ID.
       * @param message The request's message.
       */
      Request(
        const CommandType& command, const RequestID& id, const NodeID& nodeId,
        const std::shared_ptr<const Message>& message
      ) : command_(command), id_(id), nodeId_(nodeId), message_(message) {};

      /// Getter for `command_`.
      const CommandType& command() const { return this->command_; };

      /// Getter for `id_`.
      const RequestID& id() const { return this->id_; };

      /// Getter for `nodeId_`.
      const NodeID& nodeId() const { return this->nodeId_; };

      /// Getter for `answer_`.
      std::future<const std::shared_ptr<const Message>> answerFuture() { return this->answer_.get_future(); };

      /// Getter for `isAnswered_`.
      bool isAnswered() const { return this->isAnswered_; };

      /// Setter for `answer_`. Also sets `isAnswered_` to `true`.
      void setAnswer(const std::shared_ptr<const Message> answer) { answer_.set_value(answer); isAnswered_ = true; };
  };
};

#endif  // P2P_ENCODING_H
