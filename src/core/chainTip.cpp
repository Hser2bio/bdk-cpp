#include "chainTip.h"
#include "state.h"

void ChainTip::setBlockStatus(const Hash &blockHash, const BlockStatus &status) {
  internalChainTipLock.lock();
  this->cachedBlockStatus[blockHash] = status;
  internalChainTipLock.unlock();
}

BlockStatus ChainTip::getBlockStatus(const Hash &blockHash) const {
  internalChainTipLock.lock_shared();
  if (this->cachedBlockStatus.count(blockHash) > 0) {
    auto ret = this->cachedBlockStatus.find(blockHash)->second;
    internalChainTipLock.unlock_shared();
    return ret;
  }
  internalChainTipLock.unlock_shared();
  return BlockStatus::Unknown;
}

bool ChainTip::isProcessing(const Hash &blockHash) const {
  internalChainTipLock.lock_shared();
  if (this->cachedBlockStatus.count(blockHash) > 0) {
    bool ret = (this->cachedBlockStatus.find(blockHash)->second == BlockStatus::Processing) ? true : false;
    internalChainTipLock.unlock_shared();
    return ret;
  }
  internalChainTipLock.unlock_shared();
  return false;
};

bool ChainTip::accept(const Hash &blockHash, const std::shared_ptr<State> state, const std::shared_ptr<ChainHead> chainHead, const std::shared_ptr<BlockManager> blockManager) {
  internalChainTipLock.lock();
  auto it = this->internalChainTip.find(blockHash);
  if (it == this->internalChainTip.end()) {
    Utils::LogPrint(Log::chainTip, __func__, "Block not found");
    return false;
  }
  if (it->second.unique()) {
    Utils::LogPrint(Log::chainTip, __func__, "Block is unique, moving to processNewBlock.");
    state->processNewBlock(std::move(it->second), chainHead, blockManager);
    this->internalChainTip.erase(blockHash);
  } else {
    // We have to create a copy of the block to process it.
    Utils::LogPrint(Log::chainTip, __func__, "Block not unique, creating copy to processNewBlock.");
    auto block = std::make_shared<Block>(*it->second);
    state->processNewBlock(std::move(block), chainHead, blockManager);
  }
  this->cachedBlockStatus[blockHash] = BlockStatus::Accepted;
  internalChainTipLock.unlock();
  return true;
}

void ChainTip::reject(const Hash &blockHash) {
  internalChainTipLock.lock();
  this->internalChainTip.erase(blockHash);
  this->cachedBlockStatus[blockHash] = BlockStatus::Rejected;
  internalChainTipLock.unlock();
  return;
}

void ChainTip::processBlock(std::shared_ptr<Block> block) {
  this->internalChainTipLock.lock();
  this->internalChainTip[block->getBlockHash()] = block;
  this->cachedBlockStatus[block->getBlockHash()] = BlockStatus::Processing;
  this->internalChainTipLock.unlock();
}

const std::shared_ptr<const Block> ChainTip::getBlock(const Hash &blockHash) const {
  internalChainTipLock.lock_shared();
  auto it = internalChainTip.find(blockHash);
  if (it == internalChainTip.end()) { // Block not found
    internalChainTipLock.unlock_shared();
    return nullptr;
  }
  const std::shared_ptr<const Block>& ret = it->second;
  internalChainTipLock.unlock_shared();
  return ret;
};

Hash ChainTip::getPreference() const {
  internalChainTipLock.lock_shared();
  Hash ret = preferedBlockHash;
  internalChainTipLock.unlock_shared();
  return ret;
}

bool ChainTip::exists(const Hash &blockHash) const {
  internalChainTipLock.lock_shared();
  bool ret = (internalChainTip.count(blockHash) > 0) ? true : false;
  internalChainTipLock.unlock_shared();
  return ret;
}

void ChainTip::setPreference(const Hash &blockHash) {
  internalChainTipLock.lock();
  preferedBlockHash = blockHash;
  internalChainTipLock.unlock();
}

