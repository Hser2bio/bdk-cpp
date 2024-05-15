/*
Copyright (c) [2023-2024] [AppLayer Developers]

This software is distributed under the MIT License.
See the LICENSE.txt file in the project root for more information.
*/

#ifndef BLOCKCHAINWRAPPER_H
#define BLOCKCHAINWRAPPER_H

#include "../src/core/storage.h"
#include "../src/core/state.h"
#include "../src/net/p2p/managernormal.h"
#include "../src/net/http/httpserver.h"
#include "../src/utils/options.h"
#include "../src/utils/db.h"
#include "../src/core/blockchain.h"
#include "../src/utils/utils.h"

/**
 * Simple wrapper struct for management of all blockchain related objects.
 * Initializes them properly in the constructor, allowing direct access to the
 * underlying objects, but does not apply any logic or checks.
 */
struct TestBlockchainWrapper {
  const Options options;  ///< Options singleton.
  DB db;                  ///< Database.
  Storage storage;        ///< Blockchain storage.
  State state;            ///< Blockchain state.
  P2P::ManagerNormal p2p; ///< P2P connection manager.
  HTTPServer http;        ///< HTTP server.
  Syncer syncer;             ///< Blockchain syncer.
  Consensus consensus;       ///< Block and transaction processing.

  /**
   * Constructor.
   * @param options_ Reference to the Options singleton.
   */

  explicit TestBlockchainWrapper(const Options& options_) :
    options(options_),
    db(std::get<0>(DumpManager::getBestStateDBPath(options))),
    storage(options_),
    state(db, storage, p2p, std::get<1>(DumpManager::getBestStateDBPath(options)), options),
    p2p(boost::asio::ip::address::from_string("127.0.0.1"), options, storage, state),
    http(state, storage, p2p, options),
    syncer(p2p, storage, state),
    consensus(state, p2p, storage, options)
    {};

  /// Destructor.
  ~TestBlockchainWrapper() {
    state.dumpStopWorker();
    consensus.stop();
    p2p.stopDiscovery();
    p2p.stop();
    http.stop();
  }
};

// We initialize the blockchain database
// To make sure that if the genesis is changed within the main source code
// The tests will still work, as tests uses own genesis block.
inline TestBlockchainWrapper initialize(const std::vector<Hash>& validatorPrivKeys,
                                 const PrivKey& validatorKey,
                                 const uint64_t& serverPort,
                                 bool clearDb,
                                 const std::string& folderName) {
  if (clearDb) {
    if (std::filesystem::exists(folderName)) {
      std::filesystem::remove_all(folderName);
    }
  }
  std::vector<std::pair<boost::asio::ip::address, uint64_t>> discoveryNodes;
  PrivKey genesisPrivKey(Hex::toBytes("0xe89ef6409c467285bcae9f80ab1cfeb3487cfe61ab28fb7d36443e1daa0c2867"));
  uint64_t genesisTimestamp = 1656356646000000;
  FinalizedBlock genesis = FinalizedBlock::createNewValidBlock({},{}, Hash(), genesisTimestamp, 0, genesisPrivKey);
  std::vector<std::pair<Address,uint256_t>> genesisBalances = {{Address(Hex::toBytes("0x00dead00665771855a34155f5e7405489df2c3c6")), uint256_t("1000000000000000000000")}};
  std::vector<Address> genesisValidators;
  for (const auto& privKey : validatorPrivKeys) {
    genesisValidators.push_back(Secp256k1::toAddress(Secp256k1::toUPub(privKey)));
  }
  if (!validatorKey) {
    return TestBlockchainWrapper(Options(
        folderName,
        "BDK/cpp/linux_x86-64/0.2.0",
        1,
        8080,
        Address(Hex::toBytes("0x00dead00665771855a34155f5e7405489df2c3c6")),
        boost::asio::ip::address::from_string("127.0.0.1"),
        serverPort,
        9999,
        11,
        11,
        200,
        50,
        2000,
        10000,
        1000,
        4,
        discoveryNodes,
        genesis,
        genesisTimestamp,
        genesisPrivKey,
        genesisBalances,
        genesisValidators
      ));
  } else {
    return TestBlockchainWrapper(Options(
      folderName,
      "BDK/cpp/linux_x86-64/0.2.0",
      1,
      8080,
      Address(Hex::toBytes("0x00dead00665771855a34155f5e7405489df2c3c6")),
      boost::asio::ip::address::from_string("127.0.0.1"),
      serverPort,
      9999,
      11,
      11,
      200,
      50,
      2000,
      10000,
      1000,
      4,
      discoveryNodes,
      genesis,
      genesisTimestamp,
      genesisPrivKey,
      genesisBalances,
      genesisValidators,
      validatorKey
    ));
  }
}

// This creates a valid block given the state within the rdPoS class.
// Should not be used during network/thread testing, as it will automatically sign all TxValidator transactions within the block
// And that is not the purpose of network/thread testing.
inline FinalizedBlock createValidBlock(const std::vector<Hash>& validatorPrivKeys,
                                State& state, Storage& storage,
                                std::vector<TxBlock>&& txs = {}) {
  auto validators = state.rdposGetValidators();
  auto randomList = state.rdposGetRandomList();

  Hash blockSignerPrivKey;           // Private key for the block signer.
  std::vector<Hash> orderedPrivKeys; // Private keys for the rdPoS in the order of the random list, limited to rdPoS' minValidators.
  orderedPrivKeys.reserve(4);
  for (const auto& privKey : validatorPrivKeys) {
    if (Secp256k1::toAddress(Secp256k1::toUPub(privKey)) == randomList[0]) {
      blockSignerPrivKey = privKey;
      break;
    }
  }

  for (uint64_t i = 1; i < state.rdposGetMinValidators() + 1; i++) {
    for (const auto& privKey : validatorPrivKeys) {
      if (Secp256k1::toAddress(Secp256k1::toUPub(privKey)) == randomList[i]) {
        orderedPrivKeys.push_back(privKey);
        break;
      }
    }
  }

  // By now we should have randomList[0] privKey in blockSignerPrivKey and the rest in orderedPrivKeys, ordered by the random list.
  // We can proceed with creating the block, transactions have to be **ordered** by the random list.

  // Create a block with 8 TxValidator transactions, 2 for each validator, in order (randomHash and random)
  uint64_t newBlocknHeight = storage.latest()->getNHeight() + 1;
  uint64_t newBlockTimestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  Hash newBlockPrevHash = storage.latest()->getHash();
  std::vector<TxValidator> randomHashTxs;
  std::vector<TxValidator> randomTxs;

  std::vector<Hash> randomSeeds(orderedPrivKeys.size(), Hash::random());
  for (uint64_t i = 0; i < orderedPrivKeys.size(); ++i) {
    Address validatorAddress = Secp256k1::toAddress(Secp256k1::toUPub(orderedPrivKeys[i]));
    Bytes hashTxData = Hex::toBytes("0xcfffe746");
    Utils::appendBytes(hashTxData, Utils::sha3(randomSeeds[i].get()));
    Bytes randomTxData = Hex::toBytes("0x6fc5a2d6");
    Utils::appendBytes(randomTxData, randomSeeds[i].get());
    randomHashTxs.emplace_back(
      validatorAddress,
      hashTxData,
      8080,
      newBlocknHeight,
      orderedPrivKeys[i]
    );
    randomTxs.emplace_back(
      validatorAddress,
      randomTxData,
      8080,
      newBlocknHeight,
      orderedPrivKeys[i]
    );
  }
  // Append the transactions to the block.
  std::vector<TxValidator> txsValidator;
  for (const auto& tx : randomHashTxs) {
    REQUIRE(isTxStatusValid(state.rdposAddValidatorTx(tx)));
    txsValidator.emplace_back(tx);
  }
  for (const auto& tx : randomTxs) {
    REQUIRE(isTxStatusValid(state.rdposAddValidatorTx(tx)));
    txsValidator.emplace_back(tx);
  }

  // Check rdPoS mempool.
  auto rdPoSmempool = state.rdposGetMempool();
  REQUIRE(state.rdposGetMempool().size() == 8);
  for (const auto& tx : randomHashTxs) {
    REQUIRE(rdPoSmempool.contains(tx.hash()));
  }
  for (const auto& tx : randomTxs) {
    REQUIRE(rdPoSmempool.contains(tx.hash()));
  }

  // Finalize the block
  FinalizedBlock finalized = FinalizedBlock::createNewValidBlock(std::move(txs),
                                                                 std::move(txsValidator),
                                                                 newBlockPrevHash,
                                                                 newBlockTimestamp,
                                                                 newBlocknHeight,
                                                                 blockSignerPrivKey);
  return finalized;
}

#endif // BLOCKCHAINWRAPPER_H
